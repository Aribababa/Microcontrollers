//*****************************************************************************
//*****************************************************************************
//  FILENAME: SDCard_FS.c
//  Version 0.0
//  
//
//  DESCRIPTION: SDCard User Module C Language source file
//               for the PSoC family of devices
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

#include "device.h"
#include "SDCard_SPI.h" // Low level API
#include "SDCard_FS.h"  // Low level file system
#include "SDCard.h"     // User level API

ulong   SDCard_MemoryStart=0;            // Start of actual memory area (boot sector)
uchar   SDCard_FileError[SDCard_MAXFILES];     // Current error flags for file
ulong   SDCard_CurSize[SDCard_MAXFILES+1];     // Current file size (in bytes) (+1 for FAT32 DIR)
ulong   SDCard_CurOffset[SDCard_MAXFILES+1];   // Current offset into file     (+1 for FAT32 DIR)
#ifdef ENABLE_FILESYSTEM
ulong   SDCard_CurDir[SDCard_MAXFILES];        // Current directory entry address
#ifdef ENABLE_FAT32
ulong   SDCard_CurStart[SDCard_MAXFILES+1];    // Starting Fat value for file  (+1 for FAT32 DIR)
ulong   SDCard_CurFat[SDCard_MAXFILES+1];      // Current Fat value of cluster (+1 for FAT32 DIR)
#else
uint    SDCard_CurStart[SDCard_MAXFILES+1];    // Starting Fat value for file  (+1 for FAT16 DIR)
uint    SDCard_CurFat[SDCard_MAXFILES+1];      // Current Fat value of cluster (+1 for FAT16 DIR)
#endif
uchar   SDCard_CurSect[SDCard_MAXFILES+1];     // Current sector of the current cluster (+1 for FAT32 DIR)
uchar   SDCard_CurAttr[SDCard_MAXFILES];       // Current file attributes - Bits 6 and seven are unused
                                 // 0=Read Only, 1=Hidden, 2=System, 3=Volume Label, 4=SubDir, 5=Archive)
uchar   SDCard_FileMode[SDCard_MAXFILES];      // Current Mode bits = "r"=0, "w"=1, "a"=2, "+"=3, DirtyDirFlag=4, Reserved=5,6,7
uint    SDCard_DirSize = 512;           // Current directory size (defaults to 512)
ulong   SDCard_EmptyFat = 2;            // Location to start search for an empty FAT entry (Default to first FAT entry)
uchar   SDCard_ClusterSize;             // Cluster size in sectors
uchar   SDCard_FatEntrySize=2;          // Size of a FAT entry - defaults to FAT16
ulong   SDCard_Fat1Start;               // Start of 1st FAT Table (in bytes)
ulong   SDCard_Fat2Start;               // Start of 2nd FAT Table (in bytes)
ulong   SDCard_DirStart;                // Start of Root directory structure
#endif
ulong   SDCard_BuffLoc = 0;             // Location of data stored in the read/write buffer (used in SDCard_flush)
uchar   SDCard_DirtyFlag=0;             // Dirty flag 0=Clean 1=Data Dirty 2=Directory Dirty
uchar   SDCard_CardType = 0;            // Card Type:    0=None,   1=MMC,    2=SD (lower nibble)
                                                  // Fat Type:     0=None,   10=FAT12, 40=FAT16, 60=FAT16, B0=FAT32 (upper nibble)
ulong   SDCard_DataStart;               // Start of Data area
#ifdef ENABLE_FAT32
ulong   SDCard_DirFat;                  // Starting FAT entry of the root directory for FAT 32 file systems
ulong   SDCard_FatEnd=0xFFFF;           // Value of the end of a FAT chain - default FAT 16 
#else
uint    SDCard_DirFat;                  // Starting FAT of the root directory for FAT 16 file systems (set to zer0)
uint    SDCard_FatEnd=0xFFFF;           // Value of the end of a FAT chain - default FAT 16
#endif

ulong   SDCard_Address;                 // Temporary address variable used so calls don't have to put longs on the stack
uchar   *SDCard_Buffer1;                // Temporary 32 byte buffer pointer for data reads and writes
uchar   *SDCard_Buffer2;                // second Temporary 32 byte buffer pointer for data reads and writes

uchar   SDCard_Speed = 0;               // Speed at which the card is running 0=400KHz, 1=2MHz, 2=12Mhz

uchar    SDCard_SectBuf1[256];
uchar    SDCard_SectBuf2[256];
// Constant Global variables
const uchar SDCard_CmdStr00[6]  = {0x40, 0x00, 0x00, 0x00, 0x00, 0x95};      // Command 00 - Go Idle State
const uchar SDCard_BlankDir[32] = {'F','I','L','E','N','A','M','E','E','X','T',0x20,0x18,0x4F,0xAC,0x4E,0x36,0x31,0x36,0x31,0,0,0x86,0x7B,0x30,0x31,0,0,0,0,0,0};

uint8 rbuff;

//=================================================================================================
//      MMC/SD CARD COMMANDS - LIB Card command Functions
//=================================================================================================


//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   void SDCard_Start( uchar *Buf1, uchar *Buf2)
//
// DESCRIPTION:
//   This function initializes the two 256 byte buffer pointers and initializes
//   the SPI port for the correct mode of operation.
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//    Buf1:  Pointer for first 256 byte buffer.
//    Buf2:  Pointer for second 256 byte buffer.
//
//  RETURNS:
//    NA
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------

void SDCard_Start( void )  
{
     SDCard_Buffer1 = SDCard_SectBuf1;
     SDCard_Buffer2 = SDCard_SectBuf2;
     // Initialize SPI module
     SDCard_InitHdwr(0x00);   // Start SPI Master module
}

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   void SDCard_Stop(void)
//
// DESCRIPTION:
//   This function disables the SPI hardware and initializes the buffer pointers to 0xFF.
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:  NA
//
//  RETURNS:  NA
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------

void SDCard_Stop(void)
{
    SDCard_Buffer1 = (uchar *)0xFF;
    SDCard_Buffer2 = (uchar *)0xFF;
    // Disable SPI modules
    SDCard_UnInitHdwr();         // Stop SPI Master module
    // Former SPI lines should be forced high.
}


//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   uchar SDCard_fclose(uchar Fptr)
//
// DESCRIPTION:
//   Closes the file, clears the file open data, and clears the FileError flags
//   Returns 0 if successful, EOF if it fails (also sets FPE if pointer error)
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS: 
//    Fptr: File pointer index
//
//  RETURNS:
//     0  => If command is successful
//    EOF => If command fails
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------
#ifdef ENABLE_FILESYSTEM

uchar SDCard_fclose(uchar Fptr)
{
   
   uchar   flag=SDCard_FPE|SDCard_EOF;         // Temp error flag (Set to failure)
   
   if(Fptr<SDCard_MAXFILES)             // Is the file pointer valid?
   {
      // Close file - pointer is valid
      
#ifdef ENABLE_WRITE
      if((SDCard_FileMode[Fptr]&0x0F) != 1)    // If file was writeable do a flush
         SDCard_fflush(Fptr);
#endif
      
      SDCard_CurDir[Fptr] = 0;          // Clear file information
      SDCard_CurStart[Fptr] = 0;
      SDCard_CurFat[Fptr] = 0;
      SDCard_CurSize[Fptr] = 0;
      SDCard_CurOffset[Fptr]= 0;
      SDCard_CurSect[Fptr] = 0;
      SDCard_CurAttr[Fptr] = 0;   
      
      SDCard_FileMode[Fptr] = 0;        // Clear file mode
      SDCard_FileError[Fptr] = 0;       // Clear any file errors
      flag=SDCard_FileError[Fptr];      // Copy file error flag for return
   }

   return(flag);
}
#endif

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   uchar SDCard_fopen(uchar Filename[], const uchar Mode[])
//
// DESCRIPTION:
//   Opens the file "Filename" and returns a file pointer to that file.
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//    Filename - The string containing the name of the file to open (standard short filename)
//    Mode     - The string containing the attribute string to set the file mode
//               Mode bits = "r"=0, "w"=1, "a"=2, "+"=3
//
//  RETURNS:
//    Returns the pointer to the opened file (if Fptr >= MAXFILES FindFile failed or all pointers in use)
//
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------
#ifdef ENABLE_FILESYSTEM

uchar SDCard_fopen(uchar Filename[], const uchar Mode[])
{
   uchar   Fptr;              // Pointer of file to use
   uchar   x=0;               // Temp counter
   uchar   status=0;          // Status indicator
   uchar   xFilename[13];     // Temp filename so original is not corrupted
      
   for (Fptr=0; Fptr<SDCard_MAXFILES; Fptr++)
   {
      if(SDCard_CurStart[Fptr] == 0)
         break;
   }
   if(Fptr<SDCard_MAXFILES)
   {
      // === Start parsing Mode string ===
      // Parse file mode bits and check validity
      SDCard_FileMode[Fptr] = 0;            // Clear file mode bits
      while(Mode[x] != 0)            // Loop until mode string is parsed
      {
         if(Mode[x] == 'r')
         {
            SDCard_FileMode[Fptr] |= 0x01;  // If 'r' set the read mode bit
            status++;                // Add one to status count
         }
         else if(Mode[x] == 'w')
         {
            SDCard_FileMode[Fptr] |= 0x02;  // If 'w' set the write mode bit
            status++;                // Add one to status count
         }
         else if(Mode[x] == 'a')
         {
            SDCard_FileMode[Fptr] |= 0x04;  // If 'a' set the append mode bit
            status++;                // Add one to status count
         }
         else if(Mode[x] == '+')
            SDCard_FileMode[Fptr] |= 0x08;     // If '+' set the add mode bit
         
         x++;                        // Do next character
      }

      // Check that only one of the r, w, or a bits was set 
      if(status != 1)
      {
         // Set error bit - invalid value
         SDCard_FileMode[Fptr] = 0;         // Error - Clear file mode bits
         status = SDCard_FAIL;
      }
      else
           status = 0;               // No error - clear status
      // === Done  parsing Mode string ===

      // Copy filename from Filename to xFilename (copy is modified to match DIR format)
      x=0;
      while(Filename[x]!=0)
      {
         xFilename[x] = Filename[x]; // Copy the filename string
         x++;
      } 
      xFilename[x] = 0;              // Terminate the string

      // xFilename is now ready to be parsed and processed
      status = SDCard_ParseFilename(xFilename);      // Parse the filename
      if(status==0)                  // If the filename is valid, copy it to the buffer
      {
         for(x=0; x<12; x++)
         {
            SDCard_Buffer2[x] = xFilename[x];
         }
      }

      // Filename parsed - if no errors (status OK) , attempt to open file
      if(status==0)                           // If no errors proceed
      {
         SDCard_FileError[Fptr] &= ~SDCard_FNF;             // Clear the File Not Found flag bit
         SDCard_FindFile(Fptr, 0);                   // Get file info using filename
         if((SDCard_FileError[Fptr] & SDCard_FNF)!= 0)      // If file was not found
         {
            if((SDCard_FileMode[Fptr] & 0x01) == 1)  // Is the mode set to read?
            {
               Fptr = SDCard_MAXFILES;               // Yes, no file pointers available - Set error for open
            }
            else
            {

#ifdef ENABLE_WRITE
#ifdef ENABLE_WPROTECT
               if(SDCard_WriteProtect())          // Is the card switch in the protect mode?
               {
                  Fptr = SDCard_MAXFILES;            // Failure - write protected - Set error for open
               }
               else
#endif
               {
                  // Create new file (if write or append) and no file was found
                  Fptr = SDCard_NewFile(Fptr, SDCard_Buffer2);    // Create new file using filename - if failure Fptr set to SDCard_MAXFILES          
                  SDCard_FileError[Fptr] &= ~SDCard_FNF;          // Clear the File Not Found flag bit
               }

               
#else
               Fptr = SDCard_MAXFILES;               // Failure - not a valid mode - Set error for open
#endif
            }
         }
         else
         {
            // If file was found, and write mode is set - zero the file, if in append mode - don't zero and set postion to one past end
            if((SDCard_FileMode[Fptr] & 0x01) != 1)   // Is the mode set to write or append?
#ifdef ENABLE_WRITE
#ifdef ENABLE_WPROTECT
               if(SDCard_WriteProtect())           // Is the card switch in the protect mode?
                  Fptr = SDCard_MAXFILES;             // Failure - write protected - Set error for open
               else
#endif
               {
                   if((SDCard_FileMode[Fptr] & 0x02) == 2) // Is the mode set to write?
                       SDCard_FileZero(Fptr, 0);           // Set file size to zero and delete FAT chain
                   else
                       SDCard_fseek(Fptr, SDCard_CurSize[Fptr]);   // Must be in append mode move offset to end of file + 1
               }
#else
                  Fptr = SDCard_MAXFILES;             // Failure - not a valid mode - Set error for open

#endif
         }
      }
      else
      {
         Fptr = SDCard_MAXFILES;      // Failure - not a valid filename or mode - Set error for open
      }
   }
          
   return(Fptr);               // Return pointer value to use to access file
}
#endif

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:    uchar SDCard_fgetc(uchar Fptr)
//
// DESCRIPTION:
//   Reads a single byte from a file indicated by [Fptr]and uses the CurOffset[Fptr] 
//   for the actual byte position (CurOffset[Fptr] is incremented after the read.)
//
//   (Must have an file pointer previously set by the SDCard_FileOpen subroutine)
//   (If ENABLE_FILESYSTEM is undefined only the Fptr and CurOffset are used.)
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//    Fptr => File pointer of file to get character
//
//  RETURNS:
//    Returns the character read
//
//
//  SIDE EFFECTS:
//
//              ***   Global variables required   ***     
//    ClusterSize      - the size of a cluster on the current disk
//
//    CurFat[Fptr]     - contains the starting fat entry for the file
//    SDCard_CurSize[Fptr]    - contains the size of the file
//    CurOffset[Fptr]  - contains the current offset
//    CurSect[Fptr]    - contains the current sector in use of a cluster
//    Fptr             - pointer for which file to use, or index if no file system
//
//  THEORY of OPERATON or PROCEDURE:
//
//    Address equals ((cluster in FAT - 2)* cluster size + sector offset) * sector size)+ remainder of offset + start of data area
//    Address = (DataStart + ((((CurFat[Fptr] - 2) * ClusterSize) + CurSect[Fptr] ) * 512 ) + (SDCard_CurOffset[Fptr]&0x000001FF));
//
//-------------------------------------------------------------------------------------------------

uchar SDCard_fgetc(uchar Fptr)
{
   uint   offset;               // Temp offset of buffer index
   uchar   Data = 0;            // Temp holder for data to return

   if(Fptr<SDCard_MAXFILES)            // Is the file pointer valid? If yes, continue. If no, do nothing and return a zero.
   {
#ifdef ENABLE_FILESYSTEM
      if((SDCard_CurFat[Fptr] != SDCard_FatEnd) && (SDCard_CurOffset[Fptr] <= SDCard_CurSize[Fptr]))   // Not end of file?
      {
         // Address equals ((cluster in FAT - 2)* cluster size + sector offset) * sector size)+ remainder of offset + start of data area
         // Address = (DataStart + ((((CurFat[Fptr] - 2) * ClusterSize) + CurSect[Fptr] ) * 512 ) + (SDCard_CurOffset[Fptr]&0x000001FF));
         SDCard_GetAddress(Fptr, 1);         // Calculate address
#else
      {
         SDCard_Address = SDCard_CurOffset[Fptr]+SDCard_MemoryStart;
#endif      
         // If the dirty flag is clear and the sector address match use the current buffer
         // instead of reading the card. (Pending write data could contain a different value.)
         if( ((SDCard_DirtyFlag&0x01)!=0x01)||((SDCard_Address & 0xFFFFFE00)!= SDCard_BuffLoc) )
         {
            SDCard_SetSize(1);                          // Set block length back to read a single byte from the file (1)

            // CMD17 - Read single block command
            // Byte String = 0x51 (ulong Address) 0xFF 

            SDCard_Cmd(SDCard_CMD17, SDCard_Address);                 // Send CMD17 command string
   
            // Check response
            if(SDCard_CheckReply(0) != SDCard_FAIL)            // Check for status good
            {
               if(SDCard_CheckReply(0xFE) != SDCard_FAIL)      // Set check to Single Block Read data token
               {
                  // Get one byte

                  // Send a null byte to shift for btye read
                  SDCard_SendTxData( 0xFF );
                  SDCard_XferWait();                            // Wait for transfer to complete
            
                  // Send to standard output
                  Data = SDCard_bReadRxData(); // Read the data byte and save it for the return

                  // Increase byte count by one
                  SDCard_IncOffset(Fptr, 0);   // Position the next entry.
                           
               }
               else
                  SDCard_FileError[Fptr] |= SDCard_CE;                 // Set status to Card Error - failed
            }
            else
               SDCard_FileError[Fptr] |= SDCard_CE;                    // Set status to Card Error - failed
   
            SDCard_EndCmd();                                    // Finish the command cycle

         }
         else
         {
            offset = (uint)(SDCard_CurOffset[Fptr]) & 0x01FF;
            if(offset < 256)      // Get the data in the correct buffer
               Data = SDCard_Buffer1[offset];
            else
               Data = SDCard_Buffer2[offset-256];         

            // Increase byte count by one
            SDCard_IncOffset(Fptr, 0);    // Position the next entry (do not add fat).
         }
#ifdef ENABLE_FILESYSTEM
         // Check for the end of file
         if(SDCard_CurOffset[Fptr]==SDCard_CurSize[Fptr])
         SDCard_FileError[Fptr] |= SDCard_EOF;             // Set end of file flag if last character
#endif
      }
   }
   return(Data);
}
//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:    uchar SDCard_fbgetc(uchar Fptr)
//
// DESCRIPTION:
//   This faster buffered version of fgetc reads in an entire 512 byte buffer for faster 
//   access, but care must be used to do sequenced continuous reads, any read/write or  
//   random access reads will cause severe slowdowns in data throughput.
//
//   Reads a single buffered byte from a file indicated by [Fptr]and uses the CurOffset[Fptr] 
//   for the actual byte position (CurOffset[Fptr] is incremented after the read.)
//
//   (Must have an file pointer previously set by the SDCard_FileOpen subroutine)
//   (If ENABLE_FILESYSTEM is undefined only the Fptr and CurOffset are used.)
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//    Fptr => File pointer of file to get character
//
//  RETURNS:
//    Returns the character read
//
//
//  SIDE EFFECTS:
//
//              ***   Global variables required   ***     
//    ClusterSize      - the size of a cluster on the current disk
//
//    CurFat[Fptr]     - contains the starting fat entry for the file
//    SDCard_CurSize[Fptr]    - contains the size of the file
//    CurOffset[Fptr]  - contains the current offset
//    CurSect[Fptr]    - contains the current sector in use of a cluster
//    Fptr             - pointer for which file to use, or index if no file system
//
//  THEORY of OPERATON or PROCEDURE:
//
//    Address equals ((cluster in FAT - 2)* cluster size + sector offset) * sector size)+ remainder of offset + start of data area
//    Address = (DataStart + ((((CurFat[Fptr] - 2) * ClusterSize) + CurSect[Fptr] ) * 512 ) + (SDCard_CurOffset[Fptr]&0x000001FF));
//
//-------------------------------------------------------------------------------------------------

uchar SDCard_fbgetc(uchar Fptr)
{
   uint   offset;               // Temp offset of buffer index
   uchar  Data = 0;             // Temp holder for data to return

   if(Fptr<SDCard_MAXFILES)            // Is the file pointer valid? If yes, continue. If no, do nothing and return a zero.
   {
#ifdef ENABLE_FILESYSTEM
      if((SDCard_CurFat[Fptr] != SDCard_FatEnd) && (SDCard_CurOffset[Fptr] <= SDCard_CurSize[Fptr]))   // Not end of file?
      {
         SDCard_GetAddress(Fptr, 1);         // Calculate address
#else
      {
         SDCard_Address = SDCard_CurOffset[Fptr]+SDCard_MemoryStart;
#endif      
         // Is the buffer location different from the read location?
         if( (SDCard_Address & 0xFFFFFE00)!= SDCard_BuffLoc )
         {
#ifdef ENABLE_WRITE
             // Yes, buffer needs to be changed
             if((SDCard_DirtyFlag&0x01)==0x01)     // Is the data buffer dirty?
                 {
                     SDCard_WriteSect(SDCard_BuffLoc);    // Write the dirty data buffer back out
                     SDCard_DirtyFlag &= 0xEF;     // Clear dirty bit
                 }
#endif
              // Get the new read buffer to use
              SDCard_ReadSect(SDCard_Address);	// Read new sector into the buffer
         }

         offset = (uint)(SDCard_CurOffset[Fptr]) & 0x01FF;
         if(offset < 256)               // Get the data in the correct buffer
             Data = SDCard_Buffer1[offset];
         else
             Data = SDCard_Buffer2[offset-256];			

         // Increase byte count by one
         SDCard_IncOffset(Fptr, 0);            // Position the next entry (do not add FAT).

#ifdef ENABLE_FILESYSTEM
         // Check for the end of file
         if(SDCard_CurOffset[Fptr]==SDCard_CurSize[Fptr])
	         SDCard_FileError[Fptr] |= SDCard_EOF;             // Set end of file flag if last character
#endif
      }
   }
   return(Data);
}

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   uchar SDCard_fputc(uchar Data, uchar Fptr)
//
// DESCRIPTION:
//   Writes a single byte to the file indicated by [Fptr]and uses the SDCard_CurOffset[Fptr] 
//   for the actual byte position (SDCard_CurOffset[Fptr] is incremented after the write.)
//   (Must have an file pointer previously set to write by the SDCard_FileOpen subroutine)
//   (If ENABLE_FILESYSTEM is undefined only the Fptr and CurOffset are used.)
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//
//  RETURNS:
//    Returns the write status
//
//
//  SIDE EFFECTS:
//
//                 ***   Global variables required   ***     
//    ClusterSize      - the size of a cluster on the current disk
//   
//    CurFat[Fptr]     - contains the starting FAT entry for the file
//    CurSize[Fptr]    - contains the size of the file
//    CurOffset[Fptr]  - contains the current offset
//    CurSect[Fptr]    - contains the current sector in use of a cluster
//    Fptr             - pointer for which file to use, or index if no file system
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------
#ifdef ENABLE_WRITE

uchar SDCard_fputc(uchar Data, uchar Fptr)
{
   uint   offset;                  // Temp offset value
   uchar   status = SDCard_FAIL;          // Set status to fail (until cleared by sucess)

#ifdef ENABLE_WPROTECT
   if(SDCard_WriteProtect()==0)        // If write protect is on skip writes, flag error   
#endif
   {
      if(Fptr == SDCard_MAXFILES)         // Is the file pointer valid? If yes, continue. If no, do nothing and return a zero.
      {
         // Pointer invalid - Drop through and fail
      }
#ifdef ENABLE_FILESYSTEM
      else if((SDCard_FileMode[Fptr]&0x0F) == 1)   // Can the file be written to?
      {
         // Read only file - Drop through and fail
      }
      else if( ((SDCard_CurFat[Fptr] == SDCard_FatEnd) || (SDCard_CurOffset[Fptr] > SDCard_CurSize[Fptr]))&&((SDCard_FileMode[Fptr]&2)!= 2)   )   // Can the file be appended to if EOF?
      {
         // EOF and not in append mode - Drop through and fail
      }
      else if(SDCard_CurOffset[Fptr] > (SDCard_CurSize[Fptr] +1))    // Is offset beyond adding a single character?
      {
         // Offset invalid, even in append mode - Drop through and fail
      }
      else
      {
         if(SDCard_CurStart[Fptr]==0)  // No assigned FAT entries for this file?
            SDCard_AddFat(Fptr);       // Yes, Add a FAT entry to the file

         if(SDCard_CurOffset[Fptr] == (SDCard_CurSize[Fptr]))    // Adding to the current file size?
         {
            // Yes, increment file size and set dirty bit for directory
            SDCard_CurSize[Fptr]++;
            SDCard_FileMode[Fptr] |= 0x10;      // Set the dirty directory flag - Data modified
         }
         // Address equals ((cluster in FAT - 2)* cluster size + sector offset) * sector size)+ remainder of offset + start of data area
         // Address = (DataStart + ((((CurFat[Fptr] - 2) * ClusterSize) + CurSect[Fptr] ) * 512 ));

         SDCard_GetAddress(Fptr, 0);         // Calculate sector address
      
#else
      else
      {
         SDCard_Address = (SDCard_CurOffset[Fptr] & 0xFFFFFE00)+SDCard_MemoryStart;
#endif      
         // If the sector changes - update it, if it's dirty - flush it then update it.
         if(SDCard_Address != SDCard_BuffLoc)
         { 
            if((SDCard_DirtyFlag&0x01)==0x01)
            {
               SDCard_WriteSect(SDCard_BuffLoc);      // Flush buffer if data is dirty
               SDCard_DirtyFlag &= 0xFE;       // Clear dirty flag for buffer
            }
            SDCard_ReadSect(SDCard_Address);          // Read the new sector in
            SDCard_BuffLoc = SDCard_Address;          // Update the write location
         }

         offset = (uint)(SDCard_CurOffset[Fptr]) & 0x01FF;
         if(offset < 256)               // Save the data in the correct buffer
            SDCard_Buffer1[offset] = Data;
         else
            SDCard_Buffer2[offset-256] = Data;         
         
         SDCard_DirtyFlag |= 0x01;             // Set the dirty data flag - Data modified

         // Increase byte count by one
         SDCard_IncOffset(Fptr, 1);            // Position the next entry and add fat if needed

         // Set the status bit for success
         status = SDCard_PASS;
      }

   }
#ifdef ENABLE_WPROTECT
   else
      status = SDCard_FAIL;                     // Write protect on - set fail
#endif
   
   return(status);
}
#endif

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   uchar SDCard_fputs(char *str, uchar Fptr)
//
// DESCRIPTION:
//   Writes a null terminated string to the file pointed to by Fptr.
//   (Must have an file pointer previously set to write by the SDCard_FileOpen subroutine)
//   (If ENABLE_FILESYSTEM is undefined only the Fptr and CurOffset are used.)
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//
//  RETURNS:
//    Returns the write status
//
//
//  SIDE EFFECTS:
//
//                 ***   Global variables required   ***     
//    ClusterSize      - the size of a cluster on the current disk
//   
//    CurFat[Fptr]     - contains the starting FAT entry for the file
//    CurSize[Fptr]    - contains the size of the file
//    CurOffset[Fptr]  - contains the current offset
//    CurSect[Fptr]    - contains the current sector in use of a cluster
//    Fptr             - pointer for which file to use, or index if no file system
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------
#ifdef ENABLE_WRITE

uchar SDCard_fputs(char *str, uchar Fptr)
{

   uchar   status = SDCard_FAIL;                 // Set status to fail (until cleared by success)

   while(*str != 0) {
      status = SDCard_fputc(*str, Fptr);
      str++;
      if(status == SDCard_FAIL)
         break;
   }
   
   return(status);
}
#endif

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   uchar SDCard_fputcs(const char *str, uchar Fptr)
//
// DESCRIPTION:
//   Writes a null terminated const string to the file pointed to by Fptr.
//   (Must have an file pointer previously set to write by the SDCard_FileOpen subroutine)
//   (If ENABLE_FILESYSTEM is undefined only the Fptr and CurOffset are used.)
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//
//  RETURNS:
//    Returns the write status
//
//
//  SIDE EFFECTS:
//
//                 ***   Global variables required   ***     
//    ClusterSize      - the size of a cluster on the current disk
//   
//    CurFat[Fptr]     - contains the starting FAT entry for the file
//    CurSize[Fptr]    - contains the size of the file
//    CurOffset[Fptr]  - contains the current offset
//    CurSect[Fptr]    - contains the current sector in use of a cluster
//    Fptr             - pointer for which file to use, or index if no file system
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------
#ifdef ENABLE_WRITE

uchar SDCard_fputcs(const char *str, uchar Fptr)
{

   uchar   status = SDCard_FAIL;                  // Set status to fail (until cleared by sucess)

   while(*str != 0) {
      status = SDCard_fputc(*str, Fptr);
      str++;
      if(status == SDCard_FAIL)
         break;
   }
   
   return(status);
}
#endif

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   uchar SDCard_fputBuff(uchar *buff, uint count, uchar Fptr)
//
// DESCRIPTION:
//   Writes a null terminated string to the file pointed to by Fptr.
//   (Must have an file pointer previously set to write by the SDCard_FileOpen subroutine)
//   (If ENABLE_FILESYSTEM is undefined only the Fptr and CurOffset are used.)
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//
//  RETURNS:
//    Returns the write status
//
//
//  SIDE EFFECTS:
//
//                 ***   Global variables required   ***     
//    ClusterSize      - the size of a cluster on the current disk
//   
//    CurFat[Fptr]     - contains the starting FAT entry for the file
//    CurSize[Fptr]    - contains the size of the file
//    CurOffset[Fptr]  - contains the current offset
//    CurSect[Fptr]    - contains the current sector in use of a cluster
//    Fptr             - pointer for which file to use, or index if no file system
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------
#ifdef ENABLE_WRITE

uchar SDCard_fputBuff(uchar *buff, uint count, uchar Fptr)
{

   uchar   status  = SDCard_FAIL;                      // Set status to fail (until cleared by sucess)

   for( ;count > 0; count--) {
      status = SDCard_fputc(*buff, Fptr);
      buff++;
      if(status == SDCard_FAIL)
         break;
   }
   
   return(status);
}
#endif


//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   uchar SDCard_fputcBuff(const uchar *buff, uint count, uchar Fptr)
//
// DESCRIPTION:
//   Writes a null terminated string to the file pointed to by Fptr.
//   (Must have an file pointer previously set to write by the SDCard_FileOpen subroutine)
//   (If ENABLE_FILESYSTEM is undefined only the Fptr and CurOffset are used.)
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//
//  RETURNS:
//    Returns the write status
//
//
//  SIDE EFFECTS:
//
//                 ***   Global variables required   ***     
//    ClusterSize      - the size of a cluster on the current disk
//   
//    CurFat[Fptr]     - contains the starting FAT entry for the file
//    CurSize[Fptr]    - contains the size of the file
//    CurOffset[Fptr]  - contains the current offset
//    CurSect[Fptr]    - contains the current sector in use of a cluster
//    Fptr             - pointer for which file to use, or index if no file system
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------
#ifdef ENABLE_WRITE

uchar SDCard_fputcBuff(const uchar *buff, uint count, uchar Fptr)
{

   uchar   status  = SDCard_FAIL;                   // Set status to fail (until cleared by sucess)

   for( ;count > 0; count--) {
      status = SDCard_fputc(*buff, Fptr);
      buff++;
      if(status == SDCard_FAIL)
         break;
   }
   
   return(status);
}
#endif




//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   void SDCard_fflush(uchar Fptr)
//
// DESCRIPTION:
//    Writes buffers to a file and updates directory info if needed
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//    Fptr => Pointer to the file.
//
//  RETURNS:  NA
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------
#ifdef ENABLE_WRITE

void SDCard_fflush(uchar Fptr)
{
   
   if((SDCard_DirtyFlag & 0x01) == 0x01 ) // If dirty data bit is set, write buffer back
   {
      SDCard_WriteSect(SDCard_BuffLoc);   
      SDCard_DirtyFlag = 0;               // Clear dirty flag
   }    
#ifdef ENABLE_FILESYSTEM
   if((SDCard_FileMode[Fptr] & 0x10) == 0x10 ) // If dirty directory bit is set, update directory
      SDCard_UpdateDir(Fptr);   
#endif

}
#endif


//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   uchar *  SDCard_GetFilename(uint Entry)
//
// DESCRIPTION:
//   Gets the 'Entry' number valid filename from the root directory and returns a pointer to it.
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//
//  RETURNS:
//    Pointer to the file name of the 'Entry' file.
//    If returned pointer[0] = 0 then the file was not found 
//
//  SIDE EFFECTS:
//    Note: it uses both Buffer1 (all) and Buffer2 (16-28)
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------
#ifdef ENABLE_FILESYSTEM

uchar *  SDCard_GetFilename(uint Entry)
{
   uchar   x;                         // Temp loop variable
   
   if((Entry>=1)&&(Entry<=SDCard_DirSize))   // Is the entry number valid?
   {
      SDCard_FindFile(SDCard_MAXFILES, Entry);      // Yes, Get just the filename, not file info
      if(SDCard_Buffer1[0]!=0)               // File name found?
      {   
         // If the file was found - copy the filename, '.', extension, and add null to the end and
         // put it at Buffer2[16], then copy the raw filename to Buffer2[0] for the quick open feature
         for(x=0; x<8; x++)           // Copy the filename
         {
            SDCard_Buffer2[x+16] = SDCard_Buffer1[x];               
         }
         SDCard_Buffer2[24] = '.';           // Add the dot
         for(x=8; x<11; x++)          // Copy the extension
         {
            SDCard_Buffer2[x+17] = SDCard_Buffer1[x];               
         }
         SDCard_Buffer2[28] = 0;             // Add the terminating null
      }
      else
         SDCard_Buffer2[16]=0;               // File was not found - put null at start of filename buffer
   }
   else
      SDCard_Buffer2[16]=0;                  // File number not valid - put null at start of filename buffer
   
   return(&SDCard_Buffer2[16]);      
}
#endif

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   uchar SDCard_InitCard(void)
//
// DESCRIPTION:
//   Initialize MMC/SD card and get card information (must be done any time a new card is inserted)
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:  NA
//
//  RETURNS:
//    0 => No card available
//    1 => MMC type card
//    2 => SD Card type
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------

uchar SDCard_InitCard(void)
{
   uchar   loop;														// Temp counter/loop variable
   uchar   status;														// Temp status value
   uint32   TmpClkIn;													// Temp storage for default project settings.

   // Backup the initial register setting for use in future
   TmpClkIn = SDCard_SD_Clock_GetDividerRegister();
   
   // Set clock in to 32Khz oscillator for use under 400Khz (Must run slowly until card initialized as SD or MMC)
   SDCard_Speed = 0;											// Indicate Slow Speed Clock
   SDCard_SD_Clock_SetDividerRegister(1499, 1);					// Set divider to 48Mhz/1500 = 32Khz
   
   // Wait at least 1 ms after power up before attempting to do anything with the card
   SDCard_Wait2();                       						// Wait at least 1 ms

   // Send out a string of at least 74 clocked highs (10 or more 0xFF characters) previous to sending commands
   for(loop=0; loop<10; loop++)   										// Loop through the a series of 0xFF's to set data line
   {
      // Send a null byte to shift for btye read
	  SDCard_SendTxData( 0xFF );  
      SDCard_XferWait();                 						// Wait for transfer to complete
	  rbuff = SDCard_bReadRxData();
   }

   // Set card to SPI mode - Set SDCard_SELECT  (SD Card pin 1) low and then send CMD(0) Reset
   // If the CS line is low then a mode R1 SPI response is generated.
   // Status bits 0=Idle, 1=Erase Reset, 2=Illegal CMD, 3=CRC Error, 4=Erase_Seq_Error, 5=Address Error, 6=Param Error
   status = SDCard_Cmd_00();             						// Issue card reset commannd and return status

   // Test for SD card comptibility
   // The SD Card must be polled with a ACMD(41) until it's init is finished
   loop = 255;                      									// Set number of tries until you timeout
   status = SDCard_FAIL;                   					// Initialize status to non zero value   
   while((status != 0) && (loop>0)) 									// Loop until status is good, or a timeout occurs
   {
      if(SDCard_Cmd_55()==0);              					// Issue the App command and do SD OP COND command if it passes
         status = SDCard_ACmd_41();        					// See if SD OP COND passed, if so then it is an SD card
      loop--;                       									// Decrement loop until 0, then timeout
      if(loop == 0)                 									// If loop is zero, then set status to failed
         status = SDCard_FAIL;
   }

   if(status==SDCard_FAIL)   									// If the SD init failed, try the MMC init
   {
      // The MMC Card must be polled with a CMD(1) until it's init is finished
      loop = 255;                   									// Set number of tries until you timeout
      status = 0;                   									// Reset Status   
      while((SDCard_Cmd_01() != 0) && (loop>0))   			// Loop until status is good, or a timeout occurs
      {
         loop--;                    									// Decrement loop until 0, then timeout
         if(loop == 0)              									// If loop is zero, then set status to zero for failed
            status = SDCard_FAIL;
      }

      if(status != SDCard_FAIL)
      {
         // MMC init passed - set card type to MMC (1)
         SDCard_CardType = 1;
      }
      else
      {
         // Both inits failed - card not inserted properly or it's bad
         SDCard_CardType = 0;
      }
   }
   else
   {
      // SD init passed - set card type to SD (2)
      SDCard_CardType = 2;
   }

   // Set clock back to original settings used in project
   SDCard_SD_Clock_SetDividerRegister(TmpClkIn, 1);
   SDCard_Speed = 1;                       					// set clock back to fast setting

   SDCard_MemoryStart = SDCard_GetTable();        	// Get starting location of memory area

#ifdef ENABLE_FILESYSTEM
   if((SDCard_CardType&SDCard_XCARD) != 0)        	// If card passed operational testing, then finish card init
      SDCard_GetBoot(SDCard_MemoryStart);         	// Get boot sector information and load it into buffer
      
    // If card failed, or FAT type is missing, clear card structure information
    if( ((SDCard_CardType & SDCard_XCARD) == 0)|| ((SDCard_CardType & SDCard_XFAT) == 0) )
   {
      // Failed - clear the card variables - they may have been previously set
      SDCard_ClusterSize=0;
      SDCard_Fat1Start=0;
      SDCard_Fat2Start=0;
      SDCard_DirStart=0;
      SDCard_DataStart=0;
      SDCard_EmptyFat=2;                   					// Location to start search for an empty FAT entry (Default to first FAT entry)
   }
#endif

   return(SDCard_CardType);
}

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   uchar SDCard_fseek(uchar Fptr, ulong Offset)
//
// DESCRIPTION:
//   Sets the file position offset for the next character read and updates the file info to match
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//    Fprt => File Pointer
//    Offset => Offset of character
//
//  RETURNS:
//
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------

uchar SDCard_fseek(uchar Fptr, ulong Offset)
{
   uchar   flag=SDCard_FPE;                 // Temp error flag variable (Set to FPE in case bad pointer)
#ifdef ENABLE_FILESYSTEM
   ulong Clusters;                   // Number of clusters into the file
   ulong   x;                        // Loop variable large enough for any possible count

   Clusters = (Offset/(SDCard_ClusterSize * 512));   // Figure out number of clusters into the file
#endif

   
   if(Fptr<SDCard_MAXFILES)                   // Is the file pointer valid?
   {
#ifdef ENABLE_FILESYSTEM
      // Check to make sure file pointer is in use
      if(SDCard_CurStart[Fptr]!= 0)
      {
         // Valid file pointer - check to make sure Offset is valid - if append is on one extra character is valid
         if((Offset<SDCard_CurSize[Fptr])||((Offset<=SDCard_CurSize[Fptr])&&((SDCard_FileMode[Fptr]&0x04)==0x04)))
         {
            // Get starting FAT entry for file
            SDCard_CurFat[Fptr] = SDCard_CurStart[Fptr];
            
            // Calculate the current FAT entry
            for(x=0; x<Clusters; x++)
            {
               SDCard_NextFat(Fptr);         // Get next Fat entry in the chain
            }
            
            // Calculate the current sector
            SDCard_CurSect[Fptr]=( (Offset / 512) - (Clusters * SDCard_ClusterSize) );
            
            // Save the new offset
            SDCard_CurOffset[Fptr] = Offset;
         }
         else
         {
            // The offset is beyond the end of the file
            SDCard_FileError[Fptr] |= SDCard_PRE;      // Set the Parameter Range Error flag
         }
      }
#else
      // Save the new offset
      SDCard_CurOffset[Fptr] = Offset;
      SDCard_FileError[Fptr] &= ~SDCard_PRE;   // Clear the Parameter Range         
#endif
      flag=SDCard_FileError[Fptr];      // Copy file errors to flag for return
   }
   return(flag);
}

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   uchar SDCard_Present(void)
//
// DESCRIPTION:
//   Check if card is present.
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS: NA
//
//  RETURNS:
//   1 => Card is present in the socket 
//   0 => Card is not present in the socket 
//
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------
#if (SDCard_ENABLE_PRESENT )

uchar SDCard_Present(void)
{
   uchar   CardIn;
   CardIn = 0;   // Default, card is not present

   if((SDCard_CPPort & SDCard_CPMask) == 0) {     // Looks like card is present...
      SDCard_Wait();                                        // Wait for debounce
      if((SDCard_CPPort & SDCard_CPMask) == 0) {  // Double check if card is still there
         CardIn = 1;                                                  // Card is installed
      }
   }

   return(CardIn);        // return card status
}
#endif



//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   uchar SDCard_WriteProtect(void)
//
// DESCRIPTION:
//   Returns write protect status.
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:  NA
//
//  RETURNS:
//    1 => Card is write protected.
//    0 => Card is not write protected.
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------
#if (SDCard_ENABLE_WPROTECT)

uchar SDCard_WriteProtect(void)
{
   uchar   wProtect;
   wProtect = 0;   // Default, card is not write protected.

   if((SDCard_WPPort & SDCard_WPMask) != 0) {     // Looks like card is write protected...
      SDCard_Wait();                                        // Wait for debounce
      if((SDCard_WPPort & SDCard_WPMask) != 0) {  // Double check write protect is still present
         wProtect = 1;                                                // Card is Write protected
      }
   }

   return(wProtect);        // return write protect status
}
#endif


//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   uint SDCard_GetFileCount(void)
//
// DESCRIPTION:
//   Returns the number of valid files in the directory
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS: NA
//
//  RETURNS:
//    Returns the valid file count in the root directory.
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------
#ifdef ENABLE_FILESYSTEM

uint SDCard_GetFileCount(void)
{
   // Call FindFile with invalid file number and illegal file pointer, does not update file info,
   // and when no match is found the total number of valid files is returned as a default.
   
   return(SDCard_FindFile(SDCard_MAXFILES, 0xFFFE));   // Return number of valid files found
}
#endif


//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   ulong SDCard_GetFileSize(uchar Fptr)
//
// DESCRIPTION:
//    Returns the size of the file pointed to,  zero means an error occurred
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//    Fptr => File pointer
//
//  RETURNS:
//    0 => Error
//    Size of file pointed to by Fptr
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------
#ifdef ENABLE_FILESYSTEM

ulong SDCard_GetFileSize(uchar Fptr)
{
   // Returns the size of the file pointed to,  zero means an error occurred
   
   ulong size=0;                  // Temp variable for size (Set to failure condition)
   
   if(Fptr<SDCard_MAXFILES)              // Is the file pointer valid?
      size=SDCard_CurSize[Fptr];         // Yes, get size value
   
   return(size);                  // Return size of file
}
#endif

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   void SDCard_clearerr(uchar Fptr)
//
// DESCRIPTION:
//   Clear the error flags for the given file.
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//    Fptr => File pointer
//
//  RETURNS: NA
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------

void SDCard_clearerr(uchar Fptr)
{
   if(Fptr<SDCard_MAXFILES)               // Is this a valid file pointer?
      SDCard_FileError[Fptr]=0;           // Yes, clear file error flags
}


//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   uchar SDCard_feof(uchar Fptr)
//
// DESCRIPTION:
//   Returns a zero for no EOF, non-zero for EOF (End Of File)
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//    Fptr => File pointer
//
//  RETURNS:
//   zero     => No EOF, 
//   non-zero => EOF (End Of File)
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------
#ifdef ENABLE_FILESYSTEM

uchar SDCard_feof(uchar Fptr)
{
   // Returns a zero for no EOF, non-zero for EOF (End Of File)
   
   uchar flag=SDCard_EOF;                 // Temp flag variable
   
   if(Fptr<SDCard_MAXFILES)               // Is this a valid file pointer?
      flag=SDCard_FileError[Fptr]&SDCard_EOF;    // Yes, get EOF flag
      
   return(flag);                   // Return EOF status
}
#endif


//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   uchar SDCard_ferror(uchar Fptr)
//
// DESCRIPTION:
//   Returns error status.
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//    Fptr => File pointer
//
//  RETURNS:
//   zero     => No error
//   non-zero => Error type
//
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------

uchar SDCard_ferror(uchar Fptr)
{
   
   uchar flag=SDCard_FPE;                 // Temp error flag variable (Set failure)
   
   if(Fptr<SDCard_MAXFILES)               // Is this a valid file pointer?
      flag=SDCard_FileError[Fptr];        // Yes, get flags
      
   return(flag);                   // Return file error status
}


//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   ulong SDCard_ftell(uchar Fptr)
//
// DESCRIPTION:
//   Returns file position offset value of the next character to read
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//    Fptr => File pointer
//
//  RETURNS:
//    Position in file of next character to be read.
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------

ulong SDCard_ftell(uchar Fptr)
{
   ulong position=0xFFFFFFFF;       // Temp position variable (Set to failure condition)
   
   if(Fptr<SDCard_MAXFILES)                // Is this a valid file pointer?
      position=SDCard_CurOffset[Fptr];     // Yes, Save position information      
      
   return(position);                // Return file position (offset)
}

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   uchar SDCard_Remove(uchar * Filename)
//
// DESCRIPTION:
//   Deletes the file from the directory and closes it
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//    Pointer to file name string.
//
//  RETURNS:
//    Returns status of file delete.
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------
#ifdef ENABLE_FILESYSTEM
#ifdef ENABLE_WRITE
#ifdef ENABLE_FILEREMOVE

uchar SDCard_Remove(uchar * Filename)
{
   uchar   status = SDCard_FAIL;            // Temp status (defaults to failed
   uchar   Fptr;                     // Temp file pointer
   
   Fptr = SDCard_fopen(Filename, "r");   // Open file by filename for Read
   if(Fptr < SDCard_MAXFILES)               // Open sucessful?
   {
                                     // Open Successful, delete file
      SDCard_clearerr(Fptr);             // Clear error flags
      status = SDCard_FileZero(Fptr, 1);    // Call FileZero with the delete option selected
      SDCard_fclose(Fptr);               // Close the file when finished
   }
   
   return(status);                   // Return status of delete
}
#endif
#endif
#endif

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   uchar SDCard_Rename(uchar * OldFilename, uchar * NewFilename)
//
// DESCRIPTION:
//   Renames a file by altering the directory entry (changes buffer contents)
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//    OldFilename => Original file name.
//    NewFilename => New file name.
//
//  RETURNS:
//    Copy status  
//
//  SIDE EFFECTS:
//     Note: Filenames must be stored external to Buffer1 or Buffer2 (13 char each)
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------
#ifdef ENABLE_FILESYSTEM
#ifdef ENABLE_WRITE
#ifdef ENABLE_FILERENAME

uchar SDCard_Rename(uchar * OldFilename, uchar * NewFilename)
{
   
   uchar x;               // Temp loop counter
   uchar status=0;        // Local status variable
   uchar Fptr;            // Temp file pointer
   uchar * Dptr;          // Temp Directory entry pointer
   uchar xFilename[13];   // Temp filename as parse corrupts the original

   // Copy filename from Filename to xFilename (copy is modified to match DIR format)
   x=0;
   while(NewFilename[x]!=0)
   {
      xFilename[x] = NewFilename[x];        // Copy the new filename string
      x++;
   } 
   xFilename[x] = 0;                        // Terminate the string

   status = SDCard_ParseFilename(xFilename);       // Convert the copy of the new filename to 8x3 dir version
   
   if(status == 0)                          // If new filename is valid - proceed
   {
      // Check to see if new filename already exists
      Fptr = SDCard_fopen(NewFilename, "r");    // Open file by filename for Read to see if it already exists
      if(Fptr==SDCard_MAXFILES)                    // Did the open succeed? No - then continue, Yes - failure, file already exists
      {
         // Now see if the old filename can be opened
         Fptr = SDCard_fopen(OldFilename, "r"); // Open file by filename for Read
         if(Fptr!=SDCard_MAXFILES)                 // Did the open succeed?
         {
            // Yes, change the filename
            Dptr = SDCard_ReadDirSect(Fptr);       // Read directory sector and return pointer to entry
            for(x=0; x<11; x++)             // Copy the new filename over the old one
               Dptr[x] = xFilename[x];
            SDCard_WriteDirSect(Fptr);             // Write the directory sector back out
            SDCard_fclose(Fptr);                // Close the file
         }
         else
         {
            // No, file open failed - file does not exist
            status = SDCard_FAIL;
         }
      }
      else
      {
         // The file already exists - can't rename it
         SDCard_fclose(Fptr);            // Close the file
         status = SDCard_FAIL;
      }
   }
   return(status);
}
#endif
#endif
#endif

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   uchar SDCard_Copy(uchar * OldFilename, uchar * NewFilename)
//
// DESCRIPTION:
//   Copies a file from old filename to new filename
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//    OldFilename => Original file name.
//    NewFilename => Name of file copy.
//
//  RETURNS:
//    Command status, pass or fail.
//
//  SIDE EFFECTS:
//     Note: Filenames must be stored external to Buffer1 or Buffer2 (13 char each)
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------
#ifdef ENABLE_FILESYSTEM
#ifdef ENABLE_WRITE
#ifdef ENABLE_FILECOPY

uchar SDCard_Copy(uchar * OldFilename, uchar * NewFilename)
{
   uchar status=0;                        // Local status variable
   uchar Fptr,Fptr2;                      // Temp file pointers

   Fptr = SDCard_fopen(OldFilename, "r");     // Open file by filename for Read
   if(Fptr < SDCard_MAXFILES)                    // Did the open succeed?
   {
      // Yes, try to copy the file
      Fptr2 = SDCard_fopen(NewFilename, "w"); // Open second file by filename for Write
      if(Fptr2 < SDCard_MAXFILES)                // Open successful?
      {
         SDCard_clearerr(Fptr);               // Clear error flags

         // Add a new cluster to the zero length file created
         SDCard_AddFat(Fptr2);                   // Add a FAT entry to the file

         while(! SDCard_feof(Fptr))           // Check for end of input file
         {
            status=SDCard_ReadFileSect(Fptr);    // Read sector
            status=SDCard_WriteFileSect(Fptr2);  // Write sector
            SDCard_CurSize[Fptr2]+= 512;         // Increment the file size
            SDCard_NextSect(Fptr);               // Advance to next read sector
            if(! SDCard_feof(Fptr))           // If not the end of file 1, get next file 2 sector
               SDCard_NextSect(Fptr2);           // Advance to next write sector
         }
         SDCard_CopyDir(Fptr, Fptr2);            // Copy directory information from file 1 to file 2
      }
      else
      {   
         // File open failed - file does not exist or file error
         status = SDCard_FAIL;
      }
      SDCard_fclose(Fptr);                    // Close the files when finished
      SDCard_fclose(Fptr2);
   }
   else
   {
      // File open failed - file does not exist or file error
      status = SDCard_FAIL;
   }
   return(status);
}
#endif
#endif
#endif

//=================================================================================================
// SD CARD COMMANDS - LOW LEVEL
//=================================================================================================


//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   void SDCard_Cmd(uchar CmdNum, ulong Param)
//
// DESCRIPTION:
//    Send command to SD Card.
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//    CmdNum:  Command to be sent to SD card.
//    Param:   Parameter to be send with command.
//
//  RETURNS:  NA
//
//  SIDE EFFECTS:
//    NOTE: Calling routine must parse any response and bring the select line high again
//
//    The data response routine and the closing of the SPI (setting select high)
//    is the responsibility of the calling routine as the requirements differ from 
//    command to command. (Use CmdEnd(); to close most routines.
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------
void SDCard_Cmd(uchar CmdNum, ulong Param)
{

   // Send an 0xFF to insure data line is high before command sent
   SDCard_SendTxData( 0xFF );
   SDCard_XferWait();                        // Wait for transfer to complete
   rbuff = SDCard_bReadRxData();
   
   // Send an 0xFF to insure data line is high before command sent
   SDCard_SendTxData( 0xFF );
   SDCard_XferWait();                        // Wait for transfer to complete
   rbuff = SDCard_bReadRxData();
   
   SDCard_Select(SDCard_ENABLE);       // Set SD Card select line low (enables card for SPI)

   SDCard_Wait();                           // Wait a minimum setup time for the high/low transition

   // Send a byte
   SDCard_SendTxData( CmdNum );         // Command byte to send
   SDCard_XferWait();                        // Wait for transfer to complete
   rbuff = SDCard_bReadRxData();

   // Send a byte
   SDCard_SendTxData((char) (Param>>24));   // Parameter MSB (Byte 3)
   SDCard_XferWait();                        // Wait for transfer to complete
   rbuff = SDCard_bReadRxData();
   
   // Send a byte
   SDCard_SendTxData((char) (Param>>16));   // Parameter     (Byte 2)
   SDCard_XferWait();                        // Wait for transfer to complete
   rbuff = SDCard_bReadRxData();
   
   // Send a byte
   SDCard_SendTxData((char) (Param>>8) );   // Parameter     (Byte 1)
   SDCard_XferWait();                        // Wait for transfer to complete
   rbuff = SDCard_bReadRxData();
   
   // Send a byte
   SDCard_SendTxData( (char) Param );      // Parameter LSB (Byte 0)
   SDCard_XferWait();                        // Wait for transfer to complete
   rbuff = SDCard_bReadRxData();
   
   // Send an FF for the checksum byte
   SDCard_SendTxData( 0xFF );            // Checksum and End Bit)
   SDCard_XferWait();                        // Wait for transfer to complete
   rbuff = SDCard_bReadRxData();
   
   SDCard_Wait();            // Wait a minimum setup time for the high/low transition
   
   // The data response routine and the closing of the SPI (setting select high)
   // is the responsibility of the calling routine as the requirements differ from 
   // command to command. (Use CmdEnd(); to close most routines.
}

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   uchar SDCard_GetR1(void)
//
// DESCRIPTION:
//    Gets R1 command response (do not use for types (R1b, R2, or R3)
//    Status bits 0=Idle, 1=Erase Reset, 2=Illegal CMD, 3=CRC Error, 4=Erase_Seq_Error, 5=Address Error, 6=Param Error
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS: NA
//
//  RETURNS:
//    Returns R1 command response.
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------

uchar SDCard_GetR1(void)
{
   uchar   status = 0;
   uint8 test1,test2,test3;
   
   // Send a null byte to shift for btye read
   SDCard_SendTxData( 0xFF );
   SDCard_XferWait();                     // Wait for transfer to complete
   rbuff = SDCard_bReadRxData();
   test1 = SDCard_bReadRxData();
   
   // Send a null byte to shift for btye read
   SDCard_SendTxData( 0xFF );
   SDCard_XferWait();                     // Wait for transfer to complete
   
   // Status bits 0=Idle, 1=Erase Reset, 2=Illegal CMD, 3=CRC Error, 4=Erase_Seq_Error, 5=Address Error, 6=Param Error
   status = SDCard_bReadRxData();      // Read the status register and return it
   test2 = SDCard_bReadRxData();

   SDCard_SendTxData( 0xFF );
   SDCard_XferWait();                     // Wait for transfer to complete
   rbuff = SDCard_bReadRxData();
   test3 = SDCard_bReadRxData();

   return(status);
}

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   uint SDCard_GetR2(void)
//
// DESCRIPTION:
//   Gets R2 command response (do not use for types (R1, R1b, or R3)
//   Status bits 0=Idle, 1=Erase Reset, 2=Illegal CMD, 3=CRC Error, 4=Erase_Seq_Error, 5=Address Error, 6=Param Error
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS: NA
//
//  RETURNS:
//    Returns R2 command response.
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------

uint SDCard_GetR2(void)
{
   // Gets R2 command response (do not use for types (R1, R1b, or R3)

   uint   status = 0;
   uchar   temp;                     // Variable for paritial result
   
   // Send a null byte to shift for btye read
   SDCard_SendTxData( 0xFF );
   SDCard_XferWait();                       // Wait for transfer to complete
   rbuff = SDCard_bReadRxData();
   
   // Send a null byte to shift for btye read
   SDCard_SendTxData( 0xFF );
   SDCard_XferWait();                       // Wait for transfer to complete
   
   // Status bits 0=Idle, 1=Erase Reset, 2=Illegal CMD, 3=CRC Error, 4=Erase_Seq_Error, 5=Address Error, 6=Param Error
   temp = SDCard_bReadRxData();      // Read the status register and return it
   status = ((uint)temp)<<8;          // Put result in upper byte
   
   // Send a null byte to shift for btye read
   SDCard_SendTxData( 0xFF );
   SDCard_XferWait();                        // Wait for transfer to complete
   
   // Status bits 0=Idle, 1=Erase Reset, 2=Illegal CMD, 3=CRC Error, 4=Erase_Seq_Error, 5=Address Error, 6=Param Error
   status += (uint)SDCard_bReadRxData();   // Read the status register, add it to upper, and return it

   return(status);
}

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   void  SDCard_EndCmd(void)
//
// DESCRIPTION:
//   Terminates the end of a card command string
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS: NA
//
//  RETURNS: NA
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------

void  SDCard_EndCmd(void)
{
   // Send a null byte (Min 8 clock cycles required)
   SDCard_SendTxData( 0xFF ); 
   SDCard_XferWait();                    // Wait for transfer to complete
   rbuff = SDCard_bReadRxData();
         
   SDCard_Select(SDCard_DISABLE);    // Set SD Card select line high (disables card for SPI)
   SDCard_Wait();                        // Wait a minimum setup time for the high/low transition
}


//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   void SDCard_Wait(void)
//
// DESCRIPTION:
//    Waits for a certain amount of time.  // REVIEW be more specific.
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//
//  RETURNS:
//
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------

void SDCard_Wait(void)
{
   uchar   x;               // Wait loop variable

   if(SDCard_Speed == 0)            // Wait longer if the speed is slower
      CyDelay(100);
	  //for(x=0; x<10; x++);   // Wait a minimum setup time
}

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   void SDCard_Wait2(void)
//
// DESCRIPTION:
//    Waits for a certain amount of time.  // REVIEW be more specific.
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//
//  RETURNS:
//
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------

void SDCard_Wait2(void)
{
   //uint   x;                    // Wait loop variable
   //for(x=0; x<40000; x++);      // Wait a minimum setup time
   CyDelay(200);
}


//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   uchar SDCard_Cmd_00(void)
//
// DESCRIPTION:
//    Sends the six byte reset command with 7 bit CRC of 0x95 (Byte String = 0x40 0x00 0x00 0x00 0x00 0x95) 
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:  NA
//
//  RETURNS:
//     Status bits: 0=Idle, 1=Erase Reset, 2=Illegal CMD, 3=CRC Error, 4=Erase_Seq_Error, 5=Address Error, 6=Param Error
//
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------

uchar SDCard_Cmd_00(void)
{
   uchar   status = 0;
   uint   x;

   for(x=0; x<2; x++)   
   {
      // Send a null byte to shift for byte read
      SDCard_SendTxData( 0xFF );
      SDCard_XferWait();               // Wait for transfer to complete
	  rbuff = SDCard_bReadRxData();
   }

   SDCard_Wait2();                     // Wait for settling time
   
   SDCard_Select(SDCard_ENABLE);  // Set SD Card select line low (enables card for SPI)

   SDCard_Wait();                      // Wait for settling time

   for(x=0; x<6; x++)           // Loop through the command sequence
   {
      // Send a byte
      SDCard_SendTxData( SDCard_CmdStr00[x] );
      SDCard_XferWait();               // Wait for transfer to complete
	  rbuff = SDCard_bReadRxData();
   }
   
   // Status bits: 0=Idle, 1=Erase Reset, 2=Illegal CMD, 3=CRC Error, 4=Erase_Seq_Error, 5=Address Error, 6=Param Error
   status = SDCard_GetR1();            // Get the R1 command respnse   

   SDCard_EndCmd();                    // Finish the command cycle

   return(status);
}

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   uchar SDCard_Cmd_01(void)
//
// DESCRIPTION:
//    CMD1 - MMC operational condition command
//    Byte String = 0x41 0x00 0x00 0x00 0x00 0xFF 
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS: NA
//
//  RETURNS:
//    Status bits: 0=Idle, 1=Erase Reset, 2=Illegal CMD, 3=CRC Error, 4=Erase_Seq_Error, 5=Address Error, 6=Param Error
//
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------

uchar SDCard_Cmd_01(void)
{

   uchar   status = 0;

   SDCard_Cmd(SDCard_CMD1, SDCard_NOARGS);               // Send CMD1 command string
   
   // Status bits: 0=Idle, 1=Erase Reset, 2=Illegal CMD, 3=CRC Error, 4=Erase_Seq_Error, 5=Address Error, 6=Param Error
   status = SDCard_GetR1();                // Get the R1 command respnse   

   SDCard_EndCmd();                        // Finish the command cycle

   return(status);
}

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   uchar SDCard_ACmd_41(void)
//
// DESCRIPTION:
//   ACMD41 - SD operational condition command
//   Byte String = 0x69 0x00 0x00 0x00 0x00 0xFF 
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:  NA
//
//  RETURNS:
//    Status bits: 0=Idle, 1=Erase Reset, 2=Illegal CMD, 3=CRC Error, 4=Erase_Seq_Error, 5=Address Error, 6=Param Error
//
//
//  SIDE EFFECTS:
//    NOTE: REQUIRES a CMD55 before it does an ACMD41
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------

uchar SDCard_ACmd_41(void)
{
   uchar   status = 0;
   
   SDCard_Cmd(SDCard_ACMD41, SDCard_NOARGS);         // Send ACMD41 command string

   // Status bits: 0=Idle, 1=Erase Reset, 2=Illegal CMD, 3=CRC Error, 4=Erase_Seq_Error, 5=Address Error, 6=Param Error
   status = SDCard_GetR1();            // Get the R1 command respnse   

   SDCard_EndCmd();                    // Finish the command cycle

   return(status);
}

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   uchar SDCard_Cmd_55(void)
//
// DESCRIPTION:
//   CMD55 - SD application command (next command is an ACMD)
//   Byte String = 0x77 0x00 0x00 0x00 0x00 0xFF 
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS: NA
//
//  RETURNS:
//    Status bits: 0=Idle, 1=Erase Reset, 2=Illegal CMD, 3=CRC Error, 4=Erase_Seq_Error, 5=Address Error, 6=Param Error
//
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------

uchar SDCard_Cmd_55(void)
{
   uchar   status = 0;

   SDCard_Cmd(SDCard_CMD55, SDCard_NOARGS);         // Send CMD55 command string

   // Status bits: 0=Idle, 1=Erase Reset, 2=Illegal CMD, 3=CRC Error, 4=Erase_Seq_Error, 5=Address Error, 6=Param Error
   status = SDCard_GetR1();           // Get the R1 command respnse   

   SDCard_EndCmd();                   // Finish the command cycle

   return(status);
}

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   uint SDCard_Cmd_13(void)
//
// DESCRIPTION:
//   CMD13 - Get card status command
//   Byte String = 0x4D 0x00 0x00 0x00 0x00 0xFF 
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:  NA
//
//  RETURNS:
//    Status bits: 0=Idle, 1=Erase Reset, 2=Illegal CMD, 3=CRC Error, 4=Erase_Seq_Error, 5=Address Error, 6=Param Error
//
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------

uint SDCard_Cmd_13(void)
{
   uint   status = 0;

   // CMD13 - Get card status command
   // Byte String = 0x4D 0x00 0x00 0x00 0x00 0xFF 

   SDCard_Cmd(SDCard_CMD13, SDCard_NOARGS);       // Send CMD55 command string

   // Status bits: 0=Idle, 1=Erase Reset, 2=Illegal CMD, 3=CRC Error, 4=Erase_Seq_Error, 5=Address Error, 6=Param Error
   status = SDCard_GetR2();         // Get the R2 command respnse   

   SDCard_EndCmd();                 // Finish the command cycle

   return(status);
}

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   ulong SDCard_GetTable(void)
//
// DESCRIPTION:
//   Sends the sector read card command for the boot parameter table
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS: NA
//
//  RETURNS:
//     Returns zero for error - Boot sector address for success 
//
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------

ulong SDCard_GetTable(void)
{
   ulong   BootStart=0;             // Clear BootStart value
   uchar   status = 0;
   uchar   x;                       // Loop variables

   SDCard_SetSize(64);                     // Set block length to 64 for partition table reads
   
   // CMD17 - Read single block command
   // Byte String = 0x51 (ulong Address) 0xFF 

   SDCard_Cmd(SDCard_CMD17, 0x000001BE);          // Send CMD17 command string

////LCD_Position(1, 0);
////LCD_PrintString("CMD             ");
////LCD_PrintInt8(0);

   // Check response
   if(SDCard_CheckReply(0) != SDCard_FAIL)        // Check for status good
   {
      if(SDCard_CheckReply(0xFE) != SDCard_FAIL)  // Set check to Single Block Read data token
      {
                                    // Get first 64 bytes and save them
   
         SDCard_ReadBuff(SDCard_Buffer1, 32 );
         SDCard_ReadBuff(SDCard_Buffer2, 32 );
         
         // Currently assume only one partition table here
         
         // Get starting sector of the disk (Boot Sector address)and convert from array

////LCD_Position(1, 0);
////LCD_PrintString("BUFFERfailnot   ");
////LCD_PrintInt8(0);

         for(x=11; x>=8; x--)
         {
            BootStart <<=8; 
            BootStart +=  SDCard_Buffer1[x];
         }

         BootStart <<=9;            // Multiply by sector size (512)
      }
      else
         status = SDCard_FAIL;             // Set status to failed
   }
   else
      status = SDCard_FAIL;                // Set status to failed

////LCD_Position(1, 0);
////LCD_PrintString("BUFFERfailyes   ");
////LCD_PrintInt8(0);

   SDCard_EndCmd();                        // Finish the command cycle

////LCD_Position(1, 0);
////LCD_PrintString("BUFFERENDDDDD   ");
////LCD_PrintInt8(0);

   return(BootStart);               // Zero on error, BootStart on success
}

//=================================================================================================
//      CARD DISK DRIVE FUNCTIONS - Routines to setup and use the disk drive functions of the card
//=================================================================================================

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   uchar SDCard_GetBoot(ulong BootStart)
//
// DESCRIPTION:
//   Sends the sector read card command for the boot sector (uses the value in BootStart) 
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:  //  REVIEW clarify
//
//  RETURNS:
//    Pass or fail status  // REVIEW clarify
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//  Fat 16 Boot Sector
//  Boot Sector information - only the first sixty four bytes are needed (rest is boot code)
//
//   0x000     3      Jump to boot code                 - 0xEB 0xXX 0x90 (relative jump)
//   0x003     8      OEM name                          - Manufactures ASCII String
//   0x00B     2      Number of bytes per sector        - (512) Standard
//   0x00D     1      Number of sectors per cluster     - (1-64)Usually one
//   0x00E     2      Reservered sectors                - (1) Boot sector
//   0x010     1      Number of FAT tables              - (2) Standard
//   0x011     2      Number of root directory enries   - (512) Standard
//   0x013     2      Number of sectors on media        - (1-64K) if 0 use 0x020 (Total Sectors)
//   0x015     1      Media descriptor                  - (0xF8) Hard Disk
//   0x016     2      Number of sectors per FAT         - (1-64K) Depends on disk size
//   0x018     2      Number of sectors per track       - (32) No meaning
//   0x01A     2      Number of Heads                   - (2)  No meaning
//   0x01C     4      Number of hidden sectors          - (0)
//   0x020     4      Number of total sectors           - (0xXXXX) If 0x013 is 0 only
//   0x024     1      Drive number                      - (0x80) Standard
//   0x025     1      Reserved                          - Not used
//   0x026     1      Extended boot signature           - (0x29) Standard
//   0x027     4      Volume ID/Serial number           - BCD string
//   0x02B    11      Volume label                      - ASCII string
//   0x036     8      File System Type                  - ASCII string 'FAT16   '
//   0x03E   448      Boot code (if present)            - Boot code
//   0x1FE     2      Signature                         - (0x55AA) Standard
//
//  NOTE: For FAT 16 only the first 64 bytes (0x40) contain data necessary for disk access
//        For FAT 32 access must be able to read 0x052

//
//-------------------------------------------------------------------------------------------------
#ifdef ENABLE_FILESYSTEM

uchar SDCard_GetBoot(ulong BootStart)
{
   uchar   status = 0;
   uint    TempInt;       // Temporary int  for intermediate calculations
   uint   x;              // Loop variable
   ulong   TempLong = 0;  // Temporary long for intermediate calculations
   SDCard_SetSize(512);          // Set block length to 512 for boot sector reads

   if(BootStart == 0)                      // Is boot address valid?
      status = SDCard_FAIL;                       // No, set error flag
   else
   {
      // CMD17 - Read single block command
      // Byte String = 0x51 (ulong Address) 0xFF 
      SDCard_Cmd(SDCard_CMD17, BootStart);               // Send CMD17 command string
   
      // Check response
      if(SDCard_CheckReply(0) != SDCard_FAIL)            // Check for status good
      {
         if(SDCard_CheckReply(0xFE) != SDCard_FAIL)      // Set check to Single Block Read data token
         {
            // Get first 64 bytes and save them
            SDCard_ReadBuff256(SDCard_Buffer1);
            SDCard_ReadBuff256(SDCard_Buffer2);
   
            // Save the number of sectors pur cluster (number of sectors per FAT entry assigned)
            SDCard_ClusterSize = SDCard_Buffer1[0x0D];   // Save cluster size

            // Calculate the start of the first FAT table (# of boot sectors * 512 plus Boot start address)
            TempInt  =  SDCard_Buffer1[0x0F];     // Get first byte of array (# of boot sectors)
            TempInt <<= 8;                 // Put in upper byte
            TempInt +=  SDCard_Buffer1[0x0E];     // Add in lower byte to make an int
            SDCard_Fat1Start = ( ( (ulong) TempInt * 512) + BootStart );

            // Double check FAT type info
            if( (SDCard_Buffer1[0x36]== 'F')&&(SDCard_Buffer1[0x37]== 'A')&&(SDCard_Buffer1[0x38]== 'T') )
            {
               if( (SDCard_Buffer1[0x39]== '1')&&(SDCard_Buffer1[0x3A]== '2') )
               {
                  if((SDCard_CardType & SDCard_XFAT) != 0x10)
                  {
                     // If not a FAT12 file system - make it a FAT12
                     SDCard_CardType &= SDCard_XCARD;
                     SDCard_CardType |= 0x10;
                  }
               }
               else if( (SDCard_Buffer1[0x39]== '1')&&(SDCard_Buffer1[0x3A]== '6') )
               {
                  // FAT16 file system - make it a FAT16
                  SDCard_CardType &= SDCard_XCARD;
                  SDCard_CardType |= 0x40;
               }
               else
               {
                  // If not a valid FAT file system - zero FAT type
                  SDCard_CardType &= SDCard_XCARD;
               }
               
            }
            else if( (SDCard_Buffer1[0x52]== 'F')&&(SDCard_Buffer1[0x53]== 'A')&&(SDCard_Buffer1[0x54]== 'T') )
            {
               if( (SDCard_Buffer1[0x55]== '3')&&(SDCard_Buffer1[0x56]== '2') )
               {
                  if((SDCard_CardType & SDCard_XFAT) != 0xB0)
                  {
                     // If not a FAT32 file system - make it a FAT32
                     SDCard_CardType &= SDCard_XCARD;
                     SDCard_CardType |= 0xB0;
                  }
               }
            }
            else
            {
               // If not a FAT file system - zero FAT type
               if((SDCard_CardType & SDCard_XFAT) != 0)
                  SDCard_CardType &= SDCard_XCARD;
            }
            
            // Set FAT size
            SDCard_FatEnd = 0xFFFF;               // Set to FAT16 FAT parameters
            SDCard_FatEntrySize = 2;   

#ifdef ENABLE_FAT32
         // If the file type is FAT32, set the parameters to match
         if((SDCard_CardType & SDCard_XFAT) == 0xB0)
         {
            SDCard_FatEnd = 0x0FFFFFFF;           // Set to FAT32 FAT parameters
            SDCard_FatEntrySize = 4;
            SDCard_DirSize = 0xFFFE;              // Set max directory size (for int value - 0xFFFF reserved)   
         }
#endif                        
            // If FAT12 or FAT6 use basic boot sector table and formula
            if(((SDCard_CardType & SDCard_XFAT) == 0x10)||((SDCard_CardType & SDCard_XFAT) == 0x40)||((SDCard_CardType & SDCard_XFAT) == 0x60))
            {            
               // Calculate the start of the Root Directory ( Fat size * # of Fats * 512 plus Fat start address)
               TempInt  =  SDCard_Buffer1[0x17];  // Get first byte of array (FAT size)
               TempInt <<= 8;              // Put in upper byte
               TempInt +=  SDCard_Buffer1[0x16];  // Add in lower byte to make an int
               if(SDCard_Buffer1[0x10] >= 2)
                  SDCard_Fat2Start = SDCard_Fat1Start + ((ulong)TempInt * 512);
               SDCard_DirStart = ( ( (ulong) TempInt * SDCard_Buffer1[0x10] * 512 ) + SDCard_Fat1Start );
               SDCard_DirFat = 0;                 // Root Directory value is fixed - Use DirStart and length of 512 entries
               SDCard_CurStart[SDCard_MAXFILES] = 0;     // Set root FAT entry to zero - No FAT directory entry

               // Calculate start of data area (root size(usually 512) * entry size(32) plus Dir start
               TempInt  =  SDCard_Buffer1[0x12];  // Get first byte of array (Fat size)
               TempInt <<= 8;              // Put in upper byte
               TempInt +=  SDCard_Buffer1[0x11];  // Add in lower byte to make an int
               SDCard_DataStart = ( ( (ulong) TempInt * 32 ) + SDCard_DirStart );
            }
#ifdef ENABLE_FAT32
            // If FAT32 use new 32 bit boot sector table and formula
            if((SDCard_CardType & SDCard_XFAT) == 0xB0)
            {            
               // Calculate start of data area (Sectors per FAT * Number of FATs * Sector size) + Fat start
               for(x=0x27; x>=0x24; x--)
               {
               TempLong <<= 8;             // Put in next upper byte
               TempLong +=  SDCard_Buffer1[x];    // Add in next byte to make a long
               }

               if(SDCard_Buffer1[0x10] >= 2)
                  SDCard_Fat2Start = SDCard_Fat1Start + (TempLong * 512);
               SDCard_DataStart = ( ( TempLong * SDCard_Buffer1[0x10] * 512) + SDCard_Fat1Start );

               // Calculate the start of the Root Directory ((Fat entry - 2)* 512 plus Data start address)
               // Note: In FAT32 the directory is actually a file using a FAT chain.
               for(x=0x2F; x>=0x2C; x--)
               {
               TempLong <<= 8;             // Put in next upper byte
               TempLong +=  SDCard_Buffer1[x];    // Add in next byte to make a long
               }

               SDCard_DirFat   = TempLong;        // Root Directory starting FAT entry (directory size is length of FAT chain)
               SDCard_DirStart = ( ((TempLong - 2) * 512 ) + SDCard_DataStart);               
               SDCard_CurStart[SDCard_MAXFILES] = TempLong;   // Set directory to root for start      // Set root FAT entry to zero - No FAT directory entry
            }
#endif

         }
         else
            status = SDCard_FAIL;         // Set status to failed
      }
      else
         status = SDCard_FAIL;            // Set status to failed
   }
   
   SDCard_EndCmd();                       // Finish the command cycle

   return(status);
}
#endif

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   uchar SDCard_CheckReply(uchar check)
//
// DESCRIPTION:
//   Checks reply with timeout
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//    check => Reply match value
//
//  RETURNS:
//    Pass/Fail status.  // REVIEW clarify
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------

uchar SDCard_CheckReply(uchar check)
{
   uchar   status = 0;                        // Set initial status
   uchar   Timeout = 255;                     // Set number of tries until you timeout   
   uchar   Reply = check + 1;                 // Make sure check and Reply don't initially match

   while((check != Reply) && (Timeout>0))     // Loop until check = Reply, or a timeout occurs
   {
      // Send a null byte to shift for btye read
      SDCard_SendTxData( 0xFF );
      SDCard_XferWait();                             // Wait for transfer to complete

////LCD_Position(1, 0);
////LCD_PrintString("Check Reply");
////LCD_PrintInt8(0);   

	  Reply = SDCard_bReadRxData(); // Read the reply

////LCD_Position(1, 0);
////LCD_PrintString("Reply:");
////LCD_PrintInt8(Reply);

	  Timeout--;                              // Decrement loop until 0, then timeout
      if(Timeout == 0)                        // If loop is zero, then set status for failed
         status = SDCard_FAIL;
   }
   return(status);
}

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   uchar SDCard_SetSize(ulong size)
//
// DESCRIPTION:   // REVIEW Get description
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//
//  RETURNS:
//
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------

uchar SDCard_SetSize(ulong size)
{
   uchar  status = 0;

   // CMD16 - Set block size command
   // Byte String = 0x50 0x00 0x00 0x00 0x00 0xFF 

   SDCard_Cmd(SDCard_CMD16, size);      // Send CMD16 command string

   // Status bits: 0=Idle, 1=Erase Reset, 2=Illegal CMD, 3=CRC Error, 4=Erase_Seq_Error, 5=Address Error, 6=Param Error
   status = SDCard_GetR1();      // Get the R1 command respnse   

   SDCard_EndCmd();              // Finish the command cycle
               
   return(status);
}


//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   void SDCard_XferWait(void)
//
// DESCRIPTION:
//   Wait until transfer is complete
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS: NA
//
//  RETURNS: NA
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------

void SDCard_XferWait(void)
{
	uint8 stat;
	
	// Wait until transfer is complete
	stat = SDCard_bReadStatus();
	while( !(stat > 0) )
	{
		stat = SDCard_bReadStatus();
	}
}

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   void SDCard_NextFat(uchar Fptr)
//
// DESCRIPTION:
//   Searches the FAT chain for the next cluster to be used in the file and updates the file 
//   information CurFat to match. If last FAT entry in chain, write adds FAT, read does nothing.
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//    Fptr => File Pointer
//
//  RETURNS: NA
//
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------
#ifdef ENABLE_FILESYSTEM

void SDCard_NextFat(uchar Fptr)
{
#ifdef ENABLE_FAT32
   ulong  SaveFat = SDCard_CurFat[Fptr];      // Save current FAT entry
#else
   uint   SaveFat = SDCard_CurFat[Fptr];      // Save current FAT entry
#endif

   if(SDCard_CurFat[Fptr]!= SDCard_FatEnd)           // If not the last FAT entry, get the next FAT entry
   {
      SDCard_CurFat[Fptr]  = SDCard_FatTrack(SDCard_CurFat[Fptr]);   // Get next FAT entry number
#ifdef ENABLE_WRITE
      if( (SDCard_CurFat[Fptr] == SDCard_FatEnd)&&((SDCard_FileMode[Fptr]& 0x01) != 0x01) )
      {
         SDCard_CurFat[Fptr] = SaveFat;       // Restore original current FAT entry
         SDCard_AddFat(Fptr);                 // Add a new FAT entry to the chain 
         SDCard_CurFat[Fptr] = SDCard_FatTrack(SDCard_CurFat[Fptr]);   // Get next FAT entry number
      }
#endif
      SDCard_CurSect[Fptr] = 0;               // Reset sector count to zero - start of a new cluster
   }
}
#endif

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   ulong SDCard_FatTrack(ulong FatNum)
//                   uint SDCard_FatTrack(uint FatNum)
//
// DESCRIPTION:
//   Searches the FAT chain for the next cluster to be used and returns the next FAT entry value. 
//   If the FatNum = 0; then a search is done for the first free FAT entry.   
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//    FatNum => The last FAT entry value.
//
//  RETURNS:
//    The next FAT entry value to be used.
//
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------
#ifdef ENABLE_FILESYSTEM

#ifdef ENABLE_FAT32
ulong SDCard_FatTrack(ulong FatNum)
#else
uint SDCard_FatTrack(uint FatNum)
#endif
{
   // Searches the FAT chain for the next cluster to be used and returns the next FAT entry value. 
   // If the FatNum = 0; then a search is done for the first free FAT entry.   

#ifdef ENABLE_FAT32
   ulong  Nfat;           // Next FAT entry number
#else
   uint   Nfat;           // Next FAT entry number
#endif
   uchar   status = 0;
   uchar   x;             // Loop variable (signed for check later)
   uchar   Loop=1;        // Loop control
   uchar   Tmp[4];        // Temp buffer
   ulong   address;       // Temp address variable
   
   if(FatNum == 0)        // Set loop control to search until an unused FAT entry is found
   {
      Loop   = 2;         // Set loop to search for unused FAT entry
      FatNum = SDCard_EmptyFat;  // Start search at first FAT or last known empty FAT entry
   }

   SDCard_SetSize(SDCard_FatEntrySize); // Set block length back to read FAT table (2 for FAT16, 4 for FAT32)

   while(Loop != 0)
   {
      // Address for current FAT equals cluster in FAT + sector offset * sector size

#ifdef ENABLE_FAT32
      if(SDCard_FatEntrySize == 2)
         address = (SDCard_Fat1Start + (FatNum << 1));   // Fat16
      else
         address = (SDCard_Fat1Start + (FatNum << 2));   // FAT32
#else
      address = (SDCard_Fat1Start + (FatNum << 1));      // FAT16
#endif

      // CMD17 - Read single block command
      // Byte String = 0x51 (ulong Address) 0xFF 
   
      SDCard_Cmd(SDCard_CMD17, address);             // Send CMD17 command string

      // Check response
      if(SDCard_CheckReply(0) != SDCard_FAIL)        // Check for status good
      {
         if(SDCard_CheckReply(0xFE) != SDCard_FAIL)  // Set check to Single Block Read data token
         {
         
            SDCard_ReadBuff(Tmp, SDCard_FatEntrySize );

            Nfat = 0;                  // Clear value before use
            for(x=1; x<=SDCard_FatEntrySize; x++)
            {
               Nfat <<=8;              // Shift to upper byte
               Nfat += Tmp[SDCard_FatEntrySize-x];   // Add next byte
            }
         
         }
         else
            status = SDCard_FAIL;             // Set status to failed
      }
      else
         status = SDCard_FAIL;                // Set status to failed

      SDCard_EndCmd();

      if(Loop == 2)
      {
         if(Nfat == 0)
         {
            Nfat = FatNum;       // Return the first unused FAT entry number
            SDCard_EmptyFat = FatNum;   // Next time start search where it left off
            Loop = 0;            // Break the while loop when unused FAT entry is found
         }
         else
            FatNum++;            // No match yet, check next FAT entry
      }
      else
         Loop = 0;               // Only one pass through the loop
   }   
   return(Nfat);                 // Return next FAT entry (or first unused FAT entry)
}
#endif

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   ulong SDCard_LastFat(uchar Fptr)
//                   uint SDCard_LastFat(uchar Fptr)
//
// DESCRIPTION:
//   Returns the last FAT entry used in a file chain, returns zero if no FAT chain found
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//    Fptr => File pointer
//
//  RETURNS:
//    Last FAT entry used.
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------
#ifdef ENABLE_FILESYSTEM

#ifdef ENABLE_FAT32
ulong SDCard_LastFat(uchar Fptr)
#else
uint SDCard_LastFat(uchar Fptr)
#endif
{
   
#ifdef ENABLE_FAT32
   ulong Nfat;               // Temp next FAT entry variable
   ulong Cfat=0;             // Temp current FAT entry variable
#else
   uint Nfat;                // Temp next FAT entry variable
   uint Cfat=0;              // Temp current entry FAT variable
#endif

   if(SDCard_CurStart[Fptr]!=0)     // Valid FAT entry?
   {
      // Yes, seach the last FAT   
      Nfat = SDCard_CurFat[Fptr];   // Set next FAT entry to the current FAT entry (valid by default)
   
      while(Nfat != SDCard_FatEnd)
      {
         // Loop until the last FAT entry is reached
         Cfat = Nfat;         // Set the current FAT entry to the previous 'Next FAT' value
         Nfat = SDCard_FatTrack(Cfat);   // Get the next FAT entry value
      }
   }
   return(Cfat);             // Return last FAT - return zero if no FAT chain found
}
#endif

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   uchar SDCard_NextSect(uchar Fptr)
//
// DESCRIPTION:
//   Increments the next cluster to be used in the file and the next FAT if necessary
//   in the file. (Will automatically allign CurOffset to the sector start.
//   updates CurSect, CurOffset, and CurFat as required.
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//    Fptr => Current file pointer
//
//  RETURNS:
//    Pass/Fail status  // REVIEW clarify
//
//
//  SIDE EFFECTS:
//    Note:   This routine will allow writes beyond the stated CurSize value as whole
//            sectors must be written. Data values beyond file end are not guaranteed.
//            Also be aware that a whole clusters are assigned not matter how little 
//            of last one is used, and total access is possible during file copies.
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------
#ifdef ENABLE_FILESYSTEM

uchar SDCard_NextSect(uchar Fptr)
{
   uchar   status = 0;
   
   if(SDCard_CurSect[Fptr] < (SDCard_ClusterSize -1))
      SDCard_CurSect[Fptr]++;                 // If less than the last sector - increment sector
   else
   {
      SDCard_NextFat(Fptr);                   // Get next fat entry
      if(SDCard_CurFat[Fptr] != SDCard_FatEnd)       // Was this the last FAT entry?
         SDCard_CurSect[Fptr] = 0;            // No, Reset sector count
      else
      {
         // Read Mode - At or past EOF
         status = SDCard_FAIL;                // Error - Already at last FAT entry
         SDCard_FileError[Fptr] |= SDCard_EOF;       // Set the end of file flag
      }
   }
   if(status == 0)
   {
      // If no errors update the CurOffset to match
      SDCard_CurOffset[Fptr] += 512;          // Add the sector to the offset
      SDCard_CurOffset[Fptr] &= 0xFFFFFE00;   // Allign the sector boundry
   }
   return(status);
}
#endif


//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   uchar SDCard_FindFile(uchar Fptr, uint FileNum)
//
// DESCRIPTION:
//   Finds a file in the root directory. 
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
// Fptr    - If less than MAXFILES then the pointer is used for file info, if it equals maxfiles no 
//           file information is passed. (Used for just returning filenames)
//
// FileNum - If zero, it searches by filename, matching the first 11 characters in Buffer1 to Buffer2
//           If non-zero it searches by file number, if number is greater than the number of  
//           files no match will be found. (Only valid files are counted). (A limit of 0xFFFE valid entries)
//           If 0xFFFF it searches for the first unused directory entry (E5 or 00 for first character)
//
//  RETURNS:
//    Number of files found at point of match, or all valid files if no match found.
//    Note: Using a FileNum >= 512/0xFFFE will always result in no match, returning a valid file count.
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------
#ifdef ENABLE_FILESYSTEM

uchar SDCard_FindFile(uchar Fptr, uint FileNum)
{

   // Sends the sector read card command for the Directory and reads directory entries into buffer 1 until
   // Buffer1 it finds a match for the first 11 characters of Buffer2 and confirms that this is a 
   // valid filename. (not a sub-directory or a volume label).
   
   uchar   match  = 0;   // Set to 1 if a match is found
   uchar   end = 0;      // Flag for directory end found
   uchar   x;            // Loop variables
   uint    y = 0;
   uint    files = 0;    // Current count of valid files
   uchar   status = 0;   // Temp status variable
   
//   SDCard_SetSize(32);        // Set block length to 32 for directory reads

//  Directory Entry information - 32 bytes each for short file names (long filenames not supported)
//
//   0x00     8      Filename   - 8 Uppercase ASCII characters, right padded with spaces
//                                (First character special, 0x00=Unused, 0x2E=Sub-directory, 0xE5=Erased)
//   0x08     3      Extension  - 3 Uppercase ASCII characters, right padded with spaces
//   0x0B     1      Attribtes  - File attributes - bits 0-5 used, bits 6 and 7 are reserved
//                                (Bits 0=Read Only, 1=Hidden, 2=System, 3=Volume Label, 4=SubDir, 5=Archive)
//   0x0C     10     Reserved bytes
//  (0x14)    2     (Upper two bytes of FAT for FAT32 entry)
//   0x16     2      Creation/modification time in compressed format
//   0x18     2      Creation/modification date in compressed format
//   0x1A     2      Starting FAT entry in FAT table (first FAT entry is always 2 not 0)
//   0x1C     4      File size (long)

   if(SDCard_CurStart[SDCard_MAXFILES] != 0)      
   {
      // Directory is FAT based - set up FAT values
      SDCard_CurFat[SDCard_MAXFILES] = SDCard_CurStart[SDCard_MAXFILES];
      SDCard_CurSect[SDCard_MAXFILES] = 0;
      SDCard_CurOffset[SDCard_MAXFILES] = 0;   
   }

   while((match==0)&&(end==0))           // Search directory until match in Buffer2 is found
   {
      if(SDCard_CurStart[SDCard_MAXFILES] == 0)
         SDCard_Address = SDCard_DirStart + (y * 32);  // Calculate directory entry to try - Fixed size of 512 entries
      else
      {
         SDCard_GetAddress(SDCard_MAXFILES, 1);        // Calculate directory address
      }
      
      SDCard_SetSize(32);                       // Set block length to 32 for directory reads
      
      // CMD17 - Read single block command
      // Byte String = 0x51 (ulong Address) 0xFF 

      SDCard_Cmd(SDCard_CMD17, SDCard_Address);               // Send CMD17 command string
   
      // Check response
      if(SDCard_CheckReply(0) != SDCard_FAIL)          // Check for status good
      {
         if(SDCard_CheckReply(0xFE) != SDCard_FAIL)    // Set check to Single Block Read data token
         {
            // Get directory entry and save it   
            SDCard_ReadBuff(SDCard_Buffer1, 32 );

         }
         else
            status |= SDCard_CE;                // Card Error - Set status to failed
      }
      else
         status |= SDCard_CE;                   // Card Error - Set status to failed

      SDCard_EndCmd();                          // Finish the command cycle

      if(status == 0)
      {

         // If first byte is zero and not looking for unused directories - must be directory end
         if((SDCard_Buffer1[0]==0)&&(FileNum != SDCard_UNUSED))
            end=1;      // Set end of directory flag

         if(SDCard_CurStart[SDCard_MAXFILES] == 0)
         {
            // Fixed directory length of 512 entries
            y++;
            if(y >=512)
               end=1;   // Set end of directory flag
         }
         else
         {
            // Variable entry size - Fat based
            SDCard_CurOffset[SDCard_MAXFILES] += 0x20;    // Add directory size to offset
            if(SDCard_CurOffset[SDCard_MAXFILES] >= 512)  // If more than a sector update 
            {
               SDCard_CurOffset[SDCard_MAXFILES] = 0;     // Reset count
               // Increment sector
               SDCard_CurSect[SDCard_MAXFILES]++;         // Increment sector count
               if(SDCard_CurSect[SDCard_MAXFILES] >= SDCard_ClusterSize)
               {
                  SDCard_CurSect[SDCard_MAXFILES] = 0;    // Reset count
                  // Increment FAT entry
                  SDCard_NextFat(SDCard_MAXFILES);        // Get next FAT entry
                  if(SDCard_CurFat[SDCard_MAXFILES] == SDCard_FatEnd)   // Was this the last FAT entry?
                     end=1;                 // Yes, end of directory chain
               }
            }            
         }
            
         // Check for valid filename - first character must be upper ASCII and attributes set to file 
         if((SDCard_Buffer1[0]>='A')&&(SDCard_Buffer1[0]<='Z')&&((SDCard_Buffer1[0x0B] & 0x18) == 0))
         {
            files++;              // Valid filename - increment valid files

            if(FileNum == 0)      // Match by filename or filenumber?
            {
               // Compare the filenames in the first 11 bytes of Buffer1 and Buffer2
               x = 0;             // Init compare count
               while( (SDCard_Buffer1[x] == SDCard_Buffer2[x]) && (x < 11) )
                  x++;            // get next byte to match
               if(x>10)           // Check if filename matched
                  match = 1;      // Match found exit and save values
            }
            else
            {
               if(files == FileNum)  // Does the file number match?
                  match = 1;         // Match found exit and save values
            }
            
         }
         else if(FileNum == SDCard_UNUSED)  // Check for unused entries?
         {
            if((SDCard_Buffer1[0] == 0xE5)||(SDCard_Buffer1[0] == 0x00))
            {
               // Found unused directory entry to use/reuse
               files++;              // Valid filename - increment valid files
               match = 1;            // Match found exit and save values
            }
         }
      }
      else
         end=1;   // Read error - Set end of directory flag
   }
      
   // Search complete - either file was found or it was not
   
   if(Fptr<SDCard_MAXFILES)                 // If true - set file information
   {
      SDCard_FileError[Fptr] |= status;     // Set file error flags depending on status

      if(match==1)                                 
      {            
         // Match found - Buffer1 contains valid data
   
         // Get starting FAT location and convert it
#ifdef ENABLE_FAT32
         if((SDCard_CardType & SDCard_XFAT) == 0xB0)   // FAT32 Values
         {
            SDCard_CurFat[Fptr]  = SDCard_Buffer1[0x15];   // Get upper FAT32 bytes and convert them
            SDCard_CurFat[Fptr] <<=8;               // Shift to upper byte
            SDCard_CurFat[Fptr] += SDCard_Buffer1[0x14];   // Add next byte
            SDCard_CurFat[Fptr] <<=8;               // Shift to upper byte
         }
#endif   
         SDCard_CurFat[Fptr] += SDCard_Buffer1[0x1B];      // Add next byte to starting FAT
         SDCard_CurFat[Fptr] <<=8;                  // Shift to upper byte
         SDCard_CurFat[Fptr] += SDCard_Buffer1[0x1A];      // Add last byte
      
         SDCard_CurDir[Fptr] = SDCard_Address;             // Copy the current directory entry address
         SDCard_CurStart[Fptr] = SDCard_CurFat[Fptr];      // Copy FAT info into current FAT start

         for(x=0; x<4; x++)
         {
            SDCard_CurSize[Fptr] <<=8;              // Shift in upper byte
            SDCard_CurSize[Fptr] += SDCard_Buffer1[0x1C+(3-x)];   // Add next byte
         }

         SDCard_CurAttr[Fptr]= SDCard_Buffer1[0x0B];       // Copy file attribute byte
                  
      }
      else
      {
            SDCard_FileError[Fptr] |= SDCard_FNF;          // File Not Found - return error
      }
   }
   else if(match==0)
   {
      SDCard_Buffer1[0]=0;                     // Force Buffer1 directory error just to be sure that if no files
   }
   SDCard_BuffLoc = 0;         // Force update of buffer on next read/write (address zero is parameter table)
   return(files);       // Returns number of valid files found at time of match,
                        // or total number of valid files if no match was found.
}
#endif

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   uchar SDCard_WriteFileSect(uchar Fptr)
//
// DESCRIPTION:
//   Routine that will write one sector to a file. Offset must be set to the beginning
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//    Fptr => File pointer
//
//  RETURNS:
//     Pass/Fail status
//
//  SIDE EFFECTS:
//    Routine that will write one sector to a file. Offset must be set to the beginning
//    of a valid sector boundry before the call is made. (Multiple of 512, or zero)
//    It DOES NOT check to see if the file is at least one sector long - non-zero.
//    Must have an valid Fptr previously loaded by the SDCard_fopen(Filename) subroutine.
//
//              ***   Global variables required   ***     
//    ClusterSize       - the size of a cluster on the current disk
//    CurFat[Index]     - contains the starting fat entry for the file
//    CurSize[Index]    - contains the size of the file
//    CurOffset[Index]  - contains the current byte position in the file
//    CurSect[Index]    - contains the current sector in use of a cluster
//    Index             - is the passed variable of which file handle to use
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------
#ifdef ENABLE_FILESYSTEM
#ifdef ENABLE_WRITE

uchar SDCard_WriteFileSect(uchar Fptr)
{
   uchar   status = 0;                 // command status

   if((SDCard_CurFat[Fptr] != SDCard_FatEnd))
   {
      // Write out one sector

      // Address equals ((FAT entry - 2) * cluster size + sector offset) * sector size + DataStart
      SDCard_GetAddress(Fptr, 0);             // Calculate sector address

      status = SDCard_WriteSect(SDCard_Address);     // Write sector function
   }
   else
      status = SDCard_FAIL;                   // Error - Not a valid entry
      
   return(status);
}
#endif
#endif

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   uchar SDCard_WriteSect(ulong address)
//
// DESCRIPTION:
//   Routine that will write one absolute sector. Address must be set to the beginning
//   of a valid sector boundry before the call is made. (Multiple of 512, or zero)
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//    address => Address of sector to write
//
//  RETURNS:
//    Pass/Fail status
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------

uchar SDCard_WriteSect(ulong address)
{
   uchar   status = 0;            // command status
   uint   y;                      // Loop variables

#ifdef ENABLE_WPROTECT
   if(SDCard_WriteProtect()==0)       // If write protect is on skip writes, flag error   
#endif
   {
      SDCard_SetSize(512);               // Set block length to a 512 sector (512)

      // Write out one sector

      // CMD24 - Write single block command
      // Byte String = 0x58 (ulong Address) 0xFF
      
      SDCard_Cmd(SDCard_CMD24, address);                         // Send CMD24 command string

      // Get the data response token
      SDCard_SendTxData( 0xFF );
      SDCard_XferWait();                                  // Wait for transfer to complete
      status = SDCard_bReadRxData();     // Read the data response byte but ignore it.
      
      // Check response
      if(SDCard_CheckReply(0) != SDCard_FAIL)                    // Check for status good
      {
         // Send an FE to the byte shift out
         SDCard_SendTxData( 0xFE );
         SDCard_XferWait();                               // Wait for transfer to complete
		 rbuff = SDCard_bReadRxData();

         // Write one sector at a time  
         SDCard_WriteBuff256( SDCard_Buffer1 ); // Write first half of sector
         SDCard_WriteBuff256( SDCard_Buffer2 ); // Write second half of sector

         // Send byte two of checksum
         SDCard_SendTxData( 0xFF );
         SDCard_XferWait();                               // Wait for transfer to complete
		 rbuff = SDCard_bReadRxData();

         // Send Byte one of checksum
         SDCard_SendTxData( 0xFF );
         SDCard_XferWait();                               // Wait for transfer to complete
		 rbuff = SDCard_bReadRxData();
      
         // Get the data response token
         SDCard_SendTxData( 0xFF );
         SDCard_XferWait();                               // Wait for transfer to complete
         status = SDCard_bReadRxData();  // Read the data response byte
      
         if((status&0x0F)==5)                      // Was the token valid?
         {

            // Get the data response token
            SDCard_SendTxData( 0xFF );
            SDCard_XferWait();                            // Wait for transfer to complete
            status = SDCard_bReadRxData();  // Read the data response byte but ignore it.
      
            status=0;                              // Clear status before entering loop
            y=0x8000;                              // Set timeout value to 4X max time so far
         
            while(( status == 0) && (y>0))         // Loop until status is non-zero, or a timeout occurs
            {
               // Send a null byte to shift for btye read
               SDCard_SendTxData( 0xFF );
               SDCard_XferWait();                         // Wait for transfer to complete
   
               status = SDCard_bReadRxData(); // Read the reply
               y--;                                // Decrement loop until 0, then timeout
            }
            if(y == 0)                             // If loop is zero, then set status for failed
               status = SDCard_FAIL;
            else
               status = SDCard_PASS;
         }
         else
            status = SDCard_FAIL;            // Set status to failed
      }
      else
         status = SDCard_FAIL;               // Set status to failed
      
      SDCard_EndCmd();                       // Finish the command cycle

      SDCard_BuffLoc = address;              // Update write location
      SDCard_DirtyFlag &= 0xFE;              // Clear dirty buffer flag
   }
#ifdef ENABLE_WPROTECT
   else
      status = SDCard_FAIL;                  // Write protect on - set fail
#endif
   
   return(status);
}

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   uchar SDCard_ReadFileSect(uchar Fptr)
//
// DESCRIPTION:
//    Routine that will read one sector of a file. 
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//    Fptr => File pointer
//
//  RETURNS:
//    Pass/Fail status
//
//  SIDE EFFECTS:
//    Routine that will read one sector of a file. Offset must be set to the beginning
//    of a valid sector boundry before the call is made. (Multiple of 512, or zero)
//    It DOES NOT check to see if the file is at least one sector long - non-zero.
//    Must have an valid Fptr previously loaded by the SDCard_fopen(Filename) subroutine.
//
//              ***   Global variable required   ***     
//    ClusterSize      - the size of a cluster on the current disk
//
//    CurFat[Index]    - contains the starting FAT entry for the file
//    CurSize[Index]   - contains the size of the file
//    CurOffset[Index] - contains the current byte position in the file
//    CurSect[Index]   - contains the current sector in use of a cluster
//    Index            - is the passed variable of which file handle to use
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------
#ifdef ENABLE_FILESYSTEM

uchar SDCard_ReadFileSect(uchar Fptr)
{
   uchar   status = 0;              // command status

   SDCard_SetSize(512);                    // Set block length to a 512 sector (512)

   if((SDCard_CurFat[Fptr] != SDCard_FatEnd))
   {
      // Read in one sector

      // Address equals ((FAT entry - 2) * cluster size + sector offset) * sector size + DataStart
      SDCard_GetAddress(Fptr, 0);          // Calculate sector address

      status = SDCard_ReadSect(SDCard_Address);   // Read sector function
   }
   else
      status = SDCard_FAIL;                // Error - Not a valid entry

   return(status);
}
#endif

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   uchar SDCard_ReadSect(ulong address)
//
// DESCRIPTION:
//   Routine that will read one absolute sector. Address must be set to the beginning
//   of a valid sector boundry before the call is made. (Multiple of 512, or zero)
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//    address => Sector address
//
//  RETURNS:
//    Pass/Fail status
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------

uchar SDCard_ReadSect(ulong address)
{
   uchar  status = 0;         // command status

#ifdef ENABLE_WRITE
   // If sector changes flush the buffer if dirty
   if( ((SDCard_DirtyFlag&0x01)==0x01)&&(address != SDCard_BuffLoc) )
   {
      SDCard_WriteSect(SDCard_BuffLoc);     // Flush buffer if data is dirty
      SDCard_DirtyFlag &= 0xFE;      // Clear dirty flag for buffer
   }
#endif
   
   if(address != SDCard_BuffLoc)
   {
      // If the sector is not already loaded, read in one sector
      SDCard_SetSize(512);           // Set block length to a 512 sector (512)
               
      // CMD17 - Read single block command
      // Byte String = 0x51 (ulong Address) 0xFF 

      SDCard_Cmd(SDCard_CMD17, address);    // Send CMD17 command string

      // Check response
      if(SDCard_CheckReply(0) != SDCard_FAIL)            // Check for status good
      {
         if(SDCard_CheckReply(0xFE) != SDCard_FAIL)      // Set check to Single Block Read data token
         {
	         // Read an entire sector 
             SDCard_ReadBuff256( SDCard_Buffer1 );  // Read first half of sector
             SDCard_ReadBuff256( SDCard_Buffer2 );  // Read second half of sector
         }
         else
            status = SDCard_FAIL;          // Set status to failed
      }
      else
         status = SDCard_FAIL;             // Set status to failed
         
      SDCard_EndCmd();                     // Finish the command cycle
      SDCard_BuffLoc = address;            // Update write location
   }
   return(status);
}

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   uchar SDCard_NewFile(uchar Fptr, uchar Filename[])
//
// DESCRIPTION:
//   Create new file using filename  
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//    Fptr - File pointer for file to create
//
//  RETURNS:
//    Returns new file pointer value.
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------
#ifdef ENABLE_FILESYSTEM
#ifdef ENABLE_WRITE

uchar SDCard_NewFile(uchar Fptr, uchar Filename[])
{
   uchar x;                    // Temp counter
   uchar Tmp[11];              // Temp holder for filename
   uchar * Dptr;               // Create a temporary pointer for directory entry in the buffer
   
   // Make temporary copy of the filename (buffer will be overwritten)
   for(x=0; x<11; x++)
      Tmp[x] = Filename[x];

   SDCard_FindFile(Fptr, SDCard_UNUSED);     // Get first unused directory entry
   Dptr = SDCard_ReadDirSect(Fptr);   // Read  directory sector into buffers and return pointer

   // Write the filename into the directory entry in the buffer
   for(x=0; x<11; x++)
      Dptr[x] = Tmp[x];
   
   // Write the blank directory entry into the buffer
   for(x=11; x<32; x++)
      Dptr[x] = SDCard_BlankDir[x];

   SDCard_WriteDirSect(Fptr);         // Write directory sector from buffers
   
   // Clear the file info fields accordingly
   SDCard_CurAttr[Fptr]=0;
   SDCard_CurStart[Fptr]=0;
   SDCard_CurFat[Fptr]=0;
   SDCard_CurSize[Fptr]=0;
   SDCard_CurOffset[Fptr]=0;
   SDCard_CurSect[Fptr]=0;
   
   return(Fptr);
}
#endif
#endif

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:  uchar SDCard_FileZero(uchar Fptr, uchar Del)
//
// DESCRIPTION:
//   Sets file size to zero and resets FAT entries or deletes and closes file
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//    Fptr - File pointer for file to zero out
//    Del  - 0 = zero file, 1 = delete file
//
//  RETURNS:
//    Pass/Fail status
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------

#ifdef ENABLE_FILESYSTEM
#ifdef ENABLE_WRITE

uchar SDCard_FileZero(uchar Fptr, uchar Del)
{
   uchar x;                                 // Temp counter
   uint Offset;                             // Offset to Entry
   uchar * Dptr;                            // Create a temporary pointer for directory entry in the buffer
   uchar status = SDCard_FAIL;              // Temp status

   if(SDCard_CurStart[Fptr] != 0)                  // Is there a starting FAT entry present?
      SDCard_FatReclaim(SDCard_CurStart[Fptr]);           // Yes, delete the fat chain
   
   SDCard_ReadSect(SDCard_CurDir[Fptr] & 0xFFFFFE00);     // Read directory sector into buffers

   Offset = ((uint)(SDCard_CurDir[Fptr] & 0x1FF)); // Figure out the offset

   Dptr = SDCard_GetBuffPtr(Offset);               // Get the buffer pointer offset
   
   if(Del == 0)                             // Should the file be deleted or just zeroed?   
   {
      // Zero the file
      for(x=0x1A; x<0x20; x++)              // Zero the FAT entry and size fields
         Dptr[x] = 0;
      if(SDCard_FatEntrySize ==4)                  // If 32 bit FAT delete the upper FAT entry bytes as well
      {
         Dptr[0x14] = 0;
         Dptr[0x15] = 0;
      }   

      // Clear the file info fields accordingly
      SDCard_CurStart[Fptr]=0;
      SDCard_CurFat[Fptr]=0;
      SDCard_CurSize[Fptr]=0;
      SDCard_CurOffset[Fptr]=0;
      SDCard_CurSect[Fptr]=0;
   }
   else
   {
      // Delete the file from the directory
      Dptr[0] = 0xE5;                      // Mark entry as deleted   
   }
   
   SDCard_WriteSect(SDCard_CurDir[Fptr] & 0xFFFFFE00);   // Write directory sector from buffers
   
   if(Del == 1)                            // If deleting, the file must be Closed   
      status = SDCard_fclose(Fptr);
      
   return(status);
}
#endif
#endif

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   uchar SDCard_FatReclaim(ulong FirstFat)
//                  uchar SDCard_FatReclaim(uint FirstFat)
//
// DESCRIPTION:
//   Zero FAT chain starting at first FAT entry
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//    FirstFat => Top of FAT chain to clear
//
//  RETURNS:
//    Always 0  // REVIEW check this out.
//
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------
#ifdef ENABLE_FILESYSTEM
#ifdef ENABLE_WRITE

#ifdef ENABLE_FAT32
uchar SDCard_FatReclaim(ulong FirstFat)
#else
uchar SDCard_FatReclaim(uint FirstFat)
#endif
{
   
#ifdef ENABLE_FAT32
   ulong NextEntry = 0;   // Start with next entry cleared
#else
   uint NextEntry = 0;    // Start with next entry cleared
#endif
   uint  Offset;          // Temp Buffer offset
   uchar x;               // Temp counter
   uchar * Dptr;          // Create a temporary pointer for FAT entry in the sector

   // Figure out the current sector address the FAT entry is in and read it into the buffer
   SDCard_ReadFatSect(FirstFat); // Read FAT sector

   while(NextEntry != SDCard_FatEnd)
   {
      // Get next entry and zero current entry

      // Figure out the current buffer offset
      Offset = ( (uint)((FirstFat*SDCard_FatEntrySize) & 0x1FF) );

      Dptr = SDCard_GetBuffPtr(Offset);         // Get the buffer pointer offset
      // Convert the FAT entry to a long

      NextEntry = 0;                     // Clear before beginning
      for(x=1; x<=SDCard_FatEntrySize; x++)
      {
         NextEntry <<=8;                    // Shift value up 8 bits
         NextEntry += Dptr[SDCard_FatEntrySize-x]; // Add in the next byte (highest first)
         Dptr[SDCard_FatEntrySize-x] = 0;          // zero the buffer entry
      }

      // If next entry is FatEnd or out of sector - write buffers to each copy of FAT
#ifdef ENABLE_FAT32
      if( (NextEntry == SDCard_FatEnd)||(((FirstFat*SDCard_FatEntrySize)&0xFFFFFE00) != ((NextEntry*SDCard_FatEntrySize)&0xFFFFFE00)) )
#else
      if( (NextEntry == SDCard_FatEnd)||(((FirstFat*SDCard_FatEntrySize)&0xFE00) != ((NextEntry*SDCard_FatEntrySize)&0xFE00)) )
#endif
      {
         SDCard_WriteFatSect(FirstFat);            // Update current FAT sectors
         
         if(NextEntry != SDCard_FatEnd)
            SDCard_ReadFatSect(NextEntry);         // Read new FAT sector
      }
      if(FirstFat < SDCard_EmptyFat)               // Is the deleted block at a lower address than the last empty FAT entry?
         SDCard_EmptyFat = FirstFat;               // Yes, update start of empty FAT entry search   
      FirstFat = NextEntry;                 // Set Next entry equal to current entry
   }
   return(0);
}
#endif
#endif

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   uchar SDCard_AddFat(uchar Fptr)
//
// DESCRIPTION:
//   Add a new FAT entry to the FAT chain (or create it)
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//    Fptr => File pointer
//
//  RETURNS:
//    Always 0  // REVIEW check this out.
//
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------
#ifdef ENABLE_FILESYSTEM
#ifdef ENABLE_WRITE

uchar SDCard_AddFat(uchar Fptr)
{
   // Add a new FAT entry to the FAT chain (or create it)
   
#ifdef ENABLE_FAT32
   ulong FreeNum;           // Number of the first available free FAT entry
   ulong LastNum;           // Number of the last FAT entry  in the existing chain
#else
   uint FreeNum;            // Number of the first available free FAT entry
   uint LastNum;            // Number of the last FAT entry in the existing chain
#endif
   uchar x;                 // Temp counter
   uchar * Dptr;            // Create a temporary pointer for FAT entry in the sector

   FreeNum = SDCard_FatTrack(0);   // Get the next free FAT entry
   SDCard_EmptyFat = FreeNum +1;   // Increment the empty FAT entry pointer - this one is now used

   LastNum = SDCard_LastFat(Fptr); // Get the number for the last FAT entry in the existing chain

   // Find out if the file has a FAT chain yet
   if(LastNum == 0)
   {
      // No Fat chain yet, must create one from scratch
      // Must add the new FAT chain to the file info
      SDCard_CurStart[Fptr] = FreeNum;   // Update the starting FAT entry
      SDCard_CurFat[Fptr] = FreeNum;     // Update the current FAT entry
      
      // Update the directory listing for first file FAT
      Dptr = SDCard_ReadDirSect(Fptr);   // Read in the directory sector
      
      // Pass the value into the directory
      Dptr[0x1A] = ((uchar)(FreeNum));
      Dptr[0x1B] = ((uchar)(FreeNum >> 8));

#ifdef ENABLE_FAT32
      if(SDCard_FatEntrySize == 4)
      {
         Dptr[0x14] = ((uchar)(FreeNum >> 16));
         Dptr[0x15] = ((uchar)(FreeNum >> 24));
      }
#endif
      
      SDCard_WriteDirSect(Fptr);         // Write the directory sector
   }
      
   Dptr = SDCard_ReadFatSect(FreeNum);   // Get the FAT sector buffer
   
   // Copy the FAT end into the new FAT entry
   for(x=0; x<SDCard_FatEntrySize; x++)
   {
      Dptr[x] = ((uchar)(SDCard_FatEnd >> (x*8)));
   }

   if(LastNum == 0)               // Is there an old value to update?
   {
      // No, Write out the current buffer
      SDCard_WriteFatSect(FreeNum);      // Write FAT sector
   }
   else
   {
      // Yes, Need to update old FAT entry
      // Is the new FAT in the same sector?
      if(SDCard_GetFatAddr(LastNum) != SDCard_GetFatAddr(FreeNum))
      {
         // No, Write out the current buffer and open the new one
         SDCard_WriteFatSect(FreeNum);         // Write FAT sector
         Dptr = SDCard_ReadFatSect(LastNum);   // Read new FAT sector
      }
      else
      {
         // Yes still in same buffer, update the pointer
         Dptr = SDCard_GetBuffPtr((uint)((LastNum*SDCard_FatEntrySize) & 0x1FF));
      }
      
      // Copy the new FAT entry into the old FAT entry
      for(x=0; x<SDCard_FatEntrySize ; x++)
      {
         Dptr[x] = ((uchar)(FreeNum >> (x*8)));
      }
      
      SDCard_WriteFatSect(LastNum);         // Write FAT sector
      // Force a flush of the buffer just in case
      SDCard_BuffLoc=0;
   }
   return(0);
}
#endif
#endif

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   uchar * SDCard_GetBuffPtr(uint Offset)
//
// DESCRIPTION:
//   Return the pointer to the read/write buffer indicated by offset
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//    Offset => Offset to read/write buffer.
//
//  RETURNS:
//     Pointer offset.
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------
#ifdef ENABLE_FILESYSTEM

uchar * SDCard_GetBuffPtr(uint Offset)
{
   uchar * Dptr;               // Pointer variable
   
   Offset &= 0x1FF;            // Make sure offset is clean (0-511)

   if((Offset & 0x100)==0)
   {
      // Figure out offset into buffer 1 and set pointer
      Dptr = &SDCard_Buffer1[Offset];   
   }
   else 
   {
      // Figure out offset into buffer 1 and set pointer
      Dptr = &SDCard_Buffer2[Offset-0x100];   
   }
   return(Dptr);               // Return the pointer
}
#endif

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   char * SDCard_ReadDirSect(uchar Fptr)
//
// DESCRIPTION:
//   Read directory entry sector into buffer and return buffer pointer
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//    Fptr => File pointer
//
//  RETURNS:
//   Buffer pointer
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------
#ifdef ENABLE_FILESYSTEM

char * SDCard_ReadDirSect(uchar Fptr)
{
   uint Offset;                               // Temp for offset value
   
   SDCard_ReadSect(SDCard_CurDir[Fptr] & 0xFFFFFE00);       // Read  directory sector into buffers
   Offset = ((uint)(SDCard_CurDir[Fptr] & 0x1FF));   // Figure out the offset

   return(SDCard_GetBuffPtr(Offset));                // Return the buffer pointer offset
}
#endif

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   void SDCard_WriteDirSect(uchar Fptr)
//
// DESCRIPTION:
//   Write directory entry sector from the buffer
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//    Fptr => File pointer
//
//  RETURNS: NA
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------
#ifdef ENABLE_FILESYSTEM
#ifdef ENABLE_WRITE

void SDCard_WriteDirSect(uchar Fptr)
{
   SDCard_WriteSect(SDCard_CurDir[Fptr] & 0xFFFFFE00);   // Write directory sector from buffers
}
#endif
#endif

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   char * SDCard_ReadFatSect(ulong Fat)
//                  char * SDCard_ReadFatSect(uint Fat)
//
// DESCRIPTION:
//    Read directory entry sector into buffer and return buffer pointer
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//    Fat => Sector to read into buffer
//
//  RETURNS:
//    Buffer pointer offset
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------
#ifdef ENABLE_FILESYSTEM

#ifdef ENABLE_FAT32
char * SDCard_ReadFatSect(ulong Fat)
#else
char * SDCard_ReadFatSect(uint Fat)
#endif
{
   uint Offset;                                // Temp for offset value
   ulong address;                              // Temp Address variable
   
   address = SDCard_GetFatAddr(Fat);                  // Get the FAT entry address
   
   SDCard_ReadSect(address + SDCard_Fat1Start);              // Read  FAT sector into buffers
   Offset = ((uint)((Fat*SDCard_FatEntrySize) & 0x1FF));   // Figure out the offset

   return(SDCard_GetBuffPtr(Offset));                 // Return the buffer pointer offset
}
#endif

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   void SDCard_WriteFatSect(ulong Fat)
//                  void SDCard_WriteFatSect(uint Fat)
//
// DESCRIPTION:
//   Write sector buffer back into the FAT tables
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//    Fat => Sector to write
//
//  RETURNS: NA
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------
#ifdef ENABLE_FILESYSTEM
#ifdef ENABLE_WRITE

#ifdef ENABLE_FAT32
void SDCard_WriteFatSect(ulong Fat)
#else
void SDCard_WriteFatSect(uint Fat)
#endif
{
   ulong address;                       // Temp Address variable
   
   address = SDCard_GetFatAddr(Fat);           // Get the FAT entry address
   
   // Write sectors to both copies of the FAT from the buffer
   SDCard_WriteSect(address + SDCard_Fat1Start);      // Update current FAT 1 sector
   SDCard_WriteSect(address + SDCard_Fat2Start);      // Update current FAT 2 sector
}
#endif
#endif

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   ulong SDCard_GetFatAddr(ulong Fat)
//                  ulong SDCard_GetFatAddr(uint Fat)
//
// DESCRIPTION:
//   Return the sector address for the FAT entry
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//   Fat => FAT entry to find sector address
//
//  RETURNS:
//    Sector address
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------
#ifdef ENABLE_FILESYSTEM

#ifdef ENABLE_FAT32
ulong SDCard_GetFatAddr(ulong Fat)
#else
ulong SDCard_GetFatAddr(uint Fat)
#endif
{
   return((Fat*SDCard_FatEntrySize) & 0xFFFFFE00);    // Calculate sector offset
}
#endif

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   void SDCard_UpdateDir(uchar Fptr)
//
// DESCRIPTION:
//   Update the directory entry from the current file info
//   (At this time only attributes and size are changed)
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//    Fptr => File pointer
//
//  RETURNS: NA
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------

#ifdef ENABLE_FILESYSTEM
#ifdef ENABLE_WRITE

void SDCard_UpdateDir(uchar Fptr)
{
   uchar x;                    // Temp loop variable
   uchar * Dptr;               // Pointer to directory entry in buffer

   ulong TempSize;
   TempSize = SDCard_CurSize[Fptr];
   
   Dptr = SDCard_ReadDirSect(Fptr);   // Read  directory sector into buffers and return pointer

   Dptr[0x0B] = SDCard_CurAttr[Fptr]; // Copy the attribute back to the directory
   
                               // Copy the current size back to the directory
   for(x=0; x<4; x++)
   {
      Dptr[0x1C + x] = ((uchar)(SDCard_CurSize[Fptr]>>(x*8)));
   }

   SDCard_WriteDirSect(Fptr);         // Write directory sector from buffers
   SDCard_FileMode[Fptr]&=0xEF;       // Clear dirty directory flag
}
#endif
#endif
   
//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   uchar SDCard_ParseFilename(uchar * Filename)
//
// DESCRIPTION:
//   Parse the filename string and convert it into 8x3 dir format
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//    Filename => Pointer to string containing file name.
//
//  RETURNS:
//    Pass/Fail status
//
//  SIDE EFFECTS:
//    Note: Filename string must have 13 charcters resevered even 
//          if fewer characters are used (8 + dot + 3 + null = 13)
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------
#ifdef ENABLE_FILESYSTEM

uchar SDCard_ParseFilename(uchar * Filename)
{   
         
   uchar x = 0;        // Temp loop variable
   uchar y;            // Temp loop variable
   uchar status = 0;   // Temp status
   uchar Dot = 0;      // Dot present flag
   uchar Length;       // String length
   uchar Ext;          // the length of the extension

   while(Filename[x] != 0)       // Loop through Filename until null is found
   {
      y = Filename[x];           // Temp store the character as array index is code intensive
      
      if((y >= 'a')&&(y <= 'z')) //Convert ASCII characters to upper case
         Filename[x] -= 0x20;    // Convert lower to upper case

      if(y == '.')               // Is the a dot character?
	  {
         if(Dot == 0)            // Yes, Is it the first dot character?
            Dot=x;               // Yes, save the position
         else
            status=SDCard_FAIL;         // No, multiple dot characters are illegal
      }
	  
      x++;                       // Increment count
   }

   if(Dot>0)
   {                             // Figure out file length
      Length = Dot;              // Save filename length (if dot present)
      Ext = x -(Dot+1);          // Save extension length
   }
   else
   {
      Length=x;                  // Save filename length (no dot present)
      Ext = 0;                   // No extension
   }
   
   // See if the filename is valid

   // Check to make sure the first character is not a null (no string) and is valid upper case character
   if((Filename[0] < 'A')||(Filename[0] > 'Z'))    // Is the first character illegal?
      status = SDCard_FAIL;                               // ERROR - Filename missing or starts with an illegal character
   
   // Check to make sure the filename and extension are legal lengths
   if((Length>8)||(Ext>3))       // Are sizes valid?
      status = SDCard_FAIL;             // No, ERROR - filename or extension too long

   if(status != SDCard_FAIL)            // If no errors continue, otherwise exit
   {
      if(Ext!=0)                 // If there is an extension, move it to the correct position
      {
         // Move the extension to the correct position
         if(Dot<8)
         {
            // Extension should be copied from last to first
            for(x=Ext; x>0; x--)
               Filename[7+x]=Filename[Dot+x];
         }
         else
         {
            // Extension should be copied first to last
            for(x=8; x<=(Ext+7); x++)
               Filename[x]=Filename[x+1];
         }
      }
      
      // Fill remainder of extension with spaces
      for(x=10; x>(7+Ext); x--)
         Filename[x]=' ';

      // Fill remainder of filename with spaces
      for(x=Length; x<8; x++)
         Filename[x]=' ';
         
      // Add the final null
      Filename[11]=0;
   }

   return(status);
}
#endif


//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   void SDCard_CopyDir(uchar Fptr1, uchar Fptr2 )
//
// DESCRIPTION:
//   This routine copies the directory information from file 1 to file 2
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//    Fptr1 => Source file info file pointer
//    Fptr1 => Destination file info file pointer
//
//  RETURNS:  NA
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------
#ifdef ENABLE_FILESYSTEM
#ifdef ENABLE_WRITE

void SDCard_CopyDir(uchar Fptr1, uchar Fptr2 )
{
   
   // Copy file size, time, date, and attributes from the original file
   
   SDCard_CurSize[Fptr2] = SDCard_CurSize[Fptr1];      // Copy file size 
   SDCard_CurAttr[Fptr2] = SDCard_CurAttr[Fptr1];      // Copy file attributes 
   
   // Call update directory to save the new directory information
   SDCard_UpdateDir(Fptr2);
}
#endif
#endif

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   void SDCard_GetAddress(uchar Fptr, uchar mode)
//
// DESCRIPTION:
//    Calculate the address based on the current file/sub-directory position
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//    Fptr => File pointer
//    mode = 0 only the sector address is returned  
//         = 1 sector and offset
//
//  RETURNS: NA
//
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------

#ifdef ENABLE_FILESYSTEM

void SDCard_GetAddress(uchar Fptr, uchar mode)
{
   // Calculate the address based on the current file/sub-directory position
   // If mode = 0 only the sector address is returned, mode = 1 sector and offset
   
   SDCard_Address = SDCard_CurFat[Fptr] - 2;            // FAT cluster - 2 (actual offset in clusters)
   SDCard_Address *= SDCard_ClusterSize;                // Multiply by cluster size to get sectors
   SDCard_Address += SDCard_CurSect[Fptr];              // Add current sector offset
   SDCard_Address *= 512;                        // Multiply sectors by 512 to get bytes
   SDCard_Address += SDCard_DataStart;                  // Add the starting address of the data area
   
   if(mode == 1)
   {
      // Add the offset stuff to the address
      SDCard_Address += (SDCard_CurOffset[Fptr]&0x000001FF);   // Add the significant part of the current offset (less than a sector)
   }   
}
#endif

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   void SDCard_IncOffset(uchar Fptr, uchar mode)
//
// DESCRIPTION:
//    Increments the file position offset and updates trhe sector count and FAT information
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//    Fptr => File pointer
//    mode = 0 increment the position offset, the sector count, and FAT  
//         = 1 Same as mode 0 except FAT is added if needed for a write
//
//  RETURNS: NA
//
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------

void SDCard_IncOffset(uchar Fptr, uchar mode)
{

    // Increment file position (and update sector and FAT entries if FileSystem enabled)
    SDCard_CurOffset[Fptr]++;    // Advance the offset value	

#ifdef ENABLE_FILESYSTEM
    if((SDCard_CurOffset[Fptr] != 0) && ((SDCard_CurOffset[Fptr] & 0x1FF) == 0))
    {
        // End of a sector
        if(SDCard_CurSect[Fptr] == (SDCard_ClusterSize-1))
        {
            SDCard_CurSect[Fptr]=0;    // Restart sector count
            if(mode==0)
                SDCard_CurFat[Fptr]  = SDCard_FatTrack(SDCard_CurFat[Fptr]); // Get next FAT entry
            else
                SDCard_NextFat(Fptr);  // Get next FAT entry (and add it if needed)
        }
        else
            SDCard_CurSect[Fptr]++;    // Increment sector count	
    }
#endif
}

//=================================================================================================
// DEBUG FUNCTIONS - Not needed for normal operations
//=================================================================================================

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   uchar SDCard_ACmd_13(void)
//
// DESCRIPTION:
//   ACMD13 - SD card status command
//   Byte String = 0x4D 0x00 0x00 0x00 0x00 0xFF 
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:  NA
//
//  RETURNS: 
//    Pass/Fail status
//
//  SIDE EFFECTS:
//    NOTE: REQUIRES a CMD55 before it does an ACMD13
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------
#ifdef ENABLE_DEBUGFUNCT

uchar SDCard_ACmd_13(void)
{
   uchar   status = 0;
   uchar   x;
   
   SDCard_Cmd(SDCard_ACMD13, SDCard_NOARGS);                   // Send ACMD13 command string

   // Check response
   if(SDCard_CheckReply(0) != SDCard_FAIL)              // Check for status good
   {
      if(SDCard_CheckReply(0xFE) != SDCard_FAIL)        // Set check to Single Block Read data token
      {
         // Get the 64 bytes and save them
         SDCard_ReadBuff(SDCard_Buffer1, 32 );
         SDCard_ReadBuff(SDCard_Buffer2, 32 );
      }
      else
         status = SDCard_FAIL;       // Set status to failed
   }
   else
      status = SDCard_FAIL;          // Set status to failed

   SDCard_EndCmd();                  // Finish the command cycle

   return(status);
}
#endif

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   uchar SDCard_GetCID(void)
//
// DESCRIPTION:
//   Sends the get CID command (Byte String = 0x4A 0x00 0x00 0x00 0x00 0xFF) 
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:  NA
//
//  RETURNS:
//    Pass/Fail status
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------

#ifdef ENABLE_DEBUGFUNCT

uchar SDCard_GetCID(void)
{

   uchar   status = 0;
   uint   x;
   
   // CMD10 - Get CID command
   // Byte String = 0x4A 0x00 0x00 0x00 0x00 0xFF 

   SDCard_Cmd(SDCard_CMD10, SDCard_NOARGS);                    // Send CMD10 command string
   
   // Check response
   if(SDCard_CheckReply(0) != SDCard_FAIL)              // Check for status good
   {
      if(SDCard_CheckReply(0xFE) != SDCard_FAIL)        // Set check to Single Block Read data token
      {
         // Get CID data (16 bytes in SPI mode) and save them
         SDCard_ReadBuff(SDCard_Buffer1, 16 );
      }
      else
         status = SDCard_FAIL;      // Set status to failed
   }
   else
      status = SDCard_FAIL;         // Set status to failed

   SDCard_EndCmd();                 // Finish the command cycle

   return(status);
}
#endif

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   uchar SDCard_GetOCR(void)
//
// DESCRIPTION:
//    Gets OCR data and puts it into Buffer1 (0-3] 
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:  NA
//
//  RETURNS:
//    Status bits: 0=Idle, 1=Erase Reset, 2=Illegal CMD, 3=CRC Error, 4=Erase_Seq_Error, 5=Address Error, 6=Param Error
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------
#ifdef ENABLE_DEBUGFUNCT

uchar SDCard_GetOCR(void)
{
   uchar   status = 0;
   uchar   x;
   
   // CMD58 - Read OCR data command
   // Byte String = 0x7A 0x00 0x00 0x00 0x00 0xFF 

   SDCard_Cmd(SDCard_CMD58, SDCard_NOARGS);             // Send CMD58 command string
   
   // Status bits: 0=Idle, 1=Erase Reset, 2=Illegal CMD, 3=CRC Error, 4=Erase_Seq_Error, 5=Address Error, 6=Param Error
   status = SDCard_GetR1();               // Get the R1 command response   

   // Get OCR data 
   SDCard_ReadBuff(SDCard_Buffer1, 4 );

   SDCard_EndCmd();                       // Finish the command cycle

   return(status);
}
#endif

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   uchar SDCard_GetCSD(void)
//
// DESCRIPTION:
//   Sends the get CSD command (Byte String = 0x49 0x00 0x00 0x00 0x00 0xFF) 
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS: NA
//
//  RETURNS:
//    Pass/Fail status
//
//  SIDE EFFECTS:
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------
#ifdef ENABLE_DEBUGFUNCT

uchar SDCard_GetCSD(void)
{

   uchar   status = 0;
   uchar   x;
   
   // CMD9 - Get CSD command
   // Byte String = 0x49 0x00 0x00 0x00 0x00 0xFF 

   SDCard_Cmd(SDCard_CMD9, SDCard_NOARGS);                     // Send CMD9 command string
   
   // Check response
   if(SDCard_CheckReply(0) != SDCard_FAIL)              // Check for status good
   {
      if(SDCard_CheckReply(0xFE) != SDCard_FAIL)        // Set check to Single Block Read data token
      {
         // Get CSD data (16 bytes in SPI mode) and save them
         SDCard_ReadBuff(SDCard_Buffer1, 16 );
      }
      else
         status = SDCard_FAIL;       // Set status to failed
   }
   else
      status = SDCard_FAIL;          // Set status to failed
   SDCard_EndCmd();                  // Finish the command cycle

   return(status);
}
#endif

//-------------------------------------------------------------------------------------------------
// FUNCTION NAME:   uchar SDCard_ReadByte(ulong address)
//
// DESCRIPTION:
//   Return the byte pointed to by the absolute address
//
//-------------------------------------------------------------------------------------------------
//
//  ARGUMENTS:
//    address => Address of the byte to read
//
//  RETURNS:
//    The value at the given address.
//
//  SIDE EFFECTS:
//    Note: There is no error report so care must be taken to use a valid address
//
//  THEORY of OPERATON or PROCEDURE:
//
//
//-------------------------------------------------------------------------------------------------
#ifdef ENABLE_DEBUGFUNCT

uchar SDCard_ReadByte(ulong address)
{
   uchar output = 0;                   // Data variable
   
   SDCard_SetSize(1);                         // Set block length to read a single byte

   // CMD17 - Read single block command
   // Byte String = 0x51 (ulong Address) 0xFF 

   SDCard_Cmd(SDCard_CMD17, address);                // Send CMD17 command string
   
   // Check response
   if(SDCard_CheckReply(0) != SDCard_FAIL)           // Check for status good
   {
      if(SDCard_CheckReply(0xFE) != SDCard_FAIL)     // Set check to Single Block Read data token
      {
         // Get one byte

         // Send a null byte to shift for btye read
         SDCard_SendTxData( 0xFF );
         SDCard_XferWait();                  // Wait for transfer to complete
      
         // Read data byte and output it
         output = SDCard_bReadRxData();  // Read the data byte and save it

         SDCard_EndCmd();                     // Finish the command cycle
      }
   }
   return(output);
}
#endif

//=================================================================================================
// END of debug functions
//=================================================================================================

