#include <stm32f4xx.h>
#include <stm32f4xx_i2c.h>
#include <Delay.h>
#include <I2C.h>
#include <I2C_LCD.h>
#include <I2C_DS1307.h>

#define LCD_ADDRESS1 0x7E
#define DS1307_ADDRESS1 0xD0

int main(void)
{
	DS1307 miRTC = {40, 12, 19, DS1307_THURSDAY, 8, DS1307_MARCH, 18}; //14:53:50 Martes 06 de Marzo del 2018

	Delay__Init();
	I2C__Init();
	I2C__LCD_Init(LCD_ADDRESS1);

	//I2C__DS1307_ResetConfiguration(DS1307_ADDRESS1);

	//Antes que nada, revisar si el DS1307 ya tiene una fecha registrada
	if(I2C__DS1307_ReadConfiguration(DS1307_ADDRESS1) != DS1307_PREVIOUS_CONFIGURATION)
	{
		I2C__DS1307_Init(DS1307_ADDRESS1);
		I2C__DS1307_Write(DS1307_ADDRESS1, &miRTC);
		I2C__DS1307_SetConfiguration(DS1307_ADDRESS1);
	}

	while(1)
	{
		I2C__DS1307_Read(DS1307_ADDRESS1, &miRTC);

		I2C__LCD_WriteFloatingNumber(LCD_ADDRESS1, miRTC.hour, 0);
		I2C__LCD_Write(LCD_ADDRESS1, ':');
		I2C__LCD_WriteFloatingNumber(LCD_ADDRESS1, miRTC.minutes, 0);
		I2C__LCD_Write(LCD_ADDRESS1, ':');
		I2C__LCD_WriteFloatingNumber(LCD_ADDRESS1, miRTC.seconds, 0);

		I2C__LCD_Linea2(LCD_ADDRESS1);

		if(miRTC.day == 1)
			I2C__LCD_WriteString(LCD_ADDRESS1, &DS1307_Domingo[0]);
		else if(miRTC.day == 2)
			I2C__LCD_WriteString(LCD_ADDRESS1, &DS1307_Lunes[0]);
		else if(miRTC.day == 3)
			I2C__LCD_WriteString(LCD_ADDRESS1, &DS1307_Martes[0]);
		else if(miRTC.day == 4)
			I2C__LCD_WriteString(LCD_ADDRESS1, &DS1307_Miercoles[0]);
		else if(miRTC.day == 5)
			I2C__LCD_WriteString(LCD_ADDRESS1, &DS1307_Jueves[0]);
		else if(miRTC.day == 6)
			I2C__LCD_WriteString(LCD_ADDRESS1, &DS1307_Viernes[0]);
		else if(miRTC.day == 7)
			I2C__LCD_WriteString(LCD_ADDRESS1, &DS1307_Sabado[0]);
		else
		{
			if(miRTC.day == 0)
				I2C__LCD_WriteString(LCD_ADDRESS1, &DS1307_Error[0]);
			else
				I2C__LCD_WriteString(LCD_ADDRESS1, &DS1307_OK[0]);
		}

		I2C__LCD_WriteFloatingNumber(LCD_ADDRESS1, miRTC.date, 0);
		I2C__LCD_Write(LCD_ADDRESS1, '/');
		I2C__LCD_WriteFloatingNumber(LCD_ADDRESS1, miRTC.month, 0);
		I2C__LCD_Write(LCD_ADDRESS1, '/');
		I2C__LCD_WriteFloatingNumber(LCD_ADDRESS1, miRTC.year, 0);

		Delay__MS(500);
		I2C__LCD_Clear(LCD_ADDRESS1);
	}
}
