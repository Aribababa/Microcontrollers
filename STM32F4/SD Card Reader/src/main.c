
#include "stm32f4_discovery.h"
/* Include core modules */
#include "stm32f4xx.h"
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32f4_spi.h"

int main(void) {
    /* Initialize system */
    SystemInit();

    /* Initialize SPI */
    /* SCK = PA5, MOSI = PA7, MISO = PA6 */
    TM_SPI_Init(SPI1, TM_SPI_PinsPack_1);

    /* Send 0x15 over SPI1 */
    TM_SPI_Send(SPI1, 0x15);

    while(1){

    }
}
