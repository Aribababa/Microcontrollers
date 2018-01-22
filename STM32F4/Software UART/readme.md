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

> **Flags asociadas al programa :**
> - Software_UART_Data_Ready: "1" si hay un dato nuevo disponible en RSR(*Recieve Shift Register*), caso contrario "0".
> - Software_UART_Tx_THRE: "1" si el registro THR(*Transmit Hold Register*) esta vacio, caso contrario "0".
</p>

**Transmisión:**
</p>
Esta funciona mediante un módulo de Output Compare en el cual genera los tiempos dependiendo del Baudrate deseado. Cuando se llama a la funcion de *SoftwareUART_Send('0x25')* esta activa el módulo de Output Compare para que genere interrupciones. Cuando entra a la interrupción verifica cual sera el siguiente bit a enviar dado que el cambio de los GPIO´s esta hecho por el Hardware del módulo. Para ver esto se hace shift al dato un cierto número de veces y al final dejar la salida en estado IDLE para indicar que se terminó la transmisión.</p>

**Recepción:**
</p>
Para recibir los dato hay de detectar si hay un start bit en el Pin de Rx. Para esto se le asocia una interripción externa(EXTI) la cual detecta un Falling Edge. Cuando este evento es capturado, se activa un canal del módulo de Output Compare con interrupciones asociadas y se desactivan las EXTI. Se muestrean los datos justamente en el centro para tener mayor precisión. y después que se muesrearon todos los bits se desactivan las interrupciones del Output Compare y se activan las EXTI.
</p>


**Ejemplo:**
</p>
El ejemplo ejecuta un programa que recive datos de una PC o algun MCU y hace función de Echo para que los datos se puedan ver desde una terminal.
</p>

```
void main(void){
  SoftwareUART_Init(Software_UART_115900);
  for(;;){
    if(Software_UART_Data_Ready){ /* Si hay un dato disponible ... */
      SoftwareUART_Send(RSR);   /* Transmite lo que recibiste */
      Software_UART_Data_Ready = 0;   
    }
  }
}
```
RSR(Recieve Shift Register) contiene el dato más reciente que llegó.Si otro dato llega RSR se guarda en el buffer de recepción y da lugar al siguiente dato. Software_UART_Data_Ready indica si hay un nuevo dato disponible en RSR.</p>

> **Nota:**
> - El programa esta sobre el Timer 3 (TIM3) del MCU. Asi que si que hay que tomar en cuenta el CLK si se quere cambiar el timer a utilizar, ya que este CLK tiene un frecuencia de 84 MHz.
