#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

void TIM_Init()
{
    // Habilitamos TIM4
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

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

    // Inicializamos TIM4
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);

    // TIM4 START
    TIM_Cmd(TIM4, ENABLE);
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
    TIM_OCInitStruct.TIM_Pulse = 2099;
    TIM_OC1Init(TIM4, &TIM_OCInitStruct);
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
}
