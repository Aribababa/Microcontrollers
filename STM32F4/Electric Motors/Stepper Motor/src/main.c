#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

void GPIOD_Initialize();
void Delay__Init(void);
void Delay__US(uint32_t micros);

int main(void)
{
	Delay__Init();
	GPIOD_Initialize();
	unsigned char secuencia[] =
	{ 0x70, 0x30, 0xB0, 0x90, 0xD0, 0xC0, 0xE0, 0x60 };
	unsigned char i;
	i = 0;
	while(1){
		for(i = 0; i < 8; i++){
			GPIOD->BSRRH = secuencia[i++%8];
		}
	}
	/*while(1){
		GPIO_Write(GPIOD,GPIO_Pin_7);
		Delay__US(2000);
		GPIO_Write(GPIOD,GPIO_Pin_7|GPIO_Pin_5);
		Delay__US(2000);
		GPIO_Write(GPIOD,GPIO_Pin_5);
		Delay__US(2000);
		GPIO_Write(GPIOD,GPIO_Pin_5|GPIO_Pin_3);
		Delay__US(2000);
		GPIO_Write(GPIOD,GPIO_Pin_3);
		Delay__US(2000);
		GPIO_Write(GPIOD,GPIO_Pin_3|GPIO_Pin_1);
		Delay__US(2000);
		GPIO_Write(GPIOD,GPIO_Pin_1);
		Delay__US(2000);
		GPIO_Write(GPIOD,GPIO_Pin_1| GPIO_Pin_7);
		Delay__US(2000);
	}*/
}
