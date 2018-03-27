#include "stm32f4xx.h"
#include <stm32f4xx_i2c.h>
#include <math.h>
#include <I2C.h>
#include <I2C_MPU6050.h>

/*
 * Inicializa el modulo MPU6050 despertandolo del modo de bajo consumo
 * 		Requisitos previos->	I2C__Init();
 * 		Entrada->				Ninguno
 * 		Salida->				Ninguno
*/
void I2C__MPU6050_Init(uint8_t address)
{
	I2C__Start(address, I2C_Direction_Transmitter);
	I2C__Write(MPU6050_PWR_MGMT_1);
	I2C__Write(0x00); //Wake up the MPU6050
	I2C__Stop();
}

/*
 * Leer las aceleraciones y la temperatura en el formato designado a cada variable
 * 		Requisitos previos->	I2C__MPU6050_Init();
 * 		Entrada->				*myMPU6050:				direccion del objeto de tipo MPU6050 en el cual se almacenaran las lecturas
 * 															y conversiones de la aceleracion y la temperatura
 * 		Salida->				Ninguno
*/
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

	double tempAcelerometerX = (double)myMPU6050->AcelerometerX;
	double tempAcelerometerY = (double)myMPU6050->AcelerometerY;
	double tempAcelerometerZ = (double)myMPU6050->AcelerometerZ;

	//CALCULAR GRADOS EN EL EJE X y Y DEL ACELEROMETRO
	myMPU6050->AcelerometerAngleX = (float)(RAD_TO_DEG)*(atan(tempAcelerometerY / sqrt(pow(tempAcelerometerX,(double)2) + pow(tempAcelerometerZ,(double)2))));
	myMPU6050->AcelerometerAngleY = (float)(RAD_TO_DEG)*(atan(tempAcelerometerX / sqrt(pow(tempAcelerometerY,(double)2) + pow(tempAcelerometerZ,(double)2))));

	//CALCULAR GRADOS EN EL EJE X y Y DEL GIROSCOPIO
	myMPU6050->GyroscopeAngleY = (double)(myMPU6050->GyroscopeY / (double)G_R);
	myMPU6050->GyroscopeAngleX = (double)(myMPU6050->GyroscopeX / (double)G_R);
}
