#include <stm32f4xx.h>
#include <stm32f4xx_i2c.h>
#include <I2C.h>

/*
 * Inicializar el modulo I2C1 (SCL->B6 y SDA->B7) en 100Kbps y 7 bits de direccion
 * 		Requisitos previos->	Ninguno
 * 		Entrada-> 				Ninguno
 * 		Salida->				Ninguno
*/
void I2C__Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	I2C_InitTypeDef I2C_InitStruct;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);	// SCL
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1); // SDA

	I2C_InitStruct.I2C_ClockSpeed = 100000;
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_OwnAddress1 = 0x00;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Disable;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_Init(I2C1, &I2C_InitStruct);

	I2C_Cmd(I2C1, ENABLE);
}

/*
 * Genera la condicion de Start y envia la direccion del dispositivo a comunicarse
 * 		Requisitos previos->	I2C__Init();
 * 		Entrada-> 				address:		direccion I2C del dispositivo a comunicarse
 * 								direction:		indica si el maestro va a leer o escribir en el esclavo
 * 		Salida->				Ninguno
*/
void I2C__Start(uint8_t address, uint8_t direction)
{
	// wait until I2C1 is not busy anymore
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

	// Send I2C1 START condition
	I2C_GenerateSTART(I2C1, ENABLE);

	// wait for I2C1 EV5 --> Slave has acknowledged start condition
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

	// Send slave Address for write
	I2C_Send7bitAddress(I2C1, address, direction);

	/* wait for I2C1 EV6, check if
	 * either Slave has acknowledged Master transmitter or
	 * Master receiver mode, depending on the transmission
	 * direction
	 */
	if(direction == I2C_Direction_Transmitter)
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	else if(direction == I2C_Direction_Receiver)
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
}

/*
 * Genera la condicion de Restart y envia la direccion del dispositivo a comunicarse
 * 		Requisitos previos->	I2C__Init();
 * 								I2C__Start();
 * 		Entrada-> 				address:		direccion I2C del dispositivo a comunicarse
 * 								direction:		indica si el maestro va a leer o escribir en el esclavo
 * 		Salida->				Ninguno
*/
void I2C__Restart(uint8_t address, uint8_t direction)
{
	I2C_GenerateSTART(I2C1, ENABLE);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2C1, address, I2C_Direction_Receiver);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
}

/*
 * Enviar un byte
 * 		Requisitos previos->	I2C__Init();
 * 								I2C__Start();
 * 		Entrada-> 				data:			dato a enviar por la linea SDA
 * 		Salida->				Ninguno
*/
void I2C__Write(uint8_t data)
{
	I2C_SendData(I2C1, data);
	// wait for I2C1 EV8_2 --> byte has been transmitted
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
}

/*
 * Leer un byte y enviar que el dato fue reconocido
 * 		Requisitos previos->	I2C__Init();
 * 								I2C__Start(); o I2C__Restart(); dependiendo el caso
 * 		Entrada-> 				Ninguno
 * 		Salida->				Dato de lectura
*/
uint8_t I2C__Read_ACK(void)
{
	// enable acknowledge of recieved data
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	// wait until one byte has been received
	while( !I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED) );
	// read data from I2C data register and return data byte
	uint8_t data = I2C_ReceiveData(I2C1);
	return data;
}

/*
 * Leer un byte, decirle al esclavo que el dato no fue reconocido y generar condicion de Stop
 * 		Requisitos previos->	I2C__Init();
 * 								I2C__Start(); o I2C__Restart(); dependiendo el caso
 * 		Entrada-> 				Ninguno
 * 		Salida->				Dato de lectura
*/
uint8_t I2C__Read_NACK(void)
{
	// disable acknowledge of received data
	// nack also generates stop condition after last byte received
	// see reference manual for more info
	I2C_AcknowledgeConfig(I2C1, DISABLE);
	I2C_GenerateSTOP(I2C1, ENABLE);
	// wait until one byte has been received
	while( !I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED) );
	// read data from I2C data register and return data byte
	uint8_t data = I2C_ReceiveData(I2C1);
	return data;
}

/*
 * Genera la condicion de Stop
 * 		Requisitos previos->	I2C__Init();
 * 								I2C__Start(); o I2C__Restart(); dependiendo del caso
 * 		Entrada-> 				Ninguno
 * 		Salida->				Ninguno
*/
void I2C__Stop(void)
{
	// Send I2C1 STOP Condition
	I2C_GenerateSTOP(I2C1, ENABLE);
}
