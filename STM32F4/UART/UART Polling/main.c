#include "stm32f4xx.h"
#include "UART.h"

int main(void)
{
	UART__Init();

	UART__WriteFloatingNumber(-12345.987, 3);

	while(1);
}
