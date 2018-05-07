//*****************************************************************************
//*****************************************************************************
//  FILENAME: `$INSTANCE_NAME`_SPI.h
//  Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
//  
//
//  DESCRIPTION:
//    SDCard User Module low level SPI header file.
//-----------------------------------------------------------------------------
//      Copyright (c) Cypress Semiconductor 2013-14. All Rights Reserved.
//*****************************************************************************
//*****************************************************************************
#ifndef `$INSTANCE_NAME`_SPI_HEADER
#define `$INSTANCE_NAME`_SPI_HEADER

#include "cytypes.h"
#include "CyLib.h"

#include "`$INSTANCE_NAME`_SD_CD.h"
#include "`$INSTANCE_NAME`_SD_CS.h"
#include "`$INSTANCE_NAME`_SD_PWR.h"
#include "`$INSTANCE_NAME`_SD_WP.h"
#include "`$INSTANCE_NAME`_SPIM.h"

//-------------------------------------------------
// Prototypes of the SDCard API.
//-------------------------------------------------
extern void    `$INSTANCE_NAME`_InitHdwr(uint8  bConfiguration);
extern void    `$INSTANCE_NAME`_UnInitHdwr(void);
extern void    `$INSTANCE_NAME`_Select(uint8  bEnable);
extern void    `$INSTANCE_NAME`_SendTxData(uint8  bTxData);
extern uint8   `$INSTANCE_NAME`_bReadRxData(void);
extern uint8   `$INSTANCE_NAME`_bReadStatus(void);
extern void    `$INSTANCE_NAME`_WriteBuff256(char * sRamBuff);
extern void    `$INSTANCE_NAME`_ReadBuff256(char * sRamBuff);
extern void    `$INSTANCE_NAME`_WriteBuff(char * sRamBuff, uint8  bCnt);
extern void    `$INSTANCE_NAME`_ReadBuff(char * sRamBuff, uint8  bCnt);


//-------------------------------------------------
// Constants for SDCard API's.
//-------------------------------------------------

#define `$INSTANCE_NAME`_ENABLE   1
#define `$INSTANCE_NAME`_DISABLE  0

//*******************************
// SPI Configuration definitions
//*******************************
#define  `$INSTANCE_NAME`_SPIM_MODE_0            0x00      // MODE 0 - Leading edge latches data - pos clock
#define  `$INSTANCE_NAME`_SPIM_MSB_FIRST         0x00      // MSB bit transmitted/received first

//********************************
// SPI Status register masks
//********************************
#define  `$INSTANCE_NAME`_SPIM_RX_OVERRUN_ERROR  0x20      // Overrun error in received data
#define  `$INSTANCE_NAME`_SPIM_TX_BUFFER_EMPTY   0x02      // TX Buffer register is ready for next data byte
#define  `$INSTANCE_NAME`_SPIM_RX_BUFFER_FULL    0x08      // RX Buffer register has received current data
#define  `$INSTANCE_NAME`_SPIM_SPI_COMPLETE      0x01      // SPI Tx/Rx cycle has completed

#endif
// end of file `$INSTANCE_NAME`_SPI.h
