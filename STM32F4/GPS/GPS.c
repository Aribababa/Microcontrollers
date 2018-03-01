#define $GPRMC	(RxBuffer[(RxCounter-68)%128] == '$' && RxBuffer[(RxCounter-67)%128] == 'G' && RxBuffer[(RxCounter-66)%128] == 'P' && RxBuffer[(RxCounter-65)%128] == 'R')

#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "GPS.h"

GPS_t GPS;
unsigned char RxBuffer[128];
unsigned char RxCounter = 0;

void GPS_init(void){

	USART_InitTypeDef USART_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Habilitamos los CLK para cada periférico */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	/* Iniciamos los pines de transmiscion y recepción */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	/* Colocamos los pines en su función alternativa*/
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

	/* Configuramos la UART que se utilizará */
	USART_InitStruct.USART_BaudRate = 9600;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2, &USART_InitStruct);

	/* Habilitamos la interrupcion del serial para la recepción*/
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	/* Configuración de la interrupción */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 10;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 10;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Habilitamos el módulo de la comunicación Serial */
	USART_Cmd(USART2, ENABLE);
}

void USART2_IRQHandler(void){
	if(USART_GetITStatus(USART2, USART_IT_RXNE)){
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		RxBuffer[RxCounter++%128] = USART2->DR;
		if($GPRMC){
			/* Obten las horas */
			GPS.Time.Hours = RxBuffer[(RxCounter-61)%128]-0x30;
			GPS.Time.Hours = 10*GPS.Time.Hours + RxBuffer[(RxCounter-60)%128]-0x30;

			/* Obten los minutos */
			GPS.Time.Minutes =  RxBuffer[(RxCounter-59)%128]-0x30;
			GPS.Time.Minutes = 10*GPS.Time.Minutes + RxBuffer[(RxCounter-58)%128]-0x30;

			/* Obten los segundos */
			GPS.Time.Seconds =  RxBuffer[(RxCounter-57)%128]-0x30;
			GPS.Time.Seconds = 10*GPS.Time.Seconds + RxBuffer[(RxCounter-56)%128]-0x30;

			/* Latidud */
			GPS.Latitude.Degrees = RxBuffer[(RxCounter-48)%128]-0x30;
			GPS.Latitude.Degrees = 10*GPS.Latitude.Degrees + RxBuffer[(RxCounter-47)%128]-0x30;
			GPS.Latitude.Time.Hours = RxBuffer[(RxCounter-46)%128]-0x30;
			GPS.Latitude.Time.Hours = 10*GPS.Latitude.Time.Hours + RxBuffer[(RxCounter-45)%128]-0x30;
			GPS.Latitude.Time.Minutes = RxBuffer[(RxCounter-43)%128]-0x30;
			GPS.Latitude.Time.Minutes = 10*GPS.Latitude.Time.Minutes + RxBuffer[(RxCounter-42)%128]-0x30;
			GPS.Latitude.Time.Seconds = RxBuffer[(RxCounter-41)%128]-0x30;
			GPS.Latitude.Time.Seconds = 10*GPS.Latitude.Time.Seconds + RxBuffer[(RxCounter-40)%128]-0x30;
			GPS.Latitude.Coordinates = (RxBuffer[(RxCounter-38)%128] == 'N') ? NORTH : SOUTH;

			/* Longitud */
			GPS.Longitude.Degrees = RxBuffer[(RxCounter-36)%128]-0x30;
			GPS.Longitude.Degrees = 10*GPS.Longitude.Degrees + RxBuffer[(RxCounter-35)%128]-0x30;
			GPS.Longitude.Degrees = 10*GPS.Longitude.Degrees + RxBuffer[(RxCounter-34)%128]-0x30;
			GPS.Longitude.Time.Hours = RxBuffer[(RxCounter-33)%128]-0x30;
			GPS.Longitude.Time.Hours = 10*GPS.Longitude.Time.Hours + RxBuffer[(RxCounter-32)%128]-0x30;
			GPS.Longitude.Time.Minutes = RxBuffer[(RxCounter-30)%128]-0x30;
			GPS.Longitude.Time.Minutes = 10*GPS.Longitude.Time.Minutes + RxBuffer[(RxCounter-29)%128]-0x30;
			GPS.Longitude.Time.Seconds = RxBuffer[(RxCounter-28)%128]-0x30;
			GPS.Longitude.Time.Seconds = 10*GPS.Longitude.Time.Seconds + RxBuffer[(RxCounter-27)%128]-0x30;
			GPS.Longitude.Coordinates = (RxBuffer[(RxCounter-25)%128] == 'W') ? WEST : EAST;

			/* Obtenemos el dia */
			GPS.Date.Day =  RxBuffer[(RxCounter-11)%128]-0x30;
			GPS.Date.Day =  10*GPS.Date.Day + RxBuffer[(RxCounter-10)%128]-0x30;
			GPS.Date.Month =  RxBuffer[(RxCounter-9)%128]-0x30;
			GPS.Date.Month =  10*GPS.Date.Month + RxBuffer[(RxCounter-8)%128]-0x30;
			GPS.Date.Year =  RxBuffer[(RxCounter-7)%128]-0x30;
			GPS.Date.Year =  10*GPS.Date.Year + RxBuffer[(RxCounter-6)%128]-0x30;
			GPS.Date.Year += 2000;

			(void)USART2->DR;

		}
	}
}



/* Ejemplo de la trama
  eg3. $GPRMC,220516,A,5133.82,N,00042.24,W,173.8,231.8,130694,004.2,W*70
              1    2    3    4    5     6    7    8      9     10  11 12
      1   220516     Time Stamp
      2   A          validity - A-ok, V-invalid
      3   5133.82    current Latitude
      4   N          North/South
      5   00042.24   current Longitude
      6   W          East/West
      7   173.8      Speed in knots
      8   231.8      True course
      9   130694     Date Stamp
      10  004.2      Variation
      11  W          East/West
      12  *70        checksum
 * */
