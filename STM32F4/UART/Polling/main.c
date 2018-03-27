#include "stm32f4xx.h"
#include "UART.h"

int main(void)
{
	uint8_t prueba[] = {"PRUEBA\n\r"};

	UART__Init();

	UART__WriteString(&prueba[0]);
	UART__WriteFloatingNumber(-12345.678, 4);
	UART__Write('\n'); UART__Write('\r');

	while(1)
	{
		UART__Write(UART__Read());
	}
}
