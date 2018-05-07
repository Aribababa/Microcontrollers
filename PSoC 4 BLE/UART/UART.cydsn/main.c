
#include "project.h"

int main(void){
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    /** UART INIT **/
    UART_Start();
    
    UART_UartPutString("Queue");

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;){
        
        /* Place your application code here. */
    }
    return 0;
}

