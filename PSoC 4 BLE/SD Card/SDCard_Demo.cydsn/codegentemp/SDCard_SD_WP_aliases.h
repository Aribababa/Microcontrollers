/*******************************************************************************
* File Name: SDCard_SD_WP.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_SDCard_SD_WP_ALIASES_H) /* Pins SDCard_SD_WP_ALIASES_H */
#define CY_PINS_SDCard_SD_WP_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define SDCard_SD_WP_0			(SDCard_SD_WP__0__PC)
#define SDCard_SD_WP_0_PS		(SDCard_SD_WP__0__PS)
#define SDCard_SD_WP_0_PC		(SDCard_SD_WP__0__PC)
#define SDCard_SD_WP_0_DR		(SDCard_SD_WP__0__DR)
#define SDCard_SD_WP_0_SHIFT	(SDCard_SD_WP__0__SHIFT)
#define SDCard_SD_WP_0_INTR	((uint16)((uint16)0x0003u << (SDCard_SD_WP__0__SHIFT*2u)))

#define SDCard_SD_WP_INTR_ALL	 ((uint16)(SDCard_SD_WP_0_INTR))


#endif /* End Pins SDCard_SD_WP_ALIASES_H */


/* [] END OF FILE */
