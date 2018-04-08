#ifndef SPI_MCP4131_H_
#define SPI_MCP4131_H_

#define MCP4131_Resolution7Bits 	128
#define MCP4131_ResistanceW_Typical 117.5

#define MCP4131_ADDRESS_WIPER0 				0x00
#define MCP4131_ADDRESS_RESISTANCE_VALUE 	0x0C

#define MCP4131_BY_PERCENTAJE 0
#define MCP4131_BY_RESISTANCE 1

//PUBLICAS
void SPI__MCP4131_Init(uint32_t resistancePinAtoPinB);
void SPI__MCP4131_SetValues(uint8_t values);
uint32_t SPI__MCP4131_GetResistance(void);
void SPI__MCP4131_SetResistance(uint32_t resistance);

//PROTEGIDAS
uint32_t MCP4131__StepSize(void);
uint16_t MCP4131__Resistance2Wiper(uint32_t resistance);
uint32_t MCP4131__Wiper2Ohms(uint16_t wiper);
void MCP4131__Write(uint8_t commandByte, uint8_t dataByte);
uint16_t MCP4131__Read(uint8_t commandByte);
void MCP4131__WiperPosition(uint8_t potentiometerNumber, uint16_t wiper);

//GENERALIZADAS
uint16_t byte2uint16(uint8_t byteHigh, uint8_t byteLow);

#endif
