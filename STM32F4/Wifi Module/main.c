#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "WiFi.h"	/* Para el módulo ESP8266 */
#define wait(X)	while(!X)

uint8_t StateCounter = 0;
volatile char *Commands[2] = {WIFI_MULTIPLE_MODE, WIFI_WEB_SERVER};

int main(void){

	WiFi_init();

	USART_Send("AT\r\n");/* Si responde OK, comezará el programa */
	while(!OK_FLAG);
	OK_FLAG = 0;

	USART_Send(WIFI_MULTIPLE_MODE);
	while(!OK_FLAG);
	OK_FLAG = 0;

	USART_Send(WIFI_WEB_SERVER);
	while(!OK_FLAG);
	OK_FLAG = 0;


	for(;;){

		/* Si llega una solicitud, nos preparamos para enviar datos */
		if(CLIENT_CONNECTED_FLAG){
			CLIENT_CONNECTED_FLAG = 0;

			USART_Send("AT+CIPSEND=");
			USART_Send_Char(client);
			USART_Send(",12\r\n");
		}

		/* Ya que aparezaca la Promp,  comenzamnos a mondar vlos datos */
		if(PROMP_FLAG){
			PROMP_FLAG = 0;
			USART_Send("<h2>oki</h2>");

			while(!OK_FLAG);
			OK_FLAG = 0;

			USART_Send("AT+CIPCLOSE=");
			USART_Send_Char(client);
			USART_Send("\r\n");
		}
	}
}




