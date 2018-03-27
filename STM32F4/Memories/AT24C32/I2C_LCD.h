#ifndef I2C_LCD_H_
#define I2C_LCD_H_

#define LCD_Incrementos 		0b00000010
#define LCD_Decrementos 		0b00000000
#define LCD_DisplayON 			0b00000100
#define LCD_DisplayOFF 			0b00000000
#define LCD_CursorON 			0b00000010
#define LCD_CursorOFF 			0b00000000
#define LCD_CursorBlinkingON 	0b00000001
#define LCD_CursorBlinkingOFF 	0b00000000
#define LCD_DisplayShift 		0b00001000
#define LCD_CursorMove 			0b00000000
#define LCD_ShiftCursorRigth 	0b00000100
#define LCD_ShiftCursorLeft 	0b00000000
#define LCD_8Bits 				0b00010000
#define LCD_4Bits 				0b00000000
#define LCD_2Lines 				0b00001000
#define LCD_1Line 				0b00000000
#define LCD_5x10Dots 			0b00000100
#define LCD_5x8Dots 			0b00000000
#define LCD_Linea1 				0b10000000
#define LCD_Linea2 				0b11000000

//Funciones para el usuario
void I2C__LCD_Init(uint8_t address);

void I2C__LCD_EntryMode(uint8_t address, uint8_t incrementDecrement);
void I2C__LCD_ControlDisplay(uint8_t address, uint8_t displayONOFF, uint8_t cursorONOFF, uint8_t blinkingONOFF);
void I2C__LCD_CursorDisplayShift(uint8_t address, uint8_t cursorDisplayShift, uint8_t direction);
void I2C__LCD_FunctionSet(uint8_t address, uint8_t dataLength, uint8_t displayLines, uint8_t characterFont);
void I2C__LCD_SetDDRAM(uint8_t address, uint8_t position, uint8_t lineNumber);

void I2C__LCD_Clear(uint8_t address);
void I2C__LCD_Home(uint8_t address);
void I2C__LCD_4Bits2Lines5x8(uint8_t address);
void I2C__LCD_IncrementarCursor(uint8_t address);
void I2C__LCD_Linea1(uint8_t address);
void I2C__LCD_Linea2 (uint8_t address);
void I2C__LCD_positionLinea1(uint8_t address, uint8_t position);
void I2C__LCD_positionLinea2(uint8_t address, uint8_t position);
void I2C__LCD_OFF(uint8_t address);
void I2C__LCD_CursorON(uint8_t address);
void I2C__LCD_CursorOFF(uint8_t address);

void I2C__LCD_Write(uint8_t address, uint8_t data);
void I2C__LCD_WriteString(uint8_t address, uint8_t *data);
void I2C__LCD_WriteFloatingNumber(uint8_t address, float number, uint8_t decimalDigits);

//Funciones para la libreria
void I2C__LCD_Escribir(uint8_t address, uint8_t dato, uint8_t tipo);

#endif
