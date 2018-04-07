#include "stm32f4_discovery.h"
#include "stm32f4xx.h"


extern uint8_t CAN1_RX_FLAG;
extern uint8_t CAN2_RX_FLAG;

/* Configura el hardware de CAN */
uint8_t CAN_Config(CAN_TypeDef* CANx);
