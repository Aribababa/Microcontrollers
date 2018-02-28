#ifndef I2C_LCD_H_
#define I2C_LCD_H_

#define _Incrementos 0b00000010
#define _Decrementos 0b00000000
#define _DisplayON 0b00000100
#define _DisplayOFF 0b00000000
#define _CursorON 0b00000010
#define _CursorOFF 0b00000000
#define _CursorBlinkingON 0b00000001
#define _CursorBlinkingOFF 0b00000000
#define _DisplayShift 0b00001000
#define _CursorMove 0b00000000
#define _ShiftCursorRigth 0b00000100
#define _ShiftCursorLeft 0b00000000
#define _8Bits 0b00010000
#define _4Bits 0b00000000
#define _2Lines 0b00001000
#define _1Line 0b00000000
#define _5x10Dots 0b00000100
#define _5x8Dots 0b00000000
#define _Linea1 0b10000000
#define _Linea2 0b11000000

//Inicializar modulo PCF8574 y I2C_LCD
void I2C__LCD_Init(uint8_t address);

//Comandos completos al I2C_LCD
void I2C__LCD_EntryMode(uint8_t address, uint8_t incrementDecrement);
void I2C__LCD_ControlDisplay(uint8_t address, uint8_t displayONOFF, uint8_t cursorONOFF, uint8_t blinkingONOFF);
void I2C__LCD_CursorDisplayShift(uint8_t address, uint8_t cursorDisplayShift, uint8_t direction);
void I2C__LCD_FunctionSet(uint8_t address, uint8_t dataLength, uint8_t displayLines, uint8_t characterFont);
void I2C__LCD_SetDDRAM(uint8_t address, uint8_t posicion, uint8_t lineNumber);

//Comandos resumidos al I2C_LCD
void I2C__LCD_Clear(uint8_t address);
void I2C__LCD_Home(uint8_t address);
void I2C__LCD_4Bits2Lines5x8(uint8_t address);
void I2C__LCD_IncrementarCursor(uint8_t address);
void I2C__LCD_Linea1(uint8_t address);
void I2C__LCD_Linea2 (uint8_t address);
void I2C__LCD_PosicionLinea1(uint8_t address, uint8_t posicion);
void I2C__LCD_PosicionLinea2(uint8_t address, uint8_t posicion);
void I2C__LCD_OFF(uint8_t address);
void I2C__LCD_CursorON(uint8_t address);
void I2C__LCD_CursorOFF(uint8_t address);

//Escritura de datos
void I2C__LCD_Write(uint8_t address, uint8_t character);

//Funciones estaticas
void I2C__LCD_Inicializar(uint8_t address);
void I2C__LCD_Escribir(uint8_t address, uint8_t dato, uint8_t tipo);

#endif
