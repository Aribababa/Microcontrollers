#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "Vitri.h"

double magnitudVectorAceleracion;

/* Flags de eventos */
uint8_t bad_Angle_Flag = 0;
uint16_t bad_Angle_Counter = 0;

uint8_t crash_Flag = 0;
uint8_t peak_Force_G_Flag = 0;
uint16_t peak_Force_G_Counter = 0;

uint8_t temperature_Flag = 0;
uint8_t acc_Flag = 0;
uint32_t temperature_Counter = 0;

int main(void)
{
	Init_System();

	for(;;)
	{
		// MENSAJE DE CHOQUE SECO
		if(crash_Flag)
		{
			crash_Flag = 0;
			GSM_SendSMS("5555555555", EMERGENCY_MESSAGE_1);
		}

		// MENSAJE DE MALA POSICION DE LA MOTOCICLETA
		if(bad_Angle_Flag)
		{
			bad_Angle_Flag = 0;
			GSM_SendSMS("5555555555", FALLEN_MOTOCYCLE_MESSAGE);
		}

		// MENSAJE ALTA TEMPERATURA EN EL SISTEMA
		if(temperature_Flag)
		{
			temperature_Flag = 0;
			GSM_SendSMS("5555555555", OVERTEMPERATURE);
		}

		if(acc_Flag){
			acc_Flag = 0;
			GSM_SendSMS("5555555555", "Te encuentras bien?");
		}

		if(GSM_Yes_Flag){
			GSM_Yes_Flag = 0;
			GSM_SendSMS("5555555555", "Falsa Alarma");
		}

		if(Emergency_SMS_Flag){
			Emergency_SMS_Flag = 0;
			GSM_SendSMS("5555555555", "Compartiremos tu ubicacion con las autoridades");
		}

		// LECTURA DE LA IMU CADA 10ms
		if(I2C__MPU6050_Read(MPU6050_ADDRESS, &IMU_Structure) && SysTick_Flag)
		{
			SysTick_Flag = 0;

			// Aplicar filtro al angulo en el eje Y
			AngleY[angle_index] = IMU_Structure.AcelerometerAngleY;
			AngleY[angle_index] = 0.97*AngleY[angle_index] + (1-0.97)*AngleY[(angle_index-1)%ANGLE_BUFFER];

			// ESPERAR A QUE EL BUFFER SE LLENE
			if(!angle_index++%ANGLE_BUFFER)
			{
				// Obtener angulo promedio positivo
				arm_mean_f32((float32_t*)&AngleY[0], ANGLE_BUFFER, (float32_t*)&Angle_Y_Mean);
				arm_std_f32((float32_t*)&AngleY[0], ANGLE_BUFFER, (float32_t*)&Angle_Y_Std);

				if(Angle_Y_Std > 35.0){
					acc_Flag = 1;
				}

				if(Angle_Y_Mean<0)
					Angle_Y_Mean*=-1;

				// POSICION INCORRECTA DE LA MOTOCICLETA (Angulo mayor a 45°)
				if(Angle_Y_Mean > 45)
				{
					//DETECTAR LA POSICION INCORRECTA POR 25.6s PARA POSTERIORMENTE ENVIAR MENSAJE
					if(bad_Angle_Counter++ > 10)
					{
						bad_Angle_Flag = 1;
						bad_Angle_Counter = 0;
					}
				}
				else
					bad_Angle_Counter = 0;
			}

			// Una vez activado el pico de fuerza g, hay un retardo de 10s para que el sistema vuelva a enviar mensaje de
			// un nuevo pico
			if(peak_Force_G_Flag)
			{
				peak_Force_G_Counter++;
				if(peak_Force_G_Counter > 1000)
				{
					peak_Force_G_Counter = 0;
					peak_Force_G_Flag = 0;
				}
			}

			// ACELERACION MAYOR A 3g
			magnitudVectorAceleracion = sqrt(pow((double)IMU_Structure.AcelerometerX_m_s,(double)2) + pow((double)IMU_Structure.AcelerometerY_m_s,(double)2) + pow((double)IMU_Structure.AcelerometerZ_m_s,(double)2));
			if(magnitudVectorAceleracion >= (double)ACELERACION_GRAVEDAD*3.0 && peak_Force_G_Flag == 0)
			{
				peak_Force_G_Flag = 1;
				crash_Flag = 1;
			}

			// TEMPERATURA MAYOR A 70°
			if(IMU_Structure.Temperature > (float)70)
			{
				temperature_Counter++;

				//Si la temperatura permanece superior a 70° por media hora
				if(temperature_Counter > 180000)
				{
					temperature_Counter = 0;
					temperature_Flag = 1;
				}
			}
			else
				temperature_Counter = 0;

		}
	}

	return Arduino_Uno;
}
