
#include "stm32f4xx.h"
#define WIFI_MODE_CLIENT	"AT+CWMODE=1\r"
#define WIFI_MODE_HOST		"AT+CWMODE=2\r"
#define WIFI_MODE_DUAL		"AT+CWMODE=3\r"
#define WIFI_CONNECT_TO_AP	"AT+CWJAP=SSID, PASSWORD\r"
#define WIFI_LIST_AP		"AT+CWLAP\r"
#define WIFI_WEB_SERVER		"AT+CIPSERVER=1,80\r\n"
#define WIFI_MULTIPLE_MODE	"AT+CIPMUX=1\r\n"
#define WIFI_SINGLE_MODE	"AT+CIPMUX=0\r"
#define WIFI_SEND			"AT+CIPSEND=1,"



extern uint8_t OK_FLAG;
extern uint8_t CONNECT_FLAG;

/* Inicializa el móduloESP8266 */
void WiFi_init(void);

/* Manda una cadena de caracteres hacia el dispositivo conectado */
void WiFi_Connect(volatile char* SSID, volatile char* Password);

void USART_Send(volatile char *s);

void WiFi_HTTP_Server(void);
