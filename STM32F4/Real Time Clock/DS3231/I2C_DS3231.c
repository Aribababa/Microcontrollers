#include <stm32f4xx.h>
#include <stm32f4xx_i2c.h>
#include <BIN_BCD.h>
#include <I2C.h>
#include <I2C_DS3231.h>

//***********************************************************************************************
void I2C__DS3231_Init(uint8_t address)
{
	DS3231 newDS3231 = {0, 0, 0, DS3231_MONDAY, 26, DS3231_FEBRUARY, 18}; //00:00:00 Lunes 26/02/2018

	//Escribir fecha default
	I2C__DS3231_Write(address, &newDS3231);

	//Escribir bits de control
	I2C__Start(address, I2C_Direction_Transmitter);
	I2C__Write(DS3231_CONTROL_REGISTER);
	I2C__Write(0x00 & DS3231_CONTROL_MASK);
	I2C__Write(0x00 & DS3231_CONTROL_STATUS_MASK);
	I2C__Stop();
}

//***********************************************************************************************
void I2C__DS3231_Write(uint8_t address, DS3231 *myDS3231)
{
	I2C__Start(address, I2C_Direction_Transmitter);
	I2C__Write(DS3231_SECOND_REGISTER);
	I2C__Write(BIN2BCD(myDS3231->seconds) & DS3231_SECOND_MASK);
	I2C__Write(BIN2BCD(myDS3231->minutes) & DS3231_MINUTE_MASK);
	I2C__Write(BIN2BCD(myDS3231->hour) & DS3231_HOUR_MASK);
	I2C__Write(BIN2BCD(myDS3231->day) & DS3231_DAY_MASK);
	I2C__Write(BIN2BCD(myDS3231->date) & DS3231_DATE_MASK);
	I2C__Write(BIN2BCD(myDS3231->month) & DS3231_MONTH_MASK);
	I2C__Write(BIN2BCD(myDS3231->year) & DS3231_YEAR_MASK);
	I2C__Stop();
}

//***********************************************************************************************
void I2C__DS3231_Read(uint8_t address, DS3231 *myDS3231)
{
	I2C__Start(address, I2C_Direction_Transmitter);
	I2C__Write(DS3231_SECOND_REGISTER);
	I2C__Restart(address, I2C_Direction_Receiver);
	myDS3231->seconds = BCD2BIN(I2C__Read_ACK() & DS3231_SECOND_MASK);
	myDS3231->minutes = BCD2BIN(I2C__Read_ACK() & DS3231_MINUTE_MASK);
	myDS3231->hour = BCD2BIN(I2C__Read_ACK() & DS3231_HOUR_MASK);
	myDS3231->day = BCD2BIN(I2C__Read_ACK() & DS3231_DAY_MASK);
	myDS3231->date = BCD2BIN(I2C__Read_ACK() & DS3231_DATE_MASK);
	myDS3231->month = BCD2BIN(I2C__Read_ACK() & DS3231_MONTH_MASK);
	myDS3231->year = BCD2BIN(I2C__Read_NACK() & DS3231_YEAR_MASK);
}

//***********************************************************************************************
void I2C__DS3231_SetSecond(uint8_t address, uint8_t data)
{
	I2C__DS3231_Set(address, data, DS3231_SECOND_REGISTER, DS3231_SECOND_MASK);
}

void I2C__DS3231_SetMinute(uint8_t address, uint8_t data)
{
	I2C__DS3231_Set(address, data, DS3231_MINUTE_REGISTER, DS3231_MINUTE_MASK);
}

void I2C__DS3231_SetHour(uint8_t address, uint8_t data)
{
	I2C__DS3231_Set(address, data, DS3231_HOUR_REGISTER, DS3231_HOUR_MASK);
}

void I2C__DS3231_SetDay(uint8_t address, uint8_t data)
{
	I2C__DS3231_Set(address, data, DS3231_DAY_REGISTER, DS3231_DAY_MASK);
}

void I2C__DS3231_SetDate(uint8_t address, uint8_t data)
{
	I2C__DS3231_Set(address, data, DS3231_DATE_REGISTER, DS3231_DATE_MASK);
}

void I2C__DS3231_SetMonth(uint8_t address, uint8_t data)
{
	I2C__DS3231_Set(address, data, DS3231_MONTH_REGISTER, DS3231_MONTH_MASK);
}

void I2C__DS3231_SetYear(uint8_t address, uint8_t data)
{
	I2C__DS3231_Set(address, data, DS3231_YEAR_REGISTER, DS3231_YEAR_MASK);
}

void I2C__DS3231_SetControl(uint8_t address, uint8_t data)
{
	I2C__DS3231_Set(address, data, DS3231_CONTROL_REGISTER, DS3231_CONTROL_MASK);
}

void I2C__DS3231_SetControlStatus(uint8_t address, uint8_t data)
{
	I2C__DS3231_Set(address, data, DS3231_CONTROL_STATUS_REGISTER, DS3231_CONTROL_STATUS_MASK);
}

//***********************************************************************************************
uint8_t I2C__DS3231_GetSecond(uint8_t address)
{
	return I2C__DS3231_Get(address, DS3231_SECOND_REGISTER, DS3231_SECOND_MASK);
}

uint8_t I2C_DS3231_GetMinute(uint8_t address)
{
	return I2C__DS3231_Get(address, DS3231_MINUTE_REGISTER, DS3231_MINUTE_MASK);
}

uint8_t I2C_DS3231_GetHour(uint8_t address)
{
	return I2C__DS3231_Get(address, DS3231_HOUR_REGISTER, DS3231_HOUR_MASK);
}

uint8_t I2C_DS3231_GetDay(uint8_t address)
{
	return I2C__DS3231_Get(address, DS3231_DAY_REGISTER, DS3231_DAY_MASK);
}

uint8_t I2C_DS3231_GetDate(uint8_t address)
{
	return I2C__DS3231_Get(address, DS3231_DATE_REGISTER, DS3231_DATE_MASK);
}

uint8_t I2C_DS3231_GetMonth(uint8_t address)
{
	return I2C__DS3231_Get(address, DS3231_MONTH_REGISTER, DS3231_MONTH_MASK);
}

uint8_t I2C_DS3231_GetYear(uint8_t address)
{
	return I2C__DS3231_Get(address, DS3231_YEAR_REGISTER, DS3231_YEAR_MASK);
}

uint8_t I2C_DS3231_Control(uint8_t address)
{
	return I2C__DS3231_Get(address, DS3231_CONTROL_REGISTER, DS3231_CONTROL_MASK);
}

uint8_t I2C_DS3231_ControlStatus(uint8_t address)
{
	return I2C__DS3231_Get(address, DS3231_CONTROL_STATUS_REGISTER, DS3231_CONTROL_STATUS_MASK);
}

//***********************************************************************************************
void I2C__DS3231_Set(uint8_t address, uint8_t data, uint8_t registerValue, uint8_t flagValue)
{
	I2C__Start(address, I2C_Direction_Transmitter);
	I2C__Write(registerValue);
	I2C__Write(BIN2BCD(data) & flagValue);
	I2C__Stop();
}

uint8_t I2C__DS3231_Get(uint8_t address, uint8_t registerValue, uint8_t flagValue)
{
	I2C__Start(address, I2C_Direction_Transmitter);
	I2C__Write(registerValue);
	I2C__Restart(address, I2C_Direction_Receiver);
	return BCD2BIN(I2C__Read_NACK() & flagValue);
}
