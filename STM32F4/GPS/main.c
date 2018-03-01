#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "GPS.h"
			
int main(void) {

	GPS_init();
	for(;;);
}
