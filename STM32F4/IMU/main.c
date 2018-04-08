#include "stm32f4xx.h"
#include <stm32f4xx_i2c.h>
#include <Delay.h>
#include <UART.h>
#include <I2C.h>
#include <I2C_MPU6050.h>

#define MPU6050_ADDRESS1 0xD0

int main(void)
{
	MPU6050 myMPU6050;

	Delay__Init();
	I2C__Init();
	UART__Init();

	I2C__MPU6050_Init(MPU6050_ADDRESS1);

	while(1)
	{
		I2C__MPU6050_Read(MPU6050_ADDRESS1, &myMPU6050);

		UART__WriteFloatingNumber((float)myMPU6050.AcelerometerAngleX*-1, 2);
		UART__Write('\n'); UART__Write('\r');

		Delay__MS(250);
	}
}
