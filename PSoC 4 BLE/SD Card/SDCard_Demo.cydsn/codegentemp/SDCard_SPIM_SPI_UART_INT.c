/***************************************************************************//**
* \file SDCard_SPIM_SPI_UART_INT.c
* \version 3.20
*
* \brief
*  This file provides the source code to the Interrupt Service Routine for
*  the SCB Component in SPI and UART modes.
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

#include "SDCard_SPIM_PVT.h"
#include "SDCard_SPIM_SPI_UART_PVT.h"


#if (SDCard_SPIM_SCB_IRQ_INTERNAL)
/*******************************************************************************
* Function Name: SDCard_SPIM_SPI_UART_ISR
****************************************************************************//**
*
*  Handles the Interrupt Service Routine for the SCB SPI or UART modes.
*
*******************************************************************************/
CY_ISR(SDCard_SPIM_SPI_UART_ISR)
{
#if (SDCard_SPIM_INTERNAL_RX_SW_BUFFER_CONST)
    uint32 locHead;
#endif /* (SDCard_SPIM_INTERNAL_RX_SW_BUFFER_CONST) */

#if (SDCard_SPIM_INTERNAL_TX_SW_BUFFER_CONST)
    uint32 locTail;
#endif /* (SDCard_SPIM_INTERNAL_TX_SW_BUFFER_CONST) */

#ifdef SDCard_SPIM_SPI_UART_ISR_ENTRY_CALLBACK
    SDCard_SPIM_SPI_UART_ISR_EntryCallback();
#endif /* SDCard_SPIM_SPI_UART_ISR_ENTRY_CALLBACK */

    if (NULL != SDCard_SPIM_customIntrHandler)
    {
        SDCard_SPIM_customIntrHandler();
    }

    #if(SDCard_SPIM_CHECK_SPI_WAKE_ENABLE)
    {
        /* Clear SPI wakeup source */
        SDCard_SPIM_ClearSpiExtClkInterruptSource(SDCard_SPIM_INTR_SPI_EC_WAKE_UP);
    }
    #endif

    #if (SDCard_SPIM_CHECK_RX_SW_BUFFER)
    {
        if (SDCard_SPIM_CHECK_INTR_RX_MASKED(SDCard_SPIM_INTR_RX_NOT_EMPTY))
        {
            do
            {
                /* Move local head index */
                locHead = (SDCard_SPIM_rxBufferHead + 1u);

                /* Adjust local head index */
                if (SDCard_SPIM_INTERNAL_RX_BUFFER_SIZE == locHead)
                {
                    locHead = 0u;
                }

                if (locHead == SDCard_SPIM_rxBufferTail)
                {
                    #if (SDCard_SPIM_CHECK_UART_RTS_CONTROL_FLOW)
                    {
                        /* There is no space in the software buffer - disable the
                        * RX Not Empty interrupt source. The data elements are
                        * still being received into the RX FIFO until the RTS signal
                        * stops the transmitter. After the data element is read from the
                        * buffer, the RX Not Empty interrupt source is enabled to
                        * move the next data element in the software buffer.
                        */
                        SDCard_SPIM_INTR_RX_MASK_REG &= ~SDCard_SPIM_INTR_RX_NOT_EMPTY;
                        break;
                    }
                    #else
                    {
                        /* Overflow: through away received data element */
                        (void) SDCard_SPIM_RX_FIFO_RD_REG;
                        SDCard_SPIM_rxBufferOverflow = (uint8) SDCard_SPIM_INTR_RX_OVERFLOW;
                    }
                    #endif
                }
                else
                {
                    /* Store received data */
                    SDCard_SPIM_PutWordInRxBuffer(locHead, SDCard_SPIM_RX_FIFO_RD_REG);

                    /* Move head index */
                    SDCard_SPIM_rxBufferHead = locHead;
                }
            }
            while(0u != SDCard_SPIM_GET_RX_FIFO_ENTRIES);

            SDCard_SPIM_ClearRxInterruptSource(SDCard_SPIM_INTR_RX_NOT_EMPTY);
        }
    }
    #endif


    #if (SDCard_SPIM_CHECK_TX_SW_BUFFER)
    {
        if (SDCard_SPIM_CHECK_INTR_TX_MASKED(SDCard_SPIM_INTR_TX_NOT_FULL))
        {
            do
            {
                /* Check for room in TX software buffer */
                if (SDCard_SPIM_txBufferHead != SDCard_SPIM_txBufferTail)
                {
                    /* Move local tail index */
                    locTail = (SDCard_SPIM_txBufferTail + 1u);

                    /* Adjust local tail index */
                    if (SDCard_SPIM_TX_BUFFER_SIZE == locTail)
                    {
                        locTail = 0u;
                    }

                    /* Put data into TX FIFO */
                    SDCard_SPIM_TX_FIFO_WR_REG = SDCard_SPIM_GetWordFromTxBuffer(locTail);

                    /* Move tail index */
                    SDCard_SPIM_txBufferTail = locTail;
                }
                else
                {
                    /* TX software buffer is empty: complete transfer */
                    SDCard_SPIM_DISABLE_INTR_TX(SDCard_SPIM_INTR_TX_NOT_FULL);
                    break;
                }
            }
            while (SDCard_SPIM_SPI_UART_FIFO_SIZE != SDCard_SPIM_GET_TX_FIFO_ENTRIES);

            SDCard_SPIM_ClearTxInterruptSource(SDCard_SPIM_INTR_TX_NOT_FULL);
        }
    }
    #endif

#ifdef SDCard_SPIM_SPI_UART_ISR_EXIT_CALLBACK
    SDCard_SPIM_SPI_UART_ISR_ExitCallback();
#endif /* SDCard_SPIM_SPI_UART_ISR_EXIT_CALLBACK */

}

#endif /* (SDCard_SPIM_SCB_IRQ_INTERNAL) */


/* [] END OF FILE */
