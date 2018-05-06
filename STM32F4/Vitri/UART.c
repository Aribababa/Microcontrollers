#include "stm32f4xx.h"
#include "UART.h"

/*
 * Inicializar el modulo UART5: TX->C12 y RX->D2 a una velocidad de 9600, 1 bit de stop y sin paridad
 * 		Requisitos previos->	Ninguno
 * 		Entrada->				Ninguno
 * 		Salida->				Ninguno
*/
void UART__Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5); //TX
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5); //RX

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStruct);

	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = 9600;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(UART5, &USART_InitStruct);

	USART_Cmd(UART5, ENABLE);
}

/*
 * Enviar un dato que se visualizara en la consola en formato ASCII
* 		Requisitos previos->	UART__Init();
 * 		Entrada->				data:	dato que sera enviado por el pin TX
 * 		Salida->				Ninguno
*/
void UART__Write(uint8_t data)
{
	//Esperar a que el buffer de transmision este vacio para despues enviar
	while (!USART_GetFlagStatus(UART5, USART_FLAG_TXE));
	USART_SendData(UART5, data);
}

/*
 * Enviar una cadena de datos que se visualizaran en la consola en formato ASCII
* 		Requisitos previos->	UART__Init();
 * 		Entrada->				*data:	direccion del primer dato que sera enviado por el pin TX
* 		Salida->				Ninguno
*/
void UART__WriteString(uint8_t *data)
{
	while (*data)
		UART__Write(*data++);
}

/*
 * Recibir un dato en formato ASCII
 * 		Requisitos previos->	UART__Init();
 * 		Entrada->				Ninguno
 * 		Salida->				Dato leido por el pin RX en formato ASCII
*/
uint8_t UART__Read(void)
{
	//Esperar a que haya un dato en el buffer para regresarlo
	while (!USART_GetFlagStatus(UART5, USART_FLAG_RXNE));
	return (uint8_t)USART_ReceiveData(UART5);
}

/*
 * Enviar un numero en formato decimal
 *		Requisitos previos->	UART__Init();
 *								Activar la unidad de punto flotante
 * 		Entrada->				number:			numero a ser enviado. El maximo numero permitido es de 32bits
 * 								decimalDigits:	cantidad de digitos decimales del numero que desean visualizarse en consola
 * 		Salida->				Ninguno
*/
void UART__WriteFloatingNumber(float number, uint8_t decimalDigits)
{
	//Numeros negativos
	if (number < 0.0)
	{
		UART__Write('-');
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
		UART__Write('0');
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
		UART__Write(datos[i] + '0');

	//Imprimir punto decimal si es necesario
	if (decimalDigits > 0)
		UART__Write('.');

	//Extraer los digitos decimales e imprimirlos
	integerPart = (uint32_t)number;
	float remainder = number - (float)integerPart;
	while (decimalDigits-- > 0)
	{
		remainder *= 10.0;
		int32_t toPrint = (int32_t)(remainder);
		UART__Write(toPrint + '0');
		remainder -= toPrint;
	}
}

/*
 * Enviar un numero en formato decimal, hexadecimal, octal o binario para que sea escrito caracter a caracter
 *		Requisitos previos->	UART__Init();
 * 		Entrada->				base:				formato del numero que desea visualizarse. Pueden usarse las constantes
 * 														HEXADECIMAL_NUMBER, DECIMAL_NUMBER, OCTAL_NUMBER o BINARY_NUMBER para
 * 														la visualizacion de cada tipo de numero respectivamente
 * 								baseVisualization:	escribir al final del numero la base de este. Pueden usarse las constantes
 * 														TRUE o FALSE. Ejemplo:
 * 														-- UART__WriteNumber(123, DECIMAL_NUMBER, TRUE)
 * 															se escribira 123d, indicando que es un numero decimal
 * 														-- UART__WriteNumber(456, OCTAL_NUMBER, FALSE)
 * 															se escribira 456, sin indicar que es un numero octal
 * 		Salida->				Ninguno
*/
void UART__WriteNumber(uint32_t number, uint8_t base, uint8_t baseVisualization)
{
	uint8_t numero;

	//Conocer el orden del array que almacenara los digitos en el formato de la base
	if(base == HEXADECIMAL_NUMBER)
		numero = 8;
	if(base == DECIMAL_NUMBER)
		numero = 10;
	if(base == OCTAL_NUMBER)
		numero = 11;
	if(base == BINARY_NUMBER)
		numero = 32;
	else
		numero = 8;

	uint8_t digitos[numero];
	int8_t i = 0;

	if(number == 0)
	{
		UART__Write('0');
	}
	else
	{
		//Busqueda de los digitos del numero en el formato correspondiente
		while(number)
		{
			numero = number % base;

			//Para el caso de base hexadecimal
			if(numero > 9)
			{
				if(numero == 10)
					digitos[i]= 'A';
				else if(numero == 11)
					digitos[i]= 'B';
				else if(numero == 12)
					digitos[i]= 'C';
				else if(numero == 13)
					digitos[i]= 'D';
				else if(numero == 14)
					digitos[i]= 'E';
				else if(numero == 15)
					digitos[i]= 'F';
				else
					digitos[i]= '@'; //Ha ocurrido un error muy grave
			}
			else
				digitos[i]= numero + '0';

			number = (int32_t)(number / base);
			i++;
		}
	}

	//Visualizar el numero en consola
	for(i--; i > -1; i--)
		UART__Write(digitos[i]);

	//Poner la base del numero al final de este
	if(baseVisualization == TRUE)
	{
		if(base == HEXADECIMAL_NUMBER)
			UART__Write('h');
		else if(base == DECIMAL_NUMBER)
			UART__Write('d');
		else if(base == OCTAL_NUMBER)
			UART__Write('o');
		else if(base == BINARY_NUMBER)
			UART__Write('b');
		else
			UART__Write('@'); //Ha ocurrido un error muy grave
	}
}
