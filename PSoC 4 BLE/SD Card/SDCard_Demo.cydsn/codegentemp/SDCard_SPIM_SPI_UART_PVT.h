/***************************************************************************//**
* \file SDCard_SPIM_SPI_UART_PVT.h
* \version 3.20
*
* \brief
*  This private file provides constants and parameter values for the
*  SCB Component in SPI and UART modes.
*  Please do not use this file or its content in your project.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2013-2016, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_SPI_UART_PVT_SDCard_SPIM_H)
#define CY_SCB_SPI_UART_PVT_SDCard_SPIM_H

#include "SDCard_SPIM_SPI_UART.h"


/***************************************
*     Internal Global Vars
***************************************/

#if (SDCard_SPIM_INTERNAL_RX_SW_BUFFER_CONST)
    extern volatile uint32  SDCard_SPIM_rxBufferHead;
    extern volatile uint32  SDCard_SPIM_rxBufferTail;
    
    /**
    * \addtogroup group_globals
    * @{
    */
    
    /** Sets when internal software receive buffer overflow
     *  was occurred.
    */  
    extern volatile uint8   SDCard_SPIM_rxBufferOverflow;
    /** @} globals */
#endif /* (SDCard_SPIM_INTERNAL_RX_SW_BUFFER_CONST) */

#if (SDCard_SPIM_INTERNAL_TX_SW_BUFFER_CONST)
    extern volatile uint32  SDCard_SPIM_txBufferHead;
    extern volatile uint32  SDCard_SPIM_txBufferTail;
#endif /* (SDCard_SPIM_INTERNAL_TX_SW_BUFFER_CONST) */

#if (SDCard_SPIM_INTERNAL_RX_SW_BUFFER)
    extern volatile uint8 SDCard_SPIM_rxBufferInternal[SDCard_SPIM_INTERNAL_RX_BUFFER_SIZE];
#endif /* (SDCard_SPIM_INTERNAL_RX_SW_BUFFER) */

#if (SDCard_SPIM_INTERNAL_TX_SW_BUFFER)
    extern volatile uint8 SDCard_SPIM_txBufferInternal[SDCard_SPIM_TX_BUFFER_SIZE];
#endif /* (SDCard_SPIM_INTERNAL_TX_SW_BUFFER) */


/***************************************
*     Private Function Prototypes
***************************************/

void SDCard_SPIM_SpiPostEnable(void);
void SDCard_SPIM_SpiStop(void);

#if (SDCard_SPIM_SCB_MODE_SPI_CONST_CFG)
    void SDCard_SPIM_SpiInit(void);
#endif /* (SDCard_SPIM_SCB_MODE_SPI_CONST_CFG) */

#if (SDCard_SPIM_SPI_WAKE_ENABLE_CONST)
    void SDCard_SPIM_SpiSaveConfig(void);
    void SDCard_SPIM_SpiRestoreConfig(void);
#endif /* (SDCard_SPIM_SPI_WAKE_ENABLE_CONST) */

void SDCard_SPIM_UartPostEnable(void);
void SDCard_SPIM_UartStop(void);

#if (SDCard_SPIM_SCB_MODE_UART_CONST_CFG)
    void SDCard_SPIM_UartInit(void);
#endif /* (SDCard_SPIM_SCB_MODE_UART_CONST_CFG) */

#if (SDCard_SPIM_UART_WAKE_ENABLE_CONST)
    void SDCard_SPIM_UartSaveConfig(void);
    void SDCard_SPIM_UartRestoreConfig(void);
#endif /* (SDCard_SPIM_UART_WAKE_ENABLE_CONST) */


/***************************************
*         UART API Constants
***************************************/

/* UART RX and TX position to be used in SDCard_SPIM_SetPins() */
#define SDCard_SPIM_UART_RX_PIN_ENABLE    (SDCard_SPIM_UART_RX)
#define SDCard_SPIM_UART_TX_PIN_ENABLE    (SDCard_SPIM_UART_TX)

/* UART RTS and CTS position to be used in  SDCard_SPIM_SetPins() */
#define SDCard_SPIM_UART_RTS_PIN_ENABLE    (0x10u)
#define SDCard_SPIM_UART_CTS_PIN_ENABLE    (0x20u)


/***************************************
* The following code is DEPRECATED and
* must not be used.
***************************************/

/* Interrupt processing */
#define SDCard_SPIM_SpiUartEnableIntRx(intSourceMask)  SDCard_SPIM_SetRxInterruptMode(intSourceMask)
#define SDCard_SPIM_SpiUartEnableIntTx(intSourceMask)  SDCard_SPIM_SetTxInterruptMode(intSourceMask)
uint32  SDCard_SPIM_SpiUartDisableIntRx(void);
uint32  SDCard_SPIM_SpiUartDisableIntTx(void);


#endif /* (CY_SCB_SPI_UART_PVT_SDCard_SPIM_H) */


/* [] END OF FILE */
