/* Definimos los tiempos para pasar de Greenwich a otra zona horaria */
#define  UTC_0		0
#define  UTC_1		-1
#define  UTC_2		-2
#define  UTC_4		-4
#define  UTC_6		-6

/* Formato de la hora */
#define FORMAT_24_HOUR_CLK	0
#define FORMAT_12_HOUR_CLK	1

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
	unsigned char Hours;
	unsigned char Minutes;
	unsigned char Seconds;
	Coordinates_t Coordinates;
} Position_t;

/* Estrucutura en la que se almecenan los datos del GPS */
typedef struct{
	Date_t Date;
	Time_t Time;
	Position_t Latitude;
	Position_t Longitude;
} GPS_t;


/* Inicializa la comunicacion serial con el módulo GPS */
void GPS_init(GPS_t* gps, signed char Time_Difference, unsigned char TimeFormat);
