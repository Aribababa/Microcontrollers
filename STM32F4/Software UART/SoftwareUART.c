#include "SoftwareUART.h"

/* Para la transmisión */
volatile unsigned char THR;
volatile unsigned char Software_UART_Tx_Interruption;
volatile unsigned char Software_UART_Tx_THRE;
volatile uint16_t Software_UART_Baudrate;

/* Para la recepción */
volatile uint16_t Software_UART_Time_Bit_Sampling;
volatile unsigned char Software_UART_Rx_Interruption;
volatile unsigned char Software_UART_Data_Ready;
volatile unsigned char RSR;


void SoftwareUART_Init(uint16_t baudrate){
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStruct;
	RCC_ClocksTypeDef RCC_Clocks;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	RCC_GetClocksFreq(&RCC_Clocks);

	Software_UART_Baudrate = baudrate;
	Software_UART_Time_Bit_Sampling = 3*(baudrate/2);

	/* Colocamos la linea de transmision en estado IDLE */
	GPIOD->BSRRL = GPIO_Pin_7;

	/* Inicializamos los GPIO´s para la transmisión (Tx) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* Inicializamos los GPIO´s para la recepción (Rx) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* Indicamos cual pin es el que detectará el start bit*/
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource6);

	/* indicamos como se activará la interrupción */
	EXTI_InitStruct.EXTI_Line = EXTI_Line6;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStruct);

	/* Iniciamos el Timer para la refencia de tiempo */
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_Period = 65535;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	/* Configuración del Output Compare */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Active;

	/* Configurammos el Canal 01 del output compare (Tx) */
	TIM_OCInitStructure.TIM_Pulse = Software_UART_Baudrate;
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);

	/* Configurammos el Canal 02 del output compare (Rx) */
	TIM_OCInitStructure.TIM_Pulse = Software_UART_Baudrate;
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);

	/* Habilitamos la interrupción del timer */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 10;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 10;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Habilitamos la interrupcion para el start bit */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 9;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 9;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Habilitamos las interrupciones del Timer */
	TIM_ClearITPendingBit(TIM3, TIM_IT_CC1 | TIM_IT_CC2);
	TIM_ITConfig(TIM3, TIM_IT_CC1 | TIM_IT_CC2, DISABLE);

	/* Habilitamos para que comienze el envio de datos */
	Software_UART_Tx_THRE = 1;
	Software_UART_Data_Ready = 0;

	/* Activamos el timer y comienza a correr */
	TIM_Cmd(TIM3, ENABLE);
}

void SoftwareUART_Send(unsigned char data){
	TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);	/* Para asegurar que este limpia la flag del timer */
	TIM3->CCR1 = TIM3->CCR1 + Software_UART_Baudrate; 	/* Cambiamos el valor a comparar */

	Software_UART_Tx_Interruption = 10;	/* Bits a transmitir */
	GPIOD->BSRRH = GPIO_Pin_7;	/* Generamos el Start Bit */
	THR = data;
	Software_UART_Tx_THRE = 0;
	TIM_ITConfig (TIM3, TIM_IT_CC1, ENABLE);
}

void TIM3_IRQHandler (void){
	/* Para la transmisión de datos */
	if (TIM_GetITStatus (TIM3, TIM_IT_CC1) != RESET) {

		/*Apagamos la flag de la interrupción */
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);

		/* Restamos el valor del contador de la interrupción */
		--Software_UART_Tx_Interruption;

		/* Cambiamos la salida dependiendo del bit a transmitir */
		if(Software_UART_Tx_Interruption < 10 && Software_UART_Tx_Interruption > 1){
			if(THR & 1) {
				GPIOD->BSRRL = GPIO_Pin_7;
			} else {
				GPIOD->BSRRH = GPIO_Pin_7;
			}
			TIM3->CCR1 = TIM3->CCR1 + Software_UART_Baudrate;
			THR>>=1;
		}

		if(Software_UART_Tx_Interruption == 1){
			GPIOD->BSRRL = GPIO_Pin_7;	/* Stop Bit */
			TIM3->CCR1 = TIM3->CCR1 + Software_UART_Baudrate;
		}

		if(Software_UART_Tx_Interruption == 0){
			/* Deshabilitamos la interrupción */
			TIM_ITConfig(TIM3, TIM_IT_CC1, DISABLE);
			Software_UART_Tx_THRE = 1;
		}
	}

	if (TIM_GetITStatus (TIM3, TIM_IT_CC2) != RESET) {

		/*Apagamos la flag de la interrupción */
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);

		if(Software_UART_Rx_Interruption < 8){
			RSR>>=1;
			if(GPIO_ReadInputDataBit(GPIOD, GPIO_PinSource6) == 1){
				RSR |= 0x80;
			} else {
				RSR &= 0x7F;
			}
			Software_UART_Rx_Interruption++;
			TIM3->CCR2 = TIM3->CCR2 + Software_UART_Baudrate;
		} else {
			Software_UART_Data_Ready = 1;
			/* Volvemos a habilitar la interrupción */
			EXTI_ClearITPendingBit(EXTI_Line6);
			NVIC_EnableIRQ(EXTI9_5_IRQn);
			TIM_ITConfig(TIM3,TIM_IT_CC2, DISABLE);
		}
	}
}

void EXTI9_5_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line6) != RESET) {
		EXTI_ClearITPendingBit(EXTI_Line6);

		/* Deshabilitamos la interrupción */
		NVIC_DisableIRQ(EXTI9_5_IRQn);
		Software_UART_Rx_Interruption = 0;
		Software_UART_Data_Ready = 0;

		/* Configuramos para que el canal 02 del OC comienze a interrumpir */
		TIM3->CCR2 = TIM3->CCR2 + Software_UART_Time_Bit_Sampling;
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
		TIM_ITConfig(TIM3,TIM_IT_CC2, ENABLE);
	}
}
