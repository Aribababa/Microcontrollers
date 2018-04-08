#ifndef SPI_H_
#define SPI_H_

void SPI__Init(void);
void SPI__Write(uint8_t data);
uint8_t SPI__WriteRead(uint8_t data);
uint8_t SPI__Read(void);

void SPI__SlaveEnable(void);
void SPI__SlaveDisable(void);

#endif
