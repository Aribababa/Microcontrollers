/***************************************************************************//**
* \file SDCard_SPIM_SPI_UART.c
* \version 3.20
*
* \brief
*  This file provides the source code to the API for the SCB Component in
*  SPI and UART modes.
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

/***************************************
*        SPI/UART Private Vars
***************************************/

#if(SDCard_SPIM_INTERNAL_RX_SW_BUFFER_CONST)
    /* Start index to put data into the software receive buffer.*/
    volatile uint32 SDCard_SPIM_rxBufferHead;
    /* Start index to get data from the software receive buffer.*/
    volatile uint32 SDCard_SPIM_rxBufferTail;
    /**
    * \addtogroup group_globals
    * \{
    */
    /** Sets when internal software receive buffer overflow
    *  was occurred.
    */
    volatile uint8  SDCard_SPIM_rxBufferOverflow;
    /** \} globals */
#endif /* (SDCard_SPIM_INTERNAL_RX_SW_BUFFER_CONST) */

#if(SDCard_SPIM_INTERNAL_TX_SW_BUFFER_CONST)
    /* Start index to put data into the software transmit buffer.*/
    volatile uint32 SDCard_SPIM_txBufferHead;
    /* Start index to get data from the software transmit buffer.*/
    volatile uint32 SDCard_SPIM_txBufferTail;
#endif /* (SDCard_SPIM_INTERNAL_TX_SW_BUFFER_CONST) */

#if(SDCard_SPIM_INTERNAL_RX_SW_BUFFER)
    /* Add one element to the buffer to receive full packet. One byte in receive buffer is always empty */
    volatile uint8 SDCard_SPIM_rxBufferInternal[SDCard_SPIM_INTERNAL_RX_BUFFER_SIZE];
#endif /* (SDCard_SPIM_INTERNAL_RX_SW_BUFFER) */

#if(SDCard_SPIM_INTERNAL_TX_SW_BUFFER)
    volatile uint8 SDCard_SPIM_txBufferInternal[SDCard_SPIM_TX_BUFFER_SIZE];
#endif /* (SDCard_SPIM_INTERNAL_TX_SW_BUFFER) */


#if(SDCard_SPIM_RX_DIRECTION)
    /*******************************************************************************
    * Function Name: SDCard_SPIM_SpiUartReadRxData
    ****************************************************************************//**
    *
    *  Retrieves the next data element from the receive buffer.
    *   - RX software buffer is disabled: Returns data element retrieved from
    *     RX FIFO. Undefined data will be returned if the RX FIFO is empty.
    *   - RX software buffer is enabled: Returns data element from the software
    *     receive buffer. Zero value is returned if the software receive buffer
    *     is empty.
    *
    * \return
    *  Next data element from the receive buffer. 
    *  The amount of data bits to be received depends on RX data bits selection 
    *  (the data bit counting starts from LSB of return value).
    *
    * \globalvars
    *  SDCard_SPIM_rxBufferHead - the start index to put data into the 
    *  software receive buffer.
    *  SDCard_SPIM_rxBufferTail - the start index to get data from the 
    *  software receive buffer.
    *
    *******************************************************************************/
    uint32 SDCard_SPIM_SpiUartReadRxData(void)
    {
        uint32 rxData = 0u;

    #if (SDCard_SPIM_INTERNAL_RX_SW_BUFFER_CONST)
        uint32 locTail;
    #endif /* (SDCard_SPIM_INTERNAL_RX_SW_BUFFER_CONST) */

        #if (SDCard_SPIM_CHECK_RX_SW_BUFFER)
        {
            if (SDCard_SPIM_rxBufferHead != SDCard_SPIM_rxBufferTail)
            {
                /* There is data in RX software buffer */

                /* Calculate index to read from */
                locTail = (SDCard_SPIM_rxBufferTail + 1u);

                if (SDCard_SPIM_INTERNAL_RX_BUFFER_SIZE == locTail)
                {
                    locTail = 0u;
                }

                /* Get data from RX software buffer */
                rxData = SDCard_SPIM_GetWordFromRxBuffer(locTail);

                /* Change index in the buffer */
                SDCard_SPIM_rxBufferTail = locTail;

                #if (SDCard_SPIM_CHECK_UART_RTS_CONTROL_FLOW)
                {
                    /* Check if RX Not Empty is disabled in the interrupt */
                    if (0u == (SDCard_SPIM_INTR_RX_MASK_REG & SDCard_SPIM_INTR_RX_NOT_EMPTY))
                    {
                        /* Enable RX Not Empty interrupt source to continue
                        * receiving data into software buffer.
                        */
                        SDCard_SPIM_INTR_RX_MASK_REG |= SDCard_SPIM_INTR_RX_NOT_EMPTY;
                    }
                }
                #endif

            }
        }
        #else
        {
            /* Read data from RX FIFO */
            rxData = SDCard_SPIM_RX_FIFO_RD_REG;
        }
        #endif

        return (rxData);
    }


    /*******************************************************************************
    * Function Name: SDCard_SPIM_SpiUartGetRxBufferSize
    ****************************************************************************//**
    *
    *  Returns the number of received data elements in the receive buffer.
    *   - RX software buffer disabled: returns the number of used entries in
    *     RX FIFO.
    *   - RX software buffer enabled: returns the number of elements which were
    *     placed in the receive buffer. This does not include the hardware RX FIFO.
    *
    * \return
    *  Number of received data elements.
    *
    * \globalvars
    *  SDCard_SPIM_rxBufferHead - the start index to put data into the 
    *  software receive buffer.
    *  SDCard_SPIM_rxBufferTail - the start index to get data from the 
    *  software receive buffer.
    *
    *******************************************************************************/
    uint32 SDCard_SPIM_SpiUartGetRxBufferSize(void)
    {
        uint32 size;
    #if (SDCard_SPIM_INTERNAL_RX_SW_BUFFER_CONST)
        uint32 locHead;
    #endif /* (SDCard_SPIM_INTERNAL_RX_SW_BUFFER_CONST) */

        #if (SDCard_SPIM_CHECK_RX_SW_BUFFER)
        {
            locHead = SDCard_SPIM_rxBufferHead;

            if(locHead >= SDCard_SPIM_rxBufferTail)
            {
                size = (locHead - SDCard_SPIM_rxBufferTail);
            }
            else
            {
                size = (locHead + (SDCard_SPIM_INTERNAL_RX_BUFFER_SIZE - SDCard_SPIM_rxBufferTail));
            }
        }
        #else
        {
            size = SDCard_SPIM_GET_RX_FIFO_ENTRIES;
        }
        #endif

        return (size);
    }


    /*******************************************************************************
    * Function Name: SDCard_SPIM_SpiUartClearRxBuffer
    ****************************************************************************//**
    *
    *  Clears the receive buffer and RX FIFO.
    *
    * \globalvars
    *  SDCard_SPIM_rxBufferHead - the start index to put data into the 
    *  software receive buffer.
    *  SDCard_SPIM_rxBufferTail - the start index to get data from the 
    *  software receive buffer.
    *
    *******************************************************************************/
    void SDCard_SPIM_SpiUartClearRxBuffer(void)
    {
        #if (SDCard_SPIM_CHECK_RX_SW_BUFFER)
        {
            /* Lock from component interruption */
            SDCard_SPIM_DisableInt();

            /* Flush RX software buffer */
            SDCard_SPIM_rxBufferHead = SDCard_SPIM_rxBufferTail;
            SDCard_SPIM_rxBufferOverflow = 0u;

            SDCard_SPIM_CLEAR_RX_FIFO;
            SDCard_SPIM_ClearRxInterruptSource(SDCard_SPIM_INTR_RX_ALL);

            #if (SDCard_SPIM_CHECK_UART_RTS_CONTROL_FLOW)
            {
                /* Enable RX Not Empty interrupt source to continue receiving
                * data into software buffer.
                */
                SDCard_SPIM_INTR_RX_MASK_REG |= SDCard_SPIM_INTR_RX_NOT_EMPTY;
            }
            #endif
            
            /* Release lock */
            SDCard_SPIM_EnableInt();
        }
        #else
        {
            SDCard_SPIM_CLEAR_RX_FIFO;
        }
        #endif
    }

#endif /* (SDCard_SPIM_RX_DIRECTION) */


#if(SDCard_SPIM_TX_DIRECTION)
    /*******************************************************************************
    * Function Name: SDCard_SPIM_SpiUartWriteTxData
    ****************************************************************************//**
    *
    *  Places a data entry into the transmit buffer to be sent at the next available
    *  bus time.
    *  This function is blocking and waits until there is space available to put the
    *  requested data in the transmit buffer.
    *
    *  \param txDataByte: the data to be transmitted.
    *   The amount of data bits to be transmitted depends on TX data bits selection 
    *   (the data bit counting starts from LSB of txDataByte).
    *
    * \globalvars
    *  SDCard_SPIM_txBufferHead - the start index to put data into the 
    *  software transmit buffer.
    *  SDCard_SPIM_txBufferTail - start index to get data from the software
    *  transmit buffer.
    *
    *******************************************************************************/
    void SDCard_SPIM_SpiUartWriteTxData(uint32 txData)
    {
    #if (SDCard_SPIM_INTERNAL_TX_SW_BUFFER_CONST)
        uint32 locHead;
    #endif /* (SDCard_SPIM_INTERNAL_TX_SW_BUFFER_CONST) */

        #if (SDCard_SPIM_CHECK_TX_SW_BUFFER)
        {
            /* Put data directly into the TX FIFO */
            if ((SDCard_SPIM_txBufferHead == SDCard_SPIM_txBufferTail) &&
                (SDCard_SPIM_SPI_UART_FIFO_SIZE != SDCard_SPIM_GET_TX_FIFO_ENTRIES))
            {
                /* TX software buffer is empty: put data directly in TX FIFO */
                SDCard_SPIM_TX_FIFO_WR_REG = txData;
            }
            /* Put data into TX software buffer */
            else
            {
                /* Head index to put data */
                locHead = (SDCard_SPIM_txBufferHead + 1u);

                /* Adjust TX software buffer index */
                if (SDCard_SPIM_TX_BUFFER_SIZE == locHead)
                {
                    locHead = 0u;
                }

                /* Wait for space in TX software buffer */
                while (locHead == SDCard_SPIM_txBufferTail)
                {
                }

                /* TX software buffer has at least one room */

                /* Clear old status of INTR_TX_NOT_FULL. It sets at the end of transfer when TX FIFO is empty. */
                SDCard_SPIM_ClearTxInterruptSource(SDCard_SPIM_INTR_TX_NOT_FULL);

                SDCard_SPIM_PutWordInTxBuffer(locHead, txData);

                SDCard_SPIM_txBufferHead = locHead;

                /* Check if TX Not Full is disabled in interrupt */
                if (0u == (SDCard_SPIM_INTR_TX_MASK_REG & SDCard_SPIM_INTR_TX_NOT_FULL))
                {
                    /* Enable TX Not Full interrupt source to transmit from software buffer */
                    SDCard_SPIM_INTR_TX_MASK_REG |= (uint32) SDCard_SPIM_INTR_TX_NOT_FULL;
                }
            }
        }
        #else
        {
            /* Wait until TX FIFO has space to put data element */
            while (SDCard_SPIM_SPI_UART_FIFO_SIZE == SDCard_SPIM_GET_TX_FIFO_ENTRIES)
            {
            }

            SDCard_SPIM_TX_FIFO_WR_REG = txData;
        }
        #endif
    }


    /*******************************************************************************
    * Function Name: SDCard_SPIM_SpiUartPutArray
    ****************************************************************************//**
    *
    *  Places an array of data into the transmit buffer to be sent.
    *  This function is blocking and waits until there is a space available to put
    *  all the requested data in the transmit buffer. The array size can be greater
    *  than transmit buffer size.
    *
    * \param wrBuf: pointer to an array of data to be placed in transmit buffer. 
    *  The width of the data to be transmitted depends on TX data width selection 
    *  (the data bit counting starts from LSB for each array element).
    * \param count: number of data elements to be placed in the transmit buffer.
    *
    * \globalvars
    *  SDCard_SPIM_txBufferHead - the start index to put data into the 
    *  software transmit buffer.
    *  SDCard_SPIM_txBufferTail - start index to get data from the software
    *  transmit buffer.
    *
    *******************************************************************************/
    void SDCard_SPIM_SpiUartPutArray(const uint8 wrBuf[], uint32 count)
    {
        uint32 i;

        for (i=0u; i < count; i++)
        {
            SDCard_SPIM_SpiUartWriteTxData((uint32) wrBuf[i]);
        }
    }


    /*******************************************************************************
    * Function Name: SDCard_SPIM_SpiUartGetTxBufferSize
    ****************************************************************************//**
    *
    *  Returns the number of elements currently in the transmit buffer.
    *   - TX software buffer is disabled: returns the number of used entries in
    *     TX FIFO.
    *   - TX software buffer is enabled: returns the number of elements currently
    *     used in the transmit buffer. This number does not include used entries in
    *     the TX FIFO. The transmit buffer size is zero until the TX FIFO is
    *     not full.
    *
    * \return
    *  Number of data elements ready to transmit.
    *
    * \globalvars
    *  SDCard_SPIM_txBufferHead - the start index to put data into the 
    *  software transmit buffer.
    *  SDCard_SPIM_txBufferTail - start index to get data from the software
    *  transmit buffer.
    *
    *******************************************************************************/
    uint32 SDCard_SPIM_SpiUartGetTxBufferSize(void)
    {
        uint32 size;
    #if (SDCard_SPIM_INTERNAL_TX_SW_BUFFER_CONST)
        uint32 locTail;
    #endif /* (SDCard_SPIM_INTERNAL_TX_SW_BUFFER_CONST) */

        #if (SDCard_SPIM_CHECK_TX_SW_BUFFER)
        {
            /* Get current Tail index */
            locTail = SDCard_SPIM_txBufferTail;

            if (SDCard_SPIM_txBufferHead >= locTail)
            {
                size = (SDCard_SPIM_txBufferHead - locTail);
            }
            else
            {
                size = (SDCard_SPIM_txBufferHead + (SDCard_SPIM_TX_BUFFER_SIZE - locTail));
            }
        }
        #else
        {
            size = SDCard_SPIM_GET_TX_FIFO_ENTRIES;
        }
        #endif

        return (size);
    }


    /*******************************************************************************
    * Function Name: SDCard_SPIM_SpiUartClearTxBuffer
    ****************************************************************************//**
    *
    *  Clears the transmit buffer and TX FIFO.
    *
    * \globalvars
    *  SDCard_SPIM_txBufferHead - the start index to put data into the 
    *  software transmit buffer.
    *  SDCard_SPIM_txBufferTail - start index to get data from the software
    *  transmit buffer.
    *
    *******************************************************************************/
    void SDCard_SPIM_SpiUartClearTxBuffer(void)
    {
        #if (SDCard_SPIM_CHECK_TX_SW_BUFFER)
        {
            /* Lock from component interruption */
            SDCard_SPIM_DisableInt();

            /* Flush TX software buffer */
            SDCard_SPIM_txBufferHead = SDCard_SPIM_txBufferTail;

            SDCard_SPIM_INTR_TX_MASK_REG &= (uint32) ~SDCard_SPIM_INTR_TX_NOT_FULL;
            SDCard_SPIM_CLEAR_TX_FIFO;
            SDCard_SPIM_ClearTxInterruptSource(SDCard_SPIM_INTR_TX_ALL);

            /* Release lock */
            SDCard_SPIM_EnableInt();
        }
        #else
        {
            SDCard_SPIM_CLEAR_TX_FIFO;
        }
        #endif
    }

#endif /* (SDCard_SPIM_TX_DIRECTION) */


/*******************************************************************************
* Function Name: SDCard_SPIM_SpiUartDisableIntRx
****************************************************************************//**
*
*  Disables the RX interrupt sources.
*
*  \return
*   Returns the RX interrupt sources enabled before the function call.
*
*******************************************************************************/
uint32 SDCard_SPIM_SpiUartDisableIntRx(void)
{
    uint32 intSource;

    intSource = SDCard_SPIM_GetRxInterruptMode();

    SDCard_SPIM_SetRxInterruptMode(SDCard_SPIM_NO_INTR_SOURCES);

    return (intSource);
}


/*******************************************************************************
* Function Name: SDCard_SPIM_SpiUartDisableIntTx
****************************************************************************//**
*
*  Disables TX interrupt sources.
*
*  \return
*   Returns TX interrupt sources enabled before function call.
*
*******************************************************************************/
uint32 SDCard_SPIM_SpiUartDisableIntTx(void)
{
    uint32 intSourceMask;

    intSourceMask = SDCard_SPIM_GetTxInterruptMode();

    SDCard_SPIM_SetTxInterruptMode(SDCard_SPIM_NO_INTR_SOURCES);

    return (intSourceMask);
}


#if(SDCard_SPIM_SCB_MODE_UNCONFIG_CONST_CFG)
    /*******************************************************************************
    * Function Name: SDCard_SPIM_PutWordInRxBuffer
    ****************************************************************************//**
    *
    *  Stores a byte/word into the RX buffer.
    *  Only available in the Unconfigured operation mode.
    *
    *  \param index:      index to store data byte/word in the RX buffer.
    *  \param rxDataByte: byte/word to store.
    *
    *******************************************************************************/
    void SDCard_SPIM_PutWordInRxBuffer(uint32 idx, uint32 rxDataByte)
    {
        /* Put data in buffer */
        if (SDCard_SPIM_ONE_BYTE_WIDTH == SDCard_SPIM_rxDataBits)
        {
            SDCard_SPIM_rxBuffer[idx] = ((uint8) rxDataByte);
        }
        else
        {
            SDCard_SPIM_rxBuffer[(uint32)(idx << 1u)]      = LO8(LO16(rxDataByte));
            SDCard_SPIM_rxBuffer[(uint32)(idx << 1u) + 1u] = HI8(LO16(rxDataByte));
        }
    }


    /*******************************************************************************
    * Function Name: SDCard_SPIM_GetWordFromRxBuffer
    ****************************************************************************//**
    *
    *  Reads byte/word from RX buffer.
    *  Only available in the Unconfigured operation mode.
    *
    *  \return
    *   Returns byte/word read from RX buffer.
    *
    *******************************************************************************/
    uint32 SDCard_SPIM_GetWordFromRxBuffer(uint32 idx)
    {
        uint32 value;

        if (SDCard_SPIM_ONE_BYTE_WIDTH == SDCard_SPIM_rxDataBits)
        {
            value = SDCard_SPIM_rxBuffer[idx];
        }
        else
        {
            value  = (uint32) SDCard_SPIM_rxBuffer[(uint32)(idx << 1u)];
            value |= (uint32) ((uint32)SDCard_SPIM_rxBuffer[(uint32)(idx << 1u) + 1u] << 8u);
        }

        return (value);
    }


    /*******************************************************************************
    * Function Name: SDCard_SPIM_PutWordInTxBuffer
    ****************************************************************************//**
    *
    *  Stores byte/word into the TX buffer.
    *  Only available in the Unconfigured operation mode.
    *
    *  \param idx:        index to store data byte/word in the TX buffer.
    *  \param txDataByte: byte/word to store.
    *
    *******************************************************************************/
    void SDCard_SPIM_PutWordInTxBuffer(uint32 idx, uint32 txDataByte)
    {
        /* Put data in buffer */
        if (SDCard_SPIM_ONE_BYTE_WIDTH == SDCard_SPIM_txDataBits)
        {
            SDCard_SPIM_txBuffer[idx] = ((uint8) txDataByte);
        }
        else
        {
            SDCard_SPIM_txBuffer[(uint32)(idx << 1u)]      = LO8(LO16(txDataByte));
            SDCard_SPIM_txBuffer[(uint32)(idx << 1u) + 1u] = HI8(LO16(txDataByte));
        }
    }


    /*******************************************************************************
    * Function Name: SDCard_SPIM_GetWordFromTxBuffer
    ****************************************************************************//**
    *
    *  Reads byte/word from the TX buffer.
    *  Only available in the Unconfigured operation mode.
    *
    *  \param idx: index to get data byte/word from the TX buffer.
    *
    *  \return
    *   Returns byte/word read from the TX buffer.
    *
    *******************************************************************************/
    uint32 SDCard_SPIM_GetWordFromTxBuffer(uint32 idx)
    {
        uint32 value;

        if (SDCard_SPIM_ONE_BYTE_WIDTH == SDCard_SPIM_txDataBits)
        {
            value = (uint32) SDCard_SPIM_txBuffer[idx];
        }
        else
        {
            value  = (uint32) SDCard_SPIM_txBuffer[(uint32)(idx << 1u)];
            value |= (uint32) ((uint32) SDCard_SPIM_txBuffer[(uint32)(idx << 1u) + 1u] << 8u);
        }

        return (value);
    }

#endif /* (SDCard_SPIM_SCB_MODE_UNCONFIG_CONST_CFG) */


/* [] END OF FILE */
