/*
 * bootloader_command_codes.h
 *
 *  Created on: Sep 20, 2023
 *      Author: metehan.cakmak
 */

#ifndef BOOTLOADER_INC_BOOTLOADER_COMMAND_CODES_H_
#define BOOTLOADER_INC_BOOTLOADER_COMMAND_CODES_H_

#define BL_GET_VER						(0x51U)
#define BL_GET_HELP						(0x52U)
#define BL_GET_CIP						(0x53U)
#define BL_GET_RDP_STATUS				(0x54U)
#define BL_GO_TO_ADRR					(0x55U)
#define BL_FLASH_ERASE					(0x56U)
#define BL_MEM_WRITE					(0x57U)
#define BL_EN_RW_PROTECT				(0x58U)
#define BL_MEM_READ						(0x59U)
#define BL_READ_SECTOR_P_STATUS			(0x5AU)
#define BL_OTP_READ						(0x5BU)
#define BL_DIS_RW_PROTECT				(0x5CU)






#endif /* BOOTLOADER_INC_BOOTLOADER_COMMAND_CODES_H_ */
