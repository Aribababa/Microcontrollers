#include <stm32f4xx.h>

uint32_t multiplier = 0;

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

void Delay__MS(uint32_t millis)
{
    millis = 1000 * millis * multiplier - 10;
    while (millis--);
}
