#define OK RxBuffer[(RxCounter-1)%BUFFER_SIZE] == 'K' && RxBuffer[(RxCounter-2)%BUFFER_SIZE] == 'O'



#include "WiFi.h"
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

#define BUFFER_SIZE	128

static unsigned char RxCounter;
static unsigned char RxBuffer[BUFFER_SIZE];

uint8_t client;	/* Guarda cual es el cliente que se acana de conectar */

uint8_t HTTP_SERVER_FLAG = 0;	/* Indica que estará ativa la máquina de estados */
uint8_t OK_FLAG = 0;
uint8_t CONNECT_FLAG = 0;
uint8_t PROMP_FLAG = 0;
uint8_t CLIENT_CONNECTED_FLAG = 0;

static uint8_t request = 0;


void WiFi_init(void){
	USART_InitTypeDef USART_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_ClocksTypeDef RCC_Clocks;

	RCC_GetClocksFreq (&RCC_Clocks);

	/* Habilitamos los CLK para cada periférico */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	/* Iniciamos los pines de transmisión y recepción */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* Colocamos los pines en su función alternativa*/
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3); 	/* Transmisión(Tx) */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);	/* Recepción (Rx) */

	/* Configuramos la UART que se utilizará */
	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART3, &USART_InitStruct);

	/* Habilitamos la interrupcion del serial para la recepción*/
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

	/* Configuración de la interrupción */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Habilitamos el módulo de la comunicación Serial */
	USART_Cmd(USART3, ENABLE);
}

void USART_Send_Char(volatile char s){
	while(!(USART3->SR & 0x00000040)); /* Esperamso hasta que HTR este limpio */
	USART3->DR = (s & (uint16_t)0x01FF);	/* Indicamos el dato a transmitir */
}

void USART_Send(volatile char *s){
	while(*s){
		while(!(USART3->SR & 0x00000040)); /* Esperamso hasta que HTR este limpio */
		USART3->DR = (*s & (uint16_t)0x01FF);	/* Indicamos el dato a transmitir */
		(void)*s++;
	}
}

void WiFi_Connect(volatile char* SSID, volatile char* Password){
	USART_Send(SSID);
	USART_Send(",");
	USART_Send(Password);
	USART_Send("\r\n");
}

void USART3_IRQHandler(void){
	if( USART_GetITStatus(USART3, USART_IT_RXNE)){
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
		RxBuffer[RxCounter++%BUFFER_SIZE] = USART3->DR;	/* Guardamos lo que llega en un buffer circular */

		if(OK){	/* Verificamos que llegue un OK por parte del módulo */
			OK_FLAG = 1;
		}

		/* Busca si hay un request */
		if(RxBuffer[(RxCounter-3)%BUFFER_SIZE] == 'D' && RxBuffer[(RxCounter-4)%BUFFER_SIZE] == 'P' && RxBuffer[(RxCounter-5)%BUFFER_SIZE] == 'I' && RxBuffer[(RxCounter-6)%BUFFER_SIZE] == '+'){
			request = 1;
			client = RxBuffer[RxCounter-1];	/* En esta posocions e encuentra el ID del cliente */
		}

		if(request && RxBuffer[(RxCounter-1)%BUFFER_SIZE] == '\n' && RxBuffer[(RxCounter-2)%BUFFER_SIZE] == '\r' && RxBuffer[(RxCounter-3)%BUFFER_SIZE] == '\n' && RxBuffer[(RxCounter-4)%BUFFER_SIZE] == '\r'){
			request = 0;
			CLIENT_CONNECTED_FLAG = 1;
		}

		if(RxBuffer[(RxCounter-1)%BUFFER_SIZE] == '>' && RxBuffer[(RxCounter-2)%BUFFER_SIZE] == '\n'){
			PROMP_FLAG = 1;
		}
	}
}
