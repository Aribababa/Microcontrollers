#include <stm32f4xx.h>
#include <stm32f4xx_i2c.h>
#include <I2C.h>
#include <I2C_LCD.h>
#include <I2C_DS3231.h>
#include <Delay.h>

//***********************************************************************************************
#define LCD_ADDRESS1 0x7E
#define DS3231_ADDRESS1 0xD0

//***********************************************************************************************
void convertirDato(uint8_t data);

//***********************************************************************************************
uint8_t datoFormatoLCD[2];

//***********************************************************************************************
int main(void)
{
	DS3231 miRTC = {10, 20, 9, DS3231_TUESDAY, 27, DS3231_FEBRUARY, 18}; //09:20:10 Martes 27/02/2018

	Delay__Init();
	I2C__Init();
	I2C__LCD_Init(LCD_ADDRESS1);

	I2C__DS3231_Init(DS3231_ADDRESS1);
	I2C__DS3231_Write(DS3231_ADDRESS1, &miRTC);

	while(1)
	{
		I2C__DS3231_Read(DS3231_ADDRESS1, &miRTC);

		convertirDato(miRTC.seconds);
		I2C__LCD_Write(LCD_ADDRESS1, datoFormatoLCD[1]);
		I2C__LCD_Write(LCD_ADDRESS1, datoFormatoLCD[0]);

		Delay__MS(500);
		I2C__LCD_Clear(LCD_ADDRESS1);
	}
}

//***********************************************************************************************
void convertirDato(uint8_t data)
{
	datoFormatoLCD[0] = (data % 10) + '0';
	datoFormatoLCD[1] = ((data % 100 - data % 10) / 10) + '0';
}

