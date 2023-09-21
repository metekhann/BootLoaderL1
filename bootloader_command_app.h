/*
 * bootloader_command_app.h
 *
 *  Created on: Sep 20, 2023
 *      Author: metehan.cakmak
 */



#ifndef BOOTLOADER_INC_BOOTLOADER_COMMAND_APP_H_
#define BOOTLOADER_INC_BOOTLOADER_COMMAND_APP_H_

#include "main.h"


extern CRC_HandleTypeDef hcrc;
extern UART_HandleTypeDef huart1;




#define BL_VER			(0x10U)			//1.0

#define BL_CRC_FAIL		(1U)
#define BL_CRC_SUCCES	(0U)

#define BL_NACK_VALUE	(0x7FU)
#define BL_ACK_VALUE	(0xA5U)








void bootloader_get_ver_cmd(uint8_t *bl_rx_data);
void bootloader_get_help_cmd(uint8_t *bl_rx_data);
void bootloader_get_cid_cmd(uint8_t *bl_rx_data);
void bootloader_get_rdpStatus_cmd(uint8_t *bootloader_rx_data);



uint8_t bootloader_verify_crc(uint8_t *Buffer, uint32_t len, uint32_t host_crc);
void bootloader_send_ACK(uint8_t followLenght);
void bootloader_send_NACK();
void bootloader_send_to_host(uint8_t* message, uint16_t len);

#endif /* BOOTLOADER_INC_BOOTLOADER_COMMAND_APP_H_ */
