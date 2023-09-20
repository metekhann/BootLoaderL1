/*
 * bootloader_command_app.h
 *
 *  Created on: Sep 20, 2023
 *      Author: metehan.cakmak
 */

#ifndef BOOTLOADER_INC_BOOTLOADER_COMMAND_APP_H_
#define BOOTLOADER_INC_BOOTLOADER_COMMAND_APP_H_

#include "main.h"





#define BL_VER			(0x10U)			//1.0

#define BL_CRC_FAIL		(1U)
#define BL_CRC_SUCCES	(0U)









void bootloader_get_ver_cmd(uint8_t *bl_rx_data);
uint8_t bootloader_verify_crc(uint8_t *Buffer, uint32_t len, uint32_t host_crc);

#endif /* BOOTLOADER_INC_BOOTLOADER_COMMAND_APP_H_ */
