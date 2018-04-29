#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "Vitri.h"
#include "arm_math.h"

volatile float32_t mean_X_axis;
volatile float32_t mean_Y_axis;
volatile float32_t mean_Z_axis;

volatile float32_t std_X_axis;
volatile float32_t std_Y_axis;
volatile float32_t std_Z_axis;

int main(void){
	Init_System();	/* Iniciammos la rutina de inicialización y verificación */
	Turn_Off_GSM;

	for(;;){
		if(Emergency_SMS_Flag){
			Emergency_SMS_Flag = 0;
			Turn_On_GSM;
		}

		if(I2C__MPU6050_Read(MPU6050_ADDRESS, &IMU_Structure) && SysTick_Flag){
			SysTick_Flag = 0;
			angleY[angle_index] = IMU_Structure.GyroscopeY;
			angleZ[angle_index] = IMU_Structure.GyroscopeZ;
			angleX[angle_index++%ANGLE_BUFFER] = IMU_Structure.GyroscopeY;

			if(!angle_index%ANGLE_BUFFER){

				// Calculamos la media de las muestras
				arm_mean_f32((float32_t*)&angleX[0], ANGLE_BUFFER, (float32_t*)&mean_X_axis);
				arm_mean_f32((float32_t*)&angleY[0], ANGLE_BUFFER, (float32_t*)&mean_Y_axis);
				arm_mean_f32((float32_t*)&angleZ[0], ANGLE_BUFFER, (float32_t*)&mean_Z_axis);

				//Callcullamos la desviación estandar de las muestras
				arm_std_f32((float32_t*)&angleX[0], ANGLE_BUFFER, (float32_t*)&std_X_axis);
				arm_std_f32((float32_t*)&angleY[0], ANGLE_BUFFER, (float32_t*)&std_Y_axis);
				arm_std_f32((float32_t*)&angleZ[0], ANGLE_BUFFER, (float32_t*)&std_Z_axis);
			}
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
