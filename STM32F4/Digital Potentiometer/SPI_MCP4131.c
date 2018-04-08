#include "stm32f4xx.h"
#include "SPI.h"
#include "SPI_MCP4131.h"

uint8_t scale;
uint32_t resistanceW;
uint32_t resistanceAB;
uint32_t resistanceAW_Max;

void SPI__MCP4131_Init(uint32_t resistancePinAtoPinB)
{
	resistancePinAtoPinB += 90;
	resistanceAB = resistancePinAtoPinB;
	resistanceW = MCP4131_ResistanceW_Typical;
	resistanceAW_Max = resistanceAB - MCP4131_ResistanceW_Typical;
	scale = 100;
}

void SPI__MCP4131_SetValues(uint8_t values)
{
	if(values == MCP4131_BY_RESISTANCE)
		scale = resistanceAW_Max; //Para valores de resistencia en ohms
	else if(values == MCP4131_BY_PERCENTAJE)
		scale = 100; //Para valores de la resistencia en porcentajes del valor maximo
	else
		scale = 100; //Para valores de la resistencia en porcentajes del valor maximo
}

uint32_t SPI__MCP4131_GetResistance(void)
{
	return MCP4131__Wiper2Ohms((uint32_t)(0x01FF & MCP4131__Read(MCP4131_ADDRESS_WIPER0)));
}

void SPI__MCP4131_SetResistance(uint32_t resistance)
{
	MCP4131__WiperPosition(MCP4131_ADDRESS_WIPER0, MCP4131__Resistance2Wiper(resistance));
}

//PROTECTED
uint32_t MCP4131__StepSize(void)
{
	return (resistanceAW_Max - resistanceW) / MCP4131_Resolution7Bits;
}

uint16_t MCP4131__Resistance2Wiper(uint32_t resistance)
{
	if(resistance <= 0)
		return 0;
	if(scale != resistanceAW_Max)
		resistance = resistance * resistanceAW_Max / scale;

	return (uint16_t)((resistance - resistanceW) / MCP4131__StepSize() ) + 0.5;
}

uint32_t MCP4131__Wiper2Ohms(uint16_t wiper)
{
	uint32_t ohms =  resistanceW + ( (uint32_t)wiper * MCP4131__StepSize() );

	if(scale != resistanceAW_Max)
		ohms = ohms * scale / resistanceAW_Max;

	return ohms;
}

void MCP4131__Write(uint8_t commandByte, uint8_t dataByte)
{
	SPI__SlaveEnable();
	SPI__Write(commandByte);
	SPI__Write(dataByte);
	SPI__SlaveDisable();
}

uint16_t MCP4131__Read(uint8_t commandByte)
{
	commandByte |= MCP4131_ADDRESS_RESISTANCE_VALUE;
	SPI__SlaveEnable();
	uint8_t byteHigh = SPI__WriteRead(commandByte);
	uint8_t byteLow  = SPI__WriteRead(0xFF);
	SPI__SlaveDisable();
	return byte2uint16(byteHigh, byteLow);
}

void MCP4131__WiperPosition(uint8_t potentiometerNumber, uint16_t wiper)
{
	uint8_t commandByte = potentiometerNumber;
	uint8_t dataByte = 0x00;

	// Calcular los 9 bits a ser eenviados
	if(wiper > 255)
		commandByte |= 0x01;
	else
		dataByte = (uint8_t)(wiper & 0x00FF);

	MCP4131__Write(commandByte, dataByte);
}

//GENERALIZADAS
uint16_t byte2uint16(uint8_t byteHigh, uint8_t byteLow)
{
	return (uint16_t)byteHigh<<8 | (uint16_t)byteLow;
}
