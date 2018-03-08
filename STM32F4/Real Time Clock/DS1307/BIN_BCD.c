#include <stm32f4xx.h>

//***********************************************************************************************
uint8_t BIN2BCD(uint8_t binaryValue)
{
	return ((binaryValue / 10) << 4) + binaryValue % 10;
}

//***********************************************************************************************
uint8_t BCD2BIN(uint8_t bcdValue)
{
	return (bcdValue & 15) + (bcdValue >> 4) * 10;
}
