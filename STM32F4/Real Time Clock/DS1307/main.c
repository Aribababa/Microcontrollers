#include <stm32f4xx.h>
#include <stm32f4xx_i2c.h>
#include <I2C.h>
#include <I2C_LCD.h>
#include <I2C_DS1307.h>
#include <Delay.h>

//***********************************************************************************************
#define LCD_ADDRESS1 0x7E
#define DS1307_ADDRESS1 0xD0

//***********************************************************************************************
void convertirDato(uint8_t data);

//***********************************************************************************************
uint8_t datoFormatoLCD[2];

//***********************************************************************************************
int main(void)
{
	DS1307 miRTC = {30, 37, 20, MARTES, 27, FEBRERO, 18}; //00:00:00 Martes 01 de Enero del 2019
	uint8_t ramValue;

	Delay__Init();
	I2C__Init();
	I2C__LCD_Init(LCD_ADDRESS1);

	//Antes que nada, revisar si el DS1307 ya tiene una fecha registrada
	I2C__DS1307_ReadRAM(DS1307_ADDRESS1, 0x00, 1, &ramValue);
	if(ramValue != FIRST_TIME_SET)
	{
		I2C__DS1307_Init(DS1307_ADDRESS1);
		I2C__DS1307_Write(DS1307_ADDRESS1, &miRTC);
		ramValue = FIRST_TIME_SET;
		I2C__DS1307_WriteRAM(DS1307_ADDRESS1, 0x00, 1, &ramValue);
	}

	while(1)
	{
		Delay__MS(500);
		I2C__LCD_Clear(LCD_ADDRESS1);

		I2C__DS1307_Read(DS1307_ADDRESS1, &miRTC);

		convertirDato(miRTC.hour);
		I2C__LCD_Write(LCD_ADDRESS1, datoFormatoLCD[1]); I2C__LCD_Write(LCD_ADDRESS1, datoFormatoLCD[0]);
		I2C__LCD_Write(LCD_ADDRESS1, ':');

		convertirDato(miRTC.minutes);
		I2C__LCD_Write(LCD_ADDRESS1, datoFormatoLCD[1]); I2C__LCD_Write(LCD_ADDRESS1, datoFormatoLCD[0]);
		I2C__LCD_Write(LCD_ADDRESS1, ':');

		convertirDato(miRTC.seconds);
		I2C__LCD_Write(LCD_ADDRESS1, datoFormatoLCD[1]); I2C__LCD_Write(LCD_ADDRESS1, datoFormatoLCD[0]);

		I2C__LCD_Linea2(LCD_ADDRESS1);
		if(miRTC.day == 1)
		{
			I2C__LCD_Write(LCD_ADDRESS1, 'D');
			I2C__LCD_Write(LCD_ADDRESS1, 'o');
			I2C__LCD_Write(LCD_ADDRESS1, 'm');
			I2C__LCD_Write(LCD_ADDRESS1, 'i');
			I2C__LCD_Write(LCD_ADDRESS1, 'n');
			I2C__LCD_Write(LCD_ADDRESS1, 'g');
			I2C__LCD_Write(LCD_ADDRESS1, 'o');
			I2C__LCD_Write(LCD_ADDRESS1, ' ');
		}
		else if(miRTC.day == 2)
		{
			I2C__LCD_Write(LCD_ADDRESS1, 'L');
			I2C__LCD_Write(LCD_ADDRESS1, 'u');
			I2C__LCD_Write(LCD_ADDRESS1, 'n');
			I2C__LCD_Write(LCD_ADDRESS1, 'e');
			I2C__LCD_Write(LCD_ADDRESS1, 's');
			I2C__LCD_Write(LCD_ADDRESS1, ' ');
		}
		else if(miRTC.day == 3)
		{
			I2C__LCD_Write(LCD_ADDRESS1, 'M');
			I2C__LCD_Write(LCD_ADDRESS1, 'a');
			I2C__LCD_Write(LCD_ADDRESS1, 'r');
			I2C__LCD_Write(LCD_ADDRESS1, 't');
			I2C__LCD_Write(LCD_ADDRESS1, 'e');
			I2C__LCD_Write(LCD_ADDRESS1, 's');
			I2C__LCD_Write(LCD_ADDRESS1, ' ');
		}
		else if(miRTC.day == 4)
		{
			I2C__LCD_Write(LCD_ADDRESS1, 'M');
			I2C__LCD_Write(LCD_ADDRESS1, 'i');
			I2C__LCD_Write(LCD_ADDRESS1, 'e');
			I2C__LCD_Write(LCD_ADDRESS1, 'r');
			I2C__LCD_Write(LCD_ADDRESS1, 'c');
			I2C__LCD_Write(LCD_ADDRESS1, 'o');
			I2C__LCD_Write(LCD_ADDRESS1, 'l');
			I2C__LCD_Write(LCD_ADDRESS1, 'e');
			I2C__LCD_Write(LCD_ADDRESS1, 's');
			I2C__LCD_Write(LCD_ADDRESS1, ' ');
		}
		else if(miRTC.day == 5)
		{
			I2C__LCD_Write(LCD_ADDRESS1, 'J');
			I2C__LCD_Write(LCD_ADDRESS1, 'u');
			I2C__LCD_Write(LCD_ADDRESS1, 'e');
			I2C__LCD_Write(LCD_ADDRESS1, 'v');
			I2C__LCD_Write(LCD_ADDRESS1, 'e');
			I2C__LCD_Write(LCD_ADDRESS1, 's');
			I2C__LCD_Write(LCD_ADDRESS1, ' ');
		}
		else if(miRTC.day == 6)
		{
			I2C__LCD_Write(LCD_ADDRESS1, 'V');
			I2C__LCD_Write(LCD_ADDRESS1, 'i');
			I2C__LCD_Write(LCD_ADDRESS1, 'e');
			I2C__LCD_Write(LCD_ADDRESS1, 'r');
			I2C__LCD_Write(LCD_ADDRESS1, 'n');
			I2C__LCD_Write(LCD_ADDRESS1, 'e');
			I2C__LCD_Write(LCD_ADDRESS1, 's');
			I2C__LCD_Write(LCD_ADDRESS1, ' ');
		}
		else if(miRTC.day == 7)
		{
			I2C__LCD_Write(LCD_ADDRESS1, 'S');
			I2C__LCD_Write(LCD_ADDRESS1, 'a');
			I2C__LCD_Write(LCD_ADDRESS1, 'b');
			I2C__LCD_Write(LCD_ADDRESS1, 'a');
			I2C__LCD_Write(LCD_ADDRESS1, 'd');
			I2C__LCD_Write(LCD_ADDRESS1, 'o');
			I2C__LCD_Write(LCD_ADDRESS1, ' ');
		}
		else
		{
			I2C__LCD_Write(LCD_ADDRESS1, 'E');
			I2C__LCD_Write(LCD_ADDRESS1, 'R');
			I2C__LCD_Write(LCD_ADDRESS1, 'R');
			I2C__LCD_Write(LCD_ADDRESS1, 'O');
			I2C__LCD_Write(LCD_ADDRESS1, 'R');
			I2C__LCD_Write(LCD_ADDRESS1, ' ');
		}

		convertirDato(miRTC.date);
		I2C__LCD_Write(LCD_ADDRESS1, datoFormatoLCD[1]); I2C__LCD_Write(LCD_ADDRESS1, datoFormatoLCD[0]);
		I2C__LCD_Write(LCD_ADDRESS1, '/');

		convertirDato(miRTC.month);
		I2C__LCD_Write(LCD_ADDRESS1, datoFormatoLCD[1]); I2C__LCD_Write(LCD_ADDRESS1, datoFormatoLCD[0]);
		I2C__LCD_Write(LCD_ADDRESS1, '/');

		convertirDato(miRTC.year);
		I2C__LCD_Write(LCD_ADDRESS1, datoFormatoLCD[1]); I2C__LCD_Write(LCD_ADDRESS1, datoFormatoLCD[0]);
	}
}

//***********************************************************************************************
void convertirDato(uint8_t data)
{
	datoFormatoLCD[0] = (data % 10) + '0';
	datoFormatoLCD[1] = ((data % 100 - data % 10) / 10) + '0';
}
