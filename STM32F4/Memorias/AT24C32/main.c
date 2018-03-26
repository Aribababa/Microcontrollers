#include <stm32f4xx.h>
#include <Delay.h>
#include <I2C.h>
#include <I2C_LCD.h>
#include <I2C_AT24C32.h>

#define LCD_ADDRESS1 0x7E
#define AT24C32_ADDRESS1 0xA0

int main(void)
{
	Delay__Init();

	I2C__Init();
	I2C__LCD_Init(LCD_ADDRESS1);
	I2C__AT24C32_Init();

	I2C__AT24C32_WriteByte(AT24C32_ADDRESS1, (uint16_t)0x00, 0x0);
	I2C__AT24C32_WriteByte(AT24C32_ADDRESS1, (uint16_t)0x01, 0x1);

	I2C__LCD_WriteFloatingNumber(LCD_ADDRESS1, I2C__AT24C32_ReadByte(AT24C32_ADDRESS1,0x0000), 0);
	I2C__LCD_WriteFloatingNumber(LCD_ADDRESS1, I2C__AT24C32_ReadByte(AT24C32_ADDRESS1,0x0001), 0);

	while(1);
}
