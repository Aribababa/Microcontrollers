#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "GPS.h"
#include "GSM.h"

#define Arduino_Uno	0x01
#define Turn_On_GSM			GPIOA->BSRRH = GPIO_Pin_0
#define Turn_Off_GSM		GPIOA->BSRRL = GPIO_Pin_0



/* Variables del sistema */
extern GPS_t GPS_Structure;

/* Inicializa y verifica que todos los móduls que estan en la tarjeta
 * esten funcionando correctaente, antes de correr el sistema
 * */
void Init_System(void);


