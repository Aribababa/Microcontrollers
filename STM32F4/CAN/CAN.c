#include "CAN.h"

uint8_t CAN1_RX_FLAG = 0;
uint8_t CAN2_RX_FLAG = 0;

uint8_t CAN_Config(CAN_TypeDef* CANx){

	/* Definimos las estructuras de configuración */
	CAN_InitTypeDef        CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	GPIO_InitTypeDef 	   GPIO_InitStructure;
	NVIC_InitTypeDef		NVIC_InitStructure;

	/* verificamos cual módulo usará */
	if(CANx == CAN1){	/* Configuramos el CAN1 */
		/* Activamos los CLK */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD ,ENABLE);

		/* Configuración de la salida */
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_CAN1);
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_CAN1);

		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
		GPIO_Init(GPIOD, &GPIO_InitStructure);

	} else if (CANx == CAN2){	/* Configuramos el CAN2 */
		/* Para el caso del CAN2,  tanto CAN1 y CAN2 deben de estar activados */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1  | RCC_APB1Periph_CAN2, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB ,ENABLE);

		/* Configuración de la salida */
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_CAN2);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_CAN2);

		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

	} else {
		/* Si no es válido lo que colocó, entonces reresamoel cero e indicamos que no
		 * se pudo hacer la configuración */
		return 0;
	}

	/* Configuracion del controlador de CAN */
	CAN_DeInit(CANx);
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = DISABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_LoopBack;
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
	CAN_InitStructure.CAN_Prescaler = 2;
	CAN_Init(CANx, &CAN_InitStructure);

	/* Según la documentación, hay que colocar un filtro para ue se puedan
	 * recibir los mensajes.
	 * */
	CAN_FilterInitStructure.CAN_FilterNumber = 0;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);

	/* Activammos el vector de interrupción para */
	  CAN_ITConfig(CANx, CAN_IT_FMP0, ENABLE);

	  if(CANx == CAN1){
		  NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
	  } else{
		  NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
	  }

	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0A;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0A;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&NVIC_InitStructure);

	return 1;
}

/* Para este caso , en mabas interrupciones no es nesesario limpiar las banderas, ya que esto se
 * hace solo via hardware */
void  CAN1_RX0_IRQHandler(void){
	if(CAN_GetFlagStatus(CAN1,CAN_IT_FMP0) != RESET){
		CAN1_RX_FLAG = 1;
	}
}

void  CAN2_RX0_IRQHandler(void){
	if(CAN_GetFlagStatus(CAN2,CAN_IT_FMP0) != RESET){
		CAN2_RX_FLAG = 1;
	}
}

