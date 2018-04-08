/**
 *  Defines for your entire project at one place
 * 
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@version 	v1.0
 *	@ide		Keil uVision 5
 *	@license	GNU GPL v3
 *	
 * |----------------------------------------------------------------------
 * | Copyright (C) Tilen Majerle, 2014
 * | 
 * | This program is free software: you can redistribute it and/or modify
 * | it under the terms of the GNU General Public License as published by
 * | the Free Software Foundation, either version 3 of the License, or
 * | any later version.
 * |  
 * | This program is distributed in the hope that it will be useful,
 * | but WITHOUT ANY WARRANTY; without even the implied warranty of
 * | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * | GNU General Public License for more details.
 * | 
 * | You should have received a copy of the GNU General Public License
 * | along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * |----------------------------------------------------------------------
 */
#ifndef TM_DEFINES_H
#define TM_DEFINES_H

/* Use SPI communication with SDCard */
#define    FATFS_USE_SDIO                0

/* Select your SPI settings */
#define FATFS_SPI				SPI1
#define FATFS_SPI_PINSPACK		TM_SPI_PinsPack_1

/* Custom CS pin for SPI communication */
#define FATFS_CS_PORT		GPIOB
#define FATFS_CS_PIN		GPIO_Pin_5
/* Put your global defines for all libraries here used in your project */

#define TM_SPIx_PRESCALER	SPI_BaudRatePrescaler_32

//Specify datasize
#define TM_SPIx_DATASIZE 	SPI_DataSize_8b

//Specify which bit is first
#define TM_SPIx_FIRSTBIT 	SPI_FirstBit_MSB

//Mode, master or slave
#define TM_SPIx_MASTERSLAVE SPI_Mode_Master

//Specify mode of operation, clock polarity and clock phase
//Modes 0 to 3 are possible
#define TM_SPIx_MODE		TM_SPI_Mode_0


#endif
