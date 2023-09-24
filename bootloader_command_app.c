/*
 * bootloader_command_app.c
 *
 *  Created on: Sep 23, 2023
 *      Author: meteh
 */


/*
 * bootloader_command_app.c
 *
 *  Created on: Sep 20, 2023
 *      Author: metehan.cakmak
 */


#include "bootloader_command_app.h"


extern uint8_t supported_commands[12];


void bootloader_get_ver_cmd(uint8_t *bl_rx_data)
{
	uint8_t bl_version = 0;

	print_m("BL_DEBUG_MSG: BootLoader_GET_VER_CMD\n");

	uint32_t command_packet_lenght = bl_rx_data[0] + 1;
	uint32_t host_crc = *((uint32_t*)(bl_rx_data + command_packet_lenght - 4));

	// crc control


	if(!bootloader_verify_crc(bl_rx_data, command_packet_lenght - 4, host_crc))
	{
		bootloader_send_ACK(1);
		bl_version = BL_VER ;
		bootloader_send_to_host(&bl_version, 1);

	}
	else
	{
		bootloader_send_NACK();
	}

}


void bootloader_get_help_cmd(uint8_t *bl_rx_data)
{
	uint32_t command_packet_lenght = bl_rx_data[0] + 1;
	uint32_t host_crc = *((uint32_t*)(bl_rx_data + command_packet_lenght - 4));

	if(!bootloader_verify_crc(bl_rx_data, command_packet_lenght - 4, host_crc))
	{
		bootloader_send_ACK(sizeof(supported_commands));
		bootloader_send_to_host(supported_commands, sizeof(supported_commands));
	}
	else
	{
		bootloader_send_NACK();
	}

}

void bootloader_get_cid_cmd(uint8_t *bl_rx_data)
{
	//uint16_t rev_id;
	uint16_t dev_id;

	uint32_t tempREG = DBGMCU->IDCODE;

	//rev_id = (uint16_t)( (tempREG >> 16) & (0xFFFFU) );
	dev_id = (uint16_t)(tempREG & 0xFFFU);

	uint32_t command_packet_lenght = bl_rx_data[0] + 1;
	uint32_t host_crc = *((uint32_t*)(bl_rx_data + command_packet_lenght - 4));

	if(!bootloader_verify_crc(bl_rx_data, command_packet_lenght - 4, host_crc))
	{
		bootloader_send_ACK(2);
		bootloader_send_to_host((uint8_t*)&dev_id, 2);
	}
	else
	{
		bootloader_send_NACK();
	}

}

void bootloader_get_rdpStatus_cmd(uint8_t *bootloader_rx_data)
{




}


uint8_t bootloader_verify_crc(uint8_t *Buffer, uint32_t len, uint32_t host_crc)
{

	uint32_t crc_value = 0xFF;
	uint32_t data = 0;

	/*for(uint32_t i = 0; i < len; i++)
	{
		data = Buffer[i];
		crc_value = HAL_CRC_Accumulate(&hcrc, &data, 1);
	}*/

	crc_value = crc32(Buffer, len);

	__HAL_CRC_DR_RESET(&hcrc);

	if(host_crc == crc_value)
	{
		return BL_CRC_SUCCES;
	}
	return BL_CRC_FAIL;
}



void bootloader_send_ACK(uint8_t followLenght)
{
	uint8_t AckBuffer[2];
	AckBuffer[0] = BL_ACK_VALUE;
	AckBuffer[1] = followLenght;

	bootloader_send_to_host(AckBuffer, 2);
}
void bootloader_send_NACK()
{
	uint8_t nackValue = BL_NACK_VALUE;
	bootloader_send_to_host(&nackValue, 1);
}

void bootloader_send_to_host(uint8_t* message, uint16_t len)
{
	HAL_UART_Transmit(&huart2, message, len, HAL_MAX_DELAY);
}

uint32_t crc32(const uint8_t *s,size_t n)
{
	uint32_t crc=0xFFFFFFFF;

	for(size_t i=0;i<n;i++) {
		uint8_t ch=s[i];
		for(size_t j=0;j<8;j++) {
			uint32_t b=(ch^crc)&1;
			crc>>=1;
			if(b) crc=crc^0xEDB88320;
			ch>>=1;
		}
	}

	return ~crc;
}

