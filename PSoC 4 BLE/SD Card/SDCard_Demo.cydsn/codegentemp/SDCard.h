//*****************************************************************************
//*****************************************************************************
//  FILENAME: SDCard.h
//  Version 0.0
//  
//
//  DESCRIPTION: SDCard User Module C Language header file for the 
//               PSoC family of devices.  This header file contains only
//               the functions that are should be used by the programmer.
//
//-----------------------------------------------------------------------------
//  The original source for this user module was purchased from
//  Efficient Computer Systems, LLC.
//-----------------------------------------------------------------------------
//      
//  Copyright 2003-2006   Efficient Computer Systems, LLC
//  Licensed only for use on any Cypress PSOC Mixed-Signal Controllers.
//  All rights reserved
//      
//-----------------------------------------------------------------------------
//   
//         Created 12-04-03   By: Lee W. Morin and Herb Winters
//  1.00   Release 04-24-06   By: Lee Morin, Herb Winters, Eric Curtis   
//   
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//  Copyright (c) Cypress Semiconductor 2013-14. All Rights Reserved.
//*****************************************************************************
//*****************************************************************************
#if !defined(SDCard_HEADER)
#define SDCard_HEADER

#include "SDCard_SPI.h"
#include "SDCard_FS.h"

#include "SDCard_SD_CD.h"
#include "SDCard_SD_CS.h"
#include "SDCard_SD_PWR.h"
#include "SDCard_SD_WP.h"
#include "CyLib.h"

//---------------------------------------------------------------------------------------------
//                                   Function Prototypes
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
//                                Basic Read Write commands
//---------------------------------------------------------------------------------------------

void  SDCard_Start(void);                       // Starts SD card module  
void  SDCard_Stop(void);                        // Stops  SD card module  
uchar SDCard_InitCard(void);                    // Runs all commands to init card for use  
uchar SDCard_fseek(uchar Fptr, ulong Offset);   // Seeks a specific offet into file  
uchar SDCard_fgetc(uchar Fptr);                 // Returns the next character from the file pointed to 
uchar SDCard_fbgetc(uchar Fptr);                // Returns the next buffered character from the file pointed to 
void  SDCard_clearerr(uchar Fptr);              // Clears the error flags for the file    
uchar SDCard_ferror(uchar Fptr);                // Returns non-zero for file error, zero if no error  
ulong SDCard_ftell(uchar Fptr);                 // Return the current postion within the file  
uchar SDCard_ReadSect(ulong address);           // Read a sector   

#ifdef SDCard_ENABLE_PRESENT
uchar SDCard_Present(void);                     // Returns a '1' if a card is present in the socket, '0' if not  
#endif

#ifdef SDCard_ENABLE_WPROTECT
uchar SDCard_WriteProtect(void);                // Returns a '1' if the card is write protected using ther slide switch, '0' if not
#endif

#ifdef ENABLE_WRITE
uchar SDCard_WriteSect(ulong address);            // Write a sector  
uchar SDCard_fputc(uchar Data, uchar Fptr);       // Write a character to a file  
uchar SDCard_fputs(char *str, uchar Fptr);        // Writes a null-terminated string to a file. 
uchar SDCard_fputcs(const char *str, uchar Fptr); // Writes a null-terminated const string to a file. 
uchar SDCard_fputBuff(uchar *buff, uint count, uchar Fptr);
uchar SDCard_fputcBuff(const uchar *buff, uint count, uchar Fptr);
void  SDCard_fflush(uchar Fptr);                  // Flush the write buffers (to the file and update dir values in FILESYSTEM mode)  
#endif

//---------------------------
// Top level file functions
//---------------------------
#ifdef ENABLE_FILESYSTEM
uchar   SDCard_fclose(uchar Fptr);                // Closes a file pointer  
uchar   SDCard_fopen(uchar Filename[], const uchar Mode[]);   // Opens a file as type and returns the file pointer 
uchar * SDCard_GetFilename(uint Entry);           // Returns filename for directory entry specified.  
uint    SDCard_GetFileCount(void);                // Returns the number of valid files in the root directory  
ulong   SDCard_GetFileSize(uchar Fptr);           // Returns the file size of the file pointed to  
uchar   SDCard_feof(uchar Fptr);                  // Returns non-zero for EOF, zero if no EOF  

//---------------------------
// Top level writing functions
//---------------------------
#ifdef ENABLE_WRITE
uchar SDCard_Remove(uchar * Filename);         // Delete the file indicated by Filename  
uchar SDCard_Rename(uchar * OldFilename, uchar * NewFilename);  // Rename a file  
uchar SDCard_Copy(uchar * OldFilename, uchar * NewFilename);    // Copy a file  
#endif  // End ENABLE_WRITE
#endif  // End ENABLE_FILESYSTEM


//------------- cardInfo defines ---------------

#define SDCard_FORMAT_FAT12    0x10
#define SDCard_FORMAT_FAT16    0x40
#define SDCard_FORMAT_FAT16a   0x40
#define SDCard_FORMAT_FAT16b   0x60
#define SDCard_FORMAT_FAT32    0xB0
#define SDCard_FORMAT_VALID    0xE0
#define SDCard_FORMAT_MASK     0xF0

#define SDCard_TYPE_NONE       0x00
#define SDCard_TYPE_MMC        0x01
#define SDCard_TYPE_SD         0x02
#define SDCard_TYPE_MASK       0x03

#endif
