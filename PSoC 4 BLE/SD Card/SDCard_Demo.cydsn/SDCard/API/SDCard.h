//*****************************************************************************
//*****************************************************************************
//  FILENAME: `$INSTANCE_NAME`.h
//  Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
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
#if !defined(`$INSTANCE_NAME`_HEADER)
#define `$INSTANCE_NAME`_HEADER

#include "`$INSTANCE_NAME`_SPI.h"
#include "`$INSTANCE_NAME`_FS.h"

#include "`$INSTANCE_NAME`_SD_CD.h"
#include "`$INSTANCE_NAME`_SD_CS.h"
#include "`$INSTANCE_NAME`_SD_PWR.h"
#include "`$INSTANCE_NAME`_SD_WP.h"
#include "CyLib.h"

//---------------------------------------------------------------------------------------------
//                                   Function Prototypes
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
//                                Basic Read Write commands
//---------------------------------------------------------------------------------------------

void  `$INSTANCE_NAME`_Start(void);                       // Starts SD card module  
void  `$INSTANCE_NAME`_Stop(void);                        // Stops  SD card module  
uchar `$INSTANCE_NAME`_InitCard(void);                    // Runs all commands to init card for use  
uchar `$INSTANCE_NAME`_fseek(uchar Fptr, ulong Offset);   // Seeks a specific offet into file  
uchar `$INSTANCE_NAME`_fgetc(uchar Fptr);                 // Returns the next character from the file pointed to 
uchar `$INSTANCE_NAME`_fbgetc(uchar Fptr);                // Returns the next buffered character from the file pointed to 
void  `$INSTANCE_NAME`_clearerr(uchar Fptr);              // Clears the error flags for the file    
uchar `$INSTANCE_NAME`_ferror(uchar Fptr);                // Returns non-zero for file error, zero if no error  
ulong `$INSTANCE_NAME`_ftell(uchar Fptr);                 // Return the current postion within the file  
uchar `$INSTANCE_NAME`_ReadSect(ulong address);           // Read a sector   

#ifdef `$INSTANCE_NAME`_ENABLE_PRESENT
uchar `$INSTANCE_NAME`_Present(void);                     // Returns a '1' if a card is present in the socket, '0' if not  
#endif

#ifdef `$INSTANCE_NAME`_ENABLE_WPROTECT
uchar `$INSTANCE_NAME`_WriteProtect(void);                // Returns a '1' if the card is write protected using ther slide switch, '0' if not
#endif

#ifdef ENABLE_WRITE
uchar `$INSTANCE_NAME`_WriteSect(ulong address);            // Write a sector  
uchar `$INSTANCE_NAME`_fputc(uchar Data, uchar Fptr);       // Write a character to a file  
uchar `$INSTANCE_NAME`_fputs(char *str, uchar Fptr);        // Writes a null-terminated string to a file. 
uchar `$INSTANCE_NAME`_fputcs(const char *str, uchar Fptr); // Writes a null-terminated const string to a file. 
uchar `$INSTANCE_NAME`_fputBuff(uchar *buff, uint count, uchar Fptr);
uchar `$INSTANCE_NAME`_fputcBuff(const uchar *buff, uint count, uchar Fptr);
void  `$INSTANCE_NAME`_fflush(uchar Fptr);                  // Flush the write buffers (to the file and update dir values in FILESYSTEM mode)  
#endif

//---------------------------
// Top level file functions
//---------------------------
#ifdef ENABLE_FILESYSTEM
uchar   `$INSTANCE_NAME`_fclose(uchar Fptr);                // Closes a file pointer  
uchar   `$INSTANCE_NAME`_fopen(uchar Filename[], const uchar Mode[]);   // Opens a file as type and returns the file pointer 
uchar * `$INSTANCE_NAME`_GetFilename(uint Entry);           // Returns filename for directory entry specified.  
uint    `$INSTANCE_NAME`_GetFileCount(void);                // Returns the number of valid files in the root directory  
ulong   `$INSTANCE_NAME`_GetFileSize(uchar Fptr);           // Returns the file size of the file pointed to  
uchar   `$INSTANCE_NAME`_feof(uchar Fptr);                  // Returns non-zero for EOF, zero if no EOF  

//---------------------------
// Top level writing functions
//---------------------------
#ifdef ENABLE_WRITE
uchar `$INSTANCE_NAME`_Remove(uchar * Filename);         // Delete the file indicated by Filename  
uchar `$INSTANCE_NAME`_Rename(uchar * OldFilename, uchar * NewFilename);  // Rename a file  
uchar `$INSTANCE_NAME`_Copy(uchar * OldFilename, uchar * NewFilename);    // Copy a file  
#endif  // End ENABLE_WRITE
#endif  // End ENABLE_FILESYSTEM


//------------- cardInfo defines ---------------

#define `$INSTANCE_NAME`_FORMAT_FAT12    0x10
#define `$INSTANCE_NAME`_FORMAT_FAT16    0x40
#define `$INSTANCE_NAME`_FORMAT_FAT16a   0x40
#define `$INSTANCE_NAME`_FORMAT_FAT16b   0x60
#define `$INSTANCE_NAME`_FORMAT_FAT32    0xB0
#define `$INSTANCE_NAME`_FORMAT_VALID    0xE0
#define `$INSTANCE_NAME`_FORMAT_MASK     0xF0

#define `$INSTANCE_NAME`_TYPE_NONE       0x00
#define `$INSTANCE_NAME`_TYPE_MMC        0x01
#define `$INSTANCE_NAME`_TYPE_SD         0x02
#define `$INSTANCE_NAME`_TYPE_MASK       0x03

#endif
