//***********************************************************************************************
#include <stm32f4xx.h>
#include <stm32f4xx_i2c.h>
#include <I2C.h>
#include <I2C_LCD.h>

//***********************************************************************************************
#define LCD1_ADDRESS 0x7E

//***********************************************************************************************
int main(void)
{
	I2C__Init();
	I2C__LCD_Init(LCD1_ADDRESS);

	I2C__LCD_Write(LCD1_ADDRESS, (uint8_t)0x52);
	I2C__LCD_Write(LCD1_ADDRESS, (uint8_t)0x52);
	I2C__LCD_Write(LCD1_ADDRESS, (uint8_t)0x52);

	while(1)
	{
		asm ("nop");
	}
}
