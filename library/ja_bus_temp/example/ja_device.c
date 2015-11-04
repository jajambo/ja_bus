#include <stddef.h>
#include <ja_bus_protocol.h>
#include <ja_device.h>
#include <Arduino.h>
ja_device_command_t cmd_buf;
ja_device_command_t tmp_cmd_buf;
ja_dev_eeprom_t	dev_eeprom;
unsigned char	device_rank = DEV_DEFAULT_RANK;
unsigned char	device_state = DEV_WAIT_FOR_SYNC;
unsigned char	host_sn[16];
unsigned char	device_status = 0;
unsigned char	device_report;

static char update_host_id() {
	return 0;
}

static char check_host_matched() {
	return 0;
}



static char wait_for_sync() {
	char ret = NO_ERROR;
	if(device_status & DEV_RECEIVE_RELEASE_BUS) {

		if(!(device_status & DEV_HOST_MATCHED)) {
			device_state = DEV_TRY_REQUEST_ID;
		} else {
			device_state = DEV_READY;
		}
		//update_host_id();
		update_host_id();

	}
fail:
	device_status &= ~(DEV_RECEIVE_RELEASE_BUS | DEV_INTERRUPT_OCCUR);
	return ret;
}

static char send_request_id() {
	return 0;
}

static char send_report() {
	return 0;
}

static char try_request_id() {
	rank_delay(DEV_REQUEST_ID_RANK);
	if(!(device_status & DEV_INTERRUPT_OCCUR)){
		send_request_id();
		//TODO process id;
	}
fail:
	device_state = DEV_WAIT_FOR_SYNC;
	return	NO_ERROR;
}

static char try_request() {
	//only do report when we need.
	if(device_status & DEV_REQUEST_REPORT) {
		rank_delay(device_rank);
		if(device_status & DEV_RECEIVE_CMD){
			if(device_rank > 0){
				device_rank --;
			}
			goto fail;
		}
		if(!(device_status & DEV_INTERRUPT_OCCUR)){
			send_report();
			device_rank = DEV_DEFAULT_RANK;
		}
	}
	//success clear request report.
	device_status &= ~DEV_REQUEST_REPORT;
fail:
	device_state = DEV_READY;
	return	NO_ERROR;
}

static char wait_for_command() {
	//all status update here
	return 0;
}

static char process_cmd() {
	return 0;
}

static char try_process() {
	if(device_status & DEV_HOST_MATCHED) {
		process_cmd();
	} else {
		switch(cmd_buf.cmd_header.cmd) {
		case CMD_UPDATE_SN_ENABLE:
			break;
		case CMD_UPDATE_SN_DISABLE:
			break;
		}
	}
fail:
	device_state = DEV_READY;
	return 0;
}


static char run_state_machine(char dev_ready) {
	switch(device_state) {
	case DEV_WAIT_FOR_SYNC:
		wait_for_sync();
		break;
	case DEV_TRY_REQUEST_ID:
		try_request_id();
		break;
	case DEV_READY:
		if(!dev_ready)
			wait_for_command();
		break;
	case DEV_TRY_REQUEST:
		try_request();
		break;
	case DEV_TRY_PROCESS:
		try_process();
		break;
	case DEV_DEAD:
		while(1) {
		}
	default:
		break;
	}
	return 0;
}

char dispatch_cmd() {
	if(device_status & DEV_RECEIVE_RELEASE_BUS) {
		device_status &= ~DEV_RECEIVE_RELEASE_BUS;
		update_host_id();
		if(!check_host_matched()) {
			device_status &= ~DEV_HOST_MATCHED;
			device_status |= DEV_REQUEST_REPORT;
			device_report = DEV_HOST_NOT_MATCHED;
		}
		device_state = DEV_TRY_REQUEST;
		device_status &= ~DEV_INTERRUPT_OCCUR;

	}
	else if(device_status & DEV_RECEIVE_CMD) {
		device_status &= ~DEV_RECEIVE_CMD;
		device_state = DEV_TRY_PROCESS;
		device_status &= ~DEV_INTERRUPT_OCCUR;
		
	}
	while(device_state != DEV_READY) {
		run_state_machine(0);
	}

fail:
	return 0;
}

static char sync_from_eeprom() {
	int i;
	for(i=0; i<16; i++) {
		dev_eeprom.serial_id[i] = eeprom_read_byte(i);
	}
	for(i=0; i<16; i++) {
		dev_eeprom.host_serial_id[i] = eeprom_read_byte(i+
				offsetof(ja_dev_eeprom_t, host_serial_id));
	}
	dev_eeprom.slave_info.slave_id = 
		eeprom_read_byte(offsetof(ja_dev_eeprom_t,slave_info)+
				offsetof(ja_slave_info_t,slave_id));
	sync_device_data();
	return 0;
}

char device_init() {
	device_state = DEV_WAIT_FOR_SYNC;
	device_status = 0;
	sync_from_eeprom();
	check_host_matched();
	return 0;
}

char device_start() {
	/* wait for HOST_BUS_RELEASE as a sync signal*/
	do {
		run_state_machine(0);
	}while(1);
	return 0;
}
