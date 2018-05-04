#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

#define TIM3_FREQ	84000000
#define Software_UART_Baudrate_2400		35000
#define Software_UART_Baudrate_4800		17500
#define Software_UART_Baudrate_9600		8745
#define Software_UART_Baudrate_19200 	4375
#define Software_UART_Baudrate_115200	712

extern volatile unsigned char Software_UART_Tx_THRE;
extern volatile unsigned char Software_UART_Data_Ready;
extern volatile unsigned char RSR;
extern volatile unsigned char RxBuffer[64];
extern volatile unsigned char RxBuffer_index;

void SoftwareUART_Init(uint16_t baudrate);

void SoftwareUART_Send(unsigned char data);
