/***************************************************************************//**
* \file SDCard_SPIM_BOOT.c
* \version 3.20
*
* \brief
*  This file provides the source code of the bootloader communication APIs
*  for the SCB Component Unconfigured mode.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2013-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "SDCard_SPIM_BOOT.h"

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (SDCard_SPIM_BTLDR_COMM_ENABLED) && \
                                (SDCard_SPIM_SCB_MODE_UNCONFIG_CONST_CFG)

/*******************************************************************************
* Function Name: SDCard_SPIM_CyBtldrCommStart
****************************************************************************//**
*
*  Starts SDCard_SPIM component. After this function call the component is 
*  ready for communication.
*
*******************************************************************************/
void SDCard_SPIM_CyBtldrCommStart(void)
{
    if (SDCard_SPIM_SCB_MODE_I2C_RUNTM_CFG)
    {
        SDCard_SPIM_I2CCyBtldrCommStart();
    }
    else if (SDCard_SPIM_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        SDCard_SPIM_EzI2CCyBtldrCommStart();
    }
#if (!SDCard_SPIM_CY_SCBIP_V1)
    else if (SDCard_SPIM_SCB_MODE_SPI_RUNTM_CFG)
    {
        SDCard_SPIM_SpiCyBtldrCommStart();
    }
    else if (SDCard_SPIM_SCB_MODE_UART_RUNTM_CFG)
    {
        SDCard_SPIM_UartCyBtldrCommStart();
    }
#endif /* (!SDCard_SPIM_CY_SCBIP_V1) */
    else
    {
        /* Unknown mode: do nothing */
    }
}


/*******************************************************************************
* Function Name: SDCard_SPIM_CyBtldrCommStop
****************************************************************************//**
*
*  Stops SDCard_SPIM component.
*
*******************************************************************************/
void SDCard_SPIM_CyBtldrCommStop(void)
{
    if (SDCard_SPIM_SCB_MODE_I2C_RUNTM_CFG)
    {
        SDCard_SPIM_I2CCyBtldrCommStop();
    }
    else if (SDCard_SPIM_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        SDCard_SPIM_EzI2CCyBtldrCommStop();
    }
#if (!SDCard_SPIM_CY_SCBIP_V1)
    else if (SDCard_SPIM_SCB_MODE_SPI_RUNTM_CFG)
    {
        SDCard_SPIM_SpiCyBtldrCommStop();
    }
    else if (SDCard_SPIM_SCB_MODE_UART_RUNTM_CFG)
    {
        SDCard_SPIM_UartCyBtldrCommStop();
    }
#endif /* (!SDCard_SPIM_CY_SCBIP_V1) */
    else
    {
        /* Unknown mode: do nothing */
    }
}


/*******************************************************************************
* Function Name: SDCard_SPIM_CyBtldrCommReset
****************************************************************************//**
*
*  Clears SDCard_SPIM component buffers.
*
*******************************************************************************/
void SDCard_SPIM_CyBtldrCommReset(void)
{
    if(SDCard_SPIM_SCB_MODE_I2C_RUNTM_CFG)
    {
        SDCard_SPIM_I2CCyBtldrCommReset();
    }
    else if(SDCard_SPIM_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        SDCard_SPIM_EzI2CCyBtldrCommReset();
    }
#if (!SDCard_SPIM_CY_SCBIP_V1)
    else if(SDCard_SPIM_SCB_MODE_SPI_RUNTM_CFG)
    {
        SDCard_SPIM_SpiCyBtldrCommReset();
    }
    else if(SDCard_SPIM_SCB_MODE_UART_RUNTM_CFG)
    {
        SDCard_SPIM_UartCyBtldrCommReset();
    }
#endif /* (!SDCard_SPIM_CY_SCBIP_V1) */
    else
    {
        /* Unknown mode: do nothing */
    }
}


/*******************************************************************************
* Function Name: SDCard_SPIM_CyBtldrCommRead
****************************************************************************//**
*
*  Allows the caller to read data from the bootloader host (the host writes the 
*  data). The function handles polling to allow a block of data to be completely
*  received from the host device.
*
*  \param pData: Pointer to storage for the block of data to be read from the
*   bootloader host.
*  \param size: Number of bytes to be read.
*  \param count: Pointer to the variable to write the number of bytes actually
*   read.
*  \param timeOut: Number of units in 10 ms to wait before returning because of a
*   timeout.
*
* \return
*  \return
*  cystatus: Returns CYRET_SUCCESS if no problem was encountered or returns the
*  value that best describes the problem. For more information refer to 
*  the “Return Codes” section of the System Reference Guide.
*
*******************************************************************************/
cystatus SDCard_SPIM_CyBtldrCommRead(uint8 pData[], uint16 size, uint16 * count, uint8 timeOut)
{
    cystatus status;

    if(SDCard_SPIM_SCB_MODE_I2C_RUNTM_CFG)
    {
        status = SDCard_SPIM_I2CCyBtldrCommRead(pData, size, count, timeOut);
    }
    else if(SDCard_SPIM_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        status = SDCard_SPIM_EzI2CCyBtldrCommRead(pData, size, count, timeOut);
    }
#if (!SDCard_SPIM_CY_SCBIP_V1)
    else if(SDCard_SPIM_SCB_MODE_SPI_RUNTM_CFG)
    {
        status = SDCard_SPIM_SpiCyBtldrCommRead(pData, size, count, timeOut);
    }
    else if(SDCard_SPIM_SCB_MODE_UART_RUNTM_CFG)
    {
        status = SDCard_SPIM_UartCyBtldrCommRead(pData, size, count, timeOut);
    }
#endif /* (!SDCard_SPIM_CY_SCBIP_V1) */
    else
    {
        status = CYRET_INVALID_STATE; /* Unknown mode: return invalid status */
    }

    return(status);
}


/*******************************************************************************
* Function Name: SDCard_SPIM_CyBtldrCommWrite
****************************************************************************//**
*
*  Allows the caller to write data to the bootloader host (the host reads the 
*  data). The function does not use timeout and returns after data has been copied
*  into the slave read buffer. This data available to be read by the bootloader
*  host until following host data write.
*
*  \param pData: Pointer to the block of data to be written to the bootloader host.
*  \param size: Number of bytes to be written.
*  \param count: Pointer to the variable to write the number of bytes actually
*   written.
*  \param timeOut: Number of units in 10 ms to wait before returning because of a
*   timeout.
*
*  \return
*  cystatus: Returns CYRET_SUCCESS if no problem was encountered or returns the
*  value that best describes the problem. For more information refer to 
*  the “Return Codes” section of the System Reference Guide.
*
*******************************************************************************/
cystatus SDCard_SPIM_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut)
{
    cystatus status;

    if(SDCard_SPIM_SCB_MODE_I2C_RUNTM_CFG)
    {
        status = SDCard_SPIM_I2CCyBtldrCommWrite(pData, size, count, timeOut);
    }
    else if(SDCard_SPIM_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        status = SDCard_SPIM_EzI2CCyBtldrCommWrite(pData, size, count, timeOut);
    }
#if (!SDCard_SPIM_CY_SCBIP_V1)
    else if(SDCard_SPIM_SCB_MODE_SPI_RUNTM_CFG)
    {
        status = SDCard_SPIM_SpiCyBtldrCommWrite(pData, size, count, timeOut);
    }
    else if(SDCard_SPIM_SCB_MODE_UART_RUNTM_CFG)
    {
        status = SDCard_SPIM_UartCyBtldrCommWrite(pData, size, count, timeOut);
    }
#endif /* (!SDCard_SPIM_CY_SCBIP_V1) */
    else
    {
        status = CYRET_INVALID_STATE; /* Unknown mode: return invalid status */
    }

    return(status);
}

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (SDCard_SPIM_BTLDR_COMM_MODE_ENABLED) */


/* [] END OF FILE */
