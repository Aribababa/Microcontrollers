#ifndef I2C_DS3231_H_
#define I2C_DS3231_H_

#define DS3231_SUNDAY		1
#define DS3231_MONDAY		2
#define DS3231_TUESDAY		3
#define DS3231_WEDNESDAY	4
#define DS3231_THURSDAY		5
#define DS3231_FRIDAY		6
#define DS3231_SATURDAY		7

#define DS3231_JANUARY		1
#define DS3231_FEBRUARY		2
#define DS3231_MARCH		3
#define DS3231_APRIL		4
#define DS3231_MAY			5
#define DS3231_JUNE			6
#define DS3231_JULY			7
#define DS3231_AUGUST		8
#define DS3231_SEPTEMBER	9
#define DS3231_OCTOBER		10
#define DS3231_NOVEMBER		11
#define DS3231_DECEMBER		12

#define DS3231_SECOND_REGISTER    			0x00
#define DS3231_MINUTE_REGISTER    			0x01
#define DS3231_HOUR_REGISTER      			0x02
#define DS3231_DAY_REGISTER       			0x03
#define DS3231_DATE_REGISTER      			0x04
#define DS3231_MONTH_REGISTER     			0x05
#define DS3231_YEAR_REGISTER      			0x06
#define DS3231_CONTROL_REGISTER   			0x0E
#define DS3231_CONTROL_STATUS_REGISTER		0x0F

#define DS3231_SECOND_MASK        			0x7F
#define DS3231_MINUTE_MASK        			0x7F
#define DS3231_HOUR_MASK          			0x3F
#define DS3231_DAY_MASK           			0x07
#define DS3231_DATE_MASK          			0x3F
#define DS3231_MONTH_MASK         			0x1F
#define DS3231_YEAR_MASK          			0xFF
#define DS3231_CONTROL_MASK       			0xFF
#define DS3231_CONTROL_STATUS_MASK       	0x88

typedef struct
{
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hour;
	uint8_t day;
	uint8_t date;
	uint8_t month;
    uint8_t year;
} DS3231;

void I2C__DS3231_Init(uint8_t address);
void I2C__DS3231_Write(uint8_t address, DS3231 *myDS3231);
void I2C__DS3231_Read(uint8_t address, DS3231 *myDS3231);

void I2C__DS3231_SetSecond(uint8_t address, uint8_t data);
void I2C__DS3231_SetMinute(uint8_t address, uint8_t data);
void I2C__DS3231_SetHour(uint8_t address, uint8_t data);
void I2C__DS3231_SetDay(uint8_t address, uint8_t data);
void I2C__DS3231_SetDate(uint8_t address, uint8_t data);
void I2C__DS3231_SetMonth(uint8_t address, uint8_t data);
void I2C__DS3231_SetYear(uint8_t address, uint8_t data);
void I2C__DS3231_SetControl(uint8_t address, uint8_t data);
void I2C__DS3231_SetControlStatus(uint8_t address, uint8_t data);

uint8_t I2C__DS3231_GetSecond(uint8_t address);
uint8_t I2C__DS3231_GetMinute(uint8_t address);
uint8_t I2C__DS3231_GetHour(uint8_t address);
uint8_t I2C__DS3231_GetDay(uint8_t address);
uint8_t I2C__DS3231_GetDate(uint8_t address);
uint8_t I2C__DS3231_GetMonth(uint8_t address);
uint8_t I2C__DS3231_GetYear(uint8_t address);
uint8_t I2C__DS3231_Control(uint8_t address);
uint8_t I2C__DS3231_ControlStatus(uint8_t address);

void I2C__DS3231_Set(uint8_t address, uint8_t data, uint8_t registerValue, uint8_t flagValue);
uint8_t I2C__DS3231_Get(uint8_t address, uint8_t registerValue, uint8_t flagValue);

#endif
