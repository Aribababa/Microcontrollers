#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "AC_Motor.h"

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
volatile unsigned char RxBuffer[64];
volatile unsigned char RxBuffer_index = 0;


void TIM_Init()
{
	// Habilitamos TIM3
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	// Inicializamos timer
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;

	// Creamos un PWM a 1kHz
	// Frecuencia del TIM4 de 84MHz a 8.4MHz gracias al prescaler en 10
	TIM_TimeBaseInitStruct.TIM_Prescaler = 1;

	// TIM_Period determina la frecuencia del PWM con esta ecuación:
	// PWM_frequency = timer_clock / (TIM_Period + 1)
	// Si queremos 5kHz PWM we can calculate:
	// TIM_Period = (timer_clock / PWM_frequency) - 1
	// TIM_Period = (8.4MHz / 1kHz) - 1 = 8399
	TIM_TimeBaseInitStruct.TIM_Period = 8399;			// PERIODO
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;

	// Inicializamos TIM3
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);

	// TIM3 START
	TIM_Cmd(TIM3, ENABLE);
}

void PWM_Init()
{
	// Struct para inicializar PWM
	TIM_OCInitTypeDef TIM_OCInitStruct;

	// Ajustes comunes para PWM
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;

	// Ecuaciones para calculos de duty cycle:
	// TIM_Pulse = (((TIM_Period + 1) * duty_cycle) / 100) - 1
	// Ex. 25% duty cycle:
	// TIM_Pulse = (((8399 + 1) * 25) / 100) - 1 = 2099
	// TIM_Pulse = (((8399 + 1) * 75) / 100) - 1 = 6299
	TIM_OCInitStruct.TIM_Pulse = 6299;
	TIM_OC1Init(TIM3, &TIM_OCInitStruct);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
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

	/* Para la recepción de datos */
	if (TIM_GetITStatus (TIM3, TIM_IT_CC2) != RESET) {

		/*Apagamos la flag de la interrupción */
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);

		/* Verificamos qque es lo que se lee en la entrada (Rx)*/
		if(Software_UART_Rx_Interruption < 8){
			RSR >>= 1;
			if((GPIOB->IDR & GPIO_Pin_3) != 0){
				RSR |= 0x80;
			}
			++Software_UART_Rx_Interruption;
			TIM3->CCR2 = TIM3->CCR2 + Software_UART_Baudrate;
		} else{
			/* Guardamos el dato recibido en el buffer de recepción */
			RxBuffer[RxBuffer_index++%64] = RSR;
			Software_UART_Rx_Interruption = 0;

			/* Deshabilitamos el muestreo de la entrada */
			Software_UART_Data_Ready = 1;
			TIM_ITConfig(TIM3, TIM_IT_CC2, DISABLE);

			/* Habilitamos la interrupción para el start bit */
			EXTI_ClearITPendingBit(EXTI_Line3);
			NVIC_EnableIRQ(EXTI3_IRQn);
		}
	}
}

void EXTI3_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line3) != RESET){
		EXTI_ClearITPendingBit(EXTI_Line3);

		/* Deshabilitamos la interrupción */
		NVIC_DisableIRQ(EXTI3_IRQn);
		Software_UART_Data_Ready = 0;

		/* Configuramos para que el canal 02 del OC comienze a interrumpir */
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
		TIM3->CCR2 = TIM3->CNT + Software_UART_Time_Bit_Sampling;
		TIM_ITConfig(TIM3,TIM_IT_CC2, ENABLE);
	}
}

