/****************************************************************************
*****************************************************************************
  FILENAME: `$INSTANCE_NAME`_SPI.c
  Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
  

  DESCRIPTION: SDCard User Module software implementation file
               for the low level SPI hardware.

  NOTE: User Module APIs conform to the fastcall16 convention for marshalling
        arguments and observe the associated "Registers are volatile" policy.
        This means it is the caller's responsibility to preserve any values
        in the X and A registers that are still needed after the API functions
        returns. For Large Memory Model devices it is also the caller's 
        responsibility to perserve any value in the CUR_PP, IDX_PP, MVR_PP and 
        MVW_PP registers. Even though some of these registers may not be modified
        now, there is no guarantee that will remain the case in future releases.
-----------------------------------------------------------------------------
  Copyright (c) Cypress Semiconductor 2013-14. All Rights Reserved.
*****************************************************************************
*****************************************************************************
*/

#include "device.h"
#include "`$INSTANCE_NAME`_SPI.h"

/*
;-----------------------------------------------------------------------------
;  FUNCTION NAME: `$INSTANCE_NAME`_InitHdwr
;
;  DESCRIPTION:
;     Sets the start bit, SPI mode, and LSB/MSB first configuration of the SPIM
;     user module.
;
;     Transmission will begin transmitting when a byte is written into the TX buffer
;     using the `$INSTANCE_NAME`_SendTxData function.
;
;-----------------------------------------------------------------------------
;
;  ARGUMENTS:
;     uint8 bConfiguration - Consists of SPI Mode and LSB/MSB first bit.
;           Use defined masks - masks can be OR'd together.
;     PASSED in Accumulator.
;
;  RETURNS:  none
;
;  SIDE EFFECTS: 
;    The A and X registers may be modified by this or future implementations
;    of this function.  The same is true for all RAM page pointer registers in
;    the Large Memory Model.  When necessary, it is the calling function's
;    responsibility to perserve their values across calls to fastcall16 
;    functions.
;
;  THEORY of OPERATION or PROCEDURE:
;     1) Set all Slave Select outputs high
;     2) Set the specified SPI configuration bits in the Control register.
;
*/
void  `$INSTANCE_NAME`_InitHdwr(uint8 bConfiguration)
{
	`$INSTANCE_NAME`_SD_CS_Write(0);
	`$INSTANCE_NAME`_SD_Clock_SetDividerRegister(0, 1);
	`$INSTANCE_NAME`_SPIM_Start();
	`$INSTANCE_NAME`_SPIM_SpiUartClearTxBuffer();
	`$INSTANCE_NAME`_SPIM_SpiUartClearRxBuffer();
}

/*
;-----------------------------------------------------------------------------
;  FUNCTION NAME: `$INSTANCE_NAME`_Stop
;
;  DESCRIPTION:
;     Disables SPIM operation.
;
;-----------------------------------------------------------------------------
;
;  ARGUMENTS:  none
;
;  RETURNS:  none
;
;  SIDE EFFECTS: 
;    The A and X registers may be modified by this or future implementations
;    of this function.  The same is true for all RAM page pointer registers in
;    the Large Memory Model.  When necessary, it is the calling function's
;    responsibility to perserve their values across calls to fastcall16 
;    functions.
;
;  THEORY of OPERATION or PROCEDURE:
;     Clear the start bit in the Control register.
;
*/
void  `$INSTANCE_NAME`_UnInitHdwr(void)
{
	`$INSTANCE_NAME`_SD_CS_Write(1);
	`$INSTANCE_NAME`_Select(0);
	`$INSTANCE_NAME`_SPIM_Stop();
}

/*
;-----------------------------------------------------------------------------
;  FUNCTION NAME: void `$INSTANCE_NAME`_Select(uint8 bEnable)
;
;  DESCRIPTION:
;     Enable or disable card select signal.
;
;-----------------------------------------------------------------------------
;
;  ARGUMENTS:
;     uint8 bEnable => 0 Disable Card
;                  => 1 Enable Card
;     PASSED in Accumulator.
;
;  RETURNS:  none
;
;  SIDE EFFECTS: 
;    The A and X registers may be modified by this or future implementations
;    of this function.  The same is true for all RAM page pointer registers in
;    the Large Memory Model.  When necessary, it is the calling function's
;    responsibility to perserve their values across calls to fastcall16 
;    functions.
;
;  THEORY of OPERATION or PROCEDURE:
;
;-----------------------------------------------------------------------------
*/

void  `$INSTANCE_NAME`_Select(uint8 bEnable)
{
	if(bEnable)
		{
		`$INSTANCE_NAME`_SD_CS_Write(0);
		`$INSTANCE_NAME`_SD_PWR_Write(1);
		}
	else
		{
		`$INSTANCE_NAME`_SD_CS_Write(1);
		`$INSTANCE_NAME`_SD_PWR_Write(0);
		}
}

/*
;-----------------------------------------------------------------------------
;  FUNCTION NAME: `$INSTANCE_NAME`_SendTxData
;
;  DESCRIPTION:
;     Initiates an SPI data transfer.
;
;-----------------------------------------------------------------------------
;
;  ARGUMENTS:
;     uint8  bTxData - data to transmit.
;        Passed in Accumulator.
;
;  RETURNS:  none
;
;  SIDE EFFECTS: 
;    The A and X registers may be modified by this or future implementations
;    of this function.  The same is true for all RAM page pointer registers in
;    the Large Memory Model.  When necessary, it is the calling function's
;    responsibility to perserve their values across calls to fastcall16 
;    functions.
;
;  THEORY of OPERATION or PROCEDURE:
;     Writes data to the TX buffer register.
;
*/
void  `$INSTANCE_NAME`_SendTxData(uint8 bTxData)
{
	`$INSTANCE_NAME`_SPIM_SpiUartWriteTxData((uint32) bTxData);
}

/*
;-----------------------------------------------------------------------------
;  FUNCTION NAME: `$INSTANCE_NAME`_WriteBuff256(char * sRamBuff)
;
;  DESCRIPTION:
;     Writes a 256 byte buffer to the SPI port
;
;-----------------------------------------------------------------------------
;
;  ARGUMENTS:
;     A:X  Pointer to String
;          A contains MSB of string address
;          X contains LSB of string address
;
;
;  RETURNS:  none
;
;  SIDE EFFECTS: 
;    The A and X registers may be modified by this or future implementations
;    of this function.  The same is true for all RAM page pointer registers in
;    the Large Memory Model.  When necessary, it is the calling function's
;    responsibility to perserve their values across calls to fastcall16 
;    functions.
;
;
;  THEORY of OPERATION or PROCEDURE:
;     Writes data to the TX buffer register.
;
*/
void  `$INSTANCE_NAME`_WriteBuff256(char * sRamBuff)
{
	uint32 i, temp;
	for(i=0; i<256; i++)
	{
		`$INSTANCE_NAME`_SPIM_SpiUartWriteTxData((uint32)sRamBuff[i]);
		while(! (`$INSTANCE_NAME`_SPIM_SpiUartGetRxBufferSize() > 0));
		temp = `$INSTANCE_NAME`_SPIM_SpiUartReadRxData();
	}
}

/*
;-----------------------------------------------------------------------------
;  FUNCTION NAME: `$INSTANCE_NAME`_WriteBuff(uint8 * pBuff, uint8 bCnt)
;
;  DESCRIPTION:
;     Writes n bytes to the SPI port
;
;-----------------------------------------------------------------------------
;
;  ARGUMENTS:
;  FASTCALL16 ARGUMENTS:
;
;   [SP-3] => pBuff LSB Address.
;   [SP-4] => pBuff MSB Address.
;   [SP-5] => Buffer length to write.
;
;  RETURNS:  none
;
;  SIDE EFFECTS: 
;    The A and X registers may be modified by this or future implementations
;    of this function.  The same is true for all RAM page pointer registers in
;    the Large Memory Model.  When necessary, it is the calling function's
;    responsibility to perserve their values across calls to fastcall16 
;    functions.
;
;
;  THEORY of OPERATION or PROCEDURE:
;     Writes n data byte to the TX buffer register.
;
*/
void  `$INSTANCE_NAME`_WriteBuff(char * sRamBuff, uint8 bCnt)
{
	uint32 i, temp;
	for(i=0; i<bCnt; i++)
	{
		`$INSTANCE_NAME`_SPIM_SpiUartWriteTxData((uint32)sRamBuff[i]);
		while(! (`$INSTANCE_NAME`_SPIM_SpiUartGetRxBufferSize() > 0));
		temp = `$INSTANCE_NAME`_SPIM_SpiUartReadRxData();
	}
}

/*
;-----------------------------------------------------------------------------
;  FUNCTION NAME: `$INSTANCE_NAME`_ReadBuff(uint8 * pBuff, uint8 bCnt)
;
;  DESCRIPTION:
;     Reads n bytes from the SPI port
;
;-----------------------------------------------------------------------------
;
;  ARGUMENTS:
;  FASTCALL16 ARGUMENTS:
;
;   [SP-3] => pBuff LSB Address.
;   [SP-4] => pBuff MSB Address.
;   [SP-5] => Buffer length to read (bCnt).
;
;
;  RETURNS:  none
;
;  SIDE EFFECTS: 
;    The A and X registers may be modified by this or future implementations
;    of this function.  The same is true for all RAM page pointer registers in
;    the Large Memory Model.  When necessary, it is the calling function's
;    responsibility to perserve their values across calls to fastcall16 
;    functions.
;
;
;  THEORY of OPERATION or PROCEDURE:
;     Writes data to the TX buffer register.
;
*/
void  `$INSTANCE_NAME`_ReadBuff(char * sRamBuff, uint8 bCnt)
{
	uint32 i;
	for(i=0; i<bCnt; i++)
	{
		`$INSTANCE_NAME`_SPIM_SpiUartWriteTxData(0xFF);
		while(! (`$INSTANCE_NAME`_SPIM_SpiUartGetRxBufferSize() > 0));
		sRamBuff[i] = (char) `$INSTANCE_NAME`_SPIM_SpiUartReadRxData();
	}
}

/*
;-----------------------------------------------------------------------------
;  FUNCTION NAME: `$INSTANCE_NAME`_ReadBuff256(char * sRamBuff)
;
;  DESCRIPTION:
;     Reads 256 bytes into a buffer from the SPI port
;
;-----------------------------------------------------------------------------
;
;  ARGUMENTS:
;     A:X  Pointer to String
;          A contains MSB of string address
;          X contains LSB of string address
;
;
;  RETURNS:  none
;
;  SIDE EFFECTS: 
;    The A and X registers may be modified by this or future implementations
;    of this function.  The same is true for all RAM page pointer registers in
;    the Large Memory Model.  When necessary, it is the calling function's
;    responsibility to perserve their values across calls to fastcall16 
;    functions.
;
;
;  THEORY of OPERATION or PROCEDURE:
;     Reads data from the TX buffer register.
;
*/
void  `$INSTANCE_NAME`_ReadBuff256(char * sRamBuff)
{
	uint32 i;
	for(i=0; i<256; i++)
	{
		`$INSTANCE_NAME`_SPIM_SpiUartWriteTxData(0xFF);
		while(! (`$INSTANCE_NAME`_SPIM_SpiUartGetRxBufferSize() > 0));
		sRamBuff[i] = (char) `$INSTANCE_NAME`_SPIM_SpiUartReadRxData();
	}
}

/*
;-----------------------------------------------------------------------------
;  FUNCTION NAME: `$INSTANCE_NAME`_bReadRxData
;
;  DESCRIPTION:
;     Reads the RX buffer register.  Should check the status regiser to make
;     sure data is valid.
;
;-----------------------------------------------------------------------------
;
;  ARGUMENTS:  none
;
;  RETURNS:
;     bRxData - returned in A.
;
;  SIDE EFFECTS: 
;    The A and X registers may be modified by this or future implementations
;    of this function.  The same is true for all RAM page pointer registers in
;    the Large Memory Model.  When necessary, it is the calling function's
;    responsibility to perserve their values across calls to fastcall16 
;    functions.
;
;  THEORY of OPERATION or PROCEDURE:
;
*/
uint8  `$INSTANCE_NAME`_bReadRxData(void)
{
	uint8 buf;
	buf = (uint8) `$INSTANCE_NAME`_SPIM_SpiUartReadRxData();
	return(buf);
}

/*
;-----------------------------------------------------------------------------
;  FUNCTION NAME: `$INSTANCE_NAME`_ReadStatus
;
;  DESCRIPTION:
;     Reads the SPIM Status bits in the Control/Status register.
;
;-----------------------------------------------------------------------------
;
;  ARGUMENTS:  none
;
;  RETURNS:
;     uint8  bStatus - transmit status data.  Use the defined bit masks.
;        Returned in Accumulator.
;
;  SIDE EFFECTS: 
;    The A and X registers may be modified by this or future implementations
;    of this function.  The same is true for all RAM page pointer registers in
;    the Large Memory Model.  When necessary, it is the calling function's
;    responsibility to perserve their values across calls to fastcall16 
;    functions.
;
;  THEORY of OPERATION or PROCEDURE:
;     Read the status and control register.
;
*/
uint8  `$INSTANCE_NAME`_bReadStatus(void)
{
	uint8 stat;
	stat = `$INSTANCE_NAME`_SPIM_SpiUartGetRxBufferSize();
	return(stat);
}

// End of File `$INSTANCE_NAME`_SPI.c
