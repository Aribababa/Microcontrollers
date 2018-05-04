#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

uint32_t multiplier;

void GPIOD_Initialize()
{
 GPIO_InitTypeDef GPIOD_Stepper;
 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

 GPIOD_Stepper.GPIO_Mode = GPIO_Mode_OUT;
 GPIOD_Stepper.GPIO_OType = GPIO_OType_PP;
 GPIOD_Stepper.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_7;
 GPIOD_Stepper.GPIO_PuPd = GPIO_PuPd_NOPULL;
 GPIOD_Stepper.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOD,&GPIOD_Stepper);
}

void Delay__Init(void)
{
    RCC_ClocksTypeDef RCC_Clocks;
    RCC_GetClocksFreq(&RCC_Clocks);

    multiplier = RCC_Clocks.HCLK_Frequency / 4000000;
}

void Delay__US(uint32_t micros)
{
    micros = micros * multiplier - 10;
    while (micros--);
}

