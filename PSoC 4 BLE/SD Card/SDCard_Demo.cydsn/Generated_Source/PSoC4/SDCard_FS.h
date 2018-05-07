//*****************************************************************************
//*****************************************************************************
//  FILENAME: SDCard_FS.h
//  Version 0.0
//  
//
//  DESCRIPTION: SDCard User Module C Language header file for the 
//               PSoC family of devices.  This header file contains the low
//               level file function calls not intended to be used by the
//               programmer.  See the SDCard.h file for a list of
//               user APIs.
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

#ifndef SDCard_FS_HEADER
#define SDCard_FS_HEADER

#include "SDCard_SD_CD.h"
#include "SDCard_SD_CS.h"
#include "SDCard_SD_PWR.h"
#include "SDCard_SD_WP.h"
#include "cytypes.h"
#include "CyLib.h"


//---------------------------------
// Type #defines used for this UM
//---------------------------------
#define uchar   unsigned char   
#define uint    unsigned int
#define ulong   uint32


//--------------------------------------------------------------
// Optional card Socket functions
//  NOTE: selected by Module Parameters "SDCard_PRESENT" and 
//        "SDCard_WPROT" being set to any PSOC Pin. Disabled if 
//          Parameter set to NONE
//--------------------------------------------------------------
#define SDCard_ENABLE_PRESENT    1
#define SDCard_ENABLE_WPROTECT   1

#define SDCard_CSPort    SDCard_SD_CS_Read()
#define SDCard_CSMask    1

#if ( SDCard_ENABLE_PRESENT == 1 )
#define SDCard_CPPort    SDCard_SD_CD_Read()
#define SDCard_CPMask    1
#endif

#if ( SDCard_ENABLE_WPROTECT == 1 )
#define SDCard_WPPort    SDCard_SD_WP_Read()
#define SDCard_WPMask    1
#endif

#define SDCard_ENABLE  1
#define SDCard_DISABLE 0


//==============================================================
//  Define Resource usage configuration option
// Only one of these options should be use at a time 
//==============================================================
#define SDCard_Build_Config_Level   0
#define ENABLE_FULL_FILE_SYSTEM         0
#define ENABLE_STANDARD_FILE_SYSTEM     1
#define ENABLE_BASIC_FILE_SYSTEM        2
#define ENABLE_READONLY_FILE_SYSTEM     3
#define ENABLE_BASIC_READWRITE          4
#define ENABLE_CUSTOM_CONFIGURATION     5

//==============================================================
// This section works with the "Resource usage configuration"
// parameter to enable the correct options.
//==============================================================
#if  ( ENABLE_CUSTOM_CONFIGURATION == SDCard_Build_Config_Level )
// Not used. Will not work.
// #include "SDCard_Config.h"
#else
// Maximum number of files that can be open at one time
#define SDCard_MAXFILES   2
#endif

//==============================================================
// The following list of #defines can be use to create Custom
// configurations Just set the "Resource usage configuration"
// parameter to "CUSTOM" then add a header file named SDCard_Config.h
// then place any of the #defines below to create you configuration
//==============================================================
//--------------------------------------------------------------
// File systems to support if ENABLE_FILESYSTEM defined 
// Note: If file systems enabled - FAT16 is enabled
//--------------------------------------------------------------
//#define ENABLE_FILESYSTEM
//#define ENABLE_FAT32
//--------------------------------------------------------------
// Write supported
//#define ENABLE_WRITE
//--------------------------------------------------------------
// Enable subdirectory feautures (Not yet developed)
//#define ENABLE_SUBDIR
//--------------------------------------------------------------
// High level File commands
//#define ENABLE_FILECOPY
//#define ENABLE_FILERENAME
//#define ENABLE_FILEREMOVE
//--------------------------------------------------------------
// DEBUG FUNCTIONS
//#define ENABLE_DEBUGFUNCT
//==============================================================



#if ( ENABLE_FULL_FILE_SYSTEM == SDCard_Build_Config_Level )
#define ENABLE_FILESYSTEM
#define ENABLE_FAT32
#define ENABLE_WRITE   
//#define ENABLE_SUBDIR       // Not yet developed 
#define ENABLE_FILECOPY
#define ENABLE_FILERENAME
#define ENABLE_FILEREMOVE
//#define ENABLE_DEBUGFUNCT   // Only NEED for debug
#endif

#if ( ENABLE_STANDARD_FILE_SYSTEM == SDCard_Build_Config_Level )
#define ENABLE_FILESYSTEM
//#define ENABLE_FAT32
#define ENABLE_WRITE    
//#define ENABLE_SUBDIR       // Not yet developed 
#define ENABLE_FILECOPY
#define ENABLE_FILERENAME
#define ENABLE_FILEREMOVE
//#define ENABLE_DEBUGFUNCT   // Only NEED for debug
#endif

#if ( ENABLE_BASIC_FILE_SYSTEM == SDCard_Build_Config_Level )
#define ENABLE_FILESYSTEM
//#define ENABLE_FAT32
#define ENABLE_WRITE    
//#define ENABLE_SUBDIR      // Not yet developed 
//#define ENABLE_FILECOPY
//#define ENABLE_FILERENAME
#define ENABLE_FILEREMOVE
//#define ENABLE_DEBUGFUNCT   // Only NEED for debug
#endif

#if ( ENABLE_READONLY_FILE_SYSTEM == SDCard_Build_Config_Level )
#define ENABLE_FILESYSTEM
//#define ENABLE_FAT32
//#define ENABLE_WRITE    
//#define ENABLE_SUBDIR       // Not yet developed 
//#define ENABLE_FILECOPY
//#define ENABLE_FILERENAME
//#define ENABLE_FILEREMOVE
//#define ENABLE_DEBUGFUNCT   // Only NEED for debug
#endif

#if ( ENABLE_BASIC_READWRITE == SDCard_Build_Config_Level )
//#define ENABLE_FILESYSTEM
//#define ENABLE_FAT32
#define ENABLE_WRITE    
//#define ENABLE_SUBDIR       // Not yet developed 
//#define ENABLE_FILECOPY
//#define ENABLE_FILERENAME
//#define ENABLE_FILEREMOVE
//#define ENABLE_DEBUGFUNCT   // Only NEED for debug
#endif

//==============================================================
#define SDCard_ON      1
#define SDCard_OFF     0

//==============================================================

//----------------------------
// R1 mask defines
//----------------------------
#define   SDCard_IDLE              0x01
#define   SDCard_ERASE_CMD         0x02
#define   SDCard_ILLEGAL_CMD       0x04
#define   SDCard_CRC_ERROR         0x08
#define   SDCard_ERASE_SEQ_ERROR   0x10
#define   SDCard_ADDR_ERROR        0x20
#define   SDCard_PARAM_ERROR       0x40

//----------------------------
// SDCard_Status defines (SD card)
//----------------------------
// IFN   Invalid File Name
// FNF   File Not Found
// PRE   Parameter Range Error
// CE    Card Error
// FFE   File Format Error
// WRE   Write Error (Unused)
// FPE   File Pointer Error
// EOF   End Of File
//----------------------------
#define SDCard_IFN          0x01   
#define SDCard_FNF          0x02
#define SDCard_PRE          0x04
#define SDCard_CE           0x08
#define SDCard_FFE          0x10
#define SDCard_FPE          0x20
#define SDCard_WRE          0x40
#define SDCard_EOF          0x80

// System defines
#define SDCard_FAIL         0xAA
#define SDCard_PASS         0x00
#define SDCard_XCARD        0x0F
#define SDCard_XFAT         0xF0
#define SDCard_UNUSED       0xFFFF

//----------------------------
// Card command defines
//----------------------------
#define SDCard_CMD0         0x40
#define SDCard_CMD1         0x41
#define SDCard_CMD9         0x49
#define SDCard_CMD10        0x4A
#define SDCard_CMD12        0x4C
#define SDCard_CMD13        0x4D
#define SDCard_CMD16        0x50
#define SDCard_CMD17        0x51
#define SDCard_CMD18        0x52
#define SDCard_CMD24        0x58
#define SDCard_CMD25        0x59
#define SDCard_CMD27        0x5B
#define SDCard_CMD28        0x5C
#define SDCard_CMD29        0x5D
#define SDCard_CMD30        0x5E
#define SDCard_CMD32        0x60
#define SDCard_CMD33        0x61
#define SDCard_CMD38        0x66
#define SDCard_CMD55        0x77
#define SDCard_CMD56        0x78
#define SDCard_CMD58        0x7A
#define SDCard_CMD59        0x7B

#define SDCard_ACMD13       0x4D
#define SDCard_ACMD18       0x52
#define SDCard_ACMD22       0x56
#define SDCard_ACMD23       0x57
#define SDCard_ACMD25       0x59
#define SDCard_ACMD26       0x5A
#define SDCard_ACMD38       0x66
#define SDCard_ACMD41       0x69
#define SDCard_ACMD42       0x6A
#define SDCard_ACMD51       0x73

#define SDCard_NOARGS       0x00000000

//--------------------------------
// Declare external variables
//--------------------------------
#ifdef ENABLE_FILESYSTEM
#ifdef ENABLE_FAT32
extern ulong   SDCard_CurStart[SDCard_MAXFILES+1];   // Starting FAT entry value for file
extern ulong   SDCard_CurFat[SDCard_MAXFILES+1];     // Current FAT entry value of cluster
#else
extern uint    SDCard_CurStart[SDCard_MAXFILES+1];   // Starting FAT entry value for file
extern uint    SDCard_CurFat[SDCard_MAXFILES+1];     // Current FAT entry value of cluster
#endif  // End ENABLE_FAT32

extern uchar   SDCard_CurSect[SDCard_MAXFILES+1];    // Current sector of the current cluster
extern uchar   SDCard_CurAttr[SDCard_MAXFILES];      // Current file attributes - Bits 6 and seven are unused
extern uchar   SDCard_FileError[SDCard_MAXFILES];    // Current error flags for file
extern uchar   SDCard_FileMode[SDCard_MAXFILES];     // Current Mode bits = "r"=0, "w"=1, "a"=2, "+"=3
#endif // End ENABLE_FILESYSTEM

extern ulong   SDCard_CurSize[SDCard_MAXFILES+1];    // Current file size (in bytes)
extern ulong   SDCard_CurOffset[SDCard_MAXFILES+1];  // Current offset into file

//extern uchar   Status;               // Temp value for returned status byte
extern uchar   SDCard_CardType;               // Card Type:    0=None,   1=MMC,    2=SD (lower nibble)
                                                        // FAT Type:     0=None,   10=FAT12, 20=FAT16, 30=FAT32 (upper nibble)
extern uchar   *SDCard_Buffer1;               // Temporary buffer pointer for data reads and writes (part 1)
extern uchar   *SDCard_Buffer2;               // Temporary buffer pointer for data reads and writes (part 2)

#ifdef ENABLE_FILESYSTEM
extern uchar   SDCard_ClusterSize;            // Cluster size in sectors
extern ulong   SDCard_Fat1Start;              // Start of 1st FAT Table (in bytes)
extern ulong   SDCard_Fat2Start;              // Start of 2nd FAT Table (in bytes)
extern ulong   SDCard_DirStart;               // Start of Root directory structure
#endif

extern ulong   SDCard_DataStart;              // Start of Data area

//---------------------------------------------------------------------------------------------
//                                   Function Prototypes
//---------------------------------------------------------------------------------------------


//---------------------------
// Low level card commands
//---------------------------
void  SDCard_Cmd(uchar CmdNum, ulong Param);   // Sends out the command string including parameters  
uchar SDCard_GetR1(void);                      // Returns R1 type command responsed  
uint  SDCard_GetR2(void);                      // Returns R2 type command responsed  
void  SDCard_EndCmd(void);                     // Sends out the end of acommand string  
void  SDCard_Wait(void);                       // Wait a small amount of time  (Value not final)  
void  SDCard_Wait2(void);                      // Wait for approximately 50ms  (Value not final)  
uchar SDCard_Cmd_00(void);                     // Resets Card to power up condition  
uchar SDCard_Cmd_01(void);                     // Runs an MMC card's internal init routine  
uint  SDCard_Cmd_13(void);                     // Returns the card status response 
uchar SDCard_ACmd_41(void);                    // Runs an SD  card's internal init routine  
uchar SDCard_Cmd_55(void);                     // Make the next command an App command  
uchar SDCard_CheckReply(uchar check);          // Compare check value to response  
uchar SDCard_SetSize(ulong size);              // Set sector size  
ulong SDCard_GetTable(void);                   // Get Partition Table information and return boot sector start  
void  SDCard_XferWait(void);                   // Wait until transfer is complete
void  SDCard_IncOffset(uchar Fptr, uchar mode); // Increment file position for CurOffset 

//---------------------------
// Low level file commands
//---------------------------
#ifdef ENABLE_FILESYSTEM
uchar SDCard_GetBoot(ulong BootStart);        // Get boot sector information and put it in Buffer  
void  SDCard_NextFat(uchar Fptr);             // Loads CurFat[Fptr] with next FAT entry or FatEnd for end  

//------------------------------------------------
// Low level commands affecting FAT16/32 switches
//------------------------------------------------
#ifdef ENABLE_FAT32
ulong   SDCard_FatTrack(ulong FatNum);          // Returns the next FAT entry in the chain.  
ulong   SDCard_LastFat(uchar Fptr);             // Returns the last FAT entry used in a file chain  
ulong   SDCard_GetFatAddr(ulong Fat);           // Return the sector address for the FAT entry  
char  * SDCard_ReadFatSect(ulong Fat);          // Read FAT entry sector into buffer and return buffer pointer  
#else
uint    SDCard_FatTrack(uint FatNum);           // Returns the next FAT entry in the chain.  
uint    SDCard_LastFat(uchar Fptr);             // Returns the last FAT entry used in a file chain  
ulong   SDCard_GetFatAddr(uint Fat);            // Return the sector address for the FAT entry  
char  * SDCard_ReadFatSect(uint Fat);           // Read FAT entry sector into buffer and return buffer pointer  
#endif  // ENABLE_FAT32

uchar   SDCard_NextSect(uchar Fptr);                // Loads CurSect[Fptr] with next sector entry and sets FAT if needed  
uchar   SDCard_FindFile(uchar Fptr, uint FileNum);  // Find File in root directory for file pointer  
uchar   SDCard_ReadFileSect(uchar Fptr);            // Read a sector of a file   
void    SDCard_GetAddress(uchar Fptr, uchar mode);  // Calculate the address based on the current file/sub-directory position - mode 0=sector, 1=sect and offset  
uchar * SDCard_GetBuffPtr(uint Offset);         // Returns the pointer for read/write buffer offset  
char  * SDCard_ReadDirSect(uchar Fptr);         // Read directory entry sector into buffer and return buffer pointer  
uchar   SDCard_ParseFilename(uchar * Filename); // Parse the filename string and convert it into 8x3 dir format  

//--------------------------------
// Low level file write commands
//--------------------------------

#ifdef ENABLE_WRITE
void  SDCard_CopyDir(uchar Fptr1, uchar Fptr2 ); // Copy file directory information from file 1 to file 2  
void  SDCard_UpdateDir(uchar Fptr);              // Update the directory entry from the current file info  
void  SDCard_WriteDirSect(uchar Fptr);           // Write directory entry sector from the buffer  
uchar SDCard_NewFile(uchar Fptr, uchar Filename[]);   // Create new file using filename   
uchar SDCard_FileZero(uchar Fptr, uchar Del);    // Sets file size to zero and resets FAT entries or deletes file 
uchar SDCard_WriteFileSect(uchar Fptr);          // Write a sector to a file  
uchar SDCard_AddFat(uchar Fptr);                 // Add a new FAT entry to the FAT chain, or create one  

#ifdef ENABLE_FAT32
uchar SDCard_FatReclaim(ulong FirstFat);         // Zero FAT chain starting at first FAT entry  
void  SDCard_WriteFatSect(ulong Fat);            // Write FAT entry sector from the buffer  
#else
uchar SDCard_FatReclaim(uint FirstFat);          // Zero FAT chain starting at first FAT entry  
void  SDCard_WriteFatSect(uint Fat);             // Write FAT entry sector from the buffer  
#endif   // ENABLE_FAT32
#endif   // End ENABLE_WRITE
#endif   // End ENABLE_FILESYSTEM

// Debug functions
#ifdef ENABLE_DEBUGFUNCT
uchar SDCard_ACmd_13(void);              // Get SD card status command  
uchar SDCard_GetCID(void);               // Send out command to read CID data into buffer  
uchar SDCard_GetOCR(void);               // Read card's OCR register routine  
uchar SDCard_GetCSD(void);               // Send out command to read CSD data into buffer  
uchar SDCard_ReadByte(ulong address);    // Return the byte pointed to by the absolute address
#endif

#endif
// TEST ONLY
