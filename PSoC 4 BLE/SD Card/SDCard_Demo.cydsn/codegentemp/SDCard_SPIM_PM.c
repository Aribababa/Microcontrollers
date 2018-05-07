/***************************************************************************//**
* \file SDCard_SPIM_PM.c
* \version 3.20
*
* \brief
*  This file provides the source code to the Power Management support for
*  the SCB Component.
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

#include "SDCard_SPIM.h"
#include "SDCard_SPIM_PVT.h"

#if(SDCard_SPIM_SCB_MODE_I2C_INC)
    #include "SDCard_SPIM_I2C_PVT.h"
#endif /* (SDCard_SPIM_SCB_MODE_I2C_INC) */

#if(SDCard_SPIM_SCB_MODE_EZI2C_INC)
    #include "SDCard_SPIM_EZI2C_PVT.h"
#endif /* (SDCard_SPIM_SCB_MODE_EZI2C_INC) */

#if(SDCard_SPIM_SCB_MODE_SPI_INC || SDCard_SPIM_SCB_MODE_UART_INC)
    #include "SDCard_SPIM_SPI_UART_PVT.h"
#endif /* (SDCard_SPIM_SCB_MODE_SPI_INC || SDCard_SPIM_SCB_MODE_UART_INC) */


/***************************************
*   Backup Structure declaration
***************************************/

#if(SDCard_SPIM_SCB_MODE_UNCONFIG_CONST_CFG || \
   (SDCard_SPIM_SCB_MODE_I2C_CONST_CFG   && (!SDCard_SPIM_I2C_WAKE_ENABLE_CONST))   || \
   (SDCard_SPIM_SCB_MODE_EZI2C_CONST_CFG && (!SDCard_SPIM_EZI2C_WAKE_ENABLE_CONST)) || \
   (SDCard_SPIM_SCB_MODE_SPI_CONST_CFG   && (!SDCard_SPIM_SPI_WAKE_ENABLE_CONST))   || \
   (SDCard_SPIM_SCB_MODE_UART_CONST_CFG  && (!SDCard_SPIM_UART_WAKE_ENABLE_CONST)))

    SDCard_SPIM_BACKUP_STRUCT SDCard_SPIM_backup =
    {
        0u, /* enableState */
    };
#endif


/*******************************************************************************
* Function Name: SDCard_SPIM_Sleep
****************************************************************************//**
*
*  Prepares the SDCard_SPIM component to enter Deep Sleep.
*  The “Enable wakeup from Deep Sleep Mode” selection has an influence on this 
*  function implementation:
*  - Checked: configures the component to be wakeup source from Deep Sleep.
*  - Unchecked: stores the current component state (enabled or disabled) and 
*    disables the component. See SCB_Stop() function for details about component 
*    disabling.
*
*  Call the SDCard_SPIM_Sleep() function before calling the 
*  CyPmSysDeepSleep() function. 
*  Refer to the PSoC Creator System Reference Guide for more information about 
*  power management functions and Low power section of this document for the 
*  selected mode.
*
*  This function should not be called before entering Sleep.
*
*******************************************************************************/
void SDCard_SPIM_Sleep(void)
{
#if(SDCard_SPIM_SCB_MODE_UNCONFIG_CONST_CFG)

    if(SDCard_SPIM_SCB_WAKE_ENABLE_CHECK)
    {
        if(SDCard_SPIM_SCB_MODE_I2C_RUNTM_CFG)
        {
            SDCard_SPIM_I2CSaveConfig();
        }
        else if(SDCard_SPIM_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            SDCard_SPIM_EzI2CSaveConfig();
        }
    #if(!SDCard_SPIM_CY_SCBIP_V1)
        else if(SDCard_SPIM_SCB_MODE_SPI_RUNTM_CFG)
        {
            SDCard_SPIM_SpiSaveConfig();
        }
        else if(SDCard_SPIM_SCB_MODE_UART_RUNTM_CFG)
        {
            SDCard_SPIM_UartSaveConfig();
        }
    #endif /* (!SDCard_SPIM_CY_SCBIP_V1) */
        else
        {
            /* Unknown mode */
        }
    }
    else
    {
        SDCard_SPIM_backup.enableState = (uint8) SDCard_SPIM_GET_CTRL_ENABLED;

        if(0u != SDCard_SPIM_backup.enableState)
        {
            SDCard_SPIM_Stop();
        }
    }

#else

    #if (SDCard_SPIM_SCB_MODE_I2C_CONST_CFG && SDCard_SPIM_I2C_WAKE_ENABLE_CONST)
        SDCard_SPIM_I2CSaveConfig();

    #elif (SDCard_SPIM_SCB_MODE_EZI2C_CONST_CFG && SDCard_SPIM_EZI2C_WAKE_ENABLE_CONST)
        SDCard_SPIM_EzI2CSaveConfig();

    #elif (SDCard_SPIM_SCB_MODE_SPI_CONST_CFG && SDCard_SPIM_SPI_WAKE_ENABLE_CONST)
        SDCard_SPIM_SpiSaveConfig();

    #elif (SDCard_SPIM_SCB_MODE_UART_CONST_CFG && SDCard_SPIM_UART_WAKE_ENABLE_CONST)
        SDCard_SPIM_UartSaveConfig();

    #else

        SDCard_SPIM_backup.enableState = (uint8) SDCard_SPIM_GET_CTRL_ENABLED;

        if(0u != SDCard_SPIM_backup.enableState)
        {
            SDCard_SPIM_Stop();
        }

    #endif /* defined (SDCard_SPIM_SCB_MODE_I2C_CONST_CFG) && (SDCard_SPIM_I2C_WAKE_ENABLE_CONST) */

#endif /* (SDCard_SPIM_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: SDCard_SPIM_Wakeup
****************************************************************************//**
*
*  Prepares the SDCard_SPIM component for Active mode operation after 
*  Deep Sleep.
*  The “Enable wakeup from Deep Sleep Mode” selection has influence on this 
*  function implementation:
*  - Checked: restores the component Active mode configuration.
*  - Unchecked: enables the component if it was enabled before enter Deep Sleep.
*
*  This function should not be called after exiting Sleep.
*
*  \sideeffect
*   Calling the SDCard_SPIM_Wakeup() function without first calling the 
*   SDCard_SPIM_Sleep() function may produce unexpected behavior.
*
*******************************************************************************/
void SDCard_SPIM_Wakeup(void)
{
#if(SDCard_SPIM_SCB_MODE_UNCONFIG_CONST_CFG)

    if(SDCard_SPIM_SCB_WAKE_ENABLE_CHECK)
    {
        if(SDCard_SPIM_SCB_MODE_I2C_RUNTM_CFG)
        {
            SDCard_SPIM_I2CRestoreConfig();
        }
        else if(SDCard_SPIM_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            SDCard_SPIM_EzI2CRestoreConfig();
        }
    #if(!SDCard_SPIM_CY_SCBIP_V1)
        else if(SDCard_SPIM_SCB_MODE_SPI_RUNTM_CFG)
        {
            SDCard_SPIM_SpiRestoreConfig();
        }
        else if(SDCard_SPIM_SCB_MODE_UART_RUNTM_CFG)
        {
            SDCard_SPIM_UartRestoreConfig();
        }
    #endif /* (!SDCard_SPIM_CY_SCBIP_V1) */
        else
        {
            /* Unknown mode */
        }
    }
    else
    {
        if(0u != SDCard_SPIM_backup.enableState)
        {
            SDCard_SPIM_Enable();
        }
    }

#else

    #if (SDCard_SPIM_SCB_MODE_I2C_CONST_CFG  && SDCard_SPIM_I2C_WAKE_ENABLE_CONST)
        SDCard_SPIM_I2CRestoreConfig();

    #elif (SDCard_SPIM_SCB_MODE_EZI2C_CONST_CFG && SDCard_SPIM_EZI2C_WAKE_ENABLE_CONST)
        SDCard_SPIM_EzI2CRestoreConfig();

    #elif (SDCard_SPIM_SCB_MODE_SPI_CONST_CFG && SDCard_SPIM_SPI_WAKE_ENABLE_CONST)
        SDCard_SPIM_SpiRestoreConfig();

    #elif (SDCard_SPIM_SCB_MODE_UART_CONST_CFG && SDCard_SPIM_UART_WAKE_ENABLE_CONST)
        SDCard_SPIM_UartRestoreConfig();

    #else

        if(0u != SDCard_SPIM_backup.enableState)
        {
            SDCard_SPIM_Enable();
        }

    #endif /* (SDCard_SPIM_I2C_WAKE_ENABLE_CONST) */

#endif /* (SDCard_SPIM_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/* [] END OF FILE */
