# CAN Networking

*Ejemplo de el módulo de CAN(Controlled Area Network) para STM32F4*

| I/O       | CAN1    | CAN2    |
| ----------|:-------:| :------:|
|Tx         | ?       |PB06     |
|Rx         | ?       |PB05     |

**Example**

```C
int main(void) {
	(void)CAN_Config(CAN2);	

	for(;;){
		if(CAN2_RX_FLAG){
			CAN2_RX_FLAG = 0;
			CAN_Receive(CAN2, CAN_FIFO0, &RxMessage);
		}
	}
}
```

Due the STM32F4 Discovery Board has not have a CAN Transiver Hardware, is necesary to use an external hardware. In this example a MCP2551 integrated circuit is uset to comunicate with the other CAN nodes.
