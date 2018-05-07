/***************************************************************************//**
* \file SDCard_SPIM_PINS.h
* \version 3.20
*
* \brief
*  This file provides constants and parameter values for the pin components
*  buried into SCB Component.
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

#if !defined(CY_SCB_PINS_SDCard_SPIM_H)
#define CY_SCB_PINS_SDCard_SPIM_H

#include "cydevice_trm.h"
#include "cyfitter.h"
#include "cytypes.h"


/***************************************
*   Conditional Compilation Parameters
****************************************/

/* Unconfigured pins */
#define SDCard_SPIM_REMOVE_RX_WAKE_SCL_MOSI_PIN  (1u)
#define SDCard_SPIM_REMOVE_RX_SCL_MOSI_PIN      (1u)
#define SDCard_SPIM_REMOVE_TX_SDA_MISO_PIN      (1u)
#define SDCard_SPIM_REMOVE_SCLK_PIN      (1u)
#define SDCard_SPIM_REMOVE_SS0_PIN      (1u)
#define SDCard_SPIM_REMOVE_SS1_PIN                 (1u)
#define SDCard_SPIM_REMOVE_SS2_PIN                 (1u)
#define SDCard_SPIM_REMOVE_SS3_PIN                 (1u)

/* Mode defined pins */
#define SDCard_SPIM_REMOVE_I2C_PINS                (1u)
#define SDCard_SPIM_REMOVE_SPI_MASTER_PINS         (0u)
#define SDCard_SPIM_REMOVE_SPI_MASTER_SCLK_PIN     (0u)
#define SDCard_SPIM_REMOVE_SPI_MASTER_MOSI_PIN     (0u)
#define SDCard_SPIM_REMOVE_SPI_MASTER_MISO_PIN     (0u)
#define SDCard_SPIM_REMOVE_SPI_MASTER_SS0_PIN      (0u)
#define SDCard_SPIM_REMOVE_SPI_MASTER_SS1_PIN      (1u)
#define SDCard_SPIM_REMOVE_SPI_MASTER_SS2_PIN      (1u)
#define SDCard_SPIM_REMOVE_SPI_MASTER_SS3_PIN      (1u)
#define SDCard_SPIM_REMOVE_SPI_SLAVE_PINS          (1u)
#define SDCard_SPIM_REMOVE_SPI_SLAVE_MOSI_PIN      (1u)
#define SDCard_SPIM_REMOVE_SPI_SLAVE_MISO_PIN      (1u)
#define SDCard_SPIM_REMOVE_UART_TX_PIN             (1u)
#define SDCard_SPIM_REMOVE_UART_RX_TX_PIN          (1u)
#define SDCard_SPIM_REMOVE_UART_RX_PIN             (1u)
#define SDCard_SPIM_REMOVE_UART_RX_WAKE_PIN        (1u)
#define SDCard_SPIM_REMOVE_UART_RTS_PIN            (1u)
#define SDCard_SPIM_REMOVE_UART_CTS_PIN            (1u)

/* Unconfigured pins */
#define SDCard_SPIM_RX_WAKE_SCL_MOSI_PIN (0u == SDCard_SPIM_REMOVE_RX_WAKE_SCL_MOSI_PIN)
#define SDCard_SPIM_RX_SCL_MOSI_PIN     (0u == SDCard_SPIM_REMOVE_RX_SCL_MOSI_PIN)
#define SDCard_SPIM_TX_SDA_MISO_PIN     (0u == SDCard_SPIM_REMOVE_TX_SDA_MISO_PIN)
#define SDCard_SPIM_SCLK_PIN     (0u == SDCard_SPIM_REMOVE_SCLK_PIN)
#define SDCard_SPIM_SS0_PIN     (0u == SDCard_SPIM_REMOVE_SS0_PIN)
#define SDCard_SPIM_SS1_PIN                (0u == SDCard_SPIM_REMOVE_SS1_PIN)
#define SDCard_SPIM_SS2_PIN                (0u == SDCard_SPIM_REMOVE_SS2_PIN)
#define SDCard_SPIM_SS3_PIN                (0u == SDCard_SPIM_REMOVE_SS3_PIN)

/* Mode defined pins */
#define SDCard_SPIM_I2C_PINS               (0u == SDCard_SPIM_REMOVE_I2C_PINS)
#define SDCard_SPIM_SPI_MASTER_PINS        (0u == SDCard_SPIM_REMOVE_SPI_MASTER_PINS)
#define SDCard_SPIM_SPI_MASTER_SCLK_PIN    (0u == SDCard_SPIM_REMOVE_SPI_MASTER_SCLK_PIN)
#define SDCard_SPIM_SPI_MASTER_MOSI_PIN    (0u == SDCard_SPIM_REMOVE_SPI_MASTER_MOSI_PIN)
#define SDCard_SPIM_SPI_MASTER_MISO_PIN    (0u == SDCard_SPIM_REMOVE_SPI_MASTER_MISO_PIN)
#define SDCard_SPIM_SPI_MASTER_SS0_PIN     (0u == SDCard_SPIM_REMOVE_SPI_MASTER_SS0_PIN)
#define SDCard_SPIM_SPI_MASTER_SS1_PIN     (0u == SDCard_SPIM_REMOVE_SPI_MASTER_SS1_PIN)
#define SDCard_SPIM_SPI_MASTER_SS2_PIN     (0u == SDCard_SPIM_REMOVE_SPI_MASTER_SS2_PIN)
#define SDCard_SPIM_SPI_MASTER_SS3_PIN     (0u == SDCard_SPIM_REMOVE_SPI_MASTER_SS3_PIN)
#define SDCard_SPIM_SPI_SLAVE_PINS         (0u == SDCard_SPIM_REMOVE_SPI_SLAVE_PINS)
#define SDCard_SPIM_SPI_SLAVE_MOSI_PIN     (0u == SDCard_SPIM_REMOVE_SPI_SLAVE_MOSI_PIN)
#define SDCard_SPIM_SPI_SLAVE_MISO_PIN     (0u == SDCard_SPIM_REMOVE_SPI_SLAVE_MISO_PIN)
#define SDCard_SPIM_UART_TX_PIN            (0u == SDCard_SPIM_REMOVE_UART_TX_PIN)
#define SDCard_SPIM_UART_RX_TX_PIN         (0u == SDCard_SPIM_REMOVE_UART_RX_TX_PIN)
#define SDCard_SPIM_UART_RX_PIN            (0u == SDCard_SPIM_REMOVE_UART_RX_PIN)
#define SDCard_SPIM_UART_RX_WAKE_PIN       (0u == SDCard_SPIM_REMOVE_UART_RX_WAKE_PIN)
#define SDCard_SPIM_UART_RTS_PIN           (0u == SDCard_SPIM_REMOVE_UART_RTS_PIN)
#define SDCard_SPIM_UART_CTS_PIN           (0u == SDCard_SPIM_REMOVE_UART_CTS_PIN)


/***************************************
*             Includes
****************************************/

#if (SDCard_SPIM_RX_WAKE_SCL_MOSI_PIN)
    #include "SDCard_SPIM_uart_rx_wake_i2c_scl_spi_mosi.h"
#endif /* (SDCard_SPIM_RX_SCL_MOSI) */

#if (SDCard_SPIM_RX_SCL_MOSI_PIN)
    #include "SDCard_SPIM_uart_rx_i2c_scl_spi_mosi.h"
#endif /* (SDCard_SPIM_RX_SCL_MOSI) */

#if (SDCard_SPIM_TX_SDA_MISO_PIN)
    #include "SDCard_SPIM_uart_tx_i2c_sda_spi_miso.h"
#endif /* (SDCard_SPIM_TX_SDA_MISO) */

#if (SDCard_SPIM_SCLK_PIN)
    #include "SDCard_SPIM_spi_sclk.h"
#endif /* (SDCard_SPIM_SCLK) */

#if (SDCard_SPIM_SS0_PIN)
    #include "SDCard_SPIM_spi_ss0.h"
#endif /* (SDCard_SPIM_SS0_PIN) */

#if (SDCard_SPIM_SS1_PIN)
    #include "SDCard_SPIM_spi_ss1.h"
#endif /* (SDCard_SPIM_SS1_PIN) */

#if (SDCard_SPIM_SS2_PIN)
    #include "SDCard_SPIM_spi_ss2.h"
#endif /* (SDCard_SPIM_SS2_PIN) */

#if (SDCard_SPIM_SS3_PIN)
    #include "SDCard_SPIM_spi_ss3.h"
#endif /* (SDCard_SPIM_SS3_PIN) */

#if (SDCard_SPIM_I2C_PINS)
    #include "SDCard_SPIM_scl.h"
    #include "SDCard_SPIM_sda.h"
#endif /* (SDCard_SPIM_I2C_PINS) */

#if (SDCard_SPIM_SPI_MASTER_PINS)
#if (SDCard_SPIM_SPI_MASTER_SCLK_PIN)
    #include "SDCard_SPIM_sclk_m.h"
#endif /* (SDCard_SPIM_SPI_MASTER_SCLK_PIN) */

#if (SDCard_SPIM_SPI_MASTER_MOSI_PIN)
    #include "SDCard_SPIM_mosi_m.h"
#endif /* (SDCard_SPIM_SPI_MASTER_MOSI_PIN) */

#if (SDCard_SPIM_SPI_MASTER_MISO_PIN)
    #include "SDCard_SPIM_miso_m.h"
#endif /*(SDCard_SPIM_SPI_MASTER_MISO_PIN) */
#endif /* (SDCard_SPIM_SPI_MASTER_PINS) */

#if (SDCard_SPIM_SPI_SLAVE_PINS)
    #include "SDCard_SPIM_sclk_s.h"
    #include "SDCard_SPIM_ss_s.h"

#if (SDCard_SPIM_SPI_SLAVE_MOSI_PIN)
    #include "SDCard_SPIM_mosi_s.h"
#endif /* (SDCard_SPIM_SPI_SLAVE_MOSI_PIN) */

#if (SDCard_SPIM_SPI_SLAVE_MISO_PIN)
    #include "SDCard_SPIM_miso_s.h"
#endif /*(SDCard_SPIM_SPI_SLAVE_MISO_PIN) */
#endif /* (SDCard_SPIM_SPI_SLAVE_PINS) */

#if (SDCard_SPIM_SPI_MASTER_SS0_PIN)
    #include "SDCard_SPIM_ss0_m.h"
#endif /* (SDCard_SPIM_SPI_MASTER_SS0_PIN) */

#if (SDCard_SPIM_SPI_MASTER_SS1_PIN)
    #include "SDCard_SPIM_ss1_m.h"
#endif /* (SDCard_SPIM_SPI_MASTER_SS1_PIN) */

#if (SDCard_SPIM_SPI_MASTER_SS2_PIN)
    #include "SDCard_SPIM_ss2_m.h"
#endif /* (SDCard_SPIM_SPI_MASTER_SS2_PIN) */

#if (SDCard_SPIM_SPI_MASTER_SS3_PIN)
    #include "SDCard_SPIM_ss3_m.h"
#endif /* (SDCard_SPIM_SPI_MASTER_SS3_PIN) */

#if (SDCard_SPIM_UART_TX_PIN)
    #include "SDCard_SPIM_tx.h"
#endif /* (SDCard_SPIM_UART_TX_PIN) */

#if (SDCard_SPIM_UART_RX_TX_PIN)
    #include "SDCard_SPIM_rx_tx.h"
#endif /* (SDCard_SPIM_UART_RX_TX_PIN) */

#if (SDCard_SPIM_UART_RX_PIN)
    #include "SDCard_SPIM_rx.h"
#endif /* (SDCard_SPIM_UART_RX_PIN) */

#if (SDCard_SPIM_UART_RX_WAKE_PIN)
    #include "SDCard_SPIM_rx_wake.h"
#endif /* (SDCard_SPIM_UART_RX_WAKE_PIN) */

#if (SDCard_SPIM_UART_RTS_PIN)
    #include "SDCard_SPIM_rts.h"
#endif /* (SDCard_SPIM_UART_RTS_PIN) */

#if (SDCard_SPIM_UART_CTS_PIN)
    #include "SDCard_SPIM_cts.h"
#endif /* (SDCard_SPIM_UART_CTS_PIN) */


/***************************************
*              Registers
***************************************/

#if (SDCard_SPIM_RX_SCL_MOSI_PIN)
    #define SDCard_SPIM_RX_SCL_MOSI_HSIOM_REG   (*(reg32 *) SDCard_SPIM_uart_rx_i2c_scl_spi_mosi__0__HSIOM)
    #define SDCard_SPIM_RX_SCL_MOSI_HSIOM_PTR   ( (reg32 *) SDCard_SPIM_uart_rx_i2c_scl_spi_mosi__0__HSIOM)
    
    #define SDCard_SPIM_RX_SCL_MOSI_HSIOM_MASK      (SDCard_SPIM_uart_rx_i2c_scl_spi_mosi__0__HSIOM_MASK)
    #define SDCard_SPIM_RX_SCL_MOSI_HSIOM_POS       (SDCard_SPIM_uart_rx_i2c_scl_spi_mosi__0__HSIOM_SHIFT)
    #define SDCard_SPIM_RX_SCL_MOSI_HSIOM_SEL_GPIO  (SDCard_SPIM_uart_rx_i2c_scl_spi_mosi__0__HSIOM_GPIO)
    #define SDCard_SPIM_RX_SCL_MOSI_HSIOM_SEL_I2C   (SDCard_SPIM_uart_rx_i2c_scl_spi_mosi__0__HSIOM_I2C)
    #define SDCard_SPIM_RX_SCL_MOSI_HSIOM_SEL_SPI   (SDCard_SPIM_uart_rx_i2c_scl_spi_mosi__0__HSIOM_SPI)
    #define SDCard_SPIM_RX_SCL_MOSI_HSIOM_SEL_UART  (SDCard_SPIM_uart_rx_i2c_scl_spi_mosi__0__HSIOM_UART)
    
#elif (SDCard_SPIM_RX_WAKE_SCL_MOSI_PIN)
    #define SDCard_SPIM_RX_WAKE_SCL_MOSI_HSIOM_REG   (*(reg32 *) SDCard_SPIM_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM)
    #define SDCard_SPIM_RX_WAKE_SCL_MOSI_HSIOM_PTR   ( (reg32 *) SDCard_SPIM_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM)
    
    #define SDCard_SPIM_RX_WAKE_SCL_MOSI_HSIOM_MASK      (SDCard_SPIM_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM_MASK)
    #define SDCard_SPIM_RX_WAKE_SCL_MOSI_HSIOM_POS       (SDCard_SPIM_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM_SHIFT)
    #define SDCard_SPIM_RX_WAKE_SCL_MOSI_HSIOM_SEL_GPIO  (SDCard_SPIM_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM_GPIO)
    #define SDCard_SPIM_RX_WAKE_SCL_MOSI_HSIOM_SEL_I2C   (SDCard_SPIM_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM_I2C)
    #define SDCard_SPIM_RX_WAKE_SCL_MOSI_HSIOM_SEL_SPI   (SDCard_SPIM_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM_SPI)
    #define SDCard_SPIM_RX_WAKE_SCL_MOSI_HSIOM_SEL_UART  (SDCard_SPIM_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM_UART)    
   
    #define SDCard_SPIM_RX_WAKE_SCL_MOSI_INTCFG_REG (*(reg32 *) SDCard_SPIM_uart_rx_wake_i2c_scl_spi_mosi__0__INTCFG)
    #define SDCard_SPIM_RX_WAKE_SCL_MOSI_INTCFG_PTR ( (reg32 *) SDCard_SPIM_uart_rx_wake_i2c_scl_spi_mosi__0__INTCFG)
    #define SDCard_SPIM_RX_WAKE_SCL_MOSI_INTCFG_TYPE_POS  (SDCard_SPIM_uart_rx_wake_i2c_scl_spi_mosi__SHIFT)
    #define SDCard_SPIM_RX_WAKE_SCL_MOSI_INTCFG_TYPE_MASK ((uint32) SDCard_SPIM_INTCFG_TYPE_MASK << \
                                                                           SDCard_SPIM_RX_WAKE_SCL_MOSI_INTCFG_TYPE_POS)
#else
    /* None of pins SDCard_SPIM_RX_SCL_MOSI_PIN or SDCard_SPIM_RX_WAKE_SCL_MOSI_PIN present.*/
#endif /* (SDCard_SPIM_RX_SCL_MOSI_PIN) */

#if (SDCard_SPIM_TX_SDA_MISO_PIN)
    #define SDCard_SPIM_TX_SDA_MISO_HSIOM_REG   (*(reg32 *) SDCard_SPIM_uart_tx_i2c_sda_spi_miso__0__HSIOM)
    #define SDCard_SPIM_TX_SDA_MISO_HSIOM_PTR   ( (reg32 *) SDCard_SPIM_uart_tx_i2c_sda_spi_miso__0__HSIOM)
    
    #define SDCard_SPIM_TX_SDA_MISO_HSIOM_MASK      (SDCard_SPIM_uart_tx_i2c_sda_spi_miso__0__HSIOM_MASK)
    #define SDCard_SPIM_TX_SDA_MISO_HSIOM_POS       (SDCard_SPIM_uart_tx_i2c_sda_spi_miso__0__HSIOM_SHIFT)
    #define SDCard_SPIM_TX_SDA_MISO_HSIOM_SEL_GPIO  (SDCard_SPIM_uart_tx_i2c_sda_spi_miso__0__HSIOM_GPIO)
    #define SDCard_SPIM_TX_SDA_MISO_HSIOM_SEL_I2C   (SDCard_SPIM_uart_tx_i2c_sda_spi_miso__0__HSIOM_I2C)
    #define SDCard_SPIM_TX_SDA_MISO_HSIOM_SEL_SPI   (SDCard_SPIM_uart_tx_i2c_sda_spi_miso__0__HSIOM_SPI)
    #define SDCard_SPIM_TX_SDA_MISO_HSIOM_SEL_UART  (SDCard_SPIM_uart_tx_i2c_sda_spi_miso__0__HSIOM_UART)
#endif /* (SDCard_SPIM_TX_SDA_MISO_PIN) */

#if (SDCard_SPIM_SCLK_PIN)
    #define SDCard_SPIM_SCLK_HSIOM_REG   (*(reg32 *) SDCard_SPIM_spi_sclk__0__HSIOM)
    #define SDCard_SPIM_SCLK_HSIOM_PTR   ( (reg32 *) SDCard_SPIM_spi_sclk__0__HSIOM)
    
    #define SDCard_SPIM_SCLK_HSIOM_MASK      (SDCard_SPIM_spi_sclk__0__HSIOM_MASK)
    #define SDCard_SPIM_SCLK_HSIOM_POS       (SDCard_SPIM_spi_sclk__0__HSIOM_SHIFT)
    #define SDCard_SPIM_SCLK_HSIOM_SEL_GPIO  (SDCard_SPIM_spi_sclk__0__HSIOM_GPIO)
    #define SDCard_SPIM_SCLK_HSIOM_SEL_I2C   (SDCard_SPIM_spi_sclk__0__HSIOM_I2C)
    #define SDCard_SPIM_SCLK_HSIOM_SEL_SPI   (SDCard_SPIM_spi_sclk__0__HSIOM_SPI)
    #define SDCard_SPIM_SCLK_HSIOM_SEL_UART  (SDCard_SPIM_spi_sclk__0__HSIOM_UART)
#endif /* (SDCard_SPIM_SCLK_PIN) */

#if (SDCard_SPIM_SS0_PIN)
    #define SDCard_SPIM_SS0_HSIOM_REG   (*(reg32 *) SDCard_SPIM_spi_ss0__0__HSIOM)
    #define SDCard_SPIM_SS0_HSIOM_PTR   ( (reg32 *) SDCard_SPIM_spi_ss0__0__HSIOM)
    
    #define SDCard_SPIM_SS0_HSIOM_MASK      (SDCard_SPIM_spi_ss0__0__HSIOM_MASK)
    #define SDCard_SPIM_SS0_HSIOM_POS       (SDCard_SPIM_spi_ss0__0__HSIOM_SHIFT)
    #define SDCard_SPIM_SS0_HSIOM_SEL_GPIO  (SDCard_SPIM_spi_ss0__0__HSIOM_GPIO)
    #define SDCard_SPIM_SS0_HSIOM_SEL_I2C   (SDCard_SPIM_spi_ss0__0__HSIOM_I2C)
    #define SDCard_SPIM_SS0_HSIOM_SEL_SPI   (SDCard_SPIM_spi_ss0__0__HSIOM_SPI)
#if !(SDCard_SPIM_CY_SCBIP_V0 || SDCard_SPIM_CY_SCBIP_V1)
    #define SDCard_SPIM_SS0_HSIOM_SEL_UART  (SDCard_SPIM_spi_ss0__0__HSIOM_UART)
#endif /* !(SDCard_SPIM_CY_SCBIP_V0 || SDCard_SPIM_CY_SCBIP_V1) */
#endif /* (SDCard_SPIM_SS0_PIN) */

#if (SDCard_SPIM_SS1_PIN)
    #define SDCard_SPIM_SS1_HSIOM_REG  (*(reg32 *) SDCard_SPIM_spi_ss1__0__HSIOM)
    #define SDCard_SPIM_SS1_HSIOM_PTR  ( (reg32 *) SDCard_SPIM_spi_ss1__0__HSIOM)
    
    #define SDCard_SPIM_SS1_HSIOM_MASK     (SDCard_SPIM_spi_ss1__0__HSIOM_MASK)
    #define SDCard_SPIM_SS1_HSIOM_POS      (SDCard_SPIM_spi_ss1__0__HSIOM_SHIFT)
    #define SDCard_SPIM_SS1_HSIOM_SEL_GPIO (SDCard_SPIM_spi_ss1__0__HSIOM_GPIO)
    #define SDCard_SPIM_SS1_HSIOM_SEL_I2C  (SDCard_SPIM_spi_ss1__0__HSIOM_I2C)
    #define SDCard_SPIM_SS1_HSIOM_SEL_SPI  (SDCard_SPIM_spi_ss1__0__HSIOM_SPI)
#endif /* (SDCard_SPIM_SS1_PIN) */

#if (SDCard_SPIM_SS2_PIN)
    #define SDCard_SPIM_SS2_HSIOM_REG     (*(reg32 *) SDCard_SPIM_spi_ss2__0__HSIOM)
    #define SDCard_SPIM_SS2_HSIOM_PTR     ( (reg32 *) SDCard_SPIM_spi_ss2__0__HSIOM)
    
    #define SDCard_SPIM_SS2_HSIOM_MASK     (SDCard_SPIM_spi_ss2__0__HSIOM_MASK)
    #define SDCard_SPIM_SS2_HSIOM_POS      (SDCard_SPIM_spi_ss2__0__HSIOM_SHIFT)
    #define SDCard_SPIM_SS2_HSIOM_SEL_GPIO (SDCard_SPIM_spi_ss2__0__HSIOM_GPIO)
    #define SDCard_SPIM_SS2_HSIOM_SEL_I2C  (SDCard_SPIM_spi_ss2__0__HSIOM_I2C)
    #define SDCard_SPIM_SS2_HSIOM_SEL_SPI  (SDCard_SPIM_spi_ss2__0__HSIOM_SPI)
#endif /* (SDCard_SPIM_SS2_PIN) */

#if (SDCard_SPIM_SS3_PIN)
    #define SDCard_SPIM_SS3_HSIOM_REG     (*(reg32 *) SDCard_SPIM_spi_ss3__0__HSIOM)
    #define SDCard_SPIM_SS3_HSIOM_PTR     ( (reg32 *) SDCard_SPIM_spi_ss3__0__HSIOM)
    
    #define SDCard_SPIM_SS3_HSIOM_MASK     (SDCard_SPIM_spi_ss3__0__HSIOM_MASK)
    #define SDCard_SPIM_SS3_HSIOM_POS      (SDCard_SPIM_spi_ss3__0__HSIOM_SHIFT)
    #define SDCard_SPIM_SS3_HSIOM_SEL_GPIO (SDCard_SPIM_spi_ss3__0__HSIOM_GPIO)
    #define SDCard_SPIM_SS3_HSIOM_SEL_I2C  (SDCard_SPIM_spi_ss3__0__HSIOM_I2C)
    #define SDCard_SPIM_SS3_HSIOM_SEL_SPI  (SDCard_SPIM_spi_ss3__0__HSIOM_SPI)
#endif /* (SDCard_SPIM_SS3_PIN) */

#if (SDCard_SPIM_I2C_PINS)
    #define SDCard_SPIM_SCL_HSIOM_REG  (*(reg32 *) SDCard_SPIM_scl__0__HSIOM)
    #define SDCard_SPIM_SCL_HSIOM_PTR  ( (reg32 *) SDCard_SPIM_scl__0__HSIOM)
    
    #define SDCard_SPIM_SCL_HSIOM_MASK     (SDCard_SPIM_scl__0__HSIOM_MASK)
    #define SDCard_SPIM_SCL_HSIOM_POS      (SDCard_SPIM_scl__0__HSIOM_SHIFT)
    #define SDCard_SPIM_SCL_HSIOM_SEL_GPIO (SDCard_SPIM_sda__0__HSIOM_GPIO)
    #define SDCard_SPIM_SCL_HSIOM_SEL_I2C  (SDCard_SPIM_sda__0__HSIOM_I2C)
    
    #define SDCard_SPIM_SDA_HSIOM_REG  (*(reg32 *) SDCard_SPIM_sda__0__HSIOM)
    #define SDCard_SPIM_SDA_HSIOM_PTR  ( (reg32 *) SDCard_SPIM_sda__0__HSIOM)
    
    #define SDCard_SPIM_SDA_HSIOM_MASK     (SDCard_SPIM_sda__0__HSIOM_MASK)
    #define SDCard_SPIM_SDA_HSIOM_POS      (SDCard_SPIM_sda__0__HSIOM_SHIFT)
    #define SDCard_SPIM_SDA_HSIOM_SEL_GPIO (SDCard_SPIM_sda__0__HSIOM_GPIO)
    #define SDCard_SPIM_SDA_HSIOM_SEL_I2C  (SDCard_SPIM_sda__0__HSIOM_I2C)
#endif /* (SDCard_SPIM_I2C_PINS) */

#if (SDCard_SPIM_SPI_SLAVE_PINS)
    #define SDCard_SPIM_SCLK_S_HSIOM_REG   (*(reg32 *) SDCard_SPIM_sclk_s__0__HSIOM)
    #define SDCard_SPIM_SCLK_S_HSIOM_PTR   ( (reg32 *) SDCard_SPIM_sclk_s__0__HSIOM)
    
    #define SDCard_SPIM_SCLK_S_HSIOM_MASK      (SDCard_SPIM_sclk_s__0__HSIOM_MASK)
    #define SDCard_SPIM_SCLK_S_HSIOM_POS       (SDCard_SPIM_sclk_s__0__HSIOM_SHIFT)
    #define SDCard_SPIM_SCLK_S_HSIOM_SEL_GPIO  (SDCard_SPIM_sclk_s__0__HSIOM_GPIO)
    #define SDCard_SPIM_SCLK_S_HSIOM_SEL_SPI   (SDCard_SPIM_sclk_s__0__HSIOM_SPI)
    
    #define SDCard_SPIM_SS0_S_HSIOM_REG    (*(reg32 *) SDCard_SPIM_ss0_s__0__HSIOM)
    #define SDCard_SPIM_SS0_S_HSIOM_PTR    ( (reg32 *) SDCard_SPIM_ss0_s__0__HSIOM)
    
    #define SDCard_SPIM_SS0_S_HSIOM_MASK       (SDCard_SPIM_ss0_s__0__HSIOM_MASK)
    #define SDCard_SPIM_SS0_S_HSIOM_POS        (SDCard_SPIM_ss0_s__0__HSIOM_SHIFT)
    #define SDCard_SPIM_SS0_S_HSIOM_SEL_GPIO   (SDCard_SPIM_ss0_s__0__HSIOM_GPIO)  
    #define SDCard_SPIM_SS0_S_HSIOM_SEL_SPI    (SDCard_SPIM_ss0_s__0__HSIOM_SPI)
#endif /* (SDCard_SPIM_SPI_SLAVE_PINS) */

#if (SDCard_SPIM_SPI_SLAVE_MOSI_PIN)
    #define SDCard_SPIM_MOSI_S_HSIOM_REG   (*(reg32 *) SDCard_SPIM_mosi_s__0__HSIOM)
    #define SDCard_SPIM_MOSI_S_HSIOM_PTR   ( (reg32 *) SDCard_SPIM_mosi_s__0__HSIOM)
    
    #define SDCard_SPIM_MOSI_S_HSIOM_MASK      (SDCard_SPIM_mosi_s__0__HSIOM_MASK)
    #define SDCard_SPIM_MOSI_S_HSIOM_POS       (SDCard_SPIM_mosi_s__0__HSIOM_SHIFT)
    #define SDCard_SPIM_MOSI_S_HSIOM_SEL_GPIO  (SDCard_SPIM_mosi_s__0__HSIOM_GPIO)
    #define SDCard_SPIM_MOSI_S_HSIOM_SEL_SPI   (SDCard_SPIM_mosi_s__0__HSIOM_SPI)
#endif /* (SDCard_SPIM_SPI_SLAVE_MOSI_PIN) */

#if (SDCard_SPIM_SPI_SLAVE_MISO_PIN)
    #define SDCard_SPIM_MISO_S_HSIOM_REG   (*(reg32 *) SDCard_SPIM_miso_s__0__HSIOM)
    #define SDCard_SPIM_MISO_S_HSIOM_PTR   ( (reg32 *) SDCard_SPIM_miso_s__0__HSIOM)
    
    #define SDCard_SPIM_MISO_S_HSIOM_MASK      (SDCard_SPIM_miso_s__0__HSIOM_MASK)
    #define SDCard_SPIM_MISO_S_HSIOM_POS       (SDCard_SPIM_miso_s__0__HSIOM_SHIFT)
    #define SDCard_SPIM_MISO_S_HSIOM_SEL_GPIO  (SDCard_SPIM_miso_s__0__HSIOM_GPIO)
    #define SDCard_SPIM_MISO_S_HSIOM_SEL_SPI   (SDCard_SPIM_miso_s__0__HSIOM_SPI)
#endif /* (SDCard_SPIM_SPI_SLAVE_MISO_PIN) */

#if (SDCard_SPIM_SPI_MASTER_MISO_PIN)
    #define SDCard_SPIM_MISO_M_HSIOM_REG   (*(reg32 *) SDCard_SPIM_miso_m__0__HSIOM)
    #define SDCard_SPIM_MISO_M_HSIOM_PTR   ( (reg32 *) SDCard_SPIM_miso_m__0__HSIOM)
    
    #define SDCard_SPIM_MISO_M_HSIOM_MASK      (SDCard_SPIM_miso_m__0__HSIOM_MASK)
    #define SDCard_SPIM_MISO_M_HSIOM_POS       (SDCard_SPIM_miso_m__0__HSIOM_SHIFT)
    #define SDCard_SPIM_MISO_M_HSIOM_SEL_GPIO  (SDCard_SPIM_miso_m__0__HSIOM_GPIO)
    #define SDCard_SPIM_MISO_M_HSIOM_SEL_SPI   (SDCard_SPIM_miso_m__0__HSIOM_SPI)
#endif /* (SDCard_SPIM_SPI_MASTER_MISO_PIN) */

#if (SDCard_SPIM_SPI_MASTER_MOSI_PIN)
    #define SDCard_SPIM_MOSI_M_HSIOM_REG   (*(reg32 *) SDCard_SPIM_mosi_m__0__HSIOM)
    #define SDCard_SPIM_MOSI_M_HSIOM_PTR   ( (reg32 *) SDCard_SPIM_mosi_m__0__HSIOM)
    
    #define SDCard_SPIM_MOSI_M_HSIOM_MASK      (SDCard_SPIM_mosi_m__0__HSIOM_MASK)
    #define SDCard_SPIM_MOSI_M_HSIOM_POS       (SDCard_SPIM_mosi_m__0__HSIOM_SHIFT)
    #define SDCard_SPIM_MOSI_M_HSIOM_SEL_GPIO  (SDCard_SPIM_mosi_m__0__HSIOM_GPIO)
    #define SDCard_SPIM_MOSI_M_HSIOM_SEL_SPI   (SDCard_SPIM_mosi_m__0__HSIOM_SPI)
#endif /* (SDCard_SPIM_SPI_MASTER_MOSI_PIN) */

#if (SDCard_SPIM_SPI_MASTER_SCLK_PIN)
    #define SDCard_SPIM_SCLK_M_HSIOM_REG   (*(reg32 *) SDCard_SPIM_sclk_m__0__HSIOM)
    #define SDCard_SPIM_SCLK_M_HSIOM_PTR   ( (reg32 *) SDCard_SPIM_sclk_m__0__HSIOM)
    
    #define SDCard_SPIM_SCLK_M_HSIOM_MASK      (SDCard_SPIM_sclk_m__0__HSIOM_MASK)
    #define SDCard_SPIM_SCLK_M_HSIOM_POS       (SDCard_SPIM_sclk_m__0__HSIOM_SHIFT)
    #define SDCard_SPIM_SCLK_M_HSIOM_SEL_GPIO  (SDCard_SPIM_sclk_m__0__HSIOM_GPIO)
    #define SDCard_SPIM_SCLK_M_HSIOM_SEL_SPI   (SDCard_SPIM_sclk_m__0__HSIOM_SPI)
#endif /* (SDCard_SPIM_SPI_MASTER_SCLK_PIN) */

#if (SDCard_SPIM_SPI_MASTER_SS0_PIN)
    #define SDCard_SPIM_SS0_M_HSIOM_REG    (*(reg32 *) SDCard_SPIM_ss0_m__0__HSIOM)
    #define SDCard_SPIM_SS0_M_HSIOM_PTR    ( (reg32 *) SDCard_SPIM_ss0_m__0__HSIOM)
    
    #define SDCard_SPIM_SS0_M_HSIOM_MASK       (SDCard_SPIM_ss0_m__0__HSIOM_MASK)
    #define SDCard_SPIM_SS0_M_HSIOM_POS        (SDCard_SPIM_ss0_m__0__HSIOM_SHIFT)
    #define SDCard_SPIM_SS0_M_HSIOM_SEL_GPIO   (SDCard_SPIM_ss0_m__0__HSIOM_GPIO)
    #define SDCard_SPIM_SS0_M_HSIOM_SEL_SPI    (SDCard_SPIM_ss0_m__0__HSIOM_SPI)
#endif /* (SDCard_SPIM_SPI_MASTER_SS0_PIN) */

#if (SDCard_SPIM_SPI_MASTER_SS1_PIN)
    #define SDCard_SPIM_SS1_M_HSIOM_REG    (*(reg32 *) SDCard_SPIM_ss1_m__0__HSIOM)
    #define SDCard_SPIM_SS1_M_HSIOM_PTR    ( (reg32 *) SDCard_SPIM_ss1_m__0__HSIOM)
    
    #define SDCard_SPIM_SS1_M_HSIOM_MASK       (SDCard_SPIM_ss1_m__0__HSIOM_MASK)
    #define SDCard_SPIM_SS1_M_HSIOM_POS        (SDCard_SPIM_ss1_m__0__HSIOM_SHIFT)
    #define SDCard_SPIM_SS1_M_HSIOM_SEL_GPIO   (SDCard_SPIM_ss1_m__0__HSIOM_GPIO)
    #define SDCard_SPIM_SS1_M_HSIOM_SEL_SPI    (SDCard_SPIM_ss1_m__0__HSIOM_SPI)
#endif /* (SDCard_SPIM_SPI_MASTER_SS1_PIN) */

#if (SDCard_SPIM_SPI_MASTER_SS2_PIN)
    #define SDCard_SPIM_SS2_M_HSIOM_REG    (*(reg32 *) SDCard_SPIM_ss2_m__0__HSIOM)
    #define SDCard_SPIM_SS2_M_HSIOM_PTR    ( (reg32 *) SDCard_SPIM_ss2_m__0__HSIOM)
    
    #define SDCard_SPIM_SS2_M_HSIOM_MASK       (SDCard_SPIM_ss2_m__0__HSIOM_MASK)
    #define SDCard_SPIM_SS2_M_HSIOM_POS        (SDCard_SPIM_ss2_m__0__HSIOM_SHIFT)
    #define SDCard_SPIM_SS2_M_HSIOM_SEL_GPIO   (SDCard_SPIM_ss2_m__0__HSIOM_GPIO)
    #define SDCard_SPIM_SS2_M_HSIOM_SEL_SPI    (SDCard_SPIM_ss2_m__0__HSIOM_SPI)
#endif /* (SDCard_SPIM_SPI_MASTER_SS2_PIN) */

#if (SDCard_SPIM_SPI_MASTER_SS3_PIN)
    #define SDCard_SPIM_SS3_M_HSIOM_REG    (*(reg32 *) SDCard_SPIM_ss3_m__0__HSIOM)
    #define SDCard_SPIM_SS3_M_HSIOM_PTR    ( (reg32 *) SDCard_SPIM_ss3_m__0__HSIOM)
    
    #define SDCard_SPIM_SS3_M_HSIOM_MASK      (SDCard_SPIM_ss3_m__0__HSIOM_MASK)
    #define SDCard_SPIM_SS3_M_HSIOM_POS       (SDCard_SPIM_ss3_m__0__HSIOM_SHIFT)
    #define SDCard_SPIM_SS3_M_HSIOM_SEL_GPIO  (SDCard_SPIM_ss3_m__0__HSIOM_GPIO)
    #define SDCard_SPIM_SS3_M_HSIOM_SEL_SPI   (SDCard_SPIM_ss3_m__0__HSIOM_SPI)
#endif /* (SDCard_SPIM_SPI_MASTER_SS3_PIN) */

#if (SDCard_SPIM_UART_RX_PIN)
    #define SDCard_SPIM_RX_HSIOM_REG   (*(reg32 *) SDCard_SPIM_rx__0__HSIOM)
    #define SDCard_SPIM_RX_HSIOM_PTR   ( (reg32 *) SDCard_SPIM_rx__0__HSIOM)
    
    #define SDCard_SPIM_RX_HSIOM_MASK      (SDCard_SPIM_rx__0__HSIOM_MASK)
    #define SDCard_SPIM_RX_HSIOM_POS       (SDCard_SPIM_rx__0__HSIOM_SHIFT)
    #define SDCard_SPIM_RX_HSIOM_SEL_GPIO  (SDCard_SPIM_rx__0__HSIOM_GPIO)
    #define SDCard_SPIM_RX_HSIOM_SEL_UART  (SDCard_SPIM_rx__0__HSIOM_UART)
#endif /* (SDCard_SPIM_UART_RX_PIN) */

#if (SDCard_SPIM_UART_RX_WAKE_PIN)
    #define SDCard_SPIM_RX_WAKE_HSIOM_REG   (*(reg32 *) SDCard_SPIM_rx_wake__0__HSIOM)
    #define SDCard_SPIM_RX_WAKE_HSIOM_PTR   ( (reg32 *) SDCard_SPIM_rx_wake__0__HSIOM)
    
    #define SDCard_SPIM_RX_WAKE_HSIOM_MASK      (SDCard_SPIM_rx_wake__0__HSIOM_MASK)
    #define SDCard_SPIM_RX_WAKE_HSIOM_POS       (SDCard_SPIM_rx_wake__0__HSIOM_SHIFT)
    #define SDCard_SPIM_RX_WAKE_HSIOM_SEL_GPIO  (SDCard_SPIM_rx_wake__0__HSIOM_GPIO)
    #define SDCard_SPIM_RX_WAKE_HSIOM_SEL_UART  (SDCard_SPIM_rx_wake__0__HSIOM_UART)
#endif /* (SDCard_SPIM_UART_WAKE_RX_PIN) */

#if (SDCard_SPIM_UART_CTS_PIN)
    #define SDCard_SPIM_CTS_HSIOM_REG   (*(reg32 *) SDCard_SPIM_cts__0__HSIOM)
    #define SDCard_SPIM_CTS_HSIOM_PTR   ( (reg32 *) SDCard_SPIM_cts__0__HSIOM)
    
    #define SDCard_SPIM_CTS_HSIOM_MASK      (SDCard_SPIM_cts__0__HSIOM_MASK)
    #define SDCard_SPIM_CTS_HSIOM_POS       (SDCard_SPIM_cts__0__HSIOM_SHIFT)
    #define SDCard_SPIM_CTS_HSIOM_SEL_GPIO  (SDCard_SPIM_cts__0__HSIOM_GPIO)
    #define SDCard_SPIM_CTS_HSIOM_SEL_UART  (SDCard_SPIM_cts__0__HSIOM_UART)
#endif /* (SDCard_SPIM_UART_CTS_PIN) */

#if (SDCard_SPIM_UART_TX_PIN)
    #define SDCard_SPIM_TX_HSIOM_REG   (*(reg32 *) SDCard_SPIM_tx__0__HSIOM)
    #define SDCard_SPIM_TX_HSIOM_PTR   ( (reg32 *) SDCard_SPIM_tx__0__HSIOM)
    
    #define SDCard_SPIM_TX_HSIOM_MASK      (SDCard_SPIM_tx__0__HSIOM_MASK)
    #define SDCard_SPIM_TX_HSIOM_POS       (SDCard_SPIM_tx__0__HSIOM_SHIFT)
    #define SDCard_SPIM_TX_HSIOM_SEL_GPIO  (SDCard_SPIM_tx__0__HSIOM_GPIO)
    #define SDCard_SPIM_TX_HSIOM_SEL_UART  (SDCard_SPIM_tx__0__HSIOM_UART)
#endif /* (SDCard_SPIM_UART_TX_PIN) */

#if (SDCard_SPIM_UART_RX_TX_PIN)
    #define SDCard_SPIM_RX_TX_HSIOM_REG   (*(reg32 *) SDCard_SPIM_rx_tx__0__HSIOM)
    #define SDCard_SPIM_RX_TX_HSIOM_PTR   ( (reg32 *) SDCard_SPIM_rx_tx__0__HSIOM)
    
    #define SDCard_SPIM_RX_TX_HSIOM_MASK      (SDCard_SPIM_rx_tx__0__HSIOM_MASK)
    #define SDCard_SPIM_RX_TX_HSIOM_POS       (SDCard_SPIM_rx_tx__0__HSIOM_SHIFT)
    #define SDCard_SPIM_RX_TX_HSIOM_SEL_GPIO  (SDCard_SPIM_rx_tx__0__HSIOM_GPIO)
    #define SDCard_SPIM_RX_TX_HSIOM_SEL_UART  (SDCard_SPIM_rx_tx__0__HSIOM_UART)
#endif /* (SDCard_SPIM_UART_RX_TX_PIN) */

#if (SDCard_SPIM_UART_RTS_PIN)
    #define SDCard_SPIM_RTS_HSIOM_REG      (*(reg32 *) SDCard_SPIM_rts__0__HSIOM)
    #define SDCard_SPIM_RTS_HSIOM_PTR      ( (reg32 *) SDCard_SPIM_rts__0__HSIOM)
    
    #define SDCard_SPIM_RTS_HSIOM_MASK     (SDCard_SPIM_rts__0__HSIOM_MASK)
    #define SDCard_SPIM_RTS_HSIOM_POS      (SDCard_SPIM_rts__0__HSIOM_SHIFT)    
    #define SDCard_SPIM_RTS_HSIOM_SEL_GPIO (SDCard_SPIM_rts__0__HSIOM_GPIO)
    #define SDCard_SPIM_RTS_HSIOM_SEL_UART (SDCard_SPIM_rts__0__HSIOM_UART)    
#endif /* (SDCard_SPIM_UART_RTS_PIN) */


/***************************************
*        Registers Constants
***************************************/

/* HSIOM switch values. */ 
#define SDCard_SPIM_HSIOM_DEF_SEL      (0x00u)
#define SDCard_SPIM_HSIOM_GPIO_SEL     (0x00u)
/* The HSIOM values provided below are valid only for SDCard_SPIM_CY_SCBIP_V0 
* and SDCard_SPIM_CY_SCBIP_V1. It is not recommended to use them for 
* SDCard_SPIM_CY_SCBIP_V2. Use pin name specific HSIOM constants provided 
* above instead for any SCB IP block version.
*/
#define SDCard_SPIM_HSIOM_UART_SEL     (0x09u)
#define SDCard_SPIM_HSIOM_I2C_SEL      (0x0Eu)
#define SDCard_SPIM_HSIOM_SPI_SEL      (0x0Fu)

/* Pins settings index. */
#define SDCard_SPIM_RX_WAKE_SCL_MOSI_PIN_INDEX   (0u)
#define SDCard_SPIM_RX_SCL_MOSI_PIN_INDEX       (0u)
#define SDCard_SPIM_TX_SDA_MISO_PIN_INDEX       (1u)
#define SDCard_SPIM_SCLK_PIN_INDEX       (2u)
#define SDCard_SPIM_SS0_PIN_INDEX       (3u)
#define SDCard_SPIM_SS1_PIN_INDEX                  (4u)
#define SDCard_SPIM_SS2_PIN_INDEX                  (5u)
#define SDCard_SPIM_SS3_PIN_INDEX                  (6u)

/* Pins settings mask. */
#define SDCard_SPIM_RX_WAKE_SCL_MOSI_PIN_MASK ((uint32) 0x01u << SDCard_SPIM_RX_WAKE_SCL_MOSI_PIN_INDEX)
#define SDCard_SPIM_RX_SCL_MOSI_PIN_MASK     ((uint32) 0x01u << SDCard_SPIM_RX_SCL_MOSI_PIN_INDEX)
#define SDCard_SPIM_TX_SDA_MISO_PIN_MASK     ((uint32) 0x01u << SDCard_SPIM_TX_SDA_MISO_PIN_INDEX)
#define SDCard_SPIM_SCLK_PIN_MASK     ((uint32) 0x01u << SDCard_SPIM_SCLK_PIN_INDEX)
#define SDCard_SPIM_SS0_PIN_MASK     ((uint32) 0x01u << SDCard_SPIM_SS0_PIN_INDEX)
#define SDCard_SPIM_SS1_PIN_MASK                ((uint32) 0x01u << SDCard_SPIM_SS1_PIN_INDEX)
#define SDCard_SPIM_SS2_PIN_MASK                ((uint32) 0x01u << SDCard_SPIM_SS2_PIN_INDEX)
#define SDCard_SPIM_SS3_PIN_MASK                ((uint32) 0x01u << SDCard_SPIM_SS3_PIN_INDEX)

/* Pin interrupt constants. */
#define SDCard_SPIM_INTCFG_TYPE_MASK           (0x03u)
#define SDCard_SPIM_INTCFG_TYPE_FALLING_EDGE   (0x02u)

/* Pin Drive Mode constants. */
#define SDCard_SPIM_PIN_DM_ALG_HIZ  (0u)
#define SDCard_SPIM_PIN_DM_DIG_HIZ  (1u)
#define SDCard_SPIM_PIN_DM_OD_LO    (4u)
#define SDCard_SPIM_PIN_DM_STRONG   (6u)


/***************************************
*          Macro Definitions
***************************************/

/* Return drive mode of the pin */
#define SDCard_SPIM_DM_MASK    (0x7u)
#define SDCard_SPIM_DM_SIZE    (3u)
#define SDCard_SPIM_GET_P4_PIN_DM(reg, pos) \
    ( ((reg) & (uint32) ((uint32) SDCard_SPIM_DM_MASK << (SDCard_SPIM_DM_SIZE * (pos)))) >> \
                                                              (SDCard_SPIM_DM_SIZE * (pos)) )

#if (SDCard_SPIM_TX_SDA_MISO_PIN)
    #define SDCard_SPIM_CHECK_TX_SDA_MISO_PIN_USED \
                (SDCard_SPIM_PIN_DM_ALG_HIZ != \
                    SDCard_SPIM_GET_P4_PIN_DM(SDCard_SPIM_uart_tx_i2c_sda_spi_miso_PC, \
                                                   SDCard_SPIM_uart_tx_i2c_sda_spi_miso_SHIFT))
#endif /* (SDCard_SPIM_TX_SDA_MISO_PIN) */

#if (SDCard_SPIM_SS0_PIN)
    #define SDCard_SPIM_CHECK_SS0_PIN_USED \
                (SDCard_SPIM_PIN_DM_ALG_HIZ != \
                    SDCard_SPIM_GET_P4_PIN_DM(SDCard_SPIM_spi_ss0_PC, \
                                                   SDCard_SPIM_spi_ss0_SHIFT))
#endif /* (SDCard_SPIM_SS0_PIN) */

/* Set bits-mask in register */
#define SDCard_SPIM_SET_REGISTER_BITS(reg, mask, pos, mode) \
                    do                                           \
                    {                                            \
                        (reg) = (((reg) & ((uint32) ~(uint32) (mask))) | ((uint32) ((uint32) (mode) << (pos)))); \
                    }while(0)

/* Set bit in the register */
#define SDCard_SPIM_SET_REGISTER_BIT(reg, mask, val) \
                    ((val) ? ((reg) |= (mask)) : ((reg) &= ((uint32) ~((uint32) (mask)))))

#define SDCard_SPIM_SET_HSIOM_SEL(reg, mask, pos, sel) SDCard_SPIM_SET_REGISTER_BITS(reg, mask, pos, sel)
#define SDCard_SPIM_SET_INCFG_TYPE(reg, mask, pos, intType) \
                                                        SDCard_SPIM_SET_REGISTER_BITS(reg, mask, pos, intType)
#define SDCard_SPIM_SET_INP_DIS(reg, mask, val) SDCard_SPIM_SET_REGISTER_BIT(reg, mask, val)

/* SDCard_SPIM_SET_I2C_SCL_DR(val) - Sets I2C SCL DR register.
*  SDCard_SPIM_SET_I2C_SCL_HSIOM_SEL(sel) - Sets I2C SCL HSIOM settings.
*/
/* SCB I2C: scl signal */
#if (SDCard_SPIM_CY_SCBIP_V0)
#if (SDCard_SPIM_I2C_PINS)
    #define SDCard_SPIM_SET_I2C_SCL_DR(val) SDCard_SPIM_scl_Write(val)

    #define SDCard_SPIM_SET_I2C_SCL_HSIOM_SEL(sel) \
                          SDCard_SPIM_SET_HSIOM_SEL(SDCard_SPIM_SCL_HSIOM_REG,  \
                                                         SDCard_SPIM_SCL_HSIOM_MASK, \
                                                         SDCard_SPIM_SCL_HSIOM_POS,  \
                                                         (sel))
    #define SDCard_SPIM_WAIT_SCL_SET_HIGH  (0u == SDCard_SPIM_scl_Read())

/* Unconfigured SCB: scl signal */
#elif (SDCard_SPIM_RX_WAKE_SCL_MOSI_PIN)
    #define SDCard_SPIM_SET_I2C_SCL_DR(val) \
                            SDCard_SPIM_uart_rx_wake_i2c_scl_spi_mosi_Write(val)

    #define SDCard_SPIM_SET_I2C_SCL_HSIOM_SEL(sel) \
                    SDCard_SPIM_SET_HSIOM_SEL(SDCard_SPIM_RX_WAKE_SCL_MOSI_HSIOM_REG,  \
                                                   SDCard_SPIM_RX_WAKE_SCL_MOSI_HSIOM_MASK, \
                                                   SDCard_SPIM_RX_WAKE_SCL_MOSI_HSIOM_POS,  \
                                                   (sel))

    #define SDCard_SPIM_WAIT_SCL_SET_HIGH  (0u == SDCard_SPIM_uart_rx_wake_i2c_scl_spi_mosi_Read())

#elif (SDCard_SPIM_RX_SCL_MOSI_PIN)
    #define SDCard_SPIM_SET_I2C_SCL_DR(val) \
                            SDCard_SPIM_uart_rx_i2c_scl_spi_mosi_Write(val)


    #define SDCard_SPIM_SET_I2C_SCL_HSIOM_SEL(sel) \
                            SDCard_SPIM_SET_HSIOM_SEL(SDCard_SPIM_RX_SCL_MOSI_HSIOM_REG,  \
                                                           SDCard_SPIM_RX_SCL_MOSI_HSIOM_MASK, \
                                                           SDCard_SPIM_RX_SCL_MOSI_HSIOM_POS,  \
                                                           (sel))

    #define SDCard_SPIM_WAIT_SCL_SET_HIGH  (0u == SDCard_SPIM_uart_rx_i2c_scl_spi_mosi_Read())

#else
    #define SDCard_SPIM_SET_I2C_SCL_DR(val)        do{ /* Does nothing */ }while(0)
    #define SDCard_SPIM_SET_I2C_SCL_HSIOM_SEL(sel) do{ /* Does nothing */ }while(0)

    #define SDCard_SPIM_WAIT_SCL_SET_HIGH  (0u)
#endif /* (SDCard_SPIM_I2C_PINS) */

/* SCB I2C: sda signal */
#if (SDCard_SPIM_I2C_PINS)
    #define SDCard_SPIM_WAIT_SDA_SET_HIGH  (0u == SDCard_SPIM_sda_Read())
/* Unconfigured SCB: sda signal */
#elif (SDCard_SPIM_TX_SDA_MISO_PIN)
    #define SDCard_SPIM_WAIT_SDA_SET_HIGH  (0u == SDCard_SPIM_uart_tx_i2c_sda_spi_miso_Read())
#else
    #define SDCard_SPIM_WAIT_SDA_SET_HIGH  (0u)
#endif /* (SDCard_SPIM_MOSI_SCL_RX_PIN) */
#endif /* (SDCard_SPIM_CY_SCBIP_V0) */

/* Clear UART wakeup source */
#if (SDCard_SPIM_RX_SCL_MOSI_PIN)
    #define SDCard_SPIM_CLEAR_UART_RX_WAKE_INTR        do{ /* Does nothing */ }while(0)
    
#elif (SDCard_SPIM_RX_WAKE_SCL_MOSI_PIN)
    #define SDCard_SPIM_CLEAR_UART_RX_WAKE_INTR \
            do{                                      \
                (void) SDCard_SPIM_uart_rx_wake_i2c_scl_spi_mosi_ClearInterrupt(); \
            }while(0)

#elif(SDCard_SPIM_UART_RX_WAKE_PIN)
    #define SDCard_SPIM_CLEAR_UART_RX_WAKE_INTR \
            do{                                      \
                (void) SDCard_SPIM_rx_wake_ClearInterrupt(); \
            }while(0)
#else
#endif /* (SDCard_SPIM_RX_SCL_MOSI_PIN) */


/***************************************
* The following code is DEPRECATED and
* must not be used.
***************************************/

/* Unconfigured pins */
#define SDCard_SPIM_REMOVE_MOSI_SCL_RX_WAKE_PIN    SDCard_SPIM_REMOVE_RX_WAKE_SCL_MOSI_PIN
#define SDCard_SPIM_REMOVE_MOSI_SCL_RX_PIN         SDCard_SPIM_REMOVE_RX_SCL_MOSI_PIN
#define SDCard_SPIM_REMOVE_MISO_SDA_TX_PIN         SDCard_SPIM_REMOVE_TX_SDA_MISO_PIN
#ifndef SDCard_SPIM_REMOVE_SCLK_PIN
#define SDCard_SPIM_REMOVE_SCLK_PIN                SDCard_SPIM_REMOVE_SCLK_PIN
#endif /* SDCard_SPIM_REMOVE_SCLK_PIN */
#ifndef SDCard_SPIM_REMOVE_SS0_PIN
#define SDCard_SPIM_REMOVE_SS0_PIN                 SDCard_SPIM_REMOVE_SS0_PIN
#endif /* SDCard_SPIM_REMOVE_SS0_PIN */

/* Unconfigured pins */
#define SDCard_SPIM_MOSI_SCL_RX_WAKE_PIN   SDCard_SPIM_RX_WAKE_SCL_MOSI_PIN
#define SDCard_SPIM_MOSI_SCL_RX_PIN        SDCard_SPIM_RX_SCL_MOSI_PIN
#define SDCard_SPIM_MISO_SDA_TX_PIN        SDCard_SPIM_TX_SDA_MISO_PIN
#ifndef SDCard_SPIM_SCLK_PIN
#define SDCard_SPIM_SCLK_PIN               SDCard_SPIM_SCLK_PIN
#endif /* SDCard_SPIM_SCLK_PIN */
#ifndef SDCard_SPIM_SS0_PIN
#define SDCard_SPIM_SS0_PIN                SDCard_SPIM_SS0_PIN
#endif /* SDCard_SPIM_SS0_PIN */

#if (SDCard_SPIM_MOSI_SCL_RX_WAKE_PIN)
    #define SDCard_SPIM_MOSI_SCL_RX_WAKE_HSIOM_REG     SDCard_SPIM_RX_WAKE_SCL_MOSI_HSIOM_REG
    #define SDCard_SPIM_MOSI_SCL_RX_WAKE_HSIOM_PTR     SDCard_SPIM_RX_WAKE_SCL_MOSI_HSIOM_REG
    #define SDCard_SPIM_MOSI_SCL_RX_WAKE_HSIOM_MASK    SDCard_SPIM_RX_WAKE_SCL_MOSI_HSIOM_REG
    #define SDCard_SPIM_MOSI_SCL_RX_WAKE_HSIOM_POS     SDCard_SPIM_RX_WAKE_SCL_MOSI_HSIOM_REG

    #define SDCard_SPIM_MOSI_SCL_RX_WAKE_INTCFG_REG    SDCard_SPIM_RX_WAKE_SCL_MOSI_HSIOM_REG
    #define SDCard_SPIM_MOSI_SCL_RX_WAKE_INTCFG_PTR    SDCard_SPIM_RX_WAKE_SCL_MOSI_HSIOM_REG

    #define SDCard_SPIM_MOSI_SCL_RX_WAKE_INTCFG_TYPE_POS   SDCard_SPIM_RX_WAKE_SCL_MOSI_HSIOM_REG
    #define SDCard_SPIM_MOSI_SCL_RX_WAKE_INTCFG_TYPE_MASK  SDCard_SPIM_RX_WAKE_SCL_MOSI_HSIOM_REG
#endif /* (SDCard_SPIM_RX_WAKE_SCL_MOSI_PIN) */

#if (SDCard_SPIM_MOSI_SCL_RX_PIN)
    #define SDCard_SPIM_MOSI_SCL_RX_HSIOM_REG      SDCard_SPIM_RX_SCL_MOSI_HSIOM_REG
    #define SDCard_SPIM_MOSI_SCL_RX_HSIOM_PTR      SDCard_SPIM_RX_SCL_MOSI_HSIOM_PTR
    #define SDCard_SPIM_MOSI_SCL_RX_HSIOM_MASK     SDCard_SPIM_RX_SCL_MOSI_HSIOM_MASK
    #define SDCard_SPIM_MOSI_SCL_RX_HSIOM_POS      SDCard_SPIM_RX_SCL_MOSI_HSIOM_POS
#endif /* (SDCard_SPIM_MOSI_SCL_RX_PIN) */

#if (SDCard_SPIM_MISO_SDA_TX_PIN)
    #define SDCard_SPIM_MISO_SDA_TX_HSIOM_REG      SDCard_SPIM_TX_SDA_MISO_HSIOM_REG
    #define SDCard_SPIM_MISO_SDA_TX_HSIOM_PTR      SDCard_SPIM_TX_SDA_MISO_HSIOM_REG
    #define SDCard_SPIM_MISO_SDA_TX_HSIOM_MASK     SDCard_SPIM_TX_SDA_MISO_HSIOM_REG
    #define SDCard_SPIM_MISO_SDA_TX_HSIOM_POS      SDCard_SPIM_TX_SDA_MISO_HSIOM_REG
#endif /* (SDCard_SPIM_MISO_SDA_TX_PIN_PIN) */

#if (SDCard_SPIM_SCLK_PIN)
    #ifndef SDCard_SPIM_SCLK_HSIOM_REG
    #define SDCard_SPIM_SCLK_HSIOM_REG     SDCard_SPIM_SCLK_HSIOM_REG
    #define SDCard_SPIM_SCLK_HSIOM_PTR     SDCard_SPIM_SCLK_HSIOM_PTR
    #define SDCard_SPIM_SCLK_HSIOM_MASK    SDCard_SPIM_SCLK_HSIOM_MASK
    #define SDCard_SPIM_SCLK_HSIOM_POS     SDCard_SPIM_SCLK_HSIOM_POS
    #endif /* SDCard_SPIM_SCLK_HSIOM_REG */
#endif /* (SDCard_SPIM_SCLK_PIN) */

#if (SDCard_SPIM_SS0_PIN)
    #ifndef SDCard_SPIM_SS0_HSIOM_REG
    #define SDCard_SPIM_SS0_HSIOM_REG      SDCard_SPIM_SS0_HSIOM_REG
    #define SDCard_SPIM_SS0_HSIOM_PTR      SDCard_SPIM_SS0_HSIOM_PTR
    #define SDCard_SPIM_SS0_HSIOM_MASK     SDCard_SPIM_SS0_HSIOM_MASK
    #define SDCard_SPIM_SS0_HSIOM_POS      SDCard_SPIM_SS0_HSIOM_POS
    #endif /* SDCard_SPIM_SS0_HSIOM_REG */
#endif /* (SDCard_SPIM_SS0_PIN) */

#define SDCard_SPIM_MOSI_SCL_RX_WAKE_PIN_INDEX SDCard_SPIM_RX_WAKE_SCL_MOSI_PIN_INDEX
#define SDCard_SPIM_MOSI_SCL_RX_PIN_INDEX      SDCard_SPIM_RX_SCL_MOSI_PIN_INDEX
#define SDCard_SPIM_MISO_SDA_TX_PIN_INDEX      SDCard_SPIM_TX_SDA_MISO_PIN_INDEX
#ifndef SDCard_SPIM_SCLK_PIN_INDEX
#define SDCard_SPIM_SCLK_PIN_INDEX             SDCard_SPIM_SCLK_PIN_INDEX
#endif /* SDCard_SPIM_SCLK_PIN_INDEX */
#ifndef SDCard_SPIM_SS0_PIN_INDEX
#define SDCard_SPIM_SS0_PIN_INDEX              SDCard_SPIM_SS0_PIN_INDEX
#endif /* SDCard_SPIM_SS0_PIN_INDEX */

#define SDCard_SPIM_MOSI_SCL_RX_WAKE_PIN_MASK SDCard_SPIM_RX_WAKE_SCL_MOSI_PIN_MASK
#define SDCard_SPIM_MOSI_SCL_RX_PIN_MASK      SDCard_SPIM_RX_SCL_MOSI_PIN_MASK
#define SDCard_SPIM_MISO_SDA_TX_PIN_MASK      SDCard_SPIM_TX_SDA_MISO_PIN_MASK
#ifndef SDCard_SPIM_SCLK_PIN_MASK
#define SDCard_SPIM_SCLK_PIN_MASK             SDCard_SPIM_SCLK_PIN_MASK
#endif /* SDCard_SPIM_SCLK_PIN_MASK */
#ifndef SDCard_SPIM_SS0_PIN_MASK
#define SDCard_SPIM_SS0_PIN_MASK              SDCard_SPIM_SS0_PIN_MASK
#endif /* SDCard_SPIM_SS0_PIN_MASK */

#endif /* (CY_SCB_PINS_SDCard_SPIM_H) */


/* [] END OF FILE */
