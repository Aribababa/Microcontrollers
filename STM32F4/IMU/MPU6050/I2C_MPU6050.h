#ifndef I2C_MPU6050_H_
#define I2C_MPU6050_H_

#define MPU6050_ACCEL_XOUT_H	0x3B // R
#define MPU6050_ACCEL_XOUT_L    0x3C // R
#define MPU6050_ACCEL_YOUT_H    0x3D // R
#define MPU6050_ACCEL_YOUT_L    0x3E // R
#define MPU6050_ACCEL_ZOUT_H    0x3F // R
#define MPU6050_ACCEL_ZOUT_L    0x40 // R
#define MPU6050_TEMP_OUT_H      0x41 // R
#define MPU6050_TEMP_OUT_L      0x42 // R
#define MPU6050_GYRO_XOUT_H     0x43 // R
#define MPU6050_GYRO_XOUT_L     0x44 // R
#define MPU6050_GYRO_YOUT_H     0x45 // R
#define MPU6050_GYRO_YOUT_L     0x46 // R
#define MPU6050_GYRO_ZOUT_H     0x47 // R
#define MPU6050_GYRO_ZOUT_L     0x48 // R
#define MPU6050_PWR_MGMT_1     	0x6B // R/W

//Ratios de conversion
#define A_R 16384
#define G_R 131

//Conversion de radianes a grados 180/PI
#define RAD_TO_DEG 57.29578

typedef struct
{
	int16_t AcelerometerX;
	int16_t AcelerometerY;
	int16_t AcelerometerZ;
	int16_t Temperature;
	int16_t GyroscopeX;
	int16_t GyroscopeY;
	int16_t GyroscopeZ;
	float AcelerometerAngleY;
	float AcelerometerAngleX;
	float GyroscopeAngleY;
	float GyroscopeAngleX;
} MPU6050;

void I2C__MPU6050_Init(uint8_t address);
void I2C__MPU6050_Read(uint8_t address, MPU6050 *myMPU6050);

#endif
