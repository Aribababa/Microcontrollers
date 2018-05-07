/***************************************************************************//**
* \file SDCard_SPIM_SPI.c
* \version 3.20
*
* \brief
*  This file provides the source code to the API for the SCB Component in
*  SPI mode.
*
* Note:
*
*******************************************************************************
* \copyright
* Copyright 2013-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "SDCard_SPIM_PVT.h"
#include "SDCard_SPIM_SPI_UART_PVT.h"

#if(SDCard_SPIM_SCB_MODE_UNCONFIG_CONST_CFG)

    /***************************************
    *  Configuration Structure Initialization
    ***************************************/

    const SDCard_SPIM_SPI_INIT_STRUCT SDCard_SPIM_configSpi =
    {
        SDCard_SPIM_SPI_MODE,
        SDCard_SPIM_SPI_SUB_MODE,
        SDCard_SPIM_SPI_CLOCK_MODE,
        SDCard_SPIM_SPI_OVS_FACTOR,
        SDCard_SPIM_SPI_MEDIAN_FILTER_ENABLE,
        SDCard_SPIM_SPI_LATE_MISO_SAMPLE_ENABLE,
        SDCard_SPIM_SPI_WAKE_ENABLE,
        SDCard_SPIM_SPI_RX_DATA_BITS_NUM,
        SDCard_SPIM_SPI_TX_DATA_BITS_NUM,
        SDCard_SPIM_SPI_BITS_ORDER,
        SDCard_SPIM_SPI_TRANSFER_SEPARATION,
        0u,
        NULL,
        0u,
        NULL,
        (uint32) SDCard_SPIM_SCB_IRQ_INTERNAL,
        SDCard_SPIM_SPI_INTR_RX_MASK,
        SDCard_SPIM_SPI_RX_TRIGGER_LEVEL,
        SDCard_SPIM_SPI_INTR_TX_MASK,
        SDCard_SPIM_SPI_TX_TRIGGER_LEVEL,
        (uint8) SDCard_SPIM_SPI_BYTE_MODE_ENABLE,
        (uint8) SDCard_SPIM_SPI_FREE_RUN_SCLK_ENABLE,
        (uint8) SDCard_SPIM_SPI_SS_POLARITY
    };


    /*******************************************************************************
    * Function Name: SDCard_SPIM_SpiInit
    ****************************************************************************//**
    *
    *  Configures the SDCard_SPIM for SPI operation.
    *
    *  This function is intended specifically to be used when the SDCard_SPIM 
    *  configuration is set to “Unconfigured SDCard_SPIM” in the customizer. 
    *  After initializing the SDCard_SPIM in SPI mode using this function, 
    *  the component can be enabled using the SDCard_SPIM_Start() or 
    * SDCard_SPIM_Enable() function.
    *  This function uses a pointer to a structure that provides the configuration 
    *  settings. This structure contains the same information that would otherwise 
    *  be provided by the customizer settings.
    *
    *  \param config: pointer to a structure that contains the following list of 
    *   fields. These fields match the selections available in the customizer. 
    *   Refer to the customizer for further description of the settings.
    *
    *******************************************************************************/
    void SDCard_SPIM_SpiInit(const SDCard_SPIM_SPI_INIT_STRUCT *config)
    {
        if(NULL == config)
        {
            CYASSERT(0u != 0u); /* Halt execution due to bad function parameter */
        }
        else
        {
            /* Configure pins */
            SDCard_SPIM_SetPins(SDCard_SPIM_SCB_MODE_SPI, config->mode, SDCard_SPIM_DUMMY_PARAM);

            /* Store internal configuration */
            SDCard_SPIM_scbMode       = (uint8) SDCard_SPIM_SCB_MODE_SPI;
            SDCard_SPIM_scbEnableWake = (uint8) config->enableWake;
            SDCard_SPIM_scbEnableIntr = (uint8) config->enableInterrupt;

            /* Set RX direction internal variables */
            SDCard_SPIM_rxBuffer      =         config->rxBuffer;
            SDCard_SPIM_rxDataBits    = (uint8) config->rxDataBits;
            SDCard_SPIM_rxBufferSize  = (uint8) config->rxBufferSize;

            /* Set TX direction internal variables */
            SDCard_SPIM_txBuffer      =         config->txBuffer;
            SDCard_SPIM_txDataBits    = (uint8) config->txDataBits;
            SDCard_SPIM_txBufferSize  = (uint8) config->txBufferSize;

            /* Configure SPI interface */
            SDCard_SPIM_CTRL_REG     = SDCard_SPIM_GET_CTRL_OVS(config->oversample)           |
                                            SDCard_SPIM_GET_CTRL_BYTE_MODE(config->enableByteMode) |
                                            SDCard_SPIM_GET_CTRL_EC_AM_MODE(config->enableWake)    |
                                            SDCard_SPIM_CTRL_SPI;

            SDCard_SPIM_SPI_CTRL_REG = SDCard_SPIM_GET_SPI_CTRL_CONTINUOUS    (config->transferSeperation)  |
                                            SDCard_SPIM_GET_SPI_CTRL_SELECT_PRECEDE(config->submode &
                                                                          SDCard_SPIM_SPI_MODE_TI_PRECEDES_MASK) |
                                            SDCard_SPIM_GET_SPI_CTRL_SCLK_MODE     (config->sclkMode)            |
                                            SDCard_SPIM_GET_SPI_CTRL_LATE_MISO_SAMPLE(config->enableLateSampling)|
                                            SDCard_SPIM_GET_SPI_CTRL_SCLK_CONTINUOUS(config->enableFreeRunSclk)  |
                                            SDCard_SPIM_GET_SPI_CTRL_SSEL_POLARITY (config->polaritySs)          |
                                            SDCard_SPIM_GET_SPI_CTRL_SUB_MODE      (config->submode)             |
                                            SDCard_SPIM_GET_SPI_CTRL_MASTER_MODE   (config->mode);

            /* Configure RX direction */
            SDCard_SPIM_RX_CTRL_REG     =  SDCard_SPIM_GET_RX_CTRL_DATA_WIDTH(config->rxDataBits)         |
                                                SDCard_SPIM_GET_RX_CTRL_BIT_ORDER (config->bitOrder)           |
                                                SDCard_SPIM_GET_RX_CTRL_MEDIAN    (config->enableMedianFilter) |
                                                SDCard_SPIM_SPI_RX_CTRL;

            SDCard_SPIM_RX_FIFO_CTRL_REG = SDCard_SPIM_GET_RX_FIFO_CTRL_TRIGGER_LEVEL(config->rxTriggerLevel);

            /* Configure TX direction */
            SDCard_SPIM_TX_CTRL_REG      = SDCard_SPIM_GET_TX_CTRL_DATA_WIDTH(config->txDataBits) |
                                                SDCard_SPIM_GET_TX_CTRL_BIT_ORDER (config->bitOrder)   |
                                                SDCard_SPIM_SPI_TX_CTRL;

            SDCard_SPIM_TX_FIFO_CTRL_REG = SDCard_SPIM_GET_TX_FIFO_CTRL_TRIGGER_LEVEL(config->txTriggerLevel);

            /* Configure interrupt with SPI handler but do not enable it */
            CyIntDisable    (SDCard_SPIM_ISR_NUMBER);
            CyIntSetPriority(SDCard_SPIM_ISR_NUMBER, SDCard_SPIM_ISR_PRIORITY);
            (void) CyIntSetVector(SDCard_SPIM_ISR_NUMBER, &SDCard_SPIM_SPI_UART_ISR);

            /* Configure interrupt sources */
            SDCard_SPIM_INTR_I2C_EC_MASK_REG = SDCard_SPIM_NO_INTR_SOURCES;
            SDCard_SPIM_INTR_SPI_EC_MASK_REG = SDCard_SPIM_NO_INTR_SOURCES;
            SDCard_SPIM_INTR_SLAVE_MASK_REG  = SDCard_SPIM_GET_SPI_INTR_SLAVE_MASK(config->rxInterruptMask);
            SDCard_SPIM_INTR_MASTER_MASK_REG = SDCard_SPIM_GET_SPI_INTR_MASTER_MASK(config->txInterruptMask);
            SDCard_SPIM_INTR_RX_MASK_REG     = SDCard_SPIM_GET_SPI_INTR_RX_MASK(config->rxInterruptMask);
            SDCard_SPIM_INTR_TX_MASK_REG     = SDCard_SPIM_GET_SPI_INTR_TX_MASK(config->txInterruptMask);
            
            /* Configure TX interrupt sources to restore. */
            SDCard_SPIM_IntrTxMask = LO16(SDCard_SPIM_INTR_TX_MASK_REG);

            /* Set active SS0 */
            SDCard_SPIM_SpiSetActiveSlaveSelect(SDCard_SPIM_SPI_SLAVE_SELECT0);

            /* Clear RX buffer indexes */
            SDCard_SPIM_rxBufferHead     = 0u;
            SDCard_SPIM_rxBufferTail     = 0u;
            SDCard_SPIM_rxBufferOverflow = 0u;

            /* Clear TX buffer indexes */
            SDCard_SPIM_txBufferHead = 0u;
            SDCard_SPIM_txBufferTail = 0u;
        }
    }

#else

    /*******************************************************************************
    * Function Name: SDCard_SPIM_SpiInit
    ****************************************************************************//**
    *
    *  Configures the SCB for the SPI operation.
    *
    *******************************************************************************/
    void SDCard_SPIM_SpiInit(void)
    {
        /* Configure SPI interface */
        SDCard_SPIM_CTRL_REG     = SDCard_SPIM_SPI_DEFAULT_CTRL;
        SDCard_SPIM_SPI_CTRL_REG = SDCard_SPIM_SPI_DEFAULT_SPI_CTRL;

        /* Configure TX and RX direction */
        SDCard_SPIM_RX_CTRL_REG      = SDCard_SPIM_SPI_DEFAULT_RX_CTRL;
        SDCard_SPIM_RX_FIFO_CTRL_REG = SDCard_SPIM_SPI_DEFAULT_RX_FIFO_CTRL;

        /* Configure TX and RX direction */
        SDCard_SPIM_TX_CTRL_REG      = SDCard_SPIM_SPI_DEFAULT_TX_CTRL;
        SDCard_SPIM_TX_FIFO_CTRL_REG = SDCard_SPIM_SPI_DEFAULT_TX_FIFO_CTRL;

        /* Configure interrupt with SPI handler but do not enable it */
    #if(SDCard_SPIM_SCB_IRQ_INTERNAL)
            CyIntDisable    (SDCard_SPIM_ISR_NUMBER);
            CyIntSetPriority(SDCard_SPIM_ISR_NUMBER, SDCard_SPIM_ISR_PRIORITY);
            (void) CyIntSetVector(SDCard_SPIM_ISR_NUMBER, &SDCard_SPIM_SPI_UART_ISR);
    #endif /* (SDCard_SPIM_SCB_IRQ_INTERNAL) */

        /* Configure interrupt sources */
        SDCard_SPIM_INTR_I2C_EC_MASK_REG = SDCard_SPIM_SPI_DEFAULT_INTR_I2C_EC_MASK;
        SDCard_SPIM_INTR_SPI_EC_MASK_REG = SDCard_SPIM_SPI_DEFAULT_INTR_SPI_EC_MASK;
        SDCard_SPIM_INTR_SLAVE_MASK_REG  = SDCard_SPIM_SPI_DEFAULT_INTR_SLAVE_MASK;
        SDCard_SPIM_INTR_MASTER_MASK_REG = SDCard_SPIM_SPI_DEFAULT_INTR_MASTER_MASK;
        SDCard_SPIM_INTR_RX_MASK_REG     = SDCard_SPIM_SPI_DEFAULT_INTR_RX_MASK;
        SDCard_SPIM_INTR_TX_MASK_REG     = SDCard_SPIM_SPI_DEFAULT_INTR_TX_MASK;

        /* Configure TX interrupt sources to restore. */
        SDCard_SPIM_IntrTxMask = LO16(SDCard_SPIM_INTR_TX_MASK_REG);
            
        /* Set active SS0 for master */
    #if (SDCard_SPIM_SPI_MASTER_CONST)
        SDCard_SPIM_SpiSetActiveSlaveSelect(SDCard_SPIM_SPI_SLAVE_SELECT0);
    #endif /* (SDCard_SPIM_SPI_MASTER_CONST) */

    #if(SDCard_SPIM_INTERNAL_RX_SW_BUFFER_CONST)
        SDCard_SPIM_rxBufferHead     = 0u;
        SDCard_SPIM_rxBufferTail     = 0u;
        SDCard_SPIM_rxBufferOverflow = 0u;
    #endif /* (SDCard_SPIM_INTERNAL_RX_SW_BUFFER_CONST) */

    #if(SDCard_SPIM_INTERNAL_TX_SW_BUFFER_CONST)
        SDCard_SPIM_txBufferHead = 0u;
        SDCard_SPIM_txBufferTail = 0u;
    #endif /* (SDCard_SPIM_INTERNAL_TX_SW_BUFFER_CONST) */
    }
#endif /* (SDCard_SPIM_SCB_MODE_UNCONFIG_CONST_CFG) */


/*******************************************************************************
* Function Name: SDCard_SPIM_SpiPostEnable
****************************************************************************//**
*
*  Restores HSIOM settings for the SPI master output pins (SCLK and/or SS0-SS3) 
*  to be controlled by the SCB SPI.
*
*******************************************************************************/
void SDCard_SPIM_SpiPostEnable(void)
{
#if(SDCard_SPIM_SCB_MODE_UNCONFIG_CONST_CFG)

    if (SDCard_SPIM_CHECK_SPI_MASTER)
    {
    #if (SDCard_SPIM_SCLK_PIN)
        /* Set SCB SPI to drive output pin */
        SDCard_SPIM_SET_HSIOM_SEL(SDCard_SPIM_SCLK_HSIOM_REG, SDCard_SPIM_SCLK_HSIOM_MASK,
                                       SDCard_SPIM_SCLK_HSIOM_POS, SDCard_SPIM_SCLK_HSIOM_SEL_SPI);
    #endif /* (SDCard_SPIM_SCLK_PIN) */

    #if (SDCard_SPIM_SS0_PIN)
        /* Set SCB SPI to drive output pin */
        SDCard_SPIM_SET_HSIOM_SEL(SDCard_SPIM_SS0_HSIOM_REG, SDCard_SPIM_SS0_HSIOM_MASK,
                                       SDCard_SPIM_SS0_HSIOM_POS, SDCard_SPIM_SS0_HSIOM_SEL_SPI);
    #endif /* (SDCard_SPIM_SS0_PIN) */

    #if (SDCard_SPIM_SS1_PIN)
        /* Set SCB SPI to drive output pin */
        SDCard_SPIM_SET_HSIOM_SEL(SDCard_SPIM_SS1_HSIOM_REG, SDCard_SPIM_SS1_HSIOM_MASK,
                                       SDCard_SPIM_SS1_HSIOM_POS, SDCard_SPIM_SS1_HSIOM_SEL_SPI);
    #endif /* (SDCard_SPIM_SS1_PIN) */

    #if (SDCard_SPIM_SS2_PIN)
        /* Set SCB SPI to drive output pin */
        SDCard_SPIM_SET_HSIOM_SEL(SDCard_SPIM_SS2_HSIOM_REG, SDCard_SPIM_SS2_HSIOM_MASK,
                                       SDCard_SPIM_SS2_HSIOM_POS, SDCard_SPIM_SS2_HSIOM_SEL_SPI);
    #endif /* (SDCard_SPIM_SS2_PIN) */

    #if (SDCard_SPIM_SS3_PIN)
        /* Set SCB SPI to drive output pin */
        SDCard_SPIM_SET_HSIOM_SEL(SDCard_SPIM_SS3_HSIOM_REG, SDCard_SPIM_SS3_HSIOM_MASK,
                                       SDCard_SPIM_SS3_HSIOM_POS, SDCard_SPIM_SS3_HSIOM_SEL_SPI);
    #endif /* (SDCard_SPIM_SS3_PIN) */
    }

#else

    #if (SDCard_SPIM_SPI_MASTER_SCLK_PIN)
        /* Set SCB SPI to drive output pin */
        SDCard_SPIM_SET_HSIOM_SEL(SDCard_SPIM_SCLK_M_HSIOM_REG, SDCard_SPIM_SCLK_M_HSIOM_MASK,
                                       SDCard_SPIM_SCLK_M_HSIOM_POS, SDCard_SPIM_SCLK_M_HSIOM_SEL_SPI);
    #endif /* (SDCard_SPIM_MISO_SDA_TX_PIN_PIN) */

    #if (SDCard_SPIM_SPI_MASTER_SS0_PIN)
        /* Set SCB SPI to drive output pin */
        SDCard_SPIM_SET_HSIOM_SEL(SDCard_SPIM_SS0_M_HSIOM_REG, SDCard_SPIM_SS0_M_HSIOM_MASK,
                                       SDCard_SPIM_SS0_M_HSIOM_POS, SDCard_SPIM_SS0_M_HSIOM_SEL_SPI);
    #endif /* (SDCard_SPIM_SPI_MASTER_SS0_PIN) */

    #if (SDCard_SPIM_SPI_MASTER_SS1_PIN)
        /* Set SCB SPI to drive output pin */
        SDCard_SPIM_SET_HSIOM_SEL(SDCard_SPIM_SS1_M_HSIOM_REG, SDCard_SPIM_SS1_M_HSIOM_MASK,
                                       SDCard_SPIM_SS1_M_HSIOM_POS, SDCard_SPIM_SS1_M_HSIOM_SEL_SPI);
    #endif /* (SDCard_SPIM_SPI_MASTER_SS1_PIN) */

    #if (SDCard_SPIM_SPI_MASTER_SS2_PIN)
        /* Set SCB SPI to drive output pin */
        SDCard_SPIM_SET_HSIOM_SEL(SDCard_SPIM_SS2_M_HSIOM_REG, SDCard_SPIM_SS2_M_HSIOM_MASK,
                                       SDCard_SPIM_SS2_M_HSIOM_POS, SDCard_SPIM_SS2_M_HSIOM_SEL_SPI);
    #endif /* (SDCard_SPIM_SPI_MASTER_SS2_PIN) */

    #if (SDCard_SPIM_SPI_MASTER_SS3_PIN)
        /* Set SCB SPI to drive output pin */
        SDCard_SPIM_SET_HSIOM_SEL(SDCard_SPIM_SS3_M_HSIOM_REG, SDCard_SPIM_SS3_M_HSIOM_MASK,
                                       SDCard_SPIM_SS3_M_HSIOM_POS, SDCard_SPIM_SS3_M_HSIOM_SEL_SPI);
    #endif /* (SDCard_SPIM_SPI_MASTER_SS3_PIN) */

#endif /* (SDCard_SPIM_SCB_MODE_UNCONFIG_CONST_CFG) */

    /* Restore TX interrupt sources. */
    SDCard_SPIM_SetTxInterruptMode(SDCard_SPIM_IntrTxMask);
}


/*******************************************************************************
* Function Name: SDCard_SPIM_SpiStop
****************************************************************************//**
*
*  Changes the HSIOM settings for the SPI master output pins 
*  (SCLK and/or SS0-SS3) to keep them inactive after the block is disabled. 
*  The output pins are controlled by the GPIO data register.
*
*******************************************************************************/
void SDCard_SPIM_SpiStop(void)
{
#if(SDCard_SPIM_SCB_MODE_UNCONFIG_CONST_CFG)

    if (SDCard_SPIM_CHECK_SPI_MASTER)
    {
    #if (SDCard_SPIM_SCLK_PIN)
        /* Set output pin state after block is disabled */
        SDCard_SPIM_spi_sclk_Write(SDCard_SPIM_GET_SPI_SCLK_INACTIVE);

        /* Set GPIO to drive output pin */
        SDCard_SPIM_SET_HSIOM_SEL(SDCard_SPIM_SCLK_HSIOM_REG, SDCard_SPIM_SCLK_HSIOM_MASK,
                                       SDCard_SPIM_SCLK_HSIOM_POS, SDCard_SPIM_SCLK_HSIOM_SEL_GPIO);
    #endif /* (SDCard_SPIM_spi_sclk_PIN) */

    #if (SDCard_SPIM_SS0_PIN)
        /* Set output pin state after block is disabled */
        SDCard_SPIM_spi_ss0_Write(SDCard_SPIM_GET_SPI_SS0_INACTIVE);

        /* Set GPIO to drive output pin */
        SDCard_SPIM_SET_HSIOM_SEL(SDCard_SPIM_SS0_HSIOM_REG, SDCard_SPIM_SS0_HSIOM_MASK,
                                       SDCard_SPIM_SS0_HSIOM_POS, SDCard_SPIM_SS0_HSIOM_SEL_GPIO);
    #endif /* (SDCard_SPIM_spi_ss0_PIN) */

    #if (SDCard_SPIM_SS1_PIN)
        /* Set output pin state after block is disabled */
        SDCard_SPIM_spi_ss1_Write(SDCard_SPIM_GET_SPI_SS1_INACTIVE);

        /* Set GPIO to drive output pin */
        SDCard_SPIM_SET_HSIOM_SEL(SDCard_SPIM_SS1_HSIOM_REG, SDCard_SPIM_SS1_HSIOM_MASK,
                                       SDCard_SPIM_SS1_HSIOM_POS, SDCard_SPIM_SS1_HSIOM_SEL_GPIO);
    #endif /* (SDCard_SPIM_SS1_PIN) */

    #if (SDCard_SPIM_SS2_PIN)
        /* Set output pin state after block is disabled */
        SDCard_SPIM_spi_ss2_Write(SDCard_SPIM_GET_SPI_SS2_INACTIVE);

        /* Set GPIO to drive output pin */
        SDCard_SPIM_SET_HSIOM_SEL(SDCard_SPIM_SS2_HSIOM_REG, SDCard_SPIM_SS2_HSIOM_MASK,
                                       SDCard_SPIM_SS2_HSIOM_POS, SDCard_SPIM_SS2_HSIOM_SEL_GPIO);
    #endif /* (SDCard_SPIM_SS2_PIN) */

    #if (SDCard_SPIM_SS3_PIN)
        /* Set output pin state after block is disabled */
        SDCard_SPIM_spi_ss3_Write(SDCard_SPIM_GET_SPI_SS3_INACTIVE);

        /* Set GPIO to drive output pin */
        SDCard_SPIM_SET_HSIOM_SEL(SDCard_SPIM_SS3_HSIOM_REG, SDCard_SPIM_SS3_HSIOM_MASK,
                                       SDCard_SPIM_SS3_HSIOM_POS, SDCard_SPIM_SS3_HSIOM_SEL_GPIO);
    #endif /* (SDCard_SPIM_SS3_PIN) */
    
        /* Store TX interrupt sources (exclude level triggered) for master. */
        SDCard_SPIM_IntrTxMask = LO16(SDCard_SPIM_GetTxInterruptMode() & SDCard_SPIM_INTR_SPIM_TX_RESTORE);
    }
    else
    {
        /* Store TX interrupt sources (exclude level triggered) for slave. */
        SDCard_SPIM_IntrTxMask = LO16(SDCard_SPIM_GetTxInterruptMode() & SDCard_SPIM_INTR_SPIS_TX_RESTORE);
    }

#else

#if (SDCard_SPIM_SPI_MASTER_SCLK_PIN)
    /* Set output pin state after block is disabled */
    SDCard_SPIM_sclk_m_Write(SDCard_SPIM_GET_SPI_SCLK_INACTIVE);

    /* Set GPIO to drive output pin */
    SDCard_SPIM_SET_HSIOM_SEL(SDCard_SPIM_SCLK_M_HSIOM_REG, SDCard_SPIM_SCLK_M_HSIOM_MASK,
                                   SDCard_SPIM_SCLK_M_HSIOM_POS, SDCard_SPIM_SCLK_M_HSIOM_SEL_GPIO);
#endif /* (SDCard_SPIM_MISO_SDA_TX_PIN_PIN) */

#if (SDCard_SPIM_SPI_MASTER_SS0_PIN)
    /* Set output pin state after block is disabled */
    SDCard_SPIM_ss0_m_Write(SDCard_SPIM_GET_SPI_SS0_INACTIVE);

    /* Set GPIO to drive output pin */
    SDCard_SPIM_SET_HSIOM_SEL(SDCard_SPIM_SS0_M_HSIOM_REG, SDCard_SPIM_SS0_M_HSIOM_MASK,
                                   SDCard_SPIM_SS0_M_HSIOM_POS, SDCard_SPIM_SS0_M_HSIOM_SEL_GPIO);
#endif /* (SDCard_SPIM_SPI_MASTER_SS0_PIN) */

#if (SDCard_SPIM_SPI_MASTER_SS1_PIN)
    /* Set output pin state after block is disabled */
    SDCard_SPIM_ss1_m_Write(SDCard_SPIM_GET_SPI_SS1_INACTIVE);

    /* Set GPIO to drive output pin */
    SDCard_SPIM_SET_HSIOM_SEL(SDCard_SPIM_SS1_M_HSIOM_REG, SDCard_SPIM_SS1_M_HSIOM_MASK,
                                   SDCard_SPIM_SS1_M_HSIOM_POS, SDCard_SPIM_SS1_M_HSIOM_SEL_GPIO);
#endif /* (SDCard_SPIM_SPI_MASTER_SS1_PIN) */

#if (SDCard_SPIM_SPI_MASTER_SS2_PIN)
    /* Set output pin state after block is disabled */
    SDCard_SPIM_ss2_m_Write(SDCard_SPIM_GET_SPI_SS2_INACTIVE);

    /* Set GPIO to drive output pin */
    SDCard_SPIM_SET_HSIOM_SEL(SDCard_SPIM_SS2_M_HSIOM_REG, SDCard_SPIM_SS2_M_HSIOM_MASK,
                                   SDCard_SPIM_SS2_M_HSIOM_POS, SDCard_SPIM_SS2_M_HSIOM_SEL_GPIO);
#endif /* (SDCard_SPIM_SPI_MASTER_SS2_PIN) */

#if (SDCard_SPIM_SPI_MASTER_SS3_PIN)
    /* Set output pin state after block is disabled */
    SDCard_SPIM_ss3_m_Write(SDCard_SPIM_GET_SPI_SS3_INACTIVE);

    /* Set GPIO to drive output pin */
    SDCard_SPIM_SET_HSIOM_SEL(SDCard_SPIM_SS3_M_HSIOM_REG, SDCard_SPIM_SS3_M_HSIOM_MASK,
                                   SDCard_SPIM_SS3_M_HSIOM_POS, SDCard_SPIM_SS3_M_HSIOM_SEL_GPIO);
#endif /* (SDCard_SPIM_SPI_MASTER_SS3_PIN) */

    #if (SDCard_SPIM_SPI_MASTER_CONST)
        /* Store TX interrupt sources (exclude level triggered). */
        SDCard_SPIM_IntrTxMask = LO16(SDCard_SPIM_GetTxInterruptMode() & SDCard_SPIM_INTR_SPIM_TX_RESTORE);
    #else
        /* Store TX interrupt sources (exclude level triggered). */
        SDCard_SPIM_IntrTxMask = LO16(SDCard_SPIM_GetTxInterruptMode() & SDCard_SPIM_INTR_SPIS_TX_RESTORE);
    #endif /* (SDCard_SPIM_SPI_MASTER_CONST) */

#endif /* (SDCard_SPIM_SCB_MODE_UNCONFIG_CONST_CFG) */
}


#if (SDCard_SPIM_SPI_MASTER_CONST)
    /*******************************************************************************
    * Function Name: SDCard_SPIM_SetActiveSlaveSelect
    ****************************************************************************//**
    *
    *  Selects one of the four slave select lines to be active during the transfer.
    *  After initialization the active slave select line is 0.
    *  The component should be in one of the following states to change the active
    *  slave select signal source correctly:
    *   - The component is disabled.
    *   - The component has completed transfer (TX FIFO is empty and the
    *     SCB_INTR_MASTER_SPI_DONE status is set).
    *
    *  This function does not check that these conditions are met.
    *  This function is only applicable to SPI Master mode of operation.
    *
    *  \param slaveSelect: slave select line which will be active while the following
    *   transfer.
    *   - SDCard_SPIM_SPI_SLAVE_SELECT0 - Slave select 0.
    *   - SDCard_SPIM_SPI_SLAVE_SELECT1 - Slave select 1.
    *   - SDCard_SPIM_SPI_SLAVE_SELECT2 - Slave select 2.
    *   - SDCard_SPIM_SPI_SLAVE_SELECT3 - Slave select 3.
    *
    *******************************************************************************/
    void SDCard_SPIM_SpiSetActiveSlaveSelect(uint32 slaveSelect)
    {
        uint32 spiCtrl;

        spiCtrl = SDCard_SPIM_SPI_CTRL_REG;

        spiCtrl &= (uint32) ~SDCard_SPIM_SPI_CTRL_SLAVE_SELECT_MASK;
        spiCtrl |= (uint32)  SDCard_SPIM_GET_SPI_CTRL_SS(slaveSelect);

        SDCard_SPIM_SPI_CTRL_REG = spiCtrl;
    }
#endif /* (SDCard_SPIM_SPI_MASTER_CONST) */


#if !(SDCard_SPIM_CY_SCBIP_V0 || SDCard_SPIM_CY_SCBIP_V1)
    /*******************************************************************************
    * Function Name: SDCard_SPIM_SpiSetSlaveSelectPolarity
    ****************************************************************************//**
    *
    *  Sets active polarity for slave select line.
    *  The component should be in one of the following states to change the active
    *  slave select signal source correctly:
    *   - The component is disabled.
    *   - The component has completed transfer.
    *  
    *  This function does not check that these conditions are met.
    *
    *  \param slaveSelect: slave select line to change active polarity.
    *   - SDCard_SPIM_SPI_SLAVE_SELECT0 - Slave select 0.
    *   - SDCard_SPIM_SPI_SLAVE_SELECT1 - Slave select 1.
    *   - SDCard_SPIM_SPI_SLAVE_SELECT2 - Slave select 2.
    *   - SDCard_SPIM_SPI_SLAVE_SELECT3 - Slave select 3.
    *
    *  \param polarity: active polarity of slave select line.
    *   - SDCard_SPIM_SPI_SS_ACTIVE_LOW  - Slave select is active low.
    *   - SDCard_SPIM_SPI_SS_ACTIVE_HIGH - Slave select is active high.
    *
    *******************************************************************************/
    void SDCard_SPIM_SpiSetSlaveSelectPolarity(uint32 slaveSelect, uint32 polarity)
    {
        uint32 ssPolarity;

        /* Get position of the polarity bit associated with slave select line */
        ssPolarity = SDCard_SPIM_GET_SPI_CTRL_SSEL_POLARITY((uint32) 1u << slaveSelect);

        if (0u != polarity)
        {
            SDCard_SPIM_SPI_CTRL_REG |= (uint32)  ssPolarity;
        }
        else
        {
            SDCard_SPIM_SPI_CTRL_REG &= (uint32) ~ssPolarity;
        }
    }
#endif /* !(SDCard_SPIM_CY_SCBIP_V0 || SDCard_SPIM_CY_SCBIP_V1) */


#if(SDCard_SPIM_SPI_WAKE_ENABLE_CONST)
    /*******************************************************************************
    * Function Name: SDCard_SPIM_SpiSaveConfig
    ****************************************************************************//**
    *
    *  Clears INTR_SPI_EC.WAKE_UP and enables it. This interrupt
    *  source triggers when the master assigns the SS line and wakes up the device.
    *
    *******************************************************************************/
    void SDCard_SPIM_SpiSaveConfig(void)
    {
        /* Clear and enable SPI wakeup interrupt source */
        SDCard_SPIM_ClearSpiExtClkInterruptSource(SDCard_SPIM_INTR_SPI_EC_WAKE_UP);
        SDCard_SPIM_SetSpiExtClkInterruptMode(SDCard_SPIM_INTR_SPI_EC_WAKE_UP);
    }


    /*******************************************************************************
    * Function Name: SDCard_SPIM_SpiRestoreConfig
    ****************************************************************************//**
    *
    *  Disables the INTR_SPI_EC.WAKE_UP interrupt source. After wakeup
    *  slave does not drive the MISO line and the master receives 0xFF.
    *
    *******************************************************************************/
    void SDCard_SPIM_SpiRestoreConfig(void)
    {
        /* Disable SPI wakeup interrupt source */
        SDCard_SPIM_SetSpiExtClkInterruptMode(SDCard_SPIM_NO_INTR_SOURCES);
    }
#endif /* (SDCard_SPIM_SPI_WAKE_ENABLE_CONST) */


/* [] END OF FILE */
