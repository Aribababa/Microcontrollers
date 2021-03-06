/*******************************************************************************
* File Name: SDCard_SD_PWR.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_SDCard_SD_PWR_H) /* Pins SDCard_SD_PWR_H */
#define CY_PINS_SDCard_SD_PWR_H

#include "cytypes.h"
#include "cyfitter.h"
#include "SDCard_SD_PWR_aliases.h"


/***************************************
*     Data Struct Definitions
***************************************/

/**
* \addtogroup group_structures
* @{
*/
    
/* Structure for sleep mode support */
typedef struct
{
    uint32 pcState; /**< State of the port control register */
    uint32 sioState; /**< State of the SIO configuration */
    uint32 usbState; /**< State of the USBIO regulator */
} SDCard_SD_PWR_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   SDCard_SD_PWR_Read(void);
void    SDCard_SD_PWR_Write(uint8 value);
uint8   SDCard_SD_PWR_ReadDataReg(void);
#if defined(SDCard_SD_PWR__PC) || (CY_PSOC4_4200L) 
    void    SDCard_SD_PWR_SetDriveMode(uint8 mode);
#endif
void    SDCard_SD_PWR_SetInterruptMode(uint16 position, uint16 mode);
uint8   SDCard_SD_PWR_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void SDCard_SD_PWR_Sleep(void); 
void SDCard_SD_PWR_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(SDCard_SD_PWR__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define SDCard_SD_PWR_DRIVE_MODE_BITS        (3)
    #define SDCard_SD_PWR_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - SDCard_SD_PWR_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the SDCard_SD_PWR_SetDriveMode() function.
         *  @{
         */
        #define SDCard_SD_PWR_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define SDCard_SD_PWR_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define SDCard_SD_PWR_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define SDCard_SD_PWR_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define SDCard_SD_PWR_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define SDCard_SD_PWR_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define SDCard_SD_PWR_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define SDCard_SD_PWR_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define SDCard_SD_PWR_MASK               SDCard_SD_PWR__MASK
#define SDCard_SD_PWR_SHIFT              SDCard_SD_PWR__SHIFT
#define SDCard_SD_PWR_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in SDCard_SD_PWR_SetInterruptMode() function.
     *  @{
     */
        #define SDCard_SD_PWR_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define SDCard_SD_PWR_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define SDCard_SD_PWR_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define SDCard_SD_PWR_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(SDCard_SD_PWR__SIO)
    #define SDCard_SD_PWR_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(SDCard_SD_PWR__PC) && (CY_PSOC4_4200L)
    #define SDCard_SD_PWR_USBIO_ENABLE               ((uint32)0x80000000u)
    #define SDCard_SD_PWR_USBIO_DISABLE              ((uint32)(~SDCard_SD_PWR_USBIO_ENABLE))
    #define SDCard_SD_PWR_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define SDCard_SD_PWR_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define SDCard_SD_PWR_USBIO_ENTER_SLEEP          ((uint32)((1u << SDCard_SD_PWR_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << SDCard_SD_PWR_USBIO_SUSPEND_DEL_SHIFT)))
    #define SDCard_SD_PWR_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << SDCard_SD_PWR_USBIO_SUSPEND_SHIFT)))
    #define SDCard_SD_PWR_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << SDCard_SD_PWR_USBIO_SUSPEND_DEL_SHIFT)))
    #define SDCard_SD_PWR_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(SDCard_SD_PWR__PC)
    /* Port Configuration */
    #define SDCard_SD_PWR_PC                 (* (reg32 *) SDCard_SD_PWR__PC)
#endif
/* Pin State */
#define SDCard_SD_PWR_PS                     (* (reg32 *) SDCard_SD_PWR__PS)
/* Data Register */
#define SDCard_SD_PWR_DR                     (* (reg32 *) SDCard_SD_PWR__DR)
/* Input Buffer Disable Override */
#define SDCard_SD_PWR_INP_DIS                (* (reg32 *) SDCard_SD_PWR__PC2)

/* Interrupt configuration Registers */
#define SDCard_SD_PWR_INTCFG                 (* (reg32 *) SDCard_SD_PWR__INTCFG)
#define SDCard_SD_PWR_INTSTAT                (* (reg32 *) SDCard_SD_PWR__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define SDCard_SD_PWR_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(SDCard_SD_PWR__SIO)
    #define SDCard_SD_PWR_SIO_REG            (* (reg32 *) SDCard_SD_PWR__SIO)
#endif /* (SDCard_SD_PWR__SIO_CFG) */

/* USBIO registers */
#if !defined(SDCard_SD_PWR__PC) && (CY_PSOC4_4200L)
    #define SDCard_SD_PWR_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define SDCard_SD_PWR_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define SDCard_SD_PWR_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define SDCard_SD_PWR_DRIVE_MODE_SHIFT       (0x00u)
#define SDCard_SD_PWR_DRIVE_MODE_MASK        (0x07u << SDCard_SD_PWR_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins SDCard_SD_PWR_H */


/* [] END OF FILE */
