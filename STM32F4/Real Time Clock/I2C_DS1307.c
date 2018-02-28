#include <stm32f4xx.h>
#include <stm32f4xx_i2c.h>
#include "I2C.h"
#include "I2C_DS1307.h"

//***********************************************************************************************
void I2C__DS1307_SetSecond(uint8_t address, uint8_t data)
{
	I2C__DS1307_Set(address, data, SECOND_REGISTER, SECOND_MASK);
}

void I2C__DS1307_SetMinute(uint8_t address, uint8_t data)
{
	I2C__DS1307_Set(address, data, MINUTE_REGISTER, MINUTE_MASK);
}

void I2C__DS1307_SetHour(uint8_t address, uint8_t data)
{
	I2C__DS1307_Set(address, data, HOUR_REGISTER, HOUR_MASK);
}

void I2C__DS1307_SetDay(uint8_t address, uint8_t data)
{
	I2C__DS1307_Set(address, data, DAY_REGISTER, DAY_MASK);
}

void I2C__DS1307_SetDate(uint8_t address, uint8_t data)
{
	I2C__DS1307_Set(address, data, DATE_REGISTER, DATE_MASK);
}

void I2C__DS1307_SetMonth(uint8_t address, uint8_t data)
{
	I2C__DS1307_Set(address, data, MONTH_REGISTER, MONTH_MASK);
}

void I2C__DS1307_SetYear(uint8_t address, uint8_t data)
{
	I2C__DS1307_Set(address, data, YEAR_REGISTER, YEAR_MASK);
}

void I2C__DS1307_SetControl(uint8_t address, uint8_t data)
{
	I2C__DS1307_Set(address, data, CONTROL_REGISTER, CONTROL_MASK);
}

//***********************************************************************************************
uint8_t I2C__DS1307_GetSecond(uint8_t address)
{
	return I2C__DS1307_Get(address, SECOND_REGISTER, SECOND_MASK);
}

uint8_t I2C__DS1307_GetMinute(uint8_t address)
{
	return I2C__DS1307_Get(address, MINUTE_REGISTER, MINUTE_MASK);
}

uint8_t I2C__DS1307_GetHour(uint8_t address)
{
	return I2C__DS1307_Get(address, HOUR_REGISTER, HOUR_MASK);
}

uint8_t I2C__DS1307_GetDay(uint8_t address)
{
	return I2C__DS1307_Get(address, DAY_REGISTER, DAY_MASK);
}

uint8_t I2C__DS1307_GetDate(uint8_t address)
{
	return I2C__DS1307_Get(address, DATE_REGISTER, DATE_MASK);
}

uint8_t I2C__DS1307_GetMonth(uint8_t address)
{
	return I2C__DS1307_Get(address, MONTH_REGISTER,MONTH_MASK);
}

uint8_t I2C__DS1307_GetYear(uint8_t address)
{
	return I2C__DS1307_Get(address, YEAR_REGISTER, YEAR_MASK);
}

uint8_t I2C__DS1307_Control(uint8_t address)
{
	return I2C__DS1307_Get(address, CONTROL_REGISTER, CONTROL_MASK);
}

//***********************************************************************************************
void I2C__DS1307_Init(uint8_t address)
{
	DS1307 newDS1307 = {48, 59, 23, LUNES, 31, DICIEMBRE, 18};
	I2C__DS1307_Write(address, &newDS1307);

	I2C__Start(address, I2C_Direction_Transmitter);
	I2C__Write(CONTROL_REGISTER);
	I2C__Write(0x00 & CONTROL_MASK);
	I2C__Stop();
}

//***********************************************************************************************
void I2C__DS1307_Write(uint8_t address, DS1307 *newDS1307)
{
	I2C__Start(address, I2C_Direction_Transmitter);
	I2C__Write(SECOND_REGISTER);
	I2C__Write(BIN2BCD(newDS1307->seconds) & SECOND_MASK);
	I2C__Write(BIN2BCD(newDS1307->minutes) & MINUTE_MASK);
	I2C__Write(BIN2BCD(newDS1307->hour) & HOUR_MASK);
	I2C__Write(BIN2BCD(newDS1307->day) & DAY_MASK);
	I2C__Write(BIN2BCD(newDS1307->date) & DATE_MASK);
	I2C__Write(BIN2BCD(newDS1307->month) & MONTH_MASK);
	I2C__Write(BIN2BCD(newDS1307->year) & YEAR_MASK);
	I2C__Stop();
}

//***********************************************************************************************
void I2C__DS1307_Read(uint8_t address, DS1307 *newDS1307)
{
	I2C__Start(address, I2C_Direction_Transmitter);
	I2C__Write(SECOND_REGISTER);
	I2C__Restart(address, I2C_Direction_Receiver);
	newDS1307->seconds = BCD2BIN(I2C__Read_ACK() & SECOND_MASK);
	newDS1307->minutes = BCD2BIN(I2C__Read_ACK() & MINUTE_MASK);
	newDS1307->hour = BCD2BIN(I2C__Read_ACK() & HOUR_MASK);
	newDS1307->day = BCD2BIN(I2C__Read_ACK() & DAY_MASK);
	newDS1307->date = BCD2BIN(I2C__Read_ACK() & DATE_MASK);
	newDS1307->month = BCD2BIN(I2C__Read_ACK() & MONTH_MASK);
	newDS1307->year = BCD2BIN(I2C__Read_NACK() & YEAR_MASK);
}

//***********************************************************************************************
//Memoria RAM va desde la direccion 0x00 logica (0x08 fisica) hasta la 0x37 logica (0x3F fisica)
void I2C__DS1307_WriteRAM(uint8_t address, uint8_t addressRAM, uint8_t bytesToWrite, uint8_t *data)
{
	uint8_t contador = 0;
	if(bytesToWrite == 0)
		bytesToWrite = 1;
	if(addressRAM > LAST_RAM_REGISTER - FIRST_RAM_REGISTER)
		addressRAM = LAST_RAM_REGISTER - FIRST_RAM_REGISTER;
	if(addressRAM + (bytesToWrite - 1) > LAST_RAM_REGISTER - FIRST_RAM_REGISTER)
		addressRAM = LAST_RAM_REGISTER - FIRST_RAM_REGISTER - (bytesToWrite - 1);

	I2C__Start(address, I2C_Direction_Transmitter);
	I2C__Write(addressRAM + FIRST_RAM_REGISTER);
	do
		I2C__Write(data[contador]);
	while(--bytesToWrite);
	I2C__Stop();
}

//***********************************************************************************************
//Memoria RAM va desde la direccion 0x00 logica (0x08 fisica) hasta la 0x37 logica (0x3F fisica)
void I2C__DS1307_ReadRAM(uint8_t address, uint8_t addressRAM, uint8_t bytesToRead, uint8_t *data)
{
	uint8_t contador = 0;
	if(bytesToRead == 0)
		bytesToRead = 1;
	if(addressRAM > LAST_RAM_REGISTER - FIRST_RAM_REGISTER)
		addressRAM = LAST_RAM_REGISTER - FIRST_RAM_REGISTER;
	if(addressRAM + (bytesToRead - 1) > LAST_RAM_REGISTER - FIRST_RAM_REGISTER)
		addressRAM = LAST_RAM_REGISTER - FIRST_RAM_REGISTER - (bytesToRead - 1);

	I2C__Start(address, I2C_Direction_Transmitter);
	I2C__Write(addressRAM + FIRST_RAM_REGISTER);
	I2C__Restart(address, I2C_Direction_Receiver);
	while(contador < bytesToRead - 1)
	{
		data[contador] = I2C__Read_ACK();
		contador++;
	}
	data[contador] = I2C__Read_NACK();
}

//***********************************************************************************************
void I2C__DS1307_Set(uint8_t address, uint8_t data, uint8_t registerValue, uint8_t flagValue)
{
	I2C__Start(address, I2C_Direction_Transmitter);
	I2C__Write(registerValue);
	I2C__Write(BIN2BCD(data) & flagValue);
	I2C__Stop();
}

uint8_t I2C__DS1307_Get(uint8_t address, uint8_t registerValue, uint8_t flagValue)
{
	I2C__Start(address, I2C_Direction_Transmitter);
	I2C__Write(registerValue);
	I2C__Restart(address, I2C_Direction_Receiver);
	return BCD2BIN(I2C__Read_NACK() & flagValue);
}

//***********************************************************************************************
uint8_t BIN2BCD(uint8_t binaryValue)
{
	return ((binaryValue / 10) << 4) + binaryValue % 10;
}

uint8_t BCD2BIN(uint8_t bcdValue)
{
	return (bcdValue & 15) + (bcdValue >> 4) * 10;
}
