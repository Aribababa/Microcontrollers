#include "stm32f4xx.h"
#include <stm32f4xx_i2c.h>
#include <I2C.h>
#include <I2C_LCD.h>
#include <I2C_MPU6050.h>
#include <Delay.h>

//***********************************************************************************************
#define LCD_ADDRESS1 0x7E
#define MPU6050_ADDRESS1 0x68

//***********************************************************************************************
int main(void)
{
	MPU6050 myMPU6050;

	Delay__Init();
	I2C__Init();
	I2C__LCD_Init(LCD_ADDRESS1);

	I2C__MPU6050_Init(MPU6050_ADDRESS1);

	while(1)
	{
		I2C__MPU6050_Read(MPU6050_ADDRESS1, &myMPU6050);

		//Enviar los datos por UART

		Delay__MS(500);
		I2C__LCD_Clear(LCD_ADDRESS1);
	}
}
