## Enable the floating point unit on STM32F4 Discovery

To enable the Floating Point Unit, we need to go to the file *system_stm32f44xx.c* and search for the *SystemInit()* funcion
and add the following lines at the end of the function:

```C
#if (__FPU_USED == 1)
  	SCB->CPACR |= (3UL << 20) | (3UL << 22);
   	 __DSB();
    	__ISB();
#endif
```

Next, add the following directives to the IDEs
<br>
__FPU_USED = 1  <br>
__FPU_PRESENT = 1

Finally, click on the *Build proyect* option (Depends of the IDE).
