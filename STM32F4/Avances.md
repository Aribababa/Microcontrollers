# Aavances de las prácticas

**UART Por Software(Terminada)**
 Esta práctica ya esta terminada y probada con diferentes velocidades para verificar la fidelidad del programa. Esta se implementó con
 un timer en el cual tomamos un canal como output compare para hacer la transmisión y otro canal para la recepción. Para detectar el startbit 
 utilizamos una interupción de falling edge sobre ese pin.
 
**GSM/GPRS(Terminada)**
  Esta práctica ya está lista. Interfazamos el módulo con el MCU y este puede mandar mensajes, leer mensajes entranter y contestar llamadas.
  
**GPS(Terminada)**
  Esta práctica ya est lista. El MCU recive las tramas, pero solo hace caso a las cuales comienzan $GPRMC. En cuanto se hace la detección ccomienza
  a procesar. Después de procesarlo, lo guarda en una estructura donde se encuentran todos los datos contenidos en la trama.

**WIFI(Terminada)**
Esta práctica ya esta lista. El MCU crea una conexxión mediante el puerto 80. Desde una página web le mandamos los parámetros. El MCU 
recive el frame de HTTP y obtiene el ID del cliente que envió y ls parámetros que le paso. Para provarlo utlilizamos los LEDS integrados en la tarjeta.
Despues de detectar cerramos la conexión que realizó el cliente.

**CAN(No terminada)**
El avance que tenemos de esta práctica es hacer la transmisión del FRAME de CAN. La recepción aun no la hemos implementado. El problema que
tenemos con esta práctica es el ruido de la Transmisión. 

** Cámara(No terminada)**
  De esta práctica solo hemos hecho hacer que se tome la foto, pero hay problemas al guardarse en el buffer que esta de por medio. A veces se toma
  la foto u otras el modulo de DCMI del MCU manda una flag de error. faltaria investigafr mas acerca de este módulo.

**ZigBee(No terminada)**
Tenemos los módulos, pero no podemos emepzarla, ya que ocupamos montarla para poder utilizarla.

**Bluetooth(No terminada)**
Esta práctica aún no la hemos comenzado.
