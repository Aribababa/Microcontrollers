#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "WiFi.h"	/* Para el módulo ESP8266 */
#include "WebPage.h"
			
uint8_t StateCounter = 0;
volatile char *Commands[2] = {WIFI_MULTIPLE_MODE, WIFI_WEB_SERVER};

int main(void){

	WiFi_init();

	USART_Send("AT\r\n");/* Si responde, comezará el programa */

	for(;;){
		if(OK_FLAG && StateCounter < 2){
			OK_FLAG = 0;
			USART_Send(Commands[StateCounter++]);
		}

		if(CONNECT_FLAG){
			StateCounter = 3;
			USART_Send(WIFI_SEND);
			USART_Send(PageBuffer);
			USART_Send(WebPage);
		}
	}
}
