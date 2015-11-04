#include <ja-bus-protocol.h>
#include <ja-device.h>

#ifndef _JA_WATER_METER_H
#define _JA_WATER_METER_H

#define TEMPTURE_METER_SUPPORT	0x01
#define PH_METER_SUPPORT	0x02
#define EC_METER_SUPPORT	0x03
#define DO_METER_SUPPORT	0x04
#define DO_PUMP_SUPPORT		0x05

#define DO_MODE_AUTO		0x01
#define DO_MODE_HOST_CONTROL	0x00

#pragma pack(push)
#pragma pack(1)
#typedef _water_meter_eerom {
	unsigned char serial_id[SERIAL_ID_LENGTH];
	slave_info    slave_info;
	unsigned int temp_offset;
	unsigned int ec_offset;
	unsigned int ph_offset;
	unsigned int do_offset;
}water_meter_errom;

#typedef _water_meter_register {
	dev_register dev_reg;
	unsigned int water_meter_status;
	unsigned int temp_value;
	unsigned int temp_offset;
	unsigned int ph_value;
	unsigned int ph_offset;
	unsigned int ec_value;
	unsigned int ec_offset;
	unsigned int do_value;
	unsigned int do_offset;
	unsigned int do_target_value;
	unsigned int do_pump_current_value;
	unsigned char temp_sample_rate;
	unsigned char ph_sample_rate;
	unsigned char ec_sample_rate;
	unsigned char do_sample_rate;
	unsigned char do_pump_mode;
	unsigned char do_pump_start;
	unsigned char do_pump_status;
}water_meter_register;
#pragma pack(pop)
#endif //_JA_WATER_METER_H
