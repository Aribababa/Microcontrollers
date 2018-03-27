#include <stm32f4xx.h>
#include <I2C.h>
#include <I2C_LCD.h>

#define LCD1_ADDRESS 0x7E

int main(void)
{
	I2C__Init();
	I2C__LCD_Init(LCD1_ADDRESS);

	I2C__LCD_Write(LCD1_ADDRESS, 'R');
	I2C__LCD_Write(LCD1_ADDRESS, '=');
	I2C__LCD_Write(LCD1_ADDRESS, ' ');
	I2C__LCD_WriteFloatingNumber(LCD1_ADDRESS, (float)-15505.21, 3);

	while(1)
	{
		asm ("nop");
	}
}
