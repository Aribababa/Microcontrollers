#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "GPS.h"


GPS_t GPS_Structure;



int main(void) {

	GPS_init(&GPS_Structure, UTC_6, FORMAT_12_HOUR_CLK);	/* Le pasamos la estructura donde se guardarán los datos */
	for(;;);
}
