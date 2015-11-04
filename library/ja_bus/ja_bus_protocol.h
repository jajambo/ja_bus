/*
 *
 *
 *
 */
#ifndef _JA_BUS_PROTOCOL_H
#define _JA_BUS_PROTOCOL_H
//host send out data field
//
#include <stdint.h>
#define HOST_START
#define	HOST_LISITENING

//device class slave_info_t slave_class;
#define WATER_METER		2
#define	AIR_METER		3
#define	PUMP_CONTROLLER			4
#define	LIGHT_CONTROLLER	5
#define	WATER_SOLENOID		6
#define FLOW_METER		7
#define IR_COMMANDER		8
#define CO2_CONTROLLER	9

#define BOARDCAST		0xFF
//device id slave_info_t salve_id;
#define BOARDCAST_ID		BOARDCAST

#define RELEASE_BUS_TIME	0x400	//1s

#define RIGESTER_SERIAL_ID	0x0
#define SERIAL_ID_LENGTH	0x10
#define RIGESTER_SLAVE_INFO	0x10
#define SLAVE_INFO_LENGTH	0x02
#define REGISTER_SLAVE_CLASS	0x10
#define REGISTER_SLAVE_ID	0x11
#define REGISTER_SLAVE_CAPABILITY 0x12
#define CAPABILITY_LENGTH	0x04

#define NO_ERROR 0

// |0  -  	15|16	-  31|32 - 39|40   -   47|48 -      	
// |start flag	  |slave addr|cmd    |register   |write data|CRC
//
// //HOST_RELEASE_BUS //for sync and release bus for device
// 0x55 0xaa 0xFF 0xFF 0x7F 0xFF (host serial id) |CRC
#pragma pack(push)
#pragma pack(1)

typedef struct _ja_slave_info {
	unsigned char	slave_class;
	unsigned char	slave_id;
}ja_slave_info_t;

typedef struct _ja_host_command_header {
        uint16_t	start_flag;
	unsigned char	cmd;
	unsigned char	reg_index;
	ja_slave_info_t	slave_info;
}ja_host_command_header_t;



typedef struct _ja_dev_eeprom {
	unsigned char	serial_id[SERIAL_ID_LENGTH];
	unsigned char	host_serial_id[SERIAL_ID_LENGTH];
	ja_slave_info_t	slave_info;
	unsigned char	capability[CAPABILITY_LENGTH];
	unsigned char sn_program_disable;
}ja_dev_eeprom_t;

typedef struct _ja_dev_register {
	unsigned char	serial_id[SERIAL_ID_LENGTH];
	unsigned char	host_serial_id[SERIAL_ID_LENGTH];
	ja_slave_info_t	slave_info;
	unsigned char	capability[CAPABILITY_LENGTH];
}ja_dev_register_t;

typedef struct _ja_device_command {
	ja_host_command_header_t cmd_header;
	unsigned char	data[16];
        uint16_t	crc;
}ja_device_command_t;


#pragma pack(pop)



#define JA_READ		            0x80
#define JA_REG_BYTES          0x07
#define HOST_START_FLAG	      0xAA55
#define CMD_WRITE_BYTE	      0x00
#define CMD_READ_BYTE	        0x80
#define CMD_WRITE_2_BYTE	    0x01
#define CMD_READ_2_BYTE	      0x81
#define CMD_WRITE_4_BYTE	    0x02
#define CMD_READ_4_BYTE	      0x82
#define CMD_WRITE_8_BYTE	    0x03
#define CMD_READ_8_BYTE	      0x83
#define CMD_WRITE_16_BYTE	    0x04
#define CMD_READ_16_BYTE	    0x84
#define JA_DATA_LENGTH		    0x04 //0:1byte 1:2 bytes 2:4 bytes.
#define CMD_DEV_REQUEST_ID		0x91
#define CMD_SET_DEV_SN        0x14
#define CMD_SET_DEV_ID        0x11
#define CMD_DEV_REPORT        0x94

#define CMD_RELEASE_BUS		    0x74
#define CMD_FUNCTION_STOP     0x7B
#define CMD_FORCE_STOP        0x7C
#define CMD_ACK				        0x7D
#define	CMD_UPDATE_SN_ENABLE	0x7E
#define	CMD_UPDATE_SN_DISABLE	0x7F


#endif //JA_BUS_PROTOCOL_H
