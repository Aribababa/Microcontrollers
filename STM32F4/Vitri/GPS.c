#define $GPRMC	(RxBuffer[(RxCounter-5)%32] == '$' && RxBuffer[(RxCounter-4)%32] == 'G' && RxBuffer[(RxCounter-3)%32] == 'P' && RxBuffer[(RxCounter-2)%32] == 'R' && RxBuffer[(RxCounter-1)%32] == 'M')
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "GPS.h"

#define GPS_TIME_FRAME 			7
#define GPS_LATITUDE_FRAME 		26
#define GPS_LONGITUDE_FRAME 	39
#define GPS_DATE_FRAME	 		58

static signed char Time_Coordination;
static unsigned char RxBuffer[32];
static unsigned char RxCounter = 0;
static unsigned char busy = 0;
static GPS_t* GPS_direction;

/* Funciones para Parse del GPS */
static void GPS_ParseTime(GPS_t* GPS);
static void GPS_ParseLatitude(GPS_t* GPS);
static void GPS_ParseLongitude(GPS_t* GPS);
static void GPS_ParseDate(GPS_t* GPS);

void GPS_init(GPS_t* GPS, signed char Time_Difference){

	USART_InitTypeDef USART_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Habilitamos los CLK para cada periférico */
	RCC->AHB1ENR |= RCC_AHB1Periph_GPIOA;
	RCC->APB1ENR |= RCC_APB1Periph_USART2;

	/* Guardamos el apuntador de la dirección de la estructura donde se guardarán los datos */
	GPS_direction = GPS;
	Time_Coordination = Time_Difference;

	/* Iniciamos los pines de transmiscion y recepción */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	/* Colocamos los pines en su función alternativa*/
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2); 		/* Tx */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);		/* Rx */

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

/* Deshabilitaos la interrupcion desde NVIC */
void GPS_Stop(void){
	NVIC_DisableIRQ(USART2_IRQn);
}

/* Habilitamos la interrupcion desde NVIC */
void GPS_Start(void){
	NVIC_EnableIRQ(USART2_IRQn);
}

void USART2_IRQHandler(void){
	if(USART_GetITStatus(USART2, USART_IT_RXNE)){
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);	/* Apagamos la flag de la iterrupción */
		RxBuffer[RxCounter++%32] = USART2->DR;		/* Buffer circular para los datos de entrada */
		if($GPRMC){
			RxCounter = 0;	/* Iniciamos en cero para conocer la posicion del dato que queremos */
			busy = 1;		/* Indica si se enta procesando un frame de NMEA */
		}

		if(busy){
			if(RxCounter == GPS_TIME_FRAME ){				/* Procesamos la hora */
				GPS_ParseTime(GPS_direction);

			} else if(RxCounter == GPS_LATITUDE_FRAME){		/* Parse de la latitud */
				GPS_ParseLatitude(GPS_direction);

			} else if(RxCounter == GPS_LONGITUDE_FRAME){	/* Parse de la longitud */
				GPS_ParseLongitude(GPS_direction);

			} else if(RxCounter == GPS_DATE_FRAME){			/* Parse de la fecha */
				GPS_ParseDate(GPS_direction);
				busy = 0;
			}
		}
	}
}

/* Funciones para hacer el Parse de la trama de datos */
static void GPS_ParseTime(GPS_t* GPS){
	GPS->Time.Hours = RxBuffer[2]-0x30;
	GPS->Time.Hours = ((10*GPS->Time.Hours + RxBuffer[3]-0x30) + Time_Coordination)%24;
	GPS->Time.Minutes =  RxBuffer[4]-0x30;
	GPS->Time.Minutes = 10*GPS->Time.Minutes + RxBuffer[5]-0x30;
	GPS->Time.Seconds =  RxBuffer[6]-0x30;
	GPS->Time.Seconds = 10*GPS->Time.Seconds + RxBuffer[7]-0x30;
}

static void GPS_ParseLatitude(GPS_t* GPS){
	GPS->Latitude.Degrees = RxBuffer[15]-0x30;
	GPS->Latitude.Degrees = 10*GPS->Latitude.Degrees + RxBuffer[16]-0x30;
	GPS->Latitude.Hours = RxBuffer[17]-0x30;
	GPS->Latitude.Hours = 10*GPS->Latitude.Hours + RxBuffer[18]-0x30;
	GPS->Latitude.Minutes = RxBuffer[20]-0x30;
	GPS->Latitude.Minutes = 10*GPS->Latitude.Minutes + RxBuffer[21]-0x30;
	GPS->Latitude.Seconds = RxBuffer[22]-0x30;
	GPS->Latitude.Seconds = 10*GPS->Latitude.Seconds + RxBuffer[23]-0x30;
	GPS->Latitude.Coordinates = (RxBuffer[25] == 'N') ? NORTH : SOUTH;
}

static void GPS_ParseLongitude(GPS_t* GPS){
	GPS->Longitude.Degrees = RxBuffer[27]-0x30;
	GPS->Longitude.Degrees = 10*GPS->Longitude.Degrees + RxBuffer[28]-0x30;
	GPS->Longitude.Degrees = 10*GPS->Longitude.Degrees + RxBuffer[29]-0x30;
	GPS->Longitude.Hours = RxBuffer[30]-0x30;
	GPS->Longitude.Hours = 10*GPS->Longitude.Hours + RxBuffer[31]-0x30;
	GPS->Longitude.Minutes = RxBuffer[1]-0x30;
	GPS->Longitude.Minutes = 10*GPS->Longitude.Minutes + RxBuffer[2]-0x30;
	GPS->Longitude.Seconds = RxBuffer[3]-0x30;
	GPS->Longitude.Seconds = 10*GPS->Longitude.Seconds + RxBuffer[4]-0x30;
	GPS->Longitude.Coordinates = (RxBuffer[6] == 'W') ? WEST : EAST;
}

static void GPS_ParseDate(GPS_t* GPS){
	GPS->Date.Day =  RxBuffer[20]- 0x30;
	GPS->Date.Day =  10*GPS->Date.Day + RxBuffer[21]-0x30;
	GPS->Date.Month =  RxBuffer[22]-0x30;
	GPS->Date.Month =  10*GPS->Date.Month + RxBuffer[23]-0x30;
	GPS->Date.Year =  RxBuffer[24]-0x30;
	GPS->Date.Year =  10*GPS->Date.Year + RxBuffer[25]-0x30;
	GPS->Date.Year += 2000;
}

