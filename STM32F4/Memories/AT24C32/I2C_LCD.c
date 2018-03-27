#include <stm32f4xx.h>
#include <stm32f4xx_i2c.h>
#include "Delay.h"
#include "I2C.h"
#include "I2C_LCD.h"

uint8_t estadoActual;
uint8_t modo4Bits;

/*
 * Inicializar pantalla LCD encendida en formato de 4 bits y con el cursor encendido
 * 		Requisitos previos->	I2C__Init();
 * 		Entrada-> 				address:	direccion I2C del dispositivo a comunicarse
 * 		Salida->				Ninguno
*/
void I2C__LCD_Init(uint8_t address)
{
	Delay__Init(); //Inicializar funciones para delays

	modo4Bits = 0;

	//Comenzar con el modulo PCF8574 apagado y con el LCD encendido
	I2C__Start(address, I2C_Direction_Transmitter);
	I2C__Write(0x08);
	estadoActual = 0x08;
	I2C__Stop();

	//Inicializar pantalla I2C_LCD
	Delay__MS(20);
	I2C__LCD_Escribir(address, 0b00110000, 0);
	Delay__MS(5);
	I2C__LCD_Escribir(address, 0b00110000, 0);
	Delay__US(200);
	I2C__LCD_Escribir(address, 0b00110000,0);

	//Aqui las configuraciones principales
	I2C__LCD_4Bits2Lines5x8(address);
	I2C__LCD_IncrementarCursor(address);
	I2C__LCD_CursorON(address);
	I2C__LCD_Clear(address);
}

/*
 * Escribir un dato puro (sin conocer si es ASCII o numero). Se traducira en el valor de la tabla de
 * codigos CGROM contenida en la LCD
 * 		Requisitos previos->	I2C__LCD_Init();
 * 		Entrada-> 				address:	direccion I2C del dispositivo a comunicarse
 * 								data:		dato a escribir
 * 		Salida->				Ninguno
*/
void I2C__LCD_Write(uint8_t address, uint8_t data)
{
	I2C__LCD_Escribir(address, data, 1);
}

/*
 * Escribir una cadena de datos que se visualizaran en la pantalla
* 		Requisitos previos->	I2C__LCD_Init();
 * 		Entrada->				*data:	direccion del primer dato que sera escrito
* 		Salida->				Ninguno
*/
void I2C__LCD_WriteString(uint8_t address, uint8_t *data)
{
	while (*data)
		I2C__LCD_Escribir(address, *data++, 1);
}

/*
 * Escribir un numero en formato decimal
 *		Requisitos previos->	I2C__LCD_Init();
 *								Activar la unidad de punto flotante
 * 		Entrada->				address:		direccion I2C del dispositivo a comunicarse
 * 								number:			numero a ser escrito. El maximo numero permitido es de 32bits
 * 								decimalDigits:	cantidad de digitos decimales del numero que desean visualizarse en la pantalla
 * 		Salida->				Ninguno
*/
void I2C__LCD_WriteFloatingNumber(uint8_t address, float number, uint8_t decimalDigits)
{
	//Numeros negativos
	if (number < 0.0)
	{
		I2C__LCD_Escribir(address, '-', 1);
		number *= -1;
	}

	//Redondeo hacia arriba (Si le llega el numero 1.999 con redondeo de 2, imprime 2.00)
	float rounding = 0.5;
	for(uint8_t i = 0; i < decimalDigits; ++i)
		rounding /= 10.0;
	number += rounding;

	//Extraer la parte entera e imprimir
	uint32_t integerPart = (uint32_t)number;
	uint8_t datos[10];
	int8_t i = 0;
	if(integerPart == 0)
		I2C__LCD_Escribir(address, '0', 1);
	else
	{
		while(integerPart != 0)
		{
			datos[i] = (integerPart % 10);
			integerPart = (integerPart - datos[i]) / 10;
			i++;
		}
	}
	for(i--; i > -1; i--)
		I2C__LCD_Escribir(address, datos[i] + '0', 1);

	//Imprimir punto decimal si es necesario
	if (decimalDigits > 0)
		I2C__LCD_Escribir(address, '.', 1);

	//Extraer los digitos decimales e imprimirlos
	integerPart = (uint32_t)number;
	float remainder = number - (float)integerPart;
	while (decimalDigits-- > 0)
	{
		remainder *= 10.0;
		int32_t toPrint = (int32_t)(remainder);
		I2C__LCD_Escribir(address, toPrint + '0', 1);
		remainder -= toPrint;
	}
}


/*
 * Modo de configuracion: incrementos o decrementos
 * 		Requisitos previos->	I2C__LCD_Init();
 * 		Entrada-> 				address:			direccion I2C del dispositivo a comunicarse
 * 								incrementDecrement: modo incrementos o decrementos
 * 		Salida->				Ninguno
 * 		Notas->					Posibles entradas a utilizar:
 * 									LCD_Incrementos
 * 									LCD_Decrementos
*/
void I2C__LCD_EntryMode(uint8_t address, uint8_t incrementDecrement)
{
	I2C__LCD_Escribir(address, 0b00000100 | incrementDecrement, 0);
}

/*
 * Modo de configuracion: display, cursor y parpadeo del cursos
 * 		Requisitos previos->	I2C__LCD_Init();
 * 		Entrada-> 				address:		direccion I2C del dispositivo a comunicarse
 * 								displayONOFF: 	display encendido o apagado
 * 								cursorONOFF:	cursor encendido o apagado
 * 								blinkingONOFF:	parpadeo del cursos encendido o apagado
 * 		Salida->				Ninguno
 * 		Notas->					Posibles entradas a utilizar:
 * 									LCD_DisplayON
 * 									LCD_DisplayOFF
 * 									LCD_CursorON
 * 									LCD_CursorOFF
 * 									LCD_CursorBlinkingON
 * 									LCD_CursorBlinkingOFF 0b00000000
*/
void I2C__LCD_ControlDisplay(uint8_t address, uint8_t displayONOFF, uint8_t cursorONOFF, uint8_t blinkingONOFF)
{
	I2C__LCD_Escribir(address, 0b00001000 | displayONOFF | cursorONOFF | blinkingONOFF, 0);
}

/*
 * Modo de configuracion: desplazamientos del display y del cursor
 * 		Requisitos previos->	I2C__LCD_Init();
 * 		Entrada-> 				address:				direccion I2C del dispositivo a comunicarse
 * 								cursorDisplayShift: 	cursor o display en movimiento
 * 								direction:				movimientos hacia la izquierda o la derecha
 * 		Salida->				Ninguno
 * 		Notas->					Posibles entradas a utilizar:
 * 									LCD_DisplayShift
 * 									LCD_CursorMove
 * 									LCD_ShiftCursorRigth
 * 									LCD_ShiftCursorLeft
*/
void I2C__LCD_CursorDisplayShift(uint8_t address, uint8_t cursorDisplayShift, uint8_t direction)
{
	I2C__LCD_Escribir(address, 0b00010000 | cursorDisplayShift | direction, 0);
}

/*
 * Modo de configuracion: formato (4bits u 8 bits), numero de lineas de la pantalla y numero de
 * puntos por caracter
 * 		Requisitos previos->	I2C__LCD_Init();
 * 		Entrada-> 				address:		direccion I2C del dispositivo a comunicarse
 * 								dataLength: 	modo de 4 bits u 8 bits
 * 								displayLines:	numero de lineas de la pantalla
 * 								characterFont:	numero de puntos por caracter
 * 		Salida->				Ninguno
 * 		Notas->					Posibles entradas a utilizar:
 * 									LCD_8Bits
 * 									LCD_4Bits
 * 									LCD_2Lines
 * 									LCD_1Line
 * 									LCD_5x10Dots
 * 									LCD_5x8Dots
*/
void I2C__LCD_FunctionSet(uint8_t address, uint8_t dataLength, uint8_t displayLines, uint8_t characterFont)
{
	I2C__LCD_Escribir(address, 0b00100000 | dataLength | displayLines | characterFont, 0);
	if(dataLength == LCD_8Bits)
		modo4Bits = 0;
	else if(dataLength == LCD_4Bits)
		modo4Bits = 1;
	else
		asm("nop");
}

/*
 * Colocar el cursor en cierta position de una linea del LCD
 * 		Requisitos previos->	I2C__LCD_Init();
 * 		Entrada-> 				address:	direccion I2C del dispositivo a comunicarse
 * 								position: 	posicion en la cual el cursor debera colocarse
 * 								lineNumber:	linea en la cual se trabajara
 * 		Salida->				Ninguno
 * 		Notas->					Posibles entradas a utilizar:
 * 									LCD_Linea1 0b10000000
 * 									LCD_Linea2 0b11000000
*/
void I2C__LCD_SetDDRAM(uint8_t address, uint8_t position, uint8_t lineNumber)
{
	I2C__LCD_Escribir(address, (0b10000000 + position) | lineNumber, 0); //Direccion 00h de la DDRAM mas el valor de la position y linea
}

/*
 * Borra toda la pantalla, memoria DDRAM y pone el cursor al comienzo de la linea 1
 * 		Requisitos previos->	I2C__LCD_Init();
 * 		Entrada-> 				address:	direccion I2C del dispositivo a comunicarse
 * 		Salida->				Ninguno
*/
void I2C__LCD_Clear(uint8_t address)
{
	I2C__LCD_Escribir(address, 0b00000001, 0);
}

/*
 * Coloca el cursor al comienzo de la linea 1
 * 		Requisitos previos->	I2C__LCD_Init();
 * 		Entrada-> 				address:	direccion I2C del dispositivo a comunicarse
 * 		Salida->				Ninguno
*/
void I2C__LCD_Home(uint8_t address)
{
	I2C__LCD_Escribir(address, 0b00000010, 0);
}

/*
 * Interfaz de 4Bits, pantalla de 2 lineas, con caracteres de 5 x 7 puntos
 * 		Requisitos previos->	I2C__LCD_Init();
 * 		Entrada-> 				address:	direccion I2C del dispositivo a comunicarse
 * 		Salida->				Ninguno
*/
void I2C__LCD_4Bits2Lines5x8(uint8_t address)
{
	I2C__LCD_Escribir(address, 0b00101000, 0);
	modo4Bits = 1;
}

/*
 * Cada vez que se escriba un caracter, el cursos de desplazara a la derecha
 * 		Requisitos previos->	I2C__LCD_Init();
 * 		Entrada-> 				address:	direccion I2C del dispositivo a comunicarse
 * 		Salida->				Ninguno
*/
void I2C__LCD_IncrementarCursor(uint8_t address)
{
	I2C__LCD_Escribir(address, 0b00000110, 0);
}

/*
 * Cursor al comienzo de la linea 1
 * 		Requisitos previos->	I2C__LCD_Init();
 * 		Entrada-> 				address:	direccion I2C del dispositivo a comunicarse
 * 		Salida->				Ninguno
*/
void I2C__LCD_Linea1(uint8_t address)
{
	I2C__LCD_Escribir(address, 0b10000000, 0); //Direccion 00h de la DDRAM
}

/*
 * Cursor al comienzo de la linea 2
 * 		Requisitos previos->	I2C__LCD_Init();
 * 		Entrada-> 				address:	direccion I2C del dispositivo a comunicarse
 * 		Salida->				Ninguno
*/
void I2C__LCD_Linea2 (uint8_t address)
{
	I2C__LCD_Escribir(address, 0b11000000, 0); //Direccion 40h de la DDRAM
}

/*
 * Desplazar el cursor cierta cantidad de posiciones en la linea 1
 * 		Requisitos previos->	I2C__LCD_Init();
 * 		Entrada-> 				address:	direccion I2C del dispositivo a comunicarse
 * 								position:	numero de posiciones a desplazar el cursor dentro de la linea
 * 		Salida->				Ninguno
*/
void I2C__LCD_positionLinea1(uint8_t address, uint8_t position)
{
	I2C__LCD_Escribir(address, 0b10000000 + position, 0); //Direccion 00h de la DDRAM mas el valor de la position
}

/*
 * Desplazar el cursor cierta cantidad de posiciones en la linea 2
 * 		Requisitos previos->	I2C__LCD_Init();
 * 		Entrada-> 				address:	direccion I2C del dispositivo a comunicarse
 * 								position:	numero de posiciones a desplazar el cursor dentro de la linea
 * 		Salida->				Ninguno
*/
void I2C__LCD_positionLinea2(uint8_t address, uint8_t position)
{
	I2C__LCD_Escribir(address, 0b11000000 + position, 0); //Direccion 40h de la DDRAM mas el valor de la position
}

/*
 * Apagar la pantalla LCD
 * 		Requisitos previos->	I2C__LCD_Init();
 * 		Entrada-> 				address:	direccion I2C del dispositivo a comunicarse
 * 		Salida->				Ninguno
*/
void I2C__LCD_OFF(uint8_t address)
{
	I2C__LCD_Escribir(address, 0b00001000, 0);
}

/*
 * Pantalla encendida y cursor encendido
 * 		Requisitos previos->	I2C__LCD_Init();
 * 		Entrada-> 				address:	direccion I2C del dispositivo a comunicarse
 * 		Salida->				Ninguno
*/
void I2C__LCD_CursorON(uint8_t address)
{
	I2C__LCD_Escribir(address, 0b00001110, 0);
}

/*
 * Pantalla encendida y cursor apagado
 * 		Requisitos previos->	I2C__LCD_Init();
 * 		Entrada-> 				address:	direccion I2C del dispositivo a comunicarse
 * 		Salida->				Ninguno
*/
void I2C__LCD_CursorOFF(uint8_t address)
{
	I2C__LCD_Escribir(address, 0b00001100, 0);
}

//***********************************************************************************************
void I2C__LCD_Escribir(uint8_t address, uint8_t dato, uint8_t tipo)
{
	estadoActual = 0x08;
	I2C__Start(address, I2C_Direction_Transmitter);

	if(tipo == 0)
		I2C__Write(estadoActual); //Comando
	else
	{
		estadoActual = 0b00000001 | estadoActual;
		I2C__Write(estadoActual); //Dato
	}

	estadoActual = 0b00000100 | estadoActual; //Flanco de subida para comenzar a escribir
	I2C__Write(estadoActual);

	estadoActual = estadoActual | (dato & 0b11110000); //Mandar dato parte alta
	I2C__Write(estadoActual);

	estadoActual = 0b11111001 & estadoActual; //Capturar dato sin perdida de informacion
	I2C__Write(estadoActual);

	I2C__Stop();

	if(tipo == 0)
		Delay__MS(2);
	else
		Delay__US(50);

	if(modo4Bits == 1)
	{
		estadoActual = estadoActual & 0b00001111;
		I2C__Start(address, I2C_Direction_Transmitter);

		estadoActual = 0b00000100 | (estadoActual & 0b00001001); //Flanco de subida para comenzar a escribir segunda parte
		I2C__Write(estadoActual);

		estadoActual = estadoActual | (dato << 4); //Mandar dato parte baja
		I2C__Write(estadoActual);

		estadoActual = 0b11111001 & estadoActual; //Capturar dato sin perdida de informacion
		I2C__Write(estadoActual);

		I2C__Stop();

		if(tipo == 0)
			Delay__MS(2);
		else
			Delay__US(50);
	}
}
