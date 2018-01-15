UART por Software
===================

Este programa simula un modulo UART mediante el uso de timer en el MCU. Dentro del prograa hay que llamar a la función para iniciar la UART y darle como parámetro la velocidad que se utilizará. </p>

Ejemplo:
```
/* Inicialización de la UART */
SoftwareUART_Init(Software_UART_9600);
```
