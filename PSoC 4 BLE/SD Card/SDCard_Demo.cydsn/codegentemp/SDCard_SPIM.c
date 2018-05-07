/***************************************************************************//**
* \file SDCard_SPIM.c
* \version 3.20
*
* \brief
*  This file provides the source code to the API for the SCB Component.
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

#if (SDCard_SPIM_SCB_MODE_I2C_INC)
    #include "SDCard_SPIM_I2C_PVT.h"
#endif /* (SDCard_SPIM_SCB_MODE_I2C_INC) */

#if (SDCard_SPIM_SCB_MODE_EZI2C_INC)
    #include "SDCard_SPIM_EZI2C_PVT.h"
#endif /* (SDCard_SPIM_SCB_MODE_EZI2C_INC) */

#if (SDCard_SPIM_SCB_MODE_SPI_INC || SDCard_SPIM_SCB_MODE_UART_INC)
    #include "SDCard_SPIM_SPI_UART_PVT.h"
#endif /* (SDCard_SPIM_SCB_MODE_SPI_INC || SDCard_SPIM_SCB_MODE_UART_INC) */


/***************************************
*    Run Time Configuration Vars
***************************************/

/* Stores internal component configuration for Unconfigured mode */
#if (SDCard_SPIM_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common configuration variables */
    uint8 SDCard_SPIM_scbMode = SDCard_SPIM_SCB_MODE_UNCONFIG;
    uint8 SDCard_SPIM_scbEnableWake;
    uint8 SDCard_SPIM_scbEnableIntr;

    /* I2C configuration variables */
    uint8 SDCard_SPIM_mode;
    uint8 SDCard_SPIM_acceptAddr;

    /* SPI/UART configuration variables */
    volatile uint8 * SDCard_SPIM_rxBuffer;
    uint8  SDCard_SPIM_rxDataBits;
    uint32 SDCard_SPIM_rxBufferSize;

    volatile uint8 * SDCard_SPIM_txBuffer;
    uint8  SDCard_SPIM_txDataBits;
    uint32 SDCard_SPIM_txBufferSize;

    /* EZI2C configuration variables */
    uint8 SDCard_SPIM_numberOfAddr;
    uint8 SDCard_SPIM_subAddrSize;
#endif /* (SDCard_SPIM_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*     Common SCB Vars
***************************************/
/**
* \addtogroup group_general
* \{
*/

/** SDCard_SPIM_initVar indicates whether the SDCard_SPIM 
*  component has been initialized. The variable is initialized to 0 
*  and set to 1 the first time SCB_Start() is called. This allows 
*  the component to restart without reinitialization after the first 
*  call to the SDCard_SPIM_Start() routine.
*
*  If re-initialization of the component is required, then the 
*  SDCard_SPIM_Init() function can be called before the 
*  SDCard_SPIM_Start() or SDCard_SPIM_Enable() function.
*/
uint8 SDCard_SPIM_initVar = 0u;


#if (! (SDCard_SPIM_SCB_MODE_I2C_CONST_CFG || \
        SDCard_SPIM_SCB_MODE_EZI2C_CONST_CFG))
    /** This global variable stores TX interrupt sources after 
    * SDCard_SPIM_Stop() is called. Only these TX interrupt sources 
    * will be restored on a subsequent SDCard_SPIM_Enable() call.
    */
    uint16 SDCard_SPIM_IntrTxMask = 0u;
#endif /* (! (SDCard_SPIM_SCB_MODE_I2C_CONST_CFG || \
              SDCard_SPIM_SCB_MODE_EZI2C_CONST_CFG)) */
/** \} globals */

#if (SDCard_SPIM_SCB_IRQ_INTERNAL)
#if !defined (CY_REMOVE_SDCard_SPIM_CUSTOM_INTR_HANDLER)
    void (*SDCard_SPIM_customIntrHandler)(void) = NULL;
#endif /* !defined (CY_REMOVE_SDCard_SPIM_CUSTOM_INTR_HANDLER) */
#endif /* (SDCard_SPIM_SCB_IRQ_INTERNAL) */


/***************************************
*    Private Function Prototypes
***************************************/

static void SDCard_SPIM_ScbEnableIntr(void);
static void SDCard_SPIM_ScbModeStop(void);
static void SDCard_SPIM_ScbModePostEnable(void);


/*******************************************************************************
* Function Name: SDCard_SPIM_Init
****************************************************************************//**
*
*  Initializes the SDCard_SPIM component to operate in one of the selected
*  configurations: I2C, SPI, UART or EZI2C.
*  When the configuration is set to "Unconfigured SCB", this function does
*  not do any initialization. Use mode-specific initialization APIs instead:
*  SDCard_SPIM_I2CInit, SDCard_SPIM_SpiInit, 
*  SDCard_SPIM_UartInit or SDCard_SPIM_EzI2CInit.
*
*******************************************************************************/
void SDCard_SPIM_Init(void)
{
#if (SDCard_SPIM_SCB_MODE_UNCONFIG_CONST_CFG)
    if (SDCard_SPIM_SCB_MODE_UNCONFIG_RUNTM_CFG)
    {
        SDCard_SPIM_initVar = 0u;
    }
    else
    {
        /* Initialization was done before this function call */
    }

#elif (SDCard_SPIM_SCB_MODE_I2C_CONST_CFG)
    SDCard_SPIM_I2CInit();

#elif (SDCard_SPIM_SCB_MODE_SPI_CONST_CFG)
    SDCard_SPIM_SpiInit();

#elif (SDCard_SPIM_SCB_MODE_UART_CONST_CFG)
    SDCard_SPIM_UartInit();

#elif (SDCard_SPIM_SCB_MODE_EZI2C_CONST_CFG)
    SDCard_SPIM_EzI2CInit();

#endif /* (SDCard_SPIM_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: SDCard_SPIM_Enable
****************************************************************************//**
*
*  Enables SDCard_SPIM component operation: activates the hardware and 
*  internal interrupt. It also restores TX interrupt sources disabled after the 
*  SDCard_SPIM_Stop() function was called (note that level-triggered TX 
*  interrupt sources remain disabled to not cause code lock-up).
*  For I2C and EZI2C modes the interrupt is internal and mandatory for 
*  operation. For SPI and UART modes the interrupt can be configured as none, 
*  internal or external.
*  The SDCard_SPIM configuration should be not changed when the component
*  is enabled. Any configuration changes should be made after disabling the 
*  component.
*  When configuration is set to “Unconfigured SDCard_SPIM”, the component 
*  must first be initialized to operate in one of the following configurations: 
*  I2C, SPI, UART or EZ I2C, using the mode-specific initialization API. 
*  Otherwise this function does not enable the component.
*
*******************************************************************************/
void SDCard_SPIM_Enable(void)
{
#if (SDCard_SPIM_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Enable SCB block, only if it is already configured */
    if (!SDCard_SPIM_SCB_MODE_UNCONFIG_RUNTM_CFG)
    {
        SDCard_SPIM_CTRL_REG |= SDCard_SPIM_CTRL_ENABLED;

        SDCard_SPIM_ScbEnableIntr();

        /* Call PostEnable function specific to current operation mode */
        SDCard_SPIM_ScbModePostEnable();
    }
#else
    SDCard_SPIM_CTRL_REG |= SDCard_SPIM_CTRL_ENABLED;

    SDCard_SPIM_ScbEnableIntr();

    /* Call PostEnable function specific to current operation mode */
    SDCard_SPIM_ScbModePostEnable();
#endif /* (SDCard_SPIM_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: SDCard_SPIM_Start
****************************************************************************//**
*
*  Invokes SDCard_SPIM_Init() and SDCard_SPIM_Enable().
*  After this function call, the component is enabled and ready for operation.
*  When configuration is set to "Unconfigured SCB", the component must first be
*  initialized to operate in one of the following configurations: I2C, SPI, UART
*  or EZI2C. Otherwise this function does not enable the component.
*
* \globalvars
*  SDCard_SPIM_initVar - used to check initial configuration, modified
*  on first function call.
*
*******************************************************************************/
void SDCard_SPIM_Start(void)
{
    if (0u == SDCard_SPIM_initVar)
    {
        SDCard_SPIM_Init();
        SDCard_SPIM_initVar = 1u; /* Component was initialized */
    }

    SDCard_SPIM_Enable();
}


/*******************************************************************************
* Function Name: SDCard_SPIM_Stop
****************************************************************************//**
*
*  Disables the SDCard_SPIM component: disable the hardware and internal 
*  interrupt. It also disables all TX interrupt sources so as not to cause an 
*  unexpected interrupt trigger because after the component is enabled, the 
*  TX FIFO is empty.
*  Refer to the function SDCard_SPIM_Enable() for the interrupt 
*  configuration details.
*  This function disables the SCB component without checking to see if 
*  communication is in progress. Before calling this function it may be 
*  necessary to check the status of communication to make sure communication 
*  is complete. If this is not done then communication could be stopped mid 
*  byte and corrupted data could result.
*
*******************************************************************************/
void SDCard_SPIM_Stop(void)
{
#if (SDCard_SPIM_SCB_IRQ_INTERNAL)
    SDCard_SPIM_DisableInt();
#endif /* (SDCard_SPIM_SCB_IRQ_INTERNAL) */

    /* Call Stop function specific to current operation mode */
    SDCard_SPIM_ScbModeStop();

    /* Disable SCB IP */
    SDCard_SPIM_CTRL_REG &= (uint32) ~SDCard_SPIM_CTRL_ENABLED;

    /* Disable all TX interrupt sources so as not to cause an unexpected
    * interrupt trigger after the component will be enabled because the 
    * TX FIFO is empty.
    * For SCB IP v0, it is critical as it does not mask-out interrupt
    * sources when it is disabled. This can cause a code lock-up in the
    * interrupt handler because TX FIFO cannot be loaded after the block
    * is disabled.
    */
    SDCard_SPIM_SetTxInterruptMode(SDCard_SPIM_NO_INTR_SOURCES);

#if (SDCard_SPIM_SCB_IRQ_INTERNAL)
    SDCard_SPIM_ClearPendingInt();
#endif /* (SDCard_SPIM_SCB_IRQ_INTERNAL) */
}


/*******************************************************************************
* Function Name: SDCard_SPIM_SetRxFifoLevel
****************************************************************************//**
*
*  Sets level in the RX FIFO to generate a RX level interrupt.
*  When the RX FIFO has more entries than the RX FIFO level an RX level
*  interrupt request is generated.
*
*  \param level: Level in the RX FIFO to generate RX level interrupt.
*   The range of valid level values is between 0 and RX FIFO depth - 1.
*
*******************************************************************************/
void SDCard_SPIM_SetRxFifoLevel(uint32 level)
{
    uint32 rxFifoCtrl;

    rxFifoCtrl = SDCard_SPIM_RX_FIFO_CTRL_REG;

    rxFifoCtrl &= ((uint32) ~SDCard_SPIM_RX_FIFO_CTRL_TRIGGER_LEVEL_MASK); /* Clear level mask bits */
    rxFifoCtrl |= ((uint32) (SDCard_SPIM_RX_FIFO_CTRL_TRIGGER_LEVEL_MASK & level));

    SDCard_SPIM_RX_FIFO_CTRL_REG = rxFifoCtrl;
}


/*******************************************************************************
* Function Name: SDCard_SPIM_SetTxFifoLevel
****************************************************************************//**
*
*  Sets level in the TX FIFO to generate a TX level interrupt.
*  When the TX FIFO has less entries than the TX FIFO level an TX level
*  interrupt request is generated.
*
*  \param level: Level in the TX FIFO to generate TX level interrupt.
*   The range of valid level values is between 0 and TX FIFO depth - 1.
*
*******************************************************************************/
void SDCard_SPIM_SetTxFifoLevel(uint32 level)
{
    uint32 txFifoCtrl;

    txFifoCtrl = SDCard_SPIM_TX_FIFO_CTRL_REG;

    txFifoCtrl &= ((uint32) ~SDCard_SPIM_TX_FIFO_CTRL_TRIGGER_LEVEL_MASK); /* Clear level mask bits */
    txFifoCtrl |= ((uint32) (SDCard_SPIM_TX_FIFO_CTRL_TRIGGER_LEVEL_MASK & level));

    SDCard_SPIM_TX_FIFO_CTRL_REG = txFifoCtrl;
}


#if (SDCard_SPIM_SCB_IRQ_INTERNAL)
    /*******************************************************************************
    * Function Name: SDCard_SPIM_SetCustomInterruptHandler
    ****************************************************************************//**
    *
    *  Registers a function to be called by the internal interrupt handler.
    *  First the function that is registered is called, then the internal interrupt
    *  handler performs any operation such as software buffer management functions
    *  before the interrupt returns.  It is the user's responsibility not to break
    *  the software buffer operations. Only one custom handler is supported, which
    *  is the function provided by the most recent call.
    *  At the initialization time no custom handler is registered.
    *
    *  \param func: Pointer to the function to register.
    *        The value NULL indicates to remove the current custom interrupt
    *        handler.
    *
    *******************************************************************************/
    void SDCard_SPIM_SetCustomInterruptHandler(void (*func)(void))
    {
    #if !defined (CY_REMOVE_SDCard_SPIM_CUSTOM_INTR_HANDLER)
        SDCard_SPIM_customIntrHandler = func; /* Register interrupt handler */
    #else
        if (NULL != func)
        {
            /* Suppress compiler warning */
        }
    #endif /* !defined (CY_REMOVE_SDCard_SPIM_CUSTOM_INTR_HANDLER) */
    }
#endif /* (SDCard_SPIM_SCB_IRQ_INTERNAL) */


/*******************************************************************************
* Function Name: SDCard_SPIM_ScbModeEnableIntr
****************************************************************************//**
*
*  Enables an interrupt for a specific mode.
*
*******************************************************************************/
static void SDCard_SPIM_ScbEnableIntr(void)
{
#if (SDCard_SPIM_SCB_IRQ_INTERNAL)
    /* Enable interrupt in NVIC */
    #if (SDCard_SPIM_SCB_MODE_UNCONFIG_CONST_CFG)
        if (0u != SDCard_SPIM_scbEnableIntr)
        {
            SDCard_SPIM_EnableInt();
        }

    #else
        SDCard_SPIM_EnableInt();

    #endif /* (SDCard_SPIM_SCB_MODE_UNCONFIG_CONST_CFG) */
#endif /* (SDCard_SPIM_SCB_IRQ_INTERNAL) */
}


/*******************************************************************************
* Function Name: SDCard_SPIM_ScbModePostEnable
****************************************************************************//**
*
*  Calls the PostEnable function for a specific operation mode.
*
*******************************************************************************/
static void SDCard_SPIM_ScbModePostEnable(void)
{
#if (SDCard_SPIM_SCB_MODE_UNCONFIG_CONST_CFG)
#if (!SDCard_SPIM_CY_SCBIP_V1)
    if (SDCard_SPIM_SCB_MODE_SPI_RUNTM_CFG)
    {
        SDCard_SPIM_SpiPostEnable();
    }
    else if (SDCard_SPIM_SCB_MODE_UART_RUNTM_CFG)
    {
        SDCard_SPIM_UartPostEnable();
    }
    else
    {
        /* Unknown mode: do nothing */
    }
#endif /* (!SDCard_SPIM_CY_SCBIP_V1) */

#elif (SDCard_SPIM_SCB_MODE_SPI_CONST_CFG)
    SDCard_SPIM_SpiPostEnable();

#elif (SDCard_SPIM_SCB_MODE_UART_CONST_CFG)
    SDCard_SPIM_UartPostEnable();

#else
    /* Unknown mode: do nothing */
#endif /* (SDCard_SPIM_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: SDCard_SPIM_ScbModeStop
****************************************************************************//**
*
*  Calls the Stop function for a specific operation mode.
*
*******************************************************************************/
static void SDCard_SPIM_ScbModeStop(void)
{
#if (SDCard_SPIM_SCB_MODE_UNCONFIG_CONST_CFG)
    if (SDCard_SPIM_SCB_MODE_I2C_RUNTM_CFG)
    {
        SDCard_SPIM_I2CStop();
    }
    else if (SDCard_SPIM_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        SDCard_SPIM_EzI2CStop();
    }
#if (!SDCard_SPIM_CY_SCBIP_V1)
    else if (SDCard_SPIM_SCB_MODE_SPI_RUNTM_CFG)
    {
        SDCard_SPIM_SpiStop();
    }
    else if (SDCard_SPIM_SCB_MODE_UART_RUNTM_CFG)
    {
        SDCard_SPIM_UartStop();
    }
#endif /* (!SDCard_SPIM_CY_SCBIP_V1) */
    else
    {
        /* Unknown mode: do nothing */
    }
#elif (SDCard_SPIM_SCB_MODE_I2C_CONST_CFG)
    SDCard_SPIM_I2CStop();

#elif (SDCard_SPIM_SCB_MODE_EZI2C_CONST_CFG)
    SDCard_SPIM_EzI2CStop();

#elif (SDCard_SPIM_SCB_MODE_SPI_CONST_CFG)
    SDCard_SPIM_SpiStop();

#elif (SDCard_SPIM_SCB_MODE_UART_CONST_CFG)
    SDCard_SPIM_UartStop();

#else
    /* Unknown mode: do nothing */
#endif /* (SDCard_SPIM_SCB_MODE_UNCONFIG_CONST_CFG) */
}


#if (SDCard_SPIM_SCB_MODE_UNCONFIG_CONST_CFG)
    /*******************************************************************************
    * Function Name: SDCard_SPIM_SetPins
    ****************************************************************************//**
    *
    *  Sets the pins settings accordingly to the selected operation mode.
    *  Only available in the Unconfigured operation mode. The mode specific
    *  initialization function calls it.
    *  Pins configuration is set by PSoC Creator when a specific mode of operation
    *  is selected in design time.
    *
    *  \param mode:      Mode of SCB operation.
    *  \param subMode:   Sub-mode of SCB operation. It is only required for SPI and UART
    *             modes.
    *  \param uartEnableMask: enables TX or RX direction and RTS and CTS signals.
    *
    *******************************************************************************/
    void SDCard_SPIM_SetPins(uint32 mode, uint32 subMode, uint32 uartEnableMask)
    {
        uint32 pinsDm[SDCard_SPIM_SCB_PINS_NUMBER];
        uint32 i;
        
    #if (!SDCard_SPIM_CY_SCBIP_V1)
        uint32 pinsInBuf = 0u;
    #endif /* (!SDCard_SPIM_CY_SCBIP_V1) */
        
        uint32 hsiomSel[SDCard_SPIM_SCB_PINS_NUMBER] = 
        {
            SDCard_SPIM_RX_SCL_MOSI_HSIOM_SEL_GPIO,
            SDCard_SPIM_TX_SDA_MISO_HSIOM_SEL_GPIO,
            0u,
            0u,
            0u,
            0u,
            0u,
        };

    #if (SDCard_SPIM_CY_SCBIP_V1)
        /* Supress compiler warning. */
        if ((0u == subMode) || (0u == uartEnableMask))
        {
        }
    #endif /* (SDCard_SPIM_CY_SCBIP_V1) */

        /* Set default HSIOM to GPIO and Drive Mode to Analog Hi-Z */
        for (i = 0u; i < SDCard_SPIM_SCB_PINS_NUMBER; i++)
        {
            pinsDm[i] = SDCard_SPIM_PIN_DM_ALG_HIZ;
        }

        if ((SDCard_SPIM_SCB_MODE_I2C   == mode) ||
            (SDCard_SPIM_SCB_MODE_EZI2C == mode))
        {
        #if (SDCard_SPIM_RX_SCL_MOSI_PIN)
            hsiomSel[SDCard_SPIM_RX_SCL_MOSI_PIN_INDEX] = SDCard_SPIM_RX_SCL_MOSI_HSIOM_SEL_I2C;
            pinsDm  [SDCard_SPIM_RX_SCL_MOSI_PIN_INDEX] = SDCard_SPIM_PIN_DM_OD_LO;
        #elif (SDCard_SPIM_RX_WAKE_SCL_MOSI_PIN)
            hsiomSel[SDCard_SPIM_RX_WAKE_SCL_MOSI_PIN_INDEX] = SDCard_SPIM_RX_WAKE_SCL_MOSI_HSIOM_SEL_I2C;
            pinsDm  [SDCard_SPIM_RX_WAKE_SCL_MOSI_PIN_INDEX] = SDCard_SPIM_PIN_DM_OD_LO;
        #else
        #endif /* (SDCard_SPIM_RX_SCL_MOSI_PIN) */
        
        #if (SDCard_SPIM_TX_SDA_MISO_PIN)
            hsiomSel[SDCard_SPIM_TX_SDA_MISO_PIN_INDEX] = SDCard_SPIM_TX_SDA_MISO_HSIOM_SEL_I2C;
            pinsDm  [SDCard_SPIM_TX_SDA_MISO_PIN_INDEX] = SDCard_SPIM_PIN_DM_OD_LO;
        #endif /* (SDCard_SPIM_TX_SDA_MISO_PIN) */
        }
    #if (!SDCard_SPIM_CY_SCBIP_V1)
        else if (SDCard_SPIM_SCB_MODE_SPI == mode)
        {
        #if (SDCard_SPIM_RX_SCL_MOSI_PIN)
            hsiomSel[SDCard_SPIM_RX_SCL_MOSI_PIN_INDEX] = SDCard_SPIM_RX_SCL_MOSI_HSIOM_SEL_SPI;
        #elif (SDCard_SPIM_RX_WAKE_SCL_MOSI_PIN)
            hsiomSel[SDCard_SPIM_RX_WAKE_SCL_MOSI_PIN_INDEX] = SDCard_SPIM_RX_WAKE_SCL_MOSI_HSIOM_SEL_SPI;
        #else
        #endif /* (SDCard_SPIM_RX_SCL_MOSI_PIN) */
        
        #if (SDCard_SPIM_TX_SDA_MISO_PIN)
            hsiomSel[SDCard_SPIM_TX_SDA_MISO_PIN_INDEX] = SDCard_SPIM_TX_SDA_MISO_HSIOM_SEL_SPI;
        #endif /* (SDCard_SPIM_TX_SDA_MISO_PIN) */
        
        #if (SDCard_SPIM_SCLK_PIN)
            hsiomSel[SDCard_SPIM_SCLK_PIN_INDEX] = SDCard_SPIM_SCLK_HSIOM_SEL_SPI;
        #endif /* (SDCard_SPIM_SCLK_PIN) */

            if (SDCard_SPIM_SPI_SLAVE == subMode)
            {
                /* Slave */
                pinsDm[SDCard_SPIM_RX_SCL_MOSI_PIN_INDEX] = SDCard_SPIM_PIN_DM_DIG_HIZ;
                pinsDm[SDCard_SPIM_TX_SDA_MISO_PIN_INDEX] = SDCard_SPIM_PIN_DM_STRONG;
                pinsDm[SDCard_SPIM_SCLK_PIN_INDEX] = SDCard_SPIM_PIN_DM_DIG_HIZ;

            #if (SDCard_SPIM_SS0_PIN)
                /* Only SS0 is valid choice for Slave */
                hsiomSel[SDCard_SPIM_SS0_PIN_INDEX] = SDCard_SPIM_SS0_HSIOM_SEL_SPI;
                pinsDm  [SDCard_SPIM_SS0_PIN_INDEX] = SDCard_SPIM_PIN_DM_DIG_HIZ;
            #endif /* (SDCard_SPIM_SS0_PIN) */

            #if (SDCard_SPIM_TX_SDA_MISO_PIN)
                /* Disable input buffer */
                 pinsInBuf |= SDCard_SPIM_TX_SDA_MISO_PIN_MASK;
            #endif /* (SDCard_SPIM_TX_SDA_MISO_PIN) */
            }
            else 
            {
                /* (Master) */
                pinsDm[SDCard_SPIM_RX_SCL_MOSI_PIN_INDEX] = SDCard_SPIM_PIN_DM_STRONG;
                pinsDm[SDCard_SPIM_TX_SDA_MISO_PIN_INDEX] = SDCard_SPIM_PIN_DM_DIG_HIZ;
                pinsDm[SDCard_SPIM_SCLK_PIN_INDEX] = SDCard_SPIM_PIN_DM_STRONG;

            #if (SDCard_SPIM_SS0_PIN)
                hsiomSel [SDCard_SPIM_SS0_PIN_INDEX] = SDCard_SPIM_SS0_HSIOM_SEL_SPI;
                pinsDm   [SDCard_SPIM_SS0_PIN_INDEX] = SDCard_SPIM_PIN_DM_STRONG;
                pinsInBuf |= SDCard_SPIM_SS0_PIN_MASK;
            #endif /* (SDCard_SPIM_SS0_PIN) */

            #if (SDCard_SPIM_SS1_PIN)
                hsiomSel [SDCard_SPIM_SS1_PIN_INDEX] = SDCard_SPIM_SS1_HSIOM_SEL_SPI;
                pinsDm   [SDCard_SPIM_SS1_PIN_INDEX] = SDCard_SPIM_PIN_DM_STRONG;
                pinsInBuf |= SDCard_SPIM_SS1_PIN_MASK;
            #endif /* (SDCard_SPIM_SS1_PIN) */

            #if (SDCard_SPIM_SS2_PIN)
                hsiomSel [SDCard_SPIM_SS2_PIN_INDEX] = SDCard_SPIM_SS2_HSIOM_SEL_SPI;
                pinsDm   [SDCard_SPIM_SS2_PIN_INDEX] = SDCard_SPIM_PIN_DM_STRONG;
                pinsInBuf |= SDCard_SPIM_SS2_PIN_MASK;
            #endif /* (SDCard_SPIM_SS2_PIN) */

            #if (SDCard_SPIM_SS3_PIN)
                hsiomSel [SDCard_SPIM_SS3_PIN_INDEX] = SDCard_SPIM_SS3_HSIOM_SEL_SPI;
                pinsDm   [SDCard_SPIM_SS3_PIN_INDEX] = SDCard_SPIM_PIN_DM_STRONG;
                pinsInBuf |= SDCard_SPIM_SS3_PIN_MASK;
            #endif /* (SDCard_SPIM_SS3_PIN) */

                /* Disable input buffers */
            #if (SDCard_SPIM_RX_SCL_MOSI_PIN)
                pinsInBuf |= SDCard_SPIM_RX_SCL_MOSI_PIN_MASK;
            #elif (SDCard_SPIM_RX_WAKE_SCL_MOSI_PIN)
                pinsInBuf |= SDCard_SPIM_RX_WAKE_SCL_MOSI_PIN_MASK;
            #else
            #endif /* (SDCard_SPIM_RX_SCL_MOSI_PIN) */

            #if (SDCard_SPIM_SCLK_PIN)
                pinsInBuf |= SDCard_SPIM_SCLK_PIN_MASK;
            #endif /* (SDCard_SPIM_SCLK_PIN) */
            }
        }
        else /* UART */
        {
            if (SDCard_SPIM_UART_MODE_SMARTCARD == subMode)
            {
                /* SmartCard */
            #if (SDCard_SPIM_TX_SDA_MISO_PIN)
                hsiomSel[SDCard_SPIM_TX_SDA_MISO_PIN_INDEX] = SDCard_SPIM_TX_SDA_MISO_HSIOM_SEL_UART;
                pinsDm  [SDCard_SPIM_TX_SDA_MISO_PIN_INDEX] = SDCard_SPIM_PIN_DM_OD_LO;
            #endif /* (SDCard_SPIM_TX_SDA_MISO_PIN) */
            }
            else /* Standard or IrDA */
            {
                if (0u != (SDCard_SPIM_UART_RX_PIN_ENABLE & uartEnableMask))
                {
                #if (SDCard_SPIM_RX_SCL_MOSI_PIN)
                    hsiomSel[SDCard_SPIM_RX_SCL_MOSI_PIN_INDEX] = SDCard_SPIM_RX_SCL_MOSI_HSIOM_SEL_UART;
                    pinsDm  [SDCard_SPIM_RX_SCL_MOSI_PIN_INDEX] = SDCard_SPIM_PIN_DM_DIG_HIZ;
                #elif (SDCard_SPIM_RX_WAKE_SCL_MOSI_PIN)
                    hsiomSel[SDCard_SPIM_RX_WAKE_SCL_MOSI_PIN_INDEX] = SDCard_SPIM_RX_WAKE_SCL_MOSI_HSIOM_SEL_UART;
                    pinsDm  [SDCard_SPIM_RX_WAKE_SCL_MOSI_PIN_INDEX] = SDCard_SPIM_PIN_DM_DIG_HIZ;
                #else
                #endif /* (SDCard_SPIM_RX_SCL_MOSI_PIN) */
                }

                if (0u != (SDCard_SPIM_UART_TX_PIN_ENABLE & uartEnableMask))
                {
                #if (SDCard_SPIM_TX_SDA_MISO_PIN)
                    hsiomSel[SDCard_SPIM_TX_SDA_MISO_PIN_INDEX] = SDCard_SPIM_TX_SDA_MISO_HSIOM_SEL_UART;
                    pinsDm  [SDCard_SPIM_TX_SDA_MISO_PIN_INDEX] = SDCard_SPIM_PIN_DM_STRONG;
                    
                    /* Disable input buffer */
                    pinsInBuf |= SDCard_SPIM_TX_SDA_MISO_PIN_MASK;
                #endif /* (SDCard_SPIM_TX_SDA_MISO_PIN) */
                }

            #if !(SDCard_SPIM_CY_SCBIP_V0 || SDCard_SPIM_CY_SCBIP_V1)
                if (SDCard_SPIM_UART_MODE_STD == subMode)
                {
                    if (0u != (SDCard_SPIM_UART_CTS_PIN_ENABLE & uartEnableMask))
                    {
                        /* CTS input is multiplexed with SCLK */
                    #if (SDCard_SPIM_SCLK_PIN)
                        hsiomSel[SDCard_SPIM_SCLK_PIN_INDEX] = SDCard_SPIM_SCLK_HSIOM_SEL_UART;
                        pinsDm  [SDCard_SPIM_SCLK_PIN_INDEX] = SDCard_SPIM_PIN_DM_DIG_HIZ;
                    #endif /* (SDCard_SPIM_SCLK_PIN) */
                    }

                    if (0u != (SDCard_SPIM_UART_RTS_PIN_ENABLE & uartEnableMask))
                    {
                        /* RTS output is multiplexed with SS0 */
                    #if (SDCard_SPIM_SS0_PIN)
                        hsiomSel[SDCard_SPIM_SS0_PIN_INDEX] = SDCard_SPIM_SS0_HSIOM_SEL_UART;
                        pinsDm  [SDCard_SPIM_SS0_PIN_INDEX] = SDCard_SPIM_PIN_DM_STRONG;
                        
                        /* Disable input buffer */
                        pinsInBuf |= SDCard_SPIM_SS0_PIN_MASK;
                    #endif /* (SDCard_SPIM_SS0_PIN) */
                    }
                }
            #endif /* !(SDCard_SPIM_CY_SCBIP_V0 || SDCard_SPIM_CY_SCBIP_V1) */
            }
        }
    #endif /* (!SDCard_SPIM_CY_SCBIP_V1) */

    /* Configure pins: set HSIOM, DM and InputBufEnable */
    /* Note: the DR register settings do not effect the pin output if HSIOM is other than GPIO */

    #if (SDCard_SPIM_RX_SCL_MOSI_PIN)
        SDCard_SPIM_SET_HSIOM_SEL(SDCard_SPIM_RX_SCL_MOSI_HSIOM_REG,
                                       SDCard_SPIM_RX_SCL_MOSI_HSIOM_MASK,
                                       SDCard_SPIM_RX_SCL_MOSI_HSIOM_POS,
                                        hsiomSel[SDCard_SPIM_RX_SCL_MOSI_PIN_INDEX]);

        SDCard_SPIM_uart_rx_i2c_scl_spi_mosi_SetDriveMode((uint8) pinsDm[SDCard_SPIM_RX_SCL_MOSI_PIN_INDEX]);

        #if (!SDCard_SPIM_CY_SCBIP_V1)
            SDCard_SPIM_SET_INP_DIS(SDCard_SPIM_uart_rx_i2c_scl_spi_mosi_INP_DIS,
                                         SDCard_SPIM_uart_rx_i2c_scl_spi_mosi_MASK,
                                         (0u != (pinsInBuf & SDCard_SPIM_RX_SCL_MOSI_PIN_MASK)));
        #endif /* (!SDCard_SPIM_CY_SCBIP_V1) */
    
    #elif (SDCard_SPIM_RX_WAKE_SCL_MOSI_PIN)
        SDCard_SPIM_SET_HSIOM_SEL(SDCard_SPIM_RX_WAKE_SCL_MOSI_HSIOM_REG,
                                       SDCard_SPIM_RX_WAKE_SCL_MOSI_HSIOM_MASK,
                                       SDCard_SPIM_RX_WAKE_SCL_MOSI_HSIOM_POS,
                                       hsiomSel[SDCard_SPIM_RX_WAKE_SCL_MOSI_PIN_INDEX]);

        SDCard_SPIM_uart_rx_wake_i2c_scl_spi_mosi_SetDriveMode((uint8)
                                                               pinsDm[SDCard_SPIM_RX_WAKE_SCL_MOSI_PIN_INDEX]);

        SDCard_SPIM_SET_INP_DIS(SDCard_SPIM_uart_rx_wake_i2c_scl_spi_mosi_INP_DIS,
                                     SDCard_SPIM_uart_rx_wake_i2c_scl_spi_mosi_MASK,
                                     (0u != (pinsInBuf & SDCard_SPIM_RX_WAKE_SCL_MOSI_PIN_MASK)));

         /* Set interrupt on falling edge */
        SDCard_SPIM_SET_INCFG_TYPE(SDCard_SPIM_RX_WAKE_SCL_MOSI_INTCFG_REG,
                                        SDCard_SPIM_RX_WAKE_SCL_MOSI_INTCFG_TYPE_MASK,
                                        SDCard_SPIM_RX_WAKE_SCL_MOSI_INTCFG_TYPE_POS,
                                        SDCard_SPIM_INTCFG_TYPE_FALLING_EDGE);
    #else
    #endif /* (SDCard_SPIM_RX_WAKE_SCL_MOSI_PIN) */

    #if (SDCard_SPIM_TX_SDA_MISO_PIN)
        SDCard_SPIM_SET_HSIOM_SEL(SDCard_SPIM_TX_SDA_MISO_HSIOM_REG,
                                       SDCard_SPIM_TX_SDA_MISO_HSIOM_MASK,
                                       SDCard_SPIM_TX_SDA_MISO_HSIOM_POS,
                                        hsiomSel[SDCard_SPIM_TX_SDA_MISO_PIN_INDEX]);

        SDCard_SPIM_uart_tx_i2c_sda_spi_miso_SetDriveMode((uint8) pinsDm[SDCard_SPIM_TX_SDA_MISO_PIN_INDEX]);

    #if (!SDCard_SPIM_CY_SCBIP_V1)
        SDCard_SPIM_SET_INP_DIS(SDCard_SPIM_uart_tx_i2c_sda_spi_miso_INP_DIS,
                                     SDCard_SPIM_uart_tx_i2c_sda_spi_miso_MASK,
                                    (0u != (pinsInBuf & SDCard_SPIM_TX_SDA_MISO_PIN_MASK)));
    #endif /* (!SDCard_SPIM_CY_SCBIP_V1) */
    #endif /* (SDCard_SPIM_RX_SCL_MOSI_PIN) */

    #if (SDCard_SPIM_SCLK_PIN)
        SDCard_SPIM_SET_HSIOM_SEL(SDCard_SPIM_SCLK_HSIOM_REG,
                                       SDCard_SPIM_SCLK_HSIOM_MASK,
                                       SDCard_SPIM_SCLK_HSIOM_POS,
                                       hsiomSel[SDCard_SPIM_SCLK_PIN_INDEX]);

        SDCard_SPIM_spi_sclk_SetDriveMode((uint8) pinsDm[SDCard_SPIM_SCLK_PIN_INDEX]);

        SDCard_SPIM_SET_INP_DIS(SDCard_SPIM_spi_sclk_INP_DIS,
                                     SDCard_SPIM_spi_sclk_MASK,
                                     (0u != (pinsInBuf & SDCard_SPIM_SCLK_PIN_MASK)));
    #endif /* (SDCard_SPIM_SCLK_PIN) */

    #if (SDCard_SPIM_SS0_PIN)
        SDCard_SPIM_SET_HSIOM_SEL(SDCard_SPIM_SS0_HSIOM_REG,
                                       SDCard_SPIM_SS0_HSIOM_MASK,
                                       SDCard_SPIM_SS0_HSIOM_POS,
                                       hsiomSel[SDCard_SPIM_SS0_PIN_INDEX]);

        SDCard_SPIM_spi_ss0_SetDriveMode((uint8) pinsDm[SDCard_SPIM_SS0_PIN_INDEX]);

        SDCard_SPIM_SET_INP_DIS(SDCard_SPIM_spi_ss0_INP_DIS,
                                     SDCard_SPIM_spi_ss0_MASK,
                                     (0u != (pinsInBuf & SDCard_SPIM_SS0_PIN_MASK)));
    #endif /* (SDCard_SPIM_SS0_PIN) */

    #if (SDCard_SPIM_SS1_PIN)
        SDCard_SPIM_SET_HSIOM_SEL(SDCard_SPIM_SS1_HSIOM_REG,
                                       SDCard_SPIM_SS1_HSIOM_MASK,
                                       SDCard_SPIM_SS1_HSIOM_POS,
                                       hsiomSel[SDCard_SPIM_SS1_PIN_INDEX]);

        SDCard_SPIM_spi_ss1_SetDriveMode((uint8) pinsDm[SDCard_SPIM_SS1_PIN_INDEX]);

        SDCard_SPIM_SET_INP_DIS(SDCard_SPIM_spi_ss1_INP_DIS,
                                     SDCard_SPIM_spi_ss1_MASK,
                                     (0u != (pinsInBuf & SDCard_SPIM_SS1_PIN_MASK)));
    #endif /* (SDCard_SPIM_SS1_PIN) */

    #if (SDCard_SPIM_SS2_PIN)
        SDCard_SPIM_SET_HSIOM_SEL(SDCard_SPIM_SS2_HSIOM_REG,
                                       SDCard_SPIM_SS2_HSIOM_MASK,
                                       SDCard_SPIM_SS2_HSIOM_POS,
                                       hsiomSel[SDCard_SPIM_SS2_PIN_INDEX]);

        SDCard_SPIM_spi_ss2_SetDriveMode((uint8) pinsDm[SDCard_SPIM_SS2_PIN_INDEX]);

        SDCard_SPIM_SET_INP_DIS(SDCard_SPIM_spi_ss2_INP_DIS,
                                     SDCard_SPIM_spi_ss2_MASK,
                                     (0u != (pinsInBuf & SDCard_SPIM_SS2_PIN_MASK)));
    #endif /* (SDCard_SPIM_SS2_PIN) */

    #if (SDCard_SPIM_SS3_PIN)
        SDCard_SPIM_SET_HSIOM_SEL(SDCard_SPIM_SS3_HSIOM_REG,
                                       SDCard_SPIM_SS3_HSIOM_MASK,
                                       SDCard_SPIM_SS3_HSIOM_POS,
                                       hsiomSel[SDCard_SPIM_SS3_PIN_INDEX]);

        SDCard_SPIM_spi_ss3_SetDriveMode((uint8) pinsDm[SDCard_SPIM_SS3_PIN_INDEX]);

        SDCard_SPIM_SET_INP_DIS(SDCard_SPIM_spi_ss3_INP_DIS,
                                     SDCard_SPIM_spi_ss3_MASK,
                                     (0u != (pinsInBuf & SDCard_SPIM_SS3_PIN_MASK)));
    #endif /* (SDCard_SPIM_SS3_PIN) */
    }

#endif /* (SDCard_SPIM_SCB_MODE_UNCONFIG_CONST_CFG) */


#if (SDCard_SPIM_CY_SCBIP_V0 || SDCard_SPIM_CY_SCBIP_V1)
    /*******************************************************************************
    * Function Name: SDCard_SPIM_I2CSlaveNackGeneration
    ****************************************************************************//**
    *
    *  Sets command to generate NACK to the address or data.
    *
    *******************************************************************************/
    void SDCard_SPIM_I2CSlaveNackGeneration(void)
    {
        /* Check for EC_AM toggle condition: EC_AM and clock stretching for address are enabled */
        if ((0u != (SDCard_SPIM_CTRL_REG & SDCard_SPIM_CTRL_EC_AM_MODE)) &&
            (0u == (SDCard_SPIM_I2C_CTRL_REG & SDCard_SPIM_I2C_CTRL_S_NOT_READY_ADDR_NACK)))
        {
            /* Toggle EC_AM before NACK generation */
            SDCard_SPIM_CTRL_REG &= ~SDCard_SPIM_CTRL_EC_AM_MODE;
            SDCard_SPIM_CTRL_REG |=  SDCard_SPIM_CTRL_EC_AM_MODE;
        }

        SDCard_SPIM_I2C_SLAVE_CMD_REG = SDCard_SPIM_I2C_SLAVE_CMD_S_NACK;
    }
#endif /* (SDCard_SPIM_CY_SCBIP_V0 || SDCard_SPIM_CY_SCBIP_V1) */


/* [] END OF FILE */
