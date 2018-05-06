#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "GPS.h"
#include "GSM.h"
#include "I2C.h"
#include "I2C_MPU6050.h"
#include "UART.h"
#include "arm_math.h"

#define ISR(X)	void X(void)


#define Arduino_Uno	0x01
#define Turn_Off_GSM		GPIOB->BSRRH = GPIO_Pin_12;

#define IMU_t	MPU6050
#define ANGLE_BUFFER 	256

/* Variables del sistema */
extern GPS_t GPS_Structure;
extern IMU_t IMU_Structure;
extern unsigned char SysTick_Flag;

extern unsigned char angle_index;

extern float AngleY[ANGLE_BUFFER];
extern float Angle_Y_Mean;
extern float Angle_Y_Std;

/* Inicializa y verifica que todos los móduls que estan en la tarjeta
 * esten funcionando correctaente, antes de correr el sistema
 * */
void Init_System(void);

void DelayMillis(uint32_t millis);


