#include "stm32f4xx.h"

/*
 * Inicializa el modulo SPI1(SCK->A5, MISO->A6 y MOSI->7 por hardware y SS->A0 por software), configura SPI en el modo 00,
 * datos de 8 bits y primero enviar bit MSB
 * 		Requisitos previos->	Ninguno
 * 		Entrada->				Ninguno
 * 		Salida->				Ninguno
*/
void SPI__Init(void)
{
	//Reloj de los perifericos
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	SPI_InitTypeDef SPI_InitTypeDefStruct;
	GPIO_InitTypeDef GPIO_InitTypeDefStruct;

	//Configuraciones del SPI (Modo 00)
	SPI_InitTypeDefStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitTypeDefStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitTypeDefStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitTypeDefStruct.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitTypeDefStruct.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitTypeDefStruct.SPI_NSS = SPI_NSS_Soft;
	SPI_InitTypeDefStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
	SPI_InitTypeDefStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_Init(SPI1, &SPI_InitTypeDefStruct);

	//Configuracion de los pines del SPI
	GPIO_InitTypeDefStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7; //SCK, MISO, MOSI
	GPIO_InitTypeDefStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitTypeDefStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitTypeDefStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitTypeDefStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitTypeDefStruct);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);

	//Configuraciones del pin SS
	GPIO_InitTypeDefStruct.GPIO_Pin = GPIO_Pin_0; //SS
	GPIO_InitTypeDefStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitTypeDefStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitTypeDefStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitTypeDefStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOA, &GPIO_InitTypeDefStruct);

	//Comenzar sin seleccionar ningun esclavo
	GPIO_SetBits(GPIOA, GPIO_Pin_0);

	SPI_Cmd(SPI1, ENABLE);
}

/*
 * Enviar un byte al esclavo
 * 		Requisitos previos->	SPI__Init();
 * 								Haber seleccionado al menos un esclavo al cual se le enviará el dato
 * 		Entrada->				data:			dato a ser enviado
 * 		Salida->				Ninguno
*/
void SPI__Write(uint8_t data)
{
	//Enviar el dato y esperar a que se termine de enviar
	SPI_I2S_SendData(SPI1, data);
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));

	//Esperar dato de entrada en caso en caso de que mientras se transmitia, habia recepcion
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
	(void)SPI_I2S_ReceiveData(SPI1);
}

/*
 * Enviar un byte al esclavo y al mismo tiempo, recibe un byte del mismo
 * 		Requisitos previos->	SPI__Init();
 * 								Haber seleccionado al menos un esclavo al cual se le enviará el dato
 * 		Entrada->				data:			dato a ser enviado
 * 		Salida->				Dato que el lee el maestro miestras escribia un dato. Comunicacion Full duplex.
*/
uint8_t SPI__WriteRead(uint8_t data)
{
	//Enviar el dato y esperar a que se termine de enviar
	SPI_I2S_SendData(SPI1, data);
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));

	//Esperar dato de entrada en caso en caso de que mientras se transmitia, habia recepcion
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
	return SPI_I2S_ReceiveData(SPI1);
}

/*
 * Recibir un byte del esclavo
 * 		Requisitos previos->	SPI__Init();
 * 								Haber seleccionado el esclavo del cual se leera el dato
 * 		Entrada->				Ninguno
 * 		Salida->				Dato proveniente del esclavo
*/
uint8_t SPI__Read(void)
{
	//Enviar cualquier byte para brindarle reloj al esclavo
	SPI_I2S_SendData(SPI1, 0x00);
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));

	//Esperar po si la transaccion aun no se cumple
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));

	return  SPI_I2S_ReceiveData(SPI1);
}

/*
 * Seleccion de un unico esclavo conectado en GPIOA GPIO_Pin_0
 * 		Requisitos previos->	SPI__Init();
 * 		Entrada->				Ninguno
 * 		Salida->				Ninguno
*/
void SPI__SlaveEnable(void)
{
	//SS en 0
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);
}

/*
 * Desconexion del master con el esclavo conectado en GPIOA GPIO_Pin_0
 * 		Requisitos previos->	SPI__Init();
 * 		Entrada->				Ninguno
 * 		Salida->				Ninguno
*/
void SPI__SlaveDisable(void)
{
	//SS en 1
	GPIO_SetBits(GPIOA, GPIO_Pin_0);
}
