UART por Software
===================

Este programa simula un modulo UART mediante el uso de timer en el MCU. Dentro del prograa hay que llamar a la función para iniciar la UART y darle como parámetro la velocidad que se utilizará. </p>

Ejemplo:
...
//foo
SoftwareUART_Init(SoftWare_UART_9600);
...
