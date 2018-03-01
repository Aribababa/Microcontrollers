#include "stm32f4xx.h"
#include <stm32f4xx_i2c.h>
#include <math.h>
#include <I2C.h>
#include <I2C_MPU6050.h>

//***********************************************************************************************
void I2C__MPU6050_Init(uint8_t address)
{
	I2C__Start(address, I2C_Direction_Transmitter);
	I2C__Write(MPU6050_PWR_MGMT_1);
	I2C__Write(0x00); //Wake up the MPU6050
	I2C__Stop();
}

void I2C__MPU6050_Read(uint8_t address, MPU6050 *myMPU6050)
{
	I2C__Start(address, I2C_Direction_Transmitter);
	I2C__Write(MPU6050_ACCEL_XOUT_H);
	I2C__Restart(address, I2C_Direction_Receiver);
	myMPU6050->AcelerometerX = I2C__Read_ACK() << 8 | I2C__Read_ACK();
	myMPU6050->AcelerometerY = I2C__Read_ACK() << 8 | I2C__Read_ACK();
	myMPU6050->AcelerometerZ = I2C__Read_ACK() << 8 | I2C__Read_ACK();
	myMPU6050->Temperature = I2C__Read_ACK() << 8 | I2C__Read_ACK();
	myMPU6050->GyroscopeX = I2C__Read_ACK() << 8 | I2C__Read_ACK();
	myMPU6050->GyroscopeY = I2C__Read_ACK() << 8 | I2C__Read_ACK();
	myMPU6050->GyroscopeZ = I2C__Read_ACK() << 8 | I2C__Read_NACK();

	//REACOMODAR TEMPERATURA
	myMPU6050->Temperature = (myMPU6050->Temperature / 340.00) + 36.53;

	//CALCULAR GRADOS EN EL EJE X y Y DEL ACELEROMETRO
	myMPU6050->AcelerometerAngleY = RAD_TO_DEG * (float)atan2(-1 * (myMPU6050->AcelerometerY / A_R) , sqrt(pow(myMPU6050->AcelerometerX / A_R, 2) + pow(myMPU6050->AcelerometerZ / A_R, 2)));
	myMPU6050->AcelerometerAngleX = RAD_TO_DEG * (float)atan2(-1 * (myMPU6050->AcelerometerX / A_R) , sqrt(pow(myMPU6050->AcelerometerY / A_R, 2) + pow(myMPU6050->AcelerometerZ / A_R, 2)));

	//CALCULAR GRADOS EN EL EJE X y Y DEL GIROSCOPIO
	myMPU6050->GyroscopeAngleY = myMPU6050->GyroscopeAngleY / G_R;
	myMPU6050->GyroscopeAngleX = myMPU6050->GyroscopeAngleX / G_R;

	//CALCULAR EL ANGULO DEL FILTRO
	myMPU6050->AngleY = 0.98 *(myMPU6050->AngleY + myMPU6050->GyroscopeAngleY * 0.010) + 0.02 * myMPU6050->AcelerometerAngleY;
	myMPU6050->AngleX = 0.98 *(myMPU6050->AngleX + myMPU6050->GyroscopeAngleX * 0.010) + 0.02 * myMPU6050->AcelerometerAngleX;
}
