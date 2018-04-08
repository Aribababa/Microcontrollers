#include <stm32f4xx.h>
#include <stm32f4xx_i2c.h>
#include <BIN_BCD.h>
#include "I2C.h"
#include "I2C_DS1307.h"

uint8_t DS1307_Domingo[]={"Domingo "};
uint8_t DS1307_Lunes[]={"Lunes "};
uint8_t DS1307_Martes[]={"Martes "};
uint8_t DS1307_Miercoles[]={"Miercoles "};
uint8_t DS1307_Jueves[]={"Jueves "};
uint8_t DS1307_Viernes[]={"Viernes "};
uint8_t DS1307_Sabado[]={"Sabado "};
uint8_t DS1307_OK[] = {"OK "};
uint8_t DS1307_Error[] = {"Error "};

/*
 * Inicializacion del RTC en una fecha default y configuracion del funcionamiento principal del
 * dispositivo
 * 		Requisitos previos->	I2C__Init();
 * 		Entrada-> 				address:	direccion I2C del dispositivo a comunicarse
 * 		Salida->				Ninguno
*/
void I2C__DS1307_Init(uint8_t address)
{
	DS1307 newDS1307 = {30, 37, 20, DS1307_TUESDAY, 27, DS1307_FEBRUARY, 18}; //20:37:30 Martes 27 de Febrero del 2018
	I2C__DS1307_Write(address, &newDS1307);

	I2C__Start(address, I2C_Direction_Transmitter);
	I2C__Write(DS1307_CONTROL_REGISTER);
	I2C__Write(0x00 & DS1307_CONTROL_MASK);
	I2C__Stop();
}

/*
 * Escritura de la fecha completa proporcionada por el usuario
 * 		Requisitos previos->	I2C__DS1307_Init();
 * 		Entrada-> 				address:	direccion I2C del dispositivo a comunicarse
 * 								*newDS1307	direccion del objeto DS1307 en el cual se almacenara la nueva fecha designada
 * 		Salida->				Ninguno
*/
void I2C__DS1307_Write(uint8_t address, DS1307 *newDS1307)
{
	if(newDS1307->seconds > 59)
		newDS1307->seconds = 59;
	if(newDS1307->minutes > 59)
		newDS1307->minutes = 59;
	if(newDS1307->hour > 23)
		newDS1307->hour = 23;
	if(newDS1307->day < 1)
		newDS1307->day = 1;
	if(newDS1307->day > 7)
		newDS1307->day = 7;
	if(newDS1307->date < 1)
		newDS1307->date = 1;
	if(newDS1307->date > 31)
		newDS1307->date = 28;
	if(newDS1307->month < 1)
		newDS1307->month = 1;
	if(newDS1307->month > 12)
		newDS1307->month = 12;
	if(newDS1307->year > 99)
		newDS1307->year = 99;

	I2C__Start(address, I2C_Direction_Transmitter);
	I2C__Write(DS1307_SECOND_REGISTER);
	I2C__Write(BIN2BCD(newDS1307->seconds) & DS1307_SECOND_MASK);
	I2C__Write(BIN2BCD(newDS1307->minutes) & DS1307_MINUTE_MASK);
	I2C__Write(BIN2BCD(newDS1307->hour) & DS1307_HOUR_MASK);
	I2C__Write(BIN2BCD(newDS1307->day) & DS1307_DAY_MASK);
	I2C__Write(BIN2BCD(newDS1307->date) & DS1307_DATE_MASK);
	I2C__Write(BIN2BCD(newDS1307->month) & DS1307_MONTH_MASK);
	I2C__Write(BIN2BCD(newDS1307->year) & DS1307_YEAR_MASK);
	I2C__Stop();
}

/*
 * Lectura de la fecha completa registrada en el RTC
 * 		Requisitos previos->	I2C__DS1307_Init();
 * 		Entrada-> 				address:	direccion I2C del dispositivo a comunicarse
 * 								*newDS1307	direccion del objeto DS1307 creado previamente por el usuario en la
 * 												cual se almacenara la fecha actualizada leida del RTC
 * 		Salida->				Ninguno
*/
void I2C__DS1307_Read(uint8_t address, DS1307 *newDS1307)
{
	I2C__Start(address, I2C_Direction_Transmitter);
	I2C__Write(DS1307_SECOND_REGISTER);
	I2C__Restart(address, I2C_Direction_Receiver);
	newDS1307->seconds = BCD2BIN(I2C__Read_ACK() & DS1307_SECOND_MASK);
	newDS1307->minutes = BCD2BIN(I2C__Read_ACK() & DS1307_MINUTE_MASK);
	newDS1307->hour = BCD2BIN(I2C__Read_ACK() & DS1307_HOUR_MASK);
	newDS1307->day = BCD2BIN(I2C__Read_ACK() & DS1307_DAY_MASK);
	newDS1307->date = BCD2BIN(I2C__Read_ACK() & DS1307_DATE_MASK);
	newDS1307->month = BCD2BIN(I2C__Read_ACK() & DS1307_MONTH_MASK);
	newDS1307->year = BCD2BIN(I2C__Read_NACK() & DS1307_YEAR_MASK);
}


/*
 * Realizar modificaciones de segundos en el RTC
 * 		Requisitos previos->	I2C__DS1307_Init();
 * 		Entrada-> 				address:	direccion I2C del dispositivo a comunicarse
 * 								data: 		cantidad de segundos a colocar
 * 		Salida->				Ninguno
*/
void I2C__DS1307_SetSecond(uint8_t address, uint8_t data)
{
	if(data > 59)
		data = 59;
	I2C__DS1307_Set(address, data, DS1307_SECOND_REGISTER, DS1307_SECOND_MASK);
}

/*
 * Realizar modificaciones de minutos en el RTC
 * 		Requisitos previos->	I2C__DS1307_Init();
 * 		Entrada-> 	address:	direccion I2C del dispositivo a comunicarse
 * 					data: 		cantidad de minutos a colocar
 * 		Salida->				Ninguno
*/
void I2C__DS1307_SetMinute(uint8_t address, uint8_t data)
{
	if(data > 59)
		data = 59;
	I2C__DS1307_Set(address, data, DS1307_MINUTE_REGISTER, DS1307_MINUTE_MASK);
}

/*
 * Realizar modificaciones de horas en el RTC
 * 		Requisitos previos->	I2C__DS1307_Init();
 * 		Entrada-> 				address:	direccion I2C del dispositivo a comunicarse
 * 								data: 		cantidad de horas a colocar
 * 		Salida->				Ninguno
*/
void I2C__DS1307_SetHour(uint8_t address, uint8_t data)
{
	if(data > 23)
		data = 23;
	I2C__DS1307_Set(address, data, DS1307_HOUR_REGISTER, DS1307_HOUR_MASK);
}

/*
 * Realizar modificaciones del dia de la semana en el RTC
 * 		Requisitos previos->	I2C__DS1307_Init();
 * 		Entrada-> 				address:	direccion I2C del dispositivo a comunicarse
 * 								data: 		dia de la semana
 * 		Salida->				Ninguno
*/
void I2C__DS1307_SetDay(uint8_t address, uint8_t data)
{
	if(data < 1)
		data = 1;
	if(data > 7)
		data = 7;
	I2C__DS1307_Set(address, data, DS1307_DAY_REGISTER, DS1307_DAY_MASK);
}

/*
 * Realizar modificaciones del dia en el RTC
 * 		Requisitos previos->	I2C__DS1307_Init();
 * 		Entrada-> 				address:	direccion I2C del dispositivo a comunicarse
 * 								data: 		dia a colocar
 * 		Salida->				Ninguno
*/
void I2C__DS1307_SetDate(uint8_t address, uint8_t data)
{
	if(data < 1)
		data = 1;
	if(data > 31)
		data = 28;
	I2C__DS1307_Set(address, data, DS1307_DATE_REGISTER, DS1307_DATE_MASK);
}

/*
 * Realizar modificaciones del mes en el RTC
 * 		Requisitos previos->	I2C__DS1307_Init();
 * 		Entrada-> 				address:	direccion I2C del dispositivo a comunicarse
 * 								data: 		mes a colocar
 * 		Salida->				Ninguno
*/
void I2C__DS1307_SetMonth(uint8_t address, uint8_t data)
{
	if(data < 1)
		data = 1;
	if(data > 12)
		data = 12;
	I2C__DS1307_Set(address, data, DS1307_MONTH_REGISTER, DS1307_MONTH_MASK);
}

/*
 * Realizar modificaciones de años en el RTC
 * 		Requisitos previos->	I2C__DS1307_Init();
 * 		Entrada-> 				address:	direccion I2C del dispositivo a comunicarse
 * 								data: 		cantidad de años a colocar
 * 		Salida->				Ninguno
*/
void I2C__DS1307_SetYear(uint8_t address, uint8_t data)
{
	if(data > 99)
		data = 99;
	I2C__DS1307_Set(address, data, DS1307_YEAR_REGISTER, DS1307_YEAR_MASK);
}

/*
 * Obtener los segundos del RTC
 * 		Requisitos previos->	I2C__DS1307_Init();
 * 		Entrada-> 				address:	direccion I2C del dispositivo a comunicarse
 * 		Salida->				cantidad de segundos registrados en el RTC
*/
uint8_t I2C__DS1307_GetSecond(uint8_t address)
{
	return I2C__DS1307_Get(address, DS1307_SECOND_REGISTER, DS1307_SECOND_MASK);
}

/*
 * Obtener los minutos del RTC
 * 		Requisitos previos->	I2C__DS1307_Init();
 * 		Entrada-> 				address:	direccion I2C del dispositivo a comunicarse
 * 		Salida->				cantidad de minutos registrados en el RTC
*/
uint8_t I2C__DS1307_GetMinute(uint8_t address)
{
	return I2C__DS1307_Get(address, DS1307_MINUTE_REGISTER, DS1307_MINUTE_MASK);
}

/*
 * Obtener las horas del RTC
 * 		Requisitos previos->	I2C__DS1307_Init();
 * 		Entrada-> 				address:	direccion I2C del dispositivo a comunicarse
 * 		Salida->				cantidad de horas registradas en el RTC
*/
uint8_t I2C__DS1307_GetHour(uint8_t address)
{
	return I2C__DS1307_Get(address, DS1307_HOUR_REGISTER, DS1307_HOUR_MASK);
}

/*
 * Obtener el dia de la semana del RTC
 * 		Requisitos previos->	I2C__DS1307_Init();
 * 		Entrada-> 				address:	direccion I2C del dispositivo a comunicarse
 * 		Salida->				dia de la semana registrado en el RTC
*/
uint8_t I2C__DS1307_GetDay(uint8_t address)
{
	return I2C__DS1307_Get(address, DS1307_DAY_REGISTER, DS1307_DAY_MASK);
}

/*
 * Obtener dia del RTC
 * 		Requisitos previos->	I2C__DS1307_Init();
 * 		Entrada-> 				address:	direccion I2C del dispositivo a comunicarse
 * 		Salida->				dia registrado en el RTC
*/
uint8_t I2C__DS1307_GetDate(uint8_t address)
{
	return I2C__DS1307_Get(address, DS1307_DATE_REGISTER, DS1307_DATE_MASK);
}

/*
 * Obtener mes del RTC
 * 		Requisitos previos->	I2C__DS1307_Init();
 * 		Entrada-> 				address:	direccion I2C del dispositivo a comunicarse
 * 		Salida->				mes registrado en el RTC
*/
uint8_t I2C__DS1307_GetMonth(uint8_t address)
{
	return I2C__DS1307_Get(address, DS1307_MONTH_REGISTER, DS1307_MONTH_MASK);
}

/*
 * Obtener año del RTC
 * 		Requisitos previos->	I2C__DS1307_Init();
 * 		Entrada-> 				address:	direccion I2C del dispositivo a comunicarse
 * 		Salida->				año registrado en el RTC
*/
uint8_t I2C__DS1307_GetYear(uint8_t address)
{
	return I2C__DS1307_Get(address, DS1307_YEAR_REGISTER, DS1307_YEAR_MASK);
}

/*
 * Escritura de datos en la memoria RAM
 * 		Requisitos previos->	Ninguno
 * 		Entrada-> 				address:		direccion I2C del dispositivo a comunicarse
 * 								addressRAM: 	direccion virtual de la memoria RAM (desde 0x00 hasta 0x36) desde
 * 										la cual se comenzara a escribir
 * 								bytesToWrite:	cantidad de bytes a escribir a partir de la direccion virtual
 * 										de la memoria RAM dada
 * 								*data:			direccion del arreglo de datos a escribir
 * 		Salida->				Niguno
 * 		Notas->					La Memoria RAM va desde la direccion 0x00 logica (0x08 fisica) hasta la 0x36
 * 								logica (0x3E fisica) porque la direccion 0x37 logica (0x3F fisica) esta
 * 								reservada para propositos de configuraciones previas en el DS1307
*/
void I2C__DS1307_WriteRAM(uint8_t address, uint8_t addressRAM, uint8_t bytesToWrite, uint8_t *data)
{
	uint8_t contador = 0;
	if(bytesToWrite == 0)
		bytesToWrite = 1;
	if(addressRAM > DS1307_LAST_RAM_REGISTER - DS1307_FIRST_RAM_REGISTER)
		addressRAM = DS1307_LAST_RAM_REGISTER - DS1307_FIRST_RAM_REGISTER;
	if(addressRAM + (bytesToWrite - 1) > DS1307_LAST_RAM_REGISTER - DS1307_FIRST_RAM_REGISTER)
		addressRAM = DS1307_LAST_RAM_REGISTER - DS1307_FIRST_RAM_REGISTER - (bytesToWrite - 1);

	I2C__Start(address, I2C_Direction_Transmitter);
	I2C__Write(addressRAM + DS1307_FIRST_RAM_REGISTER);
	do
		I2C__Write(data[contador]);
	while(--bytesToWrite);
	I2C__Stop();
}

/*
 * Lectura de datos de la memoria RAM
 * 		Requisitos previos->	Ninguno
 * 		Entrada-> 				address:		direccion I2C del dispositivo a comunicarse
 * 								addressRAM: 	direccion virtual de la memoria RAM (desde 0x00 hasta 0x36) desde la cual se comenzara
 * 													a leer
 * 								bytesToRead:	cantidad de bytes a leer a partir de la direccion virtual
 * 													de la memoria RAM dada
 * 								*data:			direccion del arreglo de datos sobre el cual los datos seran
 * 													almacenados
 * 		Salida->				Ninguno
 * 		Notas->					La Memoria RAM va desde la direccion 0x00 logica (0x08 fisica) hasta la 0x36
 * 								logica (0x3E fisica) porque la direccion 0x37 logica (0x3F fisica) esta
 * 								reservada para propositos de configuraciones previas en el DS1307
*/
void I2C__DS1307_ReadRAM(uint8_t address, uint8_t addressRAM, uint8_t bytesToRead, uint8_t *data)
{
	uint8_t contador = 0;
	if(bytesToRead == 0)
		bytesToRead = 1;
	if(addressRAM > DS1307_LAST_RAM_REGISTER - DS1307_FIRST_RAM_REGISTER)
		addressRAM = DS1307_LAST_RAM_REGISTER - DS1307_FIRST_RAM_REGISTER;
	if(addressRAM + (bytesToRead - 1) > DS1307_LAST_RAM_REGISTER - DS1307_FIRST_RAM_REGISTER)
		addressRAM = DS1307_LAST_RAM_REGISTER - DS1307_FIRST_RAM_REGISTER - (bytesToRead - 1);

	I2C__Start(address, I2C_Direction_Transmitter);
	I2C__Write(addressRAM + DS1307_FIRST_RAM_REGISTER);
	I2C__Restart(address, I2C_Direction_Receiver);
	while(contador < bytesToRead - 1)
	{
		data[contador] = I2C__Read_ACK();
		contador++;
	}
	data[contador] = I2C__Read_NACK();
}

/*
 * Escribir en la direccion de memoria RAM reservada para indicar que el RTC seleccionado ya ha sido
 * registrado con una fecha
 *		Requisitos previos->	Ninguno
 * 		Entrada-> 				address:	direccion I2C del dispositivo a comunicarse
 * 		Salida->				Ninguno
*/
void I2C__DS1307_SetConfiguration(uint8_t address)
{
	I2C__Start(address, I2C_Direction_Transmitter);
	I2C__Write(DS1307_CONFIGURATION_REGISTER);
	I2C__Write(DS1307_PREVIOUS_CONFIGURATION);
	I2C__Stop();
}

/*
 * Escribir en la direccion de memoria RAM reservada para indicar que el RTC seleccionado no ha sido
 * programado con una fecha. Este es el metodo por software, por hardware bastara con quitar la
 * alimentacion principal y/o la bateria para que los datos en memoria RAM sean borrados
 * 		Requisitos previos->	Ninguno
 * 		Entrada-> 				address:	direccion I2C del dispositivo a comunicarse
 * 		Salida->				Ninguno
*/
void I2C__DS1307_ResetConfiguration(uint8_t address)
{
	I2C__Start(address, I2C_Direction_Transmitter);
	I2C__Write(DS1307_CONFIGURATION_REGISTER);
	I2C__Write(0x00);
	I2C__Stop();
}

/*
 * Leer la direccion de memoria RAM reservada la cual indicara si el dispositivo RTC ya fue
 * programado con una fecha anteriormente
 * 		Requisitos previos->	Ninguno
 * 		Entrada-> 				address:	direccion I2C del dispositivo a comunicarse
 * 		Salida->				DS1307_PREVIOUS_CONFIGURATION: 	si el dispositivo ya tiene una fecha guardada
 * 								otro caso: 						si el dispositivo no tiene ninguna fecha guardada
*/
uint8_t I2C__DS1307_ReadConfiguration(uint8_t address)
{
	I2C__Start(address, I2C_Direction_Transmitter);
	I2C__Write(DS1307_CONFIGURATION_REGISTER);
	I2C__Restart(address, I2C_Direction_Receiver);
	return I2C__Read_NACK();
}

//***********************************************************************************************
void I2C__DS1307_Set(uint8_t address, uint8_t data, uint8_t registerValue, uint8_t flagValue)
{
	I2C__Start(address, I2C_Direction_Transmitter);
	I2C__Write(registerValue);
	I2C__Write(BIN2BCD(data) & flagValue);
	I2C__Stop();
}

//***********************************************************************************************
uint8_t I2C__DS1307_Get(uint8_t address, uint8_t registerValue, uint8_t flagValue)
{
	I2C__Start(address, I2C_Direction_Transmitter);
	I2C__Write(registerValue);
	I2C__Restart(address, I2C_Direction_Receiver);
	return BCD2BIN(I2C__Read_NACK() & flagValue);
}
