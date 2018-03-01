#include <stm32f4xx.h>
#include <stm32f4xx_i2c.h>
#include "I2C.h"
#include "I2C_LCD.h"
#include "Delay.h"

uint8_t estadoActual;
uint8_t modo4Bits = 0;

//***********************************************************
//
// _Incrementos 0b00000010
// _Decrementos 0b00000000
//
void I2C__LCD_EntryMode(uint8_t address, uint8_t incrementDecrement)
{
	I2C__LCD_Escribir(address, 0b00000100 | incrementDecrement, 0);
}

//***********************************************************
//
// _DisplayON 0b00000100
// _DisplayOFF 0b00000000
// _CursorON 0b00000010
// _CursorOFF 0b00000000
// _CursorBlinkingON 0b00000001
// _CursorBlinkingOFF 0b00000000
//
void I2C__LCD_ControlDisplay(uint8_t address, uint8_t displayONOFF, uint8_t cursorONOFF, uint8_t blinkingONOFF)
{
	I2C__LCD_Escribir(address, 0b00001000 | displayONOFF | cursorONOFF | blinkingONOFF, 0);
}

//***********************************************************
//
// _DisplayShift 0b00001000
// _CursorMove 0b00000000
// _ShiftCursorRigth 0b00000100
// _ShiftCursorLeft 0b00000000
//
void I2C__LCD_CursorDisplayShift(uint8_t address, uint8_t cursorDisplayShift, uint8_t direction)
{
	I2C__LCD_Escribir(address, 0b00010000 | cursorDisplayShift | direction, 0);
}

//***********************************************************
//
// _8Bits 0b00010000
// _4Bits 0b00000000
// _2Lines 0b00001000
// _1Line 0b00000000
// _5x10Dots 0b00000100
//_5x8Dots 0b00000000
//
void I2C__LCD_FunctionSet(uint8_t address, uint8_t dataLength, uint8_t displayLines, uint8_t characterFont)
{
	I2C__LCD_Escribir(address, 0b00100000 | dataLength | displayLines | characterFont, 0);
}

//***********************************************************
//#define _Linea1 0b10000000
//#define _Linea2 0b11000000
//***********************************************************
void I2C__LCD_SetDDRAM(uint8_t address, uint8_t posicion, uint8_t lineNumber)
{
	I2C__LCD_Escribir(address, (0b10000000 + posicion) | lineNumber, 0); //Direccion 00h de la DDRAM mas el valor de la posicion y linea
}

//***********************************************************
//Borra toda la pantalla, memoria DDRAM y pone el cursor al comienzo de la linea 1
void I2C__LCD_Clear(uint8_t address)
{
	I2C__LCD_Escribir(address, 0b00000001, 0);
}

//***********************************************************
//Cursor al principio de la linea 1
void I2C__LCD_Home(uint8_t address)
{
	I2C__LCD_Escribir(address, 0b00000010, 0);
}

//***********************************************************
//Interfaz de 4Bits, pantalla de 2 lineas, con caracteres de 5 x 7
void I2C__LCD_4Bits2Lines5x8(uint8_t address)
{
	I2C__LCD_Escribir(address, 0b00101000, 0);
	modo4Bits = 1;
}

//***********************************************************
void I2C__LCD_IncrementarCursor(uint8_t address)
{
	I2C__LCD_Escribir(address, 0b00000110, 0);
}

//Cursor al principio de la linea 1
void I2C__LCD_Linea1(uint8_t address)
{
	I2C__LCD_Escribir(address, 0b10000000, 0); //Direccion 00h de la DDRAM
}

//***********************************************************
//Cursor al principio de la linea 2
void I2C__LCD_Linea2 (uint8_t address)
{
	I2C__LCD_Escribir(address, 0b11000000, 0); //Direccion 40h de la DDRAM
}

//***********************************************************
//Cursor a posicion de la linea 1
void I2C__LCD_PosicionLinea1(uint8_t address, uint8_t posicion)
{
	I2C__LCD_Escribir(address, 0b10000000 + posicion, 0); //Direccion 00h de la DDRAM mas el valor de la posicion
}

//***********************************************************
//Cursor a posicion de la linea 2
void I2C__LCD_PosicionLinea2(uint8_t address, uint8_t posicion)
{
	I2C__LCD_Escribir(address, 0b11000000 + posicion, 0); //Direccion 40h de la DDRAM mas el valor de la posicion
}

//***********************************************************
//Apagar pantalla
void I2C__LCD_OFF(uint8_t address)
{
	I2C__LCD_Escribir(address, 0b00001000, 0);
}

//***********************************************************
//Pantalla encendida y cursor encendido
void I2C__LCD_CursorON(uint8_t address)
{
	I2C__LCD_Escribir(address, 0b00001110, 0);
}

//***********************************************************
//Pantalla encendida y cursor apagado
void I2C__LCD_CursorOFF(uint8_t address)
{
	I2C__LCD_Escribir(address, 0b00001100, 0);
}

//***********************************************************
//Inicializar LCD
void I2C__LCD_Init(uint8_t address)
{
	Delay__Init();

	//Comenzar con el modulo PCF8574 apagado y con el LCD encendido
	I2C__Start(address, I2C_Direction_Transmitter);
	I2C__Write(0x08);
	estadoActual = 0x08;
	I2C__Stop();

	//Inicializar pantalla I2C_LCD
	I2C__LCD_Inicializar(address);
}

//***********************************************************
//Escribir character
void I2C__LCD_Write(uint8_t address, uint8_t character)
{
	I2C__LCD_Escribir(address, character, 1);
}

//***********************************************************
void I2C__LCD_Inicializar(uint8_t address)
{
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

//***********************************************************
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
