#include "stm32f4xx.h"
#include <stm32f4xx_i2c.h>
#include <math.h>
#include <I2C.h>
#include <I2C_MPU6050.h>

#define BUFFER_DATOS_ENTRADA 13

uint8_t datos[BUFFER_DATOS_ENTRADA];
uint8_t estadoMaquinaEstados = 0;
uint8_t flagLecturaTerminada;
uint8_t numeroBytesRecibidos;

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

	//Configuracion 8G
	uint8_t valorConfiguracionAcelerometro = I2C__MPU6050_ReadAccelerometerConfig(MPU6050_ADDRESS);
	I2C__MPU6050_WriteAccelerometerConfig(MPU6050_ADDRESS, (valorConfiguracionAcelerometro & 0b11100111) | 0b00010000);
}

/*
 * Leer las aceleraciones y la temperatura en el formato designado a cada variable
 * 		Requisitos previos->	I2C__MPU6050_Init();
 * 		Entrada->				*myMPU6050:				direccion del objeto de tipo MPU6050 en el cual se almacenaran las lecturas
 * 															y conversiones de la aceleracion y la temperatura
 * 		Salida->				Estado de la lectura de los datos:
 * 									0 = aun en proceso de lectura
 * 									1 = lectura de los datos finalizada. La conversion ya existe en el objeto apuntado por *myMPU6050
*/
uint8_t I2C__MPU6050_Read(uint8_t address, MPU6050 *myMPU6050)
{
	flagLecturaTerminada = 0;
	if(estadoMaquinaEstados == 0)
	{
		if(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) == 0) // wait until I2C1 is not busy anymore
		{
			estadoMaquinaEstados = 1;
			I2C_GenerateSTART(I2C1, ENABLE); // Send I2C1 START condition
		}
	}
	else if(estadoMaquinaEstados == 1)
	{
		if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT) == 1) // wait for I2C1 EV5 --> Slave has acknowledged start condition
		{
			estadoMaquinaEstados = 2;
			I2C_Send7bitAddress(I2C1, address, I2C_Direction_Transmitter); // Send slave Address for write
		}
	}
	else if(estadoMaquinaEstados == 2)
	{
		if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == 1) // wait for I2C1 EV6, check if either Slave has acknowledged Master transmitter direction
		{
			estadoMaquinaEstados = 3;
			I2C_SendData(I2C1, MPU6050_ACCEL_XOUT_H); // Primer registro sobre el cual comenzara la lectura
		}
	}
	else if(estadoMaquinaEstados == 3)
	{
		if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED) == 1) // wait for I2C1 EV8_2 --> byte has been transmitted
		{
			estadoMaquinaEstados = 4;
			I2C_GenerateSTART(I2C1, ENABLE); //Condicion de restart
		}
	}
	else if(estadoMaquinaEstados == 4)
	{
		if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT) == 1) // wait for I2C1 EV5 --> Slave has acknowledged restart condition
		{
			estadoMaquinaEstados = 5;
			I2C_Send7bitAddress(I2C1, address, I2C_Direction_Receiver); // Send slave Address for read
		}
	}
	else if(estadoMaquinaEstados == 5)
	{
		if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) == 1) // wait for I2C1 EV6, check if either Slave has acknowledged Master receiver direction
		{
			estadoMaquinaEstados = 6;
			I2C_AcknowledgeConfig(I2C1, ENABLE); // enable acknowledge of recieved data
			numeroBytesRecibidos = 0;
		}
	}
	else if(estadoMaquinaEstados == 6)
	{
		if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED) == 1) // wait until bytes have been received
		{
			datos[numeroBytesRecibidos++] = I2C_ReceiveData(I2C1);
			if(numeroBytesRecibidos > BUFFER_DATOS_ENTRADA - 1)
			{
				I2C_AcknowledgeConfig(I2C1, DISABLE); // disable acknowledge of recieved data
				I2C_GenerateSTOP(I2C1, ENABLE); // stop condition
				estadoMaquinaEstados = 7;
			}
		}
	}
	else if(estadoMaquinaEstados == 7)
	{
		if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED) == 1) // wait until last byte has been received
		{
			estadoMaquinaEstados = 0;
			flagLecturaTerminada = 1;
			datos[numeroBytesRecibidos] = I2C_ReceiveData(I2C1);

			myMPU6050->AcelerometerX = datos[0] << 8 | datos[1];
			myMPU6050->AcelerometerY = datos[2] << 8 | datos[3];
			myMPU6050->AcelerometerZ = datos[4] << 8 | datos[5];
			myMPU6050->Temperature = datos[6] << 8 | datos[7];
			myMPU6050->GyroscopeX = datos[8] << 8 | datos[9];
			myMPU6050->GyroscopeY = datos[10] << 8 | datos[11];
			myMPU6050->GyroscopeZ = datos[12] << 8 | datos[13];

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

			//CALCULAR ACELERACIONES EN m/s^2
			myMPU6050->AcelerometerX_m_s = myMPU6050->AcelerometerX * (9.81/16384.0);
			myMPU6050->AcelerometerY_m_s = myMPU6050->AcelerometerY * (9.81/16384.0);
			myMPU6050->AcelerometerZ_m_s = myMPU6050->AcelerometerZ * (9.81/16384.0);

			//CALCULAR VELOCIDADES ANGULARES EN grados/s
			myMPU6050->GiroscopeX_degree_s = myMPU6050->AcelerometerX * (250.0/32768.0);
			myMPU6050->GiroscopeY_degree_s = myMPU6050->AcelerometerY * (250.0/32768.0);
			myMPU6050->GiroscopeZ_degree_s = myMPU6050->AcelerometerZ * (250.0/32768.0);
		}
	}
	else
	{
		asm("nop");
	}
	return flagLecturaTerminada;
}

uint8_t I2C__MPU6050_ReadAccelerometerConfig(uint8_t address)
{
	I2C__Start(address, I2C_Direction_Transmitter);
	I2C__Write(MPU6050_ACCEL_CONFIG);
	I2C__Restart(address, I2C_Direction_Receiver);
	return I2C__Read_NACK();
}

void I2C__MPU6050_WriteAccelerometerConfig(uint8_t address, uint8_t data)
{
	I2C__Start(address, I2C_Direction_Transmitter);
	I2C__Write(MPU6050_ACCEL_CONFIG);
	I2C__Write(data);
	I2C__Stop();
}
