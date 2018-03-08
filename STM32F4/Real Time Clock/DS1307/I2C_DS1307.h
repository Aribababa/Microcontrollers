#ifndef I2C_DS1307_H
#define I2C_DS1307_H

//Definiciones para el usuario
#define DS1307_JANUARY		0X01
#define DS1307_FEBRUARY		0X02
#define DS1307_MARCH		0X03
#define DS1307_APRIL		0X04
#define DS1307_MAY			0X05
#define DS1307_JUNE			0X06
#define DS1307_JULY			0X07
#define DS1307_AUGUST		0X08
#define DS1307_SEPTEMBER	0X09
#define DS1307_OCTOBER		0X0A
#define DS1307_NOVEMBER		0X0B
#define DS1307_DECEMBER		0X0C

#define DS1307_SUNDAY		0X01
#define DS1307_MONDAY		0X02
#define DS1307_TUESDAY		0X03
#define DS1307_WEDNESDAY	0X04
#define DS1307_THURSDAY		0X05
#define DS1307_FRIDAY		0X06
#define DS1307_SATURDAY		0X07

#define DS1307_SECOND_REGISTER    	0x00
#define DS1307_MINUTE_REGISTER    	0x01
#define DS1307_HOUR_REGISTER      	0x02
#define DS1307_DAY_REGISTER       	0x03
#define DS1307_DATE_REGISTER      	0x04
#define DS1307_MONTH_REGISTER     	0x05
#define DS1307_YEAR_REGISTER      	0x06
#define DS1307_CONTROL_REGISTER   	0x07

#define DS1307_PREVIOUS_CONFIGURATION	0xAA

//Definiciones para la libreria
#define DS1307_SECOND_MASK        	0x7F
#define DS1307_MINUTE_MASK        	0x7F
#define DS1307_HOUR_MASK          	0x3F
#define DS1307_DAY_MASK           	0x07
#define DS1307_DATE_MASK          	0x3F
#define DS1307_MONTH_MASK         	0x1F
#define DS1307_YEAR_MASK          	0xFF
#define DS1307_CONTROL_MASK       	0xFF

#define DS1307_FIRST_RAM_REGISTER 	0x08
#define DS1307_LAST_RAM_REGISTER  	0x3E
#define DS1307_CONFIGURATION_REGISTER	0x3F

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

//Funciones para el usuario
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

void I2C__DS1307_SetConfiguration(uint8_t address);
void I2C__DS1307_ResetConfiguration(uint8_t address);
uint8_t I2C__DS1307_ReadConfiguration(uint8_t address);

//Funciones para la libreria
uint8_t I2C__DS1307_Get(uint8_t address, uint8_t registerValue, uint8_t flagValue);
void I2C__DS1307_Set(uint8_t address, uint8_t data, uint8_t registerValue, uint8_t flagValue);

#endif
