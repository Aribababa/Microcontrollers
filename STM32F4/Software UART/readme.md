UART por Software
===================

Este programa simula un modulo UART mediante el uso de timer en el MCU. Dentro del prograa hay que llamar a la función para iniciar la UART y darle como parámetro la velocidad que se utilizará. </p>

Ejemplo:
```
/* Inicialización de la UART */
SoftwareUART_Init(Software_UART_9600);
```

> **Nota:**

> - El programa esta sobre el Timer 3 (TIM3) del MCU. Asi que si que hay que tomar en cuenta el CLK si se quere cambiar el timer a utilizar.
