#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "Vitri.h"


int main(void){
	Init_System();	/* Iniciammos la rutina de inicialización y verificación */
	Turn_Off_GSM;	/* Apagamos el GSM ppara ahorrar energía */


	for(;;){
		if(Incoming_call_flag){
			Incoming_call_flag = 0;
		}
	}
	return Arduino_Uno;
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line){
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	printf("El Arduino dejó de funcionar, profe");
	while(1);
}
#endif
