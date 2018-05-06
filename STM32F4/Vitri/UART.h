#ifndef UART_H_
#define UART_H_

#define HEXADECIMAL_NUMBER 	16
#define DECIMAL_NUMBER 		10
#define OCTAL_NUMBER 		8
#define BINARY_NUMBER 		2

#define TRUE	1
#define FALSE	0

void UART__Init(void);
void UART__Write(uint8_t data);
void UART__WriteString(uint8_t *data);
void UART__WriteNumber(uint32_t number, uint8_t base, uint8_t baseVisualization);
uint8_t UART__Read(void);
void UART__WriteFloatingNumber(float number, uint8_t decimalDigits);

#endif
