#include <stm32f4xx.h>

/*
 * Realiza la conversion de un numero binario a BCD
 * 		Requisitos previos->	Ninguno
 * 		Entrada->				Numero binario a ser convertido
 * 		Salida->				Numero BCD resultado de la conversion
*/
uint8_t BIN2BCD(uint8_t binaryValue)
{
	return ((binaryValue / 10) << 4) + binaryValue % 10;
}

/*
 * Realiza la conversion de un numero binario a BCD
 * 		Requisitos previos->	Ninguno
 * 		Entrada->				Numero BCD a ser convertido
 * 		Salida->				Numero binario resultado de la conversion
*/
uint8_t BCD2BIN(uint8_t bcdValue)
{
	return (bcdValue & 15) + (bcdValue >> 4) * 10;
}
