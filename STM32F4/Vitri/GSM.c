#include "GSM.h"
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

unsigned char RxBuffer[128];	/* Buffer con los datos de entrada */
static unsigned char RxCounter;	/* Apinta donde va el buffer */
unsigned char Incoming_call_flag = 0;	/* No indica que hay una llamada entrante */
unsigned char New_Message_Flag = 0;
unsigned char Emergency_SMS_Flag = 0;
unsigned char GSM_OK_Flag = 0;
unsigned char GSM_Yes_Flag = 0;

volatile char *number;			/* Para guardar temporalmente el número */
volatile char *content;			/* Para guardar temporalmente el número */

unsigned char SMS_State = SMS_IDLE_STATE;

void GSM_init(void){
	USART_InitTypeDef USART_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_ClocksTypeDef RCC_Clocks;

	RCC_GetClocksFreq (&RCC_Clocks);

	/* Habilitamos los CLK para cada periférico */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	/* Iniciamos los pines de transmiscion y recepción */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* Colocamos los pines en su función alternativa*/
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);

	/* Configuramos la UART que se utilizará */
	USART_InitStruct.USART_BaudRate = 9600;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStruct);
	USART_Cmd(USART1, ENABLE);

	/* Habilitamos la interrupcion del serial para la recepción*/
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	/* Configuración de la interrupción */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 10;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 10;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Habilitamos el módulo de la comunicación Serial */
	USART_Cmd(USART1, ENABLE);
}

void GSM_Send(volatile char *s){
	while(*s){
		while(!(USART1->SR & 0x00000040)); /* Esperamos hasta que el registro de datos este limpio */
		USART1->DR = (*s & (uint16_t)0x01FF);	/* Indicamos el dato a transmitir */
		(void)*s++;
	}
}

void GSM_SendSMS(volatile char *PhoneNumber, volatile char *Content){
	if(SMS_State == SMS_IDLE_STATE){
		number = PhoneNumber;	/* Guardamos el número de teléfono */
		content = Content;

		/* Hacemos que la máquina comience a funcionar */
		SMS_State = SMS_MODE_STATE;
		GSM_Send(Check);	/* Si regresa un 'OK', la máquina de estados comenzara a funcionar */
	}
}

/* Interrucpión para la recepción de datos */
void USART1_IRQHandler(void){
	if( USART_GetITStatus(USART1, USART_IT_RXNE)){
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);

		/* Guardamos el dato en el buffer de entrada */
		RxBuffer[RxCounter++%128] = USART1->DR;

		/* Verificamos un 'OK' */
		if(RxBuffer[(RxCounter-2)%128] == 'K' && RxBuffer[(RxCounter-3)%128] == 'O'){
			GSM_OK_Flag = 1;
			if(SMS_State != SMS_IDLE_STATE){
				GSM_SMS_StateMachine();
			}
		}

		if(RxBuffer[(RxCounter-2)%128] == '>'){
			if(SMS_State == SMS_CONTENT_STATE){
				GSM_SMS_StateMachine();
			}
		}

		if(New_Message_Flag){
			if(RxBuffer[(RxCounter-1)%128] == 'i' && RxBuffer[(RxCounter-2)%128] == 'S'){
				New_Message_Flag = 0;
				GSM_Yes_Flag = 1;
			}

			if(RxBuffer[(RxCounter-1)%128] == 'o' && RxBuffer[(RxCounter-2)%128] == 'N'){
				New_Message_Flag = 0;
				Emergency_SMS_Flag = 1;
			}
		}

		/* Verificamos si hay un mensaje entrante */
		if(RxBuffer[(RxCounter-1)%128] == 'T' && RxBuffer[(RxCounter-2)%128] == 'M' && RxBuffer[(RxCounter-3)%128] == 'C' && RxBuffer[(RxCounter-4)%128] == '+'){
			New_Message_Flag = 1;
		}
	}
}

/* Máquinas de estado para controlar los eventos */

void GSM_SMS_StateMachine(void){
	if(SMS_State == SMS_MODE_STATE){	/* Colocamos el modo de enviar mensaje */
		GSM_Send(SendSMS_Mode);
		SMS_State = SMS_NUMBER_STATE;

	} else if(SMS_State == SMS_NUMBER_STATE){	/* Le pasamos el número a cual se le enviará el mensaje */
		GSM_Send(SetSMS);
		GSM_Send(Quotation_Marks);
		GSM_Send(number);
		GSM_Send(Quotation_Marks);
		GSM_Send(Terminator);
		SMS_State = SMS_CONTENT_STATE;

	} else if(SMS_State == SMS_CONTENT_STATE){	/* Le damos el contenido del mensaje */
		GSM_Send(content);
		while(!(USART1->SR & 0x00000040));
		USART1->DR = FinishSMS;
		SMS_State = SMS_IDLE_STATE;
	}
}
