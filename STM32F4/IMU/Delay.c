#include <stm32f4xx.h>
#include <Delay.h>

uint32_t multiplier;

/*
 * Inicializa la variable que permite el conteo del tiempo
 * 		Requisitos previos->	Ninguno
 * 		Entrada->				Ninguno
 * 		Salida->				Ninguno
*/
void Delay__Init(void)
{
    RCC_ClocksTypeDef RCC_Clocks;
    RCC_GetClocksFreq(&RCC_Clocks);

    multiplier = RCC_Clocks.HCLK_Frequency / 4000000;
}

/*
 * Genera un retardo en el orden de microsegundos
 * 		Requisitos previos->	Delay__Init();
 * 		Entrada->				micros:		tiempo en us del delay
 * 		Salida->				Ninguno
*/
void Delay__US(uint32_t micros)
{
    micros = micros * multiplier - 10;
    while (micros--);
}

/*
 * Genera un retardo en el orden de milisegundos
 * 		Requisitos previos->	Delay__Init();
 * 		Entrada->				millis:		tiempo en ms del delay
 * 		Salida->				Ninguno
*/
void Delay__MS(uint32_t millis)
{
    millis = 1000 * millis * multiplier - 10;
    while (millis--);
}
