#include "Vitri.h"

#define INPUT	GPIO_Mode_IN
#define	OUTPUT	GPIO_Mode_OUT

GPS_t GPS_Structure;
IMU_t IMU_Structure;
unsigned char SysTick_Flag = 0;

volatile float angleY[ANGLE_BUFFER];
volatile float angleX[ANGLE_BUFFER];
volatile float angleZ[ANGLE_BUFFER];

unsigned char angle_index;
static uint32_t multiplier;

static void PinMode(GPIO_TypeDef* GPIOx, uint32_t GPIO_PIN, uint8_t MODE);
//static void DelayMillis(uint32_t millis);


void Init_System(void){
	STM_EVAL_LEDInit(LED4);
	RCC_ClocksTypeDef RCC_Clocks;

	PinMode(GPIOB, GPIO_Pin_5, OUTPUT);
	Turn_On_GSM;

	RCC_GetClocksFreq(&RCC_Clocks);
	multiplier = RCC_Clocks.HCLK_Frequency / 4000000;

	/* Iniciamos el módulo de GSM */
	//DelayMillis(3500);	/* Esperamos a que el modulo GSM entre en modo de entrada AT */

	GSM_init();
	GSM_Send(Check);		/* Verificamos que este conectado */

//	while(!GSM_OK_Flag);	/* Espperamos a que responda el dispositivo */
//	GSM_OK_Flag = 0;

	/* Inicilizacion de la IMU */

	I2C__Init();
	I2C__MPU6050_Init(MPU6050_ADDRESS);

	/* iniciamos el modulo de GPS */
	GPS_init(&GPS_Structure, UTC_6);
	GPS_Stop();

	/* Muestreo de las medidas */
	SysTick_Config(SystemCoreClock/50);

}

static void PinMode(GPIO_TypeDef* GPIOx, uint32_t GPIO_PIN, uint8_t MODE){
	GPIO_InitTypeDef 	   GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Mode = MODE;
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOx, &GPIO_InitStructure);
}

/*
static void DelayMillis(uint32_t millis) {
	millis = 1000 * millis * multiplier - 10;
	while (millis--);
}
*/

/* Interrupciones del sistema */
ISR(SysTick_Handler){
	STM_EVAL_LEDToggle(LED4);
	SysTick_Flag = 1;
}

