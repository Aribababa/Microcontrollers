#include <stm32f4xx.h>
#include <stm32f4xx_i2c.h>
#include "Delay.h"
#include "I2C.h"
#include <I2C_AT24C32.h>

/*
 * Inicializa el temporizador para permitir la correcta escritura en la memoria EEPROM
 * 		Requisitos previos->	I2C__Init();
 * 		Entrada->				Ninguno
 * 		Salida->				Ninguno
*/
void I2C__AT24C32_Init(void)
{
	Delay__Init();
}

/*
 * Escribir un byte en la direccion de memoria interna asignada
 * 		Requisitos previos->	I2C__AT24C32_Init();
 * 		Entrada->				address:				direccion del dispositivo I2C a comunicarse
 * 								addressEEPROM:			direccion interna de la memoria EEPROM en la cual se escribira el dato
 *								data:					dato a ser escrito
 * 		Salida->				Ninguno
*/
void I2C__AT24C32_WriteByte(uint8_t address, uint16_t addressEEPROM, uint8_t data)
{
	if(addressEEPROM > AT24C32_LAST_RAM_ADDRESS)
		addressEEPROM = AT24C32_LAST_RAM_ADDRESS;
	I2C__Start(address, I2C_Direction_Transmitter);
	I2C__Write((uint8_t)addressEEPROM >> 8);
	I2C__Write((uint8_t)addressEEPROM);
	I2C__Write(data);
	I2C__Stop();
	Delay__MS(10); //Tiempo que lleva escribir en la memoria EEPROM
}

/*
 * Escribir una seccion de datos a partir de una direccion de memoria interna asignada
 * 		Requisitos previos->	I2C__AT24C32_Init();
 * 		Entrada->				address:				direccion del dispositivo I2C a comunicarse
 * 								addressEEPROM:			direccion interna de la memoria EEPROM en la cual se escribira el primer dato
 *								*data:					direccion del primer dato a ser escrito
 * 		Salida->				Ninguno
*/
void I2C__AT24C32_WritePage(uint8_t address, uint16_t addressEEPROM, uint8_t bytesToWrite, uint8_t *data)
{
	uint8_t contador = 0;
	if(bytesToWrite == 0)
		bytesToWrite = 1;
	if(addressEEPROM > AT24C32_LAST_RAM_ADDRESS)
		addressEEPROM = AT24C32_LAST_RAM_ADDRESS;
	if(addressEEPROM + (bytesToWrite - 1) > AT24C32_LAST_RAM_ADDRESS)
		addressEEPROM = AT24C32_LAST_RAM_ADDRESS - (bytesToWrite - 1);

	I2C__Start(address, I2C_Direction_Transmitter);
	I2C__Write((uint8_t)addressEEPROM >> 8);
	I2C__Write((uint8_t)addressEEPROM);
	do
		I2C__Write(data[contador]);
	while(--bytesToWrite);
	I2C__Stop();
	Delay__MS(10); //Tiempo que lleva escribir en la memoria EEPROM
}

/*
 * Leer un dato proveniente de una direccion interna de la memoria EEPROM
 * 		Requisitos previos->	Ninguno
 * 		Entrada->				address:			direccion del dispositivo I2C a comunicarse
 * 								addressEEPROM:		direccion interna de la memoria EEPROM que sera leida
 * 		Salida->				Dato almacenado de la direccion proporcionada
*/
uint8_t I2C__AT24C32_ReadByte(uint8_t address, uint16_t addressEEPROM)
{
	if(addressEEPROM > AT24C32_LAST_RAM_ADDRESS)
		addressEEPROM = AT24C32_LAST_RAM_ADDRESS;
	I2C__Start(address, I2C_Direction_Transmitter);
	I2C__Write((uint8_t)addressEEPROM >> 8);
	I2C__Write((uint8_t)addressEEPROM);
	I2C__Restart(address, I2C_Direction_Receiver);
	return I2C__Read_NACK();
}

/*
 * Leer una secuencia de datos provenientes a partir de una direccion interna de la memoria EEPROM
 * 		Requisitos previos->	Ninguno
 * 		Entrada->				address:			direccion del dispositivo I2C a comunicarse
 * 								addressEEPROM:		direccion interna de la memoria EEPROM desde la cual se leeran datos
 * 								*data:				direccion del primer espacio del buffer que almacenara los datos leidos
 * 		Salida->				Ninguno
*/
void I2C__AT24C32_PageRead(uint8_t address, uint16_t addressEEPROM, uint8_t bytesToRead, uint8_t *data)
{
	uint8_t contador = 0;
	if(bytesToRead == 0)
		bytesToRead = 1;
	if(addressEEPROM > AT24C32_LAST_RAM_ADDRESS)
		addressEEPROM = AT24C32_LAST_RAM_ADDRESS;
	if(addressEEPROM + (bytesToRead - 1) > AT24C32_LAST_RAM_ADDRESS)
		addressEEPROM = AT24C32_LAST_RAM_ADDRESS - (bytesToRead - 1);

	I2C__Start(address, I2C_Direction_Transmitter);
	I2C__Write((uint8_t)addressEEPROM >> 8);
	I2C__Write((uint8_t)addressEEPROM);
	I2C__Restart(address, I2C_Direction_Receiver);
	while(contador < bytesToRead - 1)
	{
		data[contador] = I2C__Read_ACK();
		contador++;
	}
	data[contador] = I2C__Read_NACK();
}
