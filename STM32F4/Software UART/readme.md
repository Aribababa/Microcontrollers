UART por Software
===================

Este programa simula un módulo UART mediante el uso de un timer del MCU. Dentro del programa hay que llamar a la función para iniciar la UART y darle como parámetro la velocidad que se utilizará. </p>

**Ejecutar la UART por Software:**
```
/* Inicialización de la UART */
SoftwareUART_Init(Software_UART_9600);
```
Se le puede configurar distintas velocidades de transmisión donde el maximo Baudrate es de 115900.
En la recepción este tiene un buffer circular de 64 bytes que guarda lo que llega.</p>

El ejemplo ejecuta un programa que recive datos de una PC o algun MCU y hace función de Echo para que los datos se puedan ver desde una terminal.</p>

**Ejemplo:**
```
void main(void){
  SoftwareUART_Init(Software_UART_115900;
  for(;;){
    if(Software_UART_Data_Ready){
      SoftwareUART_Send(RSR);
      Software_UART_Data_Ready = 0;
    }
  }
return 0;
}
```

> **Nota:**
> - El programa esta sobre el Timer 3 (TIM3) del MCU. Asi que si que hay que tomar en cuenta el CLK si se quere cambiar el timer a utilizar.
