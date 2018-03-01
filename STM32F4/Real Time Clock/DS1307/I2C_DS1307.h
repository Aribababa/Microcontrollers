#ifndef RTC_DS1307_H
#define RTC_DS1307_H

#define ENERO		0X01
#define FEBRERO		0X02
#define MARZO		0X03
#define ABRIL		0X04
#define MAYO		0X05
#define JUNIO		0X06
#define JULIO		0X07
#define AGOSTO		0X08
#define SEPTIEMBRE	0X09
#define OCTUBRE		0X0A
#define NOVIEMBRE	0X0B
#define DICIEMBRE	0X0C

#define DOMINGO		0X01
#define LUNES		0X02
#define MARTES		0X03
#define MIERCOLES	0X04
#define JUEVES		0X05
#define VIERNES		0X06
#define SABADO		0X07

#define SECOND_REGISTER    	0x00
#define MINUTE_REGISTER    	0x01
#define HOUR_REGISTER      	0x02
#define DAY_REGISTER       	0x03
#define DATE_REGISTER      	0x04
#define MONTH_REGISTER     	0x05
#define YEAR_REGISTER      	0x06
#define CONTROL_REGISTER   	0x07
#define FIRST_RAM_REGISTER 	0x08
#define LAST_RAM_REGISTER  	0x3F

#define SECOND_MASK        	0x7F
#define MINUTE_MASK        	0x7F
#define HOUR_MASK          	0x3F
#define DAY_MASK           	0x07
#define DATE_MASK          	0x3F
#define MONTH_MASK         	0x1F
#define YEAR_MASK          	0xFF
#define CONTROL_MASK       	0xFF

#define FIRST_TIME_SET		0xAA

typedef struct
{
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hour;
	uint8_t day;
	uint8_t date;
	uint8_t month;
    uint8_t year;
} DS1307;

void I2C__DS1307_SetSecond(uint8_t address, uint8_t data);
void I2C__DS1307_SetMinute(uint8_t address, uint8_t data);
void I2C__DS1307_SetHour(uint8_t address, uint8_t data);
void I2C__DS1307_SetDay(uint8_t address, uint8_t data);
void I2C__DS1307_SetDate(uint8_t address, uint8_t data);
void I2C__DS1307_SetMonth(uint8_t address, uint8_t data);
void I2C__DS1307_SetYear(uint8_t address, uint8_t data);
void I2C__DS1307_SetControl(uint8_t address, uint8_t data);

uint8_t I2C__DS1307_GetSecond(uint8_t address);
uint8_t I2C__DS1307_GetMinute(uint8_t address);
uint8_t I2C__DS1307_GetHour(uint8_t address);
uint8_t I2C__DS1307_GetDay(uint8_t address);
uint8_t I2C__DS1307_GetDate(uint8_t address);
uint8_t I2C__DS1307_GetMonth(uint8_t address);
uint8_t I2C__DS1307_GetYear(uint8_t address);
uint8_t I2C__DS1307_Control(uint8_t address);

void I2C__DS1307_Init(uint8_t address);
void I2C__DS1307_Write(uint8_t address, DS1307 *newDS3231);
void I2C__DS1307_Read(uint8_t address, DS1307 *newDS3231);

void I2C__DS1307_WriteRAM(uint8_t address, uint8_t addressRAM, uint8_t bytesToWrite, uint8_t *data);
void I2C__DS1307_ReadRAM(uint8_t address, uint8_t addressRAM, uint8_t bytesToRead, uint8_t *data);

uint8_t I2C__DS1307_Get(uint8_t address, uint8_t registerValue, uint8_t flagValue);
void I2C__DS1307_Set(uint8_t address, uint8_t data, uint8_t registerValue, uint8_t flagValue);

uint8_t BIN2BCD(uint8_t binaryValue);
uint8_t BCD2BIN(uint8_t bcdValue);

#endif
