UART por Software
===================

Este programa simula un módulo UART mediante el uso de un timer del MCU. Dentro del programa hay que llamar a la función para iniciar la UART y darle como parámetro la velocidad que se utilizará. </p>

**Ejemplo:**
```
/* Inicialización de la UART */
SoftwareUART_Init(Software_UART_9600);
```
Se le puede configurar distintas velocidades de transmisión donde el maximo Baudrate es de 115900.
En la recepción este tiene un buffer circular de 64 bytes que guarda lo que llega.</p>

> **Nota:**
> - El programa esta sobre el Timer 3 (TIM3) del MCU. Asi que si que hay que tomar en cuenta el CLK si se quere cambiar el timer a utilizar.
