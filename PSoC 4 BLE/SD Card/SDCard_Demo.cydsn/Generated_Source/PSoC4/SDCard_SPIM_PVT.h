/***************************************************************************//**
* \file .h
* \version 3.20
*
* \brief
*  This private file provides constants and parameter values for the
*  SCB Component.
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

#if !defined(CY_SCB_PVT_SDCard_SPIM_H)
#define CY_SCB_PVT_SDCard_SPIM_H

#include "SDCard_SPIM.h"


/***************************************
*     Private Function Prototypes
***************************************/

/* APIs to service INTR_I2C_EC register */
#define SDCard_SPIM_SetI2CExtClkInterruptMode(interruptMask) SDCard_SPIM_WRITE_INTR_I2C_EC_MASK(interruptMask)
#define SDCard_SPIM_ClearI2CExtClkInterruptSource(interruptMask) SDCard_SPIM_CLEAR_INTR_I2C_EC(interruptMask)
#define SDCard_SPIM_GetI2CExtClkInterruptSource()                (SDCard_SPIM_INTR_I2C_EC_REG)
#define SDCard_SPIM_GetI2CExtClkInterruptMode()                  (SDCard_SPIM_INTR_I2C_EC_MASK_REG)
#define SDCard_SPIM_GetI2CExtClkInterruptSourceMasked()          (SDCard_SPIM_INTR_I2C_EC_MASKED_REG)

#if (!SDCard_SPIM_CY_SCBIP_V1)
    /* APIs to service INTR_SPI_EC register */
    #define SDCard_SPIM_SetSpiExtClkInterruptMode(interruptMask) \
                                                                SDCard_SPIM_WRITE_INTR_SPI_EC_MASK(interruptMask)
    #define SDCard_SPIM_ClearSpiExtClkInterruptSource(interruptMask) \
                                                                SDCard_SPIM_CLEAR_INTR_SPI_EC(interruptMask)
    #define SDCard_SPIM_GetExtSpiClkInterruptSource()                 (SDCard_SPIM_INTR_SPI_EC_REG)
    #define SDCard_SPIM_GetExtSpiClkInterruptMode()                   (SDCard_SPIM_INTR_SPI_EC_MASK_REG)
    #define SDCard_SPIM_GetExtSpiClkInterruptSourceMasked()           (SDCard_SPIM_INTR_SPI_EC_MASKED_REG)
#endif /* (!SDCard_SPIM_CY_SCBIP_V1) */

#if(SDCard_SPIM_SCB_MODE_UNCONFIG_CONST_CFG)
    extern void SDCard_SPIM_SetPins(uint32 mode, uint32 subMode, uint32 uartEnableMask);
#endif /* (SDCard_SPIM_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*     Vars with External Linkage
***************************************/

#if (SDCard_SPIM_SCB_IRQ_INTERNAL)
#if !defined (CY_REMOVE_SDCard_SPIM_CUSTOM_INTR_HANDLER)
    extern cyisraddress SDCard_SPIM_customIntrHandler;
#endif /* !defined (CY_REMOVE_SDCard_SPIM_CUSTOM_INTR_HANDLER) */
#endif /* (SDCard_SPIM_SCB_IRQ_INTERNAL) */

extern SDCard_SPIM_BACKUP_STRUCT SDCard_SPIM_backup;

#if(SDCard_SPIM_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common configuration variables */
    extern uint8 SDCard_SPIM_scbMode;
    extern uint8 SDCard_SPIM_scbEnableWake;
    extern uint8 SDCard_SPIM_scbEnableIntr;

    /* I2C configuration variables */
    extern uint8 SDCard_SPIM_mode;
    extern uint8 SDCard_SPIM_acceptAddr;

    /* SPI/UART configuration variables */
    extern volatile uint8 * SDCard_SPIM_rxBuffer;
    extern uint8   SDCard_SPIM_rxDataBits;
    extern uint32  SDCard_SPIM_rxBufferSize;

    extern volatile uint8 * SDCard_SPIM_txBuffer;
    extern uint8   SDCard_SPIM_txDataBits;
    extern uint32  SDCard_SPIM_txBufferSize;

    /* EZI2C configuration variables */
    extern uint8 SDCard_SPIM_numberOfAddr;
    extern uint8 SDCard_SPIM_subAddrSize;
#endif /* (SDCard_SPIM_SCB_MODE_UNCONFIG_CONST_CFG) */

#if (! (SDCard_SPIM_SCB_MODE_I2C_CONST_CFG || \
        SDCard_SPIM_SCB_MODE_EZI2C_CONST_CFG))
    extern uint16 SDCard_SPIM_IntrTxMask;
#endif /* (! (SDCard_SPIM_SCB_MODE_I2C_CONST_CFG || \
              SDCard_SPIM_SCB_MODE_EZI2C_CONST_CFG)) */


/***************************************
*        Conditional Macro
****************************************/

#if(SDCard_SPIM_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Defines run time operation mode */
    #define SDCard_SPIM_SCB_MODE_I2C_RUNTM_CFG     (SDCard_SPIM_SCB_MODE_I2C      == SDCard_SPIM_scbMode)
    #define SDCard_SPIM_SCB_MODE_SPI_RUNTM_CFG     (SDCard_SPIM_SCB_MODE_SPI      == SDCard_SPIM_scbMode)
    #define SDCard_SPIM_SCB_MODE_UART_RUNTM_CFG    (SDCard_SPIM_SCB_MODE_UART     == SDCard_SPIM_scbMode)
    #define SDCard_SPIM_SCB_MODE_EZI2C_RUNTM_CFG   (SDCard_SPIM_SCB_MODE_EZI2C    == SDCard_SPIM_scbMode)
    #define SDCard_SPIM_SCB_MODE_UNCONFIG_RUNTM_CFG \
                                                        (SDCard_SPIM_SCB_MODE_UNCONFIG == SDCard_SPIM_scbMode)

    /* Defines wakeup enable */
    #define SDCard_SPIM_SCB_WAKE_ENABLE_CHECK       (0u != SDCard_SPIM_scbEnableWake)
#endif /* (SDCard_SPIM_SCB_MODE_UNCONFIG_CONST_CFG) */

/* Defines maximum number of SCB pins */
#if (!SDCard_SPIM_CY_SCBIP_V1)
    #define SDCard_SPIM_SCB_PINS_NUMBER    (7u)
#else
    #define SDCard_SPIM_SCB_PINS_NUMBER    (2u)
#endif /* (!SDCard_SPIM_CY_SCBIP_V1) */

#endif /* (CY_SCB_PVT_SDCard_SPIM_H) */


/* [] END OF FILE */
