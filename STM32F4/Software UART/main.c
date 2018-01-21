#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "SoftwareUART.h"
volatile unsigned char arduino;


int main(void){


	SoftwareUART_Init(Software_UART_Baudrate_9600);

	for(;;){


	}
}
