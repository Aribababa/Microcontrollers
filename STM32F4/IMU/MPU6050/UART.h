#ifndef UART_H_
#define UART_H_

void UART__Init(void);
void UART__Write(uint8_t data);
void UART__WriteString(uint8_t *data);
uint8_t UART__Read(void);
void UART__WriteFloatingNumber(float number, uint8_t decimalDigits);

#endif
