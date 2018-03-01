/* Definimos los tiempos para pasar de Greenwich a otra zona horaria */

/* Definimos estructuras para el GPS */
typedef enum {
	JAN = 1, FEB, MAR, APR, MAY, JUN, JUL, AUG, SEP, OCT, NOV, DEC
} Months_t;

typedef enum {
	NORTH = 0, SOUTH, EAST, WEST
} Coordinates_t;

typedef struct{
	unsigned char Day;
	Months_t Month;
	unsigned int Year;
} Date_t;

typedef struct{
	unsigned char Hours;
	unsigned char Minutes;
	unsigned char Seconds;
} Time_t;

typedef struct{
	unsigned char Degrees;
	Time_t Time;
	Coordinates_t Coordinates;
} Position_t;

typedef struct{
	Date_t Date;
	Time_t Time;
	Position_t Latitude;
	Position_t Longitude;
} GPS_t;

extern GPS_t GPS;
/* Inicializa la comunicacion serial con el módulo GPS */
void GPS_init(void);
