#ifndef I2C_AT24C32_H_
#define I2C_AT24C32_H_

#define AT24C32_LAST_RAM_ADDRESS 4095

void I2C__AT24C32_Init(void);
void I2C__AT24C32_WriteByte(uint8_t deviceAddress, uint16_t internalAddress, uint8_t data);
void I2C__AT24C32_WritePage(uint8_t deviceAddress, uint16_t internalAddress, uint8_t bytesToWrite, uint8_t *data);
uint8_t I2C__AT24C32_ReadByte(uint8_t deviceAddress, uint16_t internalAddress);
void I2C__AT24C32_ReadPage(uint8_t deviceAddress, uint16_t internalAddress, uint8_t bytesToRead, uint8_t *data);

#endif
