#include "Vitri.h"

#define INPUT	GPIO_Mode_IN
#define	OUTPUT	GPIO_Mode_OUT

GPS_t GPS_Structure;
static uint32_t multiplier;

static void PinMode(GPIO_TypeDef* GPIOx, uint32_t GPIO_PIN, uint8_t MODE);

static void DelayMillis(uint32_t millis) {
    millis = 1000 * millis * multiplier - 10;
    while (millis--);
}

void Init_System(void){
	RCC_ClocksTypeDef RCC_Clocks;

	RCC_GetClocksFreq(&RCC_Clocks);
	multiplier = RCC_Clocks.HCLK_Frequency / 4000000;

	/* Iniciamos el módulo de GSM */
	DelayMillis(3500);	/* Esperamos a que el modulo GSM entre en modo de entrada AT */

	GSM_init();
	GSM_Send(Check);		/* Verificamos que este conectado */
	while(!GSM_OK_Flag);	/* Espperamos a que responda el dispositivo */
	GSM_OK_Flag = 0;

	/* iniciamos el modulo de GPS */
	GPS_init(&GPS_Structure, UTC_6);
	GPS_Stop();

	/* Inicialzacíon del la IMU */

	/* Inicialización del pin */
	GPIOA->BSRRH = GPIO_Pin_0;
	PinMode(GPIOA, GPIO_Pin_0, OUTPUT);
}

static void PinMode(GPIO_TypeDef* GPIOx, uint32_t GPIO_PIN, uint8_t MODE){
	GPIO_InitTypeDef 	   GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = MODE;
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOx, &GPIO_InitStructure);
}
