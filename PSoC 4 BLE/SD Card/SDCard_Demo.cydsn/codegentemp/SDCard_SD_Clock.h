/*******************************************************************************
* File Name: SDCard_SD_Clock.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_SDCard_SD_Clock_H)
#define CY_CLOCK_SDCard_SD_Clock_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void SDCard_SD_Clock_StartEx(uint32 alignClkDiv);
#define SDCard_SD_Clock_Start() \
    SDCard_SD_Clock_StartEx(SDCard_SD_Clock__PA_DIV_ID)

#else

void SDCard_SD_Clock_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void SDCard_SD_Clock_Stop(void);

void SDCard_SD_Clock_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 SDCard_SD_Clock_GetDividerRegister(void);
uint8  SDCard_SD_Clock_GetFractionalDividerRegister(void);

#define SDCard_SD_Clock_Enable()                         SDCard_SD_Clock_Start()
#define SDCard_SD_Clock_Disable()                        SDCard_SD_Clock_Stop()
#define SDCard_SD_Clock_SetDividerRegister(clkDivider, reset)  \
    SDCard_SD_Clock_SetFractionalDividerRegister((clkDivider), 0u)
#define SDCard_SD_Clock_SetDivider(clkDivider)           SDCard_SD_Clock_SetDividerRegister((clkDivider), 1u)
#define SDCard_SD_Clock_SetDividerValue(clkDivider)      SDCard_SD_Clock_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define SDCard_SD_Clock_DIV_ID     SDCard_SD_Clock__DIV_ID

#define SDCard_SD_Clock_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define SDCard_SD_Clock_CTRL_REG   (*(reg32 *)SDCard_SD_Clock__CTRL_REGISTER)
#define SDCard_SD_Clock_DIV_REG    (*(reg32 *)SDCard_SD_Clock__DIV_REGISTER)

#define SDCard_SD_Clock_CMD_DIV_SHIFT          (0u)
#define SDCard_SD_Clock_CMD_PA_DIV_SHIFT       (8u)
#define SDCard_SD_Clock_CMD_DISABLE_SHIFT      (30u)
#define SDCard_SD_Clock_CMD_ENABLE_SHIFT       (31u)

#define SDCard_SD_Clock_CMD_DISABLE_MASK       ((uint32)((uint32)1u << SDCard_SD_Clock_CMD_DISABLE_SHIFT))
#define SDCard_SD_Clock_CMD_ENABLE_MASK        ((uint32)((uint32)1u << SDCard_SD_Clock_CMD_ENABLE_SHIFT))

#define SDCard_SD_Clock_DIV_FRAC_MASK  (0x000000F8u)
#define SDCard_SD_Clock_DIV_FRAC_SHIFT (3u)
#define SDCard_SD_Clock_DIV_INT_MASK   (0xFFFFFF00u)
#define SDCard_SD_Clock_DIV_INT_SHIFT  (8u)

#else 

#define SDCard_SD_Clock_DIV_REG        (*(reg32 *)SDCard_SD_Clock__REGISTER)
#define SDCard_SD_Clock_ENABLE_REG     SDCard_SD_Clock_DIV_REG
#define SDCard_SD_Clock_DIV_FRAC_MASK  SDCard_SD_Clock__FRAC_MASK
#define SDCard_SD_Clock_DIV_FRAC_SHIFT (16u)
#define SDCard_SD_Clock_DIV_INT_MASK   SDCard_SD_Clock__DIVIDER_MASK
#define SDCard_SD_Clock_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_SDCard_SD_Clock_H) */

/* [] END OF FILE */
