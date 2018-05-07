//*****************************************************************************
//*****************************************************************************
//  FILENAME: SDCard_SPI.h
//  Version 0.0
//  
//
//  DESCRIPTION:
//    SDCard User Module low level SPI header file.
//-----------------------------------------------------------------------------
//      Copyright (c) Cypress Semiconductor 2013-14. All Rights Reserved.
//*****************************************************************************
//*****************************************************************************
#ifndef SDCard_SPI_HEADER
#define SDCard_SPI_HEADER

#include "cytypes.h"
#include "CyLib.h"

#include "SDCard_SD_CD.h"
#include "SDCard_SD_CS.h"
#include "SDCard_SD_PWR.h"
#include "SDCard_SD_WP.h"
#include "SDCard_SPIM.h"

//-------------------------------------------------
// Prototypes of the SDCard API.
//-------------------------------------------------
extern void    SDCard_InitHdwr(uint8  bConfiguration);
extern void    SDCard_UnInitHdwr(void);
extern void    SDCard_Select(uint8  bEnable);
extern void    SDCard_SendTxData(uint8  bTxData);
extern uint8   SDCard_bReadRxData(void);
extern uint8   SDCard_bReadStatus(void);
extern void    SDCard_WriteBuff256(char * sRamBuff);
extern void    SDCard_ReadBuff256(char * sRamBuff);
extern void    SDCard_WriteBuff(char * sRamBuff, uint8  bCnt);
extern void    SDCard_ReadBuff(char * sRamBuff, uint8  bCnt);


//-------------------------------------------------
// Constants for SDCard API's.
//-------------------------------------------------

#define SDCard_ENABLE   1
#define SDCard_DISABLE  0

//*******************************
// SPI Configuration definitions
//*******************************
#define  SDCard_SPIM_MODE_0            0x00      // MODE 0 - Leading edge latches data - pos clock
#define  SDCard_SPIM_MSB_FIRST         0x00      // MSB bit transmitted/received first

//********************************
// SPI Status register masks
//********************************
#define  SDCard_SPIM_RX_OVERRUN_ERROR  0x20      // Overrun error in received data
#define  SDCard_SPIM_TX_BUFFER_EMPTY   0x02      // TX Buffer register is ready for next data byte
#define  SDCard_SPIM_RX_BUFFER_FULL    0x08      // RX Buffer register has received current data
#define  SDCard_SPIM_SPI_COMPLETE      0x01      // SPI Tx/Rx cycle has completed

#endif
// end of file SDCard_SPI.h
