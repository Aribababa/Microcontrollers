/***************************************************************************//**
* \file SDCard_SPIM_BOOT.h
* \version 3.20
*
* \brief
*  This file provides constants and parameter values of the bootloader
*  communication APIs for the SCB Component.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2014-2016, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_BOOT_SDCard_SPIM_H)
#define CY_SCB_BOOT_SDCard_SPIM_H

#include "SDCard_SPIM_PVT.h"

#if (SDCard_SPIM_SCB_MODE_I2C_INC)
    #include "SDCard_SPIM_I2C.h"
#endif /* (SDCard_SPIM_SCB_MODE_I2C_INC) */

#if (SDCard_SPIM_SCB_MODE_EZI2C_INC)
    #include "SDCard_SPIM_EZI2C.h"
#endif /* (SDCard_SPIM_SCB_MODE_EZI2C_INC) */

#if (SDCard_SPIM_SCB_MODE_SPI_INC || SDCard_SPIM_SCB_MODE_UART_INC)
    #include "SDCard_SPIM_SPI_UART.h"
#endif /* (SDCard_SPIM_SCB_MODE_SPI_INC || SDCard_SPIM_SCB_MODE_UART_INC) */


/***************************************
*  Conditional Compilation Parameters
****************************************/

/* Bootloader communication interface enable */
#define SDCard_SPIM_BTLDR_COMM_ENABLED ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_SDCard_SPIM) || \
                                             (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))

/* Enable I2C bootloader communication */
#if (SDCard_SPIM_SCB_MODE_I2C_INC)
    #define SDCard_SPIM_I2C_BTLDR_COMM_ENABLED     (SDCard_SPIM_BTLDR_COMM_ENABLED && \
                                                            (SDCard_SPIM_SCB_MODE_UNCONFIG_CONST_CFG || \
                                                             SDCard_SPIM_I2C_SLAVE_CONST))
#else
     #define SDCard_SPIM_I2C_BTLDR_COMM_ENABLED    (0u)
#endif /* (SDCard_SPIM_SCB_MODE_I2C_INC) */

/* EZI2C does not support bootloader communication. Provide empty APIs */
#if (SDCard_SPIM_SCB_MODE_EZI2C_INC)
    #define SDCard_SPIM_EZI2C_BTLDR_COMM_ENABLED   (SDCard_SPIM_BTLDR_COMM_ENABLED && \
                                                         SDCard_SPIM_SCB_MODE_UNCONFIG_CONST_CFG)
#else
    #define SDCard_SPIM_EZI2C_BTLDR_COMM_ENABLED   (0u)
#endif /* (SDCard_SPIM_EZI2C_BTLDR_COMM_ENABLED) */

/* Enable SPI bootloader communication */
#if (SDCard_SPIM_SCB_MODE_SPI_INC)
    #define SDCard_SPIM_SPI_BTLDR_COMM_ENABLED     (SDCard_SPIM_BTLDR_COMM_ENABLED && \
                                                            (SDCard_SPIM_SCB_MODE_UNCONFIG_CONST_CFG || \
                                                             SDCard_SPIM_SPI_SLAVE_CONST))
#else
        #define SDCard_SPIM_SPI_BTLDR_COMM_ENABLED (0u)
#endif /* (SDCard_SPIM_SPI_BTLDR_COMM_ENABLED) */

/* Enable UART bootloader communication */
#if (SDCard_SPIM_SCB_MODE_UART_INC)
       #define SDCard_SPIM_UART_BTLDR_COMM_ENABLED    (SDCard_SPIM_BTLDR_COMM_ENABLED && \
                                                            (SDCard_SPIM_SCB_MODE_UNCONFIG_CONST_CFG || \
                                                             (SDCard_SPIM_UART_RX_DIRECTION && \
                                                              SDCard_SPIM_UART_TX_DIRECTION)))
#else
     #define SDCard_SPIM_UART_BTLDR_COMM_ENABLED   (0u)
#endif /* (SDCard_SPIM_UART_BTLDR_COMM_ENABLED) */

/* Enable bootloader communication */
#define SDCard_SPIM_BTLDR_COMM_MODE_ENABLED    (SDCard_SPIM_I2C_BTLDR_COMM_ENABLED   || \
                                                     SDCard_SPIM_SPI_BTLDR_COMM_ENABLED   || \
                                                     SDCard_SPIM_EZI2C_BTLDR_COMM_ENABLED || \
                                                     SDCard_SPIM_UART_BTLDR_COMM_ENABLED)


/***************************************
*        Function Prototypes
***************************************/

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (SDCard_SPIM_I2C_BTLDR_COMM_ENABLED)
    /* I2C Bootloader physical layer functions */
    void SDCard_SPIM_I2CCyBtldrCommStart(void);
    void SDCard_SPIM_I2CCyBtldrCommStop (void);
    void SDCard_SPIM_I2CCyBtldrCommReset(void);
    cystatus SDCard_SPIM_I2CCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus SDCard_SPIM_I2CCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map I2C specific bootloader communication APIs to SCB specific APIs */
    #if (SDCard_SPIM_SCB_MODE_I2C_CONST_CFG)
        #define SDCard_SPIM_CyBtldrCommStart   SDCard_SPIM_I2CCyBtldrCommStart
        #define SDCard_SPIM_CyBtldrCommStop    SDCard_SPIM_I2CCyBtldrCommStop
        #define SDCard_SPIM_CyBtldrCommReset   SDCard_SPIM_I2CCyBtldrCommReset
        #define SDCard_SPIM_CyBtldrCommRead    SDCard_SPIM_I2CCyBtldrCommRead
        #define SDCard_SPIM_CyBtldrCommWrite   SDCard_SPIM_I2CCyBtldrCommWrite
    #endif /* (SDCard_SPIM_SCB_MODE_I2C_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (SDCard_SPIM_I2C_BTLDR_COMM_ENABLED) */


#if defined(CYDEV_BOOTLOADER_IO_COMP) && (SDCard_SPIM_EZI2C_BTLDR_COMM_ENABLED)
    /* Bootloader physical layer functions */
    void SDCard_SPIM_EzI2CCyBtldrCommStart(void);
    void SDCard_SPIM_EzI2CCyBtldrCommStop (void);
    void SDCard_SPIM_EzI2CCyBtldrCommReset(void);
    cystatus SDCard_SPIM_EzI2CCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus SDCard_SPIM_EzI2CCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map EZI2C specific bootloader communication APIs to SCB specific APIs */
    #if (SDCard_SPIM_SCB_MODE_EZI2C_CONST_CFG)
        #define SDCard_SPIM_CyBtldrCommStart   SDCard_SPIM_EzI2CCyBtldrCommStart
        #define SDCard_SPIM_CyBtldrCommStop    SDCard_SPIM_EzI2CCyBtldrCommStop
        #define SDCard_SPIM_CyBtldrCommReset   SDCard_SPIM_EzI2CCyBtldrCommReset
        #define SDCard_SPIM_CyBtldrCommRead    SDCard_SPIM_EzI2CCyBtldrCommRead
        #define SDCard_SPIM_CyBtldrCommWrite   SDCard_SPIM_EzI2CCyBtldrCommWrite
    #endif /* (SDCard_SPIM_SCB_MODE_EZI2C_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (SDCard_SPIM_EZI2C_BTLDR_COMM_ENABLED) */

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (SDCard_SPIM_SPI_BTLDR_COMM_ENABLED)
    /* SPI Bootloader physical layer functions */
    void SDCard_SPIM_SpiCyBtldrCommStart(void);
    void SDCard_SPIM_SpiCyBtldrCommStop (void);
    void SDCard_SPIM_SpiCyBtldrCommReset(void);
    cystatus SDCard_SPIM_SpiCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus SDCard_SPIM_SpiCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map SPI specific bootloader communication APIs to SCB specific APIs */
    #if (SDCard_SPIM_SCB_MODE_SPI_CONST_CFG)
        #define SDCard_SPIM_CyBtldrCommStart   SDCard_SPIM_SpiCyBtldrCommStart
        #define SDCard_SPIM_CyBtldrCommStop    SDCard_SPIM_SpiCyBtldrCommStop
        #define SDCard_SPIM_CyBtldrCommReset   SDCard_SPIM_SpiCyBtldrCommReset
        #define SDCard_SPIM_CyBtldrCommRead    SDCard_SPIM_SpiCyBtldrCommRead
        #define SDCard_SPIM_CyBtldrCommWrite   SDCard_SPIM_SpiCyBtldrCommWrite
    #endif /* (SDCard_SPIM_SCB_MODE_SPI_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (SDCard_SPIM_SPI_BTLDR_COMM_ENABLED) */

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (SDCard_SPIM_UART_BTLDR_COMM_ENABLED)
    /* UART Bootloader physical layer functions */
    void SDCard_SPIM_UartCyBtldrCommStart(void);
    void SDCard_SPIM_UartCyBtldrCommStop (void);
    void SDCard_SPIM_UartCyBtldrCommReset(void);
    cystatus SDCard_SPIM_UartCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus SDCard_SPIM_UartCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map UART specific bootloader communication APIs to SCB specific APIs */
    #if (SDCard_SPIM_SCB_MODE_UART_CONST_CFG)
        #define SDCard_SPIM_CyBtldrCommStart   SDCard_SPIM_UartCyBtldrCommStart
        #define SDCard_SPIM_CyBtldrCommStop    SDCard_SPIM_UartCyBtldrCommStop
        #define SDCard_SPIM_CyBtldrCommReset   SDCard_SPIM_UartCyBtldrCommReset
        #define SDCard_SPIM_CyBtldrCommRead    SDCard_SPIM_UartCyBtldrCommRead
        #define SDCard_SPIM_CyBtldrCommWrite   SDCard_SPIM_UartCyBtldrCommWrite
    #endif /* (SDCard_SPIM_SCB_MODE_UART_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (SDCard_SPIM_UART_BTLDR_COMM_ENABLED) */

/**
* \addtogroup group_bootloader
* @{
*/

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (SDCard_SPIM_BTLDR_COMM_ENABLED)
    #if (SDCard_SPIM_SCB_MODE_UNCONFIG_CONST_CFG)
        /* Bootloader physical layer functions */
        void SDCard_SPIM_CyBtldrCommStart(void);
        void SDCard_SPIM_CyBtldrCommStop (void);
        void SDCard_SPIM_CyBtldrCommReset(void);
        cystatus SDCard_SPIM_CyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
        cystatus SDCard_SPIM_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    #endif /* (SDCard_SPIM_SCB_MODE_UNCONFIG_CONST_CFG) */

    /* Map SCB specific bootloader communication APIs to common APIs */
    #if (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_SDCard_SPIM)
        #define CyBtldrCommStart    SDCard_SPIM_CyBtldrCommStart
        #define CyBtldrCommStop     SDCard_SPIM_CyBtldrCommStop
        #define CyBtldrCommReset    SDCard_SPIM_CyBtldrCommReset
        #define CyBtldrCommWrite    SDCard_SPIM_CyBtldrCommWrite
        #define CyBtldrCommRead     SDCard_SPIM_CyBtldrCommRead
    #endif /* (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_SDCard_SPIM) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (SDCard_SPIM_BTLDR_COMM_ENABLED) */

/** @} group_bootloader */

/***************************************
*           API Constants
***************************************/

/* Timeout unit in milliseconds */
#define SDCard_SPIM_WAIT_1_MS  (1u)

/* Return number of bytes to copy into bootloader buffer */
#define SDCard_SPIM_BYTES_TO_COPY(actBufSize, bufSize) \
                            ( ((uint32)(actBufSize) < (uint32)(bufSize)) ? \
                                ((uint32) (actBufSize)) : ((uint32) (bufSize)) )

/* Size of Read/Write buffers for I2C bootloader  */
#define SDCard_SPIM_I2C_BTLDR_SIZEOF_READ_BUFFER   (64u)
#define SDCard_SPIM_I2C_BTLDR_SIZEOF_WRITE_BUFFER  (64u)

/* Byte to byte time interval: calculated basing on current component
* data rate configuration, can be defined in project if required.
*/
#ifndef SDCard_SPIM_SPI_BYTE_TO_BYTE
    #define SDCard_SPIM_SPI_BYTE_TO_BYTE   (3001u)
#endif

/* Byte to byte time interval: calculated basing on current component
* baud rate configuration, can be defined in the project if required.
*/
#ifndef SDCard_SPIM_UART_BYTE_TO_BYTE
    #define SDCard_SPIM_UART_BYTE_TO_BYTE  (2500u)
#endif /* SDCard_SPIM_UART_BYTE_TO_BYTE */

#endif /* (CY_SCB_BOOT_SDCard_SPIM_H) */


/* [] END OF FILE */
