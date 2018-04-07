#include "stm32f4_discovery.h"
#include "stm32f4xx.h"
#include "CAN.h"

/* Estructuras de los frames de CAN */
CanRxMsg RxMessage;
CanTxMsg TxMessage;

int main(void) {
	 (void)CAN_Config(CAN2);	/* Configuramos el módulo de CAN */

	 SysTick_Config(SystemCoreClock/440);
	for(;;){

		/* Si llega un mensaje de CAN */
		if(CAN2_RX_FLAG){
			CAN2_RX_FLAG = 0;
			CAN_Receive(CAN2, CAN_FIFO0, &RxMessage);
		}
	}
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line){
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1);
}
#endif
