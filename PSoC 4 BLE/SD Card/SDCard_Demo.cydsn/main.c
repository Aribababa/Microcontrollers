/* ========================================
 *
 * The following firmware was developed by Cypress Semiconductor
 * This work is licensed under a Creative Commons Attribution 3.0 Unported License.
 * 
 * http://creativecommons.org/licenses/by/3.0/deed.en_US
 * 
 * You are free to:
 * -To Share — to copy, distribute and transmit the work 
 * -To Remix — to adapt the work 
 * -To make commercial use of the work
 *
 * ========================================
 */

#include <device.h>

uint32 i;
char * fnam;
uint32 siz;
uint8 no,fcnt,fre,j,bufnn;

// The main
int main()
{
    char   cardInfo;							// Card information  
    char   fp;									// File Pointer  
    uint8  cnt;									// Card inserted state variables  
	uint8  load = 2;							// Card initialization variable
	char   data_r;								// Data read from Card
	
	CyGlobalIntEnable;							// Enable all global interrupts
	
	// Setup and initialize the SD Card
	do
	{
		SDCard_Start();							// Initialize SPI Bus for communication
		SDCard_Select(SDCard_DISABLE); 			// Select card 
			
		cardInfo = 0;  
		cnt = 0;
	    while ( ! cardInfo )          			// Wait for card to communicate  
	    {  
		   cnt++;
		   if(cnt == 0xFF) {cnt = 0;}
	       cardInfo = SDCard_InitCard();
	    }  
			
		load = load - 1;
	}while(load > 0);							// Stay in the loop till the card is initialized.
	
	fcnt = SDCard_GetFileCount();				// Get number of files on Card
	
	// Loop through all the existing files
	// in the SD Card, one by one
	// or skip is count = 0
	for (cnt = 0; cnt < fcnt; cnt++)
	{
		fnam = SDCard_GetFilename(cnt);			// Get the filename of the file with index value in "cnt"
	}
	
	// *****************************************
	// 1. Write File
	// *****************************************
	fp = SDCard_fopen("file1.txt", "w");		// Open a file with write mode access. If the file is not present, a new file is created.
	SDCard_fputcs("This is my 1st file.", fp);	// Write a constant string into the file
	SDCard_fclose(fp);           				// Close file
	fp = SDCard_fopen("file_tmp.txt", "w");		// Open a file with write mode access. If the file is not present, a new file is created.
	SDCard_fputcs("This is a temp file.", fp);	// Write a constant string into the file
	SDCard_fclose(fp);           				// Close file
	
	// *****************************************
	// 2. Append File
	// *****************************************
	fp = SDCard_fopen("file2.txt", "w");      	// Open a file with write mode access. If the file is not present, a new file is created.
	SDCard_fputcs("This is my 2nd file.", fp);	// Write a constant string into the file
	SDCard_fclose(fp);           				// Close file
	fp = SDCard_fopen("file2.txt", "a");		// Open the file with append mode access.
	SDCard_fputcs("I appended this text.", fp);	// Write a constant string into the file
	SDCard_fclose(fp);           				// Close file
	
	// *****************************************
	// 3. Read Read
	// *****************************************
	fp = SDCard_fopen("file2.txt", "r");		// Open a file with readonly access.
	siz  = SDCard_GetFileSize(fp);				// Get the file size.
	SDCard_fseek(fp, 0);						// Move to the offset-0, which is the beginning of file
	while(!SDCard_feof(fp))						// Loop till end of file
	{
		data_r = SDCard_fgetc(fp);				// Read the character pointed by fp
	}
	SDCard_fclose(fp);           				// Close file
	
	// *****************************************
	// 4. Rename File
	// *****************************************
	SDCard_Rename("file1.txt", "file_ren.txt");	// Rename file1.txt to file_ren.txt
	
	// *****************************************
	// 5. Copy File
	// *****************************************
	SDCard_Copy("file_tmp.txt", "file_cpy.txt");// Copy the contents of file_tmp.txt to file_cpy.txt
	
	// *****************************************
	// 6. Delete File
	// *****************************************
	SDCard_Remove("file_tmp.txt");				// Delete file_tmp.txt

	SDCard_Select(SDCard_DISABLE);         		// Deselect card  
	SDCard_Stop();								// Stop the communication interface
	
	while(1);									// The End
}

/* [] END OF FILE */
