# Habilitar librerias de CMSIS en STM32F4 Discovery.

*Todo esto se realizó en el IDE AC6 para stm32*<br>
 1. Paso:
 Añadir las directivas en el IDE o en el entorno que se esta trabajando.<br>
 
 ARM_MATH_CM4
 
 2. Crear una carpeta en el proyecto para colocar el archivo de CMSIS.
 
 3. Descargar la libreria desde la página de CMSIS. <br>
 *Es el archivo que se encuentra en esta carpeta.*

4. Agregar el path de donde se encuentra la libreria y el nombre del archivo .a en el entorno de trabajo del proyecto.<br>

5. Compilar el código e incluir la libreria en en archvio que se desea utilizar.<br>
```C
#include "arm_math.h"

void main(void){
 for(;;);
}
```

