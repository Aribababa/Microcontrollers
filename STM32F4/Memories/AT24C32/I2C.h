#ifndef I2C_H_
#define I2C_H_

void I2C__Init(void);
void I2C__Start(uint8_t address, uint8_t direction);
void I2C__Restart(uint8_t address, uint8_t direction);
void I2C__Write(uint8_t data);
uint8_t I2C__Read_ACK(void);
uint8_t I2C__Read_NACK(void);
void I2C__Stop(void);

#endif
