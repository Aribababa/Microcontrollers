#define $GPRMC	(RxBuffer[(RxCounter-5)%32] == '$' && RxBuffer[(RxCounter-4)%32] == 'G' && RxBuffer[(RxCounter-3)%32] == 'P' && RxBuffer[(RxCounter-2)%32] == 'R' && RxBuffer[(RxCounter-1)%32] == 'M')
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "GPS.h"


#define GPS_TIME_FRAME 			7
#define GPS_LATITUDE_FRAME 		26
#define GPS_LONGITUDE_FRAME 	39
#define GPS_DATE_FRAME	 		58

GPS_t GPS;
unsigned char RxBuffer[32];
unsigned char RxCounter = 0;
static unsigned char GPS_Frame = 0;
static unsigned char busy = 0;


/**/
static void GPS_ParseTime(void);
static void GPS_ParseLatitude(void);
static void GPS_ParseLongitude(void);
static void GPS_ParseDate(void);


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
		RxBuffer[RxCounter++%32] = USART2->DR;
		if($GPRMC){
			GPS_Frame = 0; /* Comenzamos el procesamiento */
			busy = 1;
		}

		if(busy){
			if(GPS_Frame == GPS_TIME_FRAME ){	/* Procesamos la hora */
				GPS_ParseTime();

			} else if(GPS_Frame == GPS_LATITUDE_FRAME){	/* Parse de la latitud */
				GPS_ParseLatitude();

			} else if(GPS_Frame == GPS_LONGITUDE_FRAME){	/* Parse de la longitud */
				GPS_ParseLongitude();

			} else if(GPS_Frame == GPS_DATE_FRAME){	/* Parse de la fecha */
				GPS_ParseDate();
				busy = 0;
			}
		}
		GPS_Frame++;
	}
}

/* Funciones para hacer el Parse de la trama de datos */
static void GPS_ParseTime(void){
	GPS.Time.Hours = RxBuffer[(RxCounter-5)%32]-0x30;
	GPS.Time.Hours = 10*GPS.Time.Hours + RxBuffer[(RxCounter-4)%32]-0x30;
	GPS.Time.Minutes =  RxBuffer[(RxCounter-3)%32]-0x30;
	GPS.Time.Minutes = 10*GPS.Time.Minutes + RxBuffer[(RxCounter-2)%32]-0x30;
	GPS.Time.Seconds =  RxBuffer[(RxCounter-1)%32]-0x30;
	GPS.Time.Seconds = 10*GPS.Time.Seconds + RxBuffer[(RxCounter)%32]-0x30;
}

static void GPS_ParseLatitude(void){
	GPS.Latitude.Degrees = RxBuffer[(RxCounter-11)%32]-0x30;
	GPS.Latitude.Degrees = 10*GPS.Latitude.Degrees + RxBuffer[(RxCounter-10)%32]-0x30;
	GPS.Latitude.Hours = RxBuffer[(RxCounter-9)%32]-0x30;
	GPS.Latitude.Hours = 10*GPS.Latitude.Hours + RxBuffer[(RxCounter-8)%32]-0x30;
	GPS.Latitude.Minutes = RxBuffer[(RxCounter-6)%32]-0x30;
	GPS.Latitude.Minutes = 10*GPS.Latitude.Minutes + RxBuffer[(RxCounter-5)%32]-0x30;
	GPS.Latitude.Seconds = RxBuffer[(RxCounter-4)%32]-0x30;
	GPS.Latitude.Seconds = 10*GPS.Latitude.Seconds + RxBuffer[(RxCounter-3)%32]-0x30;
	GPS.Latitude.Coordinates = (RxBuffer[(RxCounter-1)%32] == 'N') ? NORTH : SOUTH;
}

static void GPS_ParseLongitude(void){
	GPS.Longitude.Degrees = RxBuffer[(RxCounter-12)%32]-0x30;
	GPS.Longitude.Degrees = 10*GPS.Longitude.Degrees + RxBuffer[(RxCounter-11)%32]-0x30;
	GPS.Longitude.Degrees = 10*GPS.Longitude.Degrees + RxBuffer[(RxCounter-10)%32]-0x30;
	GPS.Longitude.Hours = RxBuffer[(RxCounter-9)%32]-0x30;
	GPS.Longitude.Hours = 10*GPS.Longitude.Hours + RxBuffer[(RxCounter-8)%32]-0x30;
	GPS.Longitude.Minutes = RxBuffer[(RxCounter-6)%32]-0x30;
	GPS.Longitude.Minutes = 10*GPS.Longitude.Minutes + RxBuffer[(RxCounter-5)%32]-0x30;
	GPS.Longitude.Seconds = RxBuffer[(RxCounter-4)%32]-0x30;
	GPS.Longitude.Seconds = 10*GPS.Longitude.Seconds + RxBuffer[(RxCounter-3)%32]-0x30;
	GPS.Longitude.Coordinates = (RxBuffer[(RxCounter-1)%32] == 'W') ? WEST : EAST;
}

static void GPS_ParseDate(void){
	GPS.Date.Day =  RxBuffer[(RxCounter-6)%32]-0x30;
	GPS.Date.Day =  10*GPS.Date.Day + RxBuffer[(RxCounter-5)%128]-0x30;
	GPS.Date.Month =  RxBuffer[(RxCounter-4)%32]-0x30;
	GPS.Date.Month =  10*GPS.Date.Month + RxBuffer[(RxCounter-3)%32]-0x30;
	GPS.Date.Year =  RxBuffer[(RxCounter-2)%32]-0x30;
	GPS.Date.Year =  10*GPS.Date.Year + RxBuffer[(RxCounter-1)%32]-0x30;
	GPS.Date.Year += 2000;
}





