/*
 * bootloader_command_app.c
 *
 *  Created on: Sep 20, 2023
 *      Author: metehan.cakmak
 */


#include "bootloader_command_app.h"



void bootloader_get_ver_cmd(uint8_t *bl_rx_data)
{
	uint8_t bl_version = 0;

	print_m("BL_DEBUG_MSG: BootLoader_GET_VER_CMD\n");

	uint32_t command_packet_lenght = bl_rx_data[0] + 1;
	uint32_t host_crc = *((uint32_t*)(bl_rx_data + command_packet_lenght - 4));

	// crc control



}

uint8_t bootloader_verify_crc(uint8_t *Buffer, uint32_t len, uint32_t host_crc)
{

	uint32_t crc_value = 0xFF;
	uint32_t data = 0;

	for(uint32_t i = 0; i < len; i++)
	{
		data = Buffer[i];
		crc_value = HAL_CRC_Accumulate(&hcrc, &data, 1);
	}

	__HAL_CRC_DR_RESET(&hcrc);

	if(host_crc == crc_value)
	{
		return BL_CRC_SUCCES;
	}
	return BL_CRC_FAIL;
}

