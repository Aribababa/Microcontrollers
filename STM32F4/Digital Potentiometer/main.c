#include "stm32f4xx.h"
#include "Delay.h"
#include "SPI.h"
#include "SPI_MCP4131.h"

int main(void)
{
	Delay__Init();
	uint8_t resistencia = 0;

	SPI__Init();
	SPI__MCP4131_Init(100000);
	SPI__MCP4131_SetResistance(60);

	while(1);
}
