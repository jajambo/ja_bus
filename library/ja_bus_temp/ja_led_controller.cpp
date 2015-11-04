#include <ja_led_controller.h>
#include <Arduino.h>
#include <stddef.h>



void JALedControllerDevice::syncFromEeprom(void) {
	int i;
	char *dest = (char *) &dev_eeprom;
  //read device serial_id from eeprom
	for(i=0; i<SERIAL_ID_LENGTH; i++) {
		dev_eeprom.serial_id[i] = eeprom_read_byte((uint8_t *)i);
	}
  //read host serial_id from eeprom
	for(i=0; i<SERIAL_ID_LENGTH; i++) {
		dev_eeprom.host_serial_id[i] = eeprom_read_byte((uint8_t *)(i+
				offsetof(ja_dev_eeprom_t, host_serial_id)));
	}
  //read device id from eeprom
	dev_eeprom.slave_info.slave_class = 
		eeprom_read_byte((uint8_t *)(offsetof(ja_dev_eeprom_t,slave_info)+
				offsetof(ja_slave_info_t,slave_class)));
	dev_eeprom.slave_info.slave_id = 
		eeprom_read_byte((uint8_t *)(offsetof(ja_dev_eeprom_t,slave_info)+
				offsetof(ja_slave_info_t,slave_id)));
	for(i=0; i < CAPABILITY_LENGTH; i++)
			dev_eeprom.capability[i] = 
				eeprom_read_byte((uint8_t *)(offsetof(ja_dev_eeprom_t,capability) + i));
	
	dev_eeprom.sn_program_disable = 
		eeprom_read_byte((uint8_t *)(offsetof(ja_dev_eeprom_t,sn_program_disable)));
#if JA_DEBUG
	char t;
	Serial.write("eeprom when sync <<\n");
	for(i=0; i<sizeof(dev_eeprom); i++){
		byte_ascii(*dest,t);
		Serial.write("\n");
		dest ++;
	}
	Serial.write("eeprom when sync >>\b");
	dest = (char *) &dev_eeprom;
#endif
	i = 0;
	while(*dest && i < SERIAL_ID_LENGTH){
		i ++;
	}
	//device sn doesn't empty
	if( i == SERIAL_ID_LENGTH)
		dev_sn_empty = false;
	else
		dev_sn_empty = true;

	dest = (char *) &dev_eeprom + offsetof(ja_dev_eeprom_t,host_serial_id);
	i = 0;
	while(*dest && i < SERIAL_ID_LENGTH){
		i ++;
	}
	//device sn doesn't empty
	if( i == SERIAL_ID_LENGTH)
		host_sn_empty = false;
	else
		host_sn_empty = true;
		
	if(host_sn_empty || dev_sn_empty){
#if JA_DEBUG
		Serial.write("device SN doesn't existed\n");
#endif
		//device sn is empty;
		//set slave_info as some device class and boradcast id
		dev_eeprom.slave_info.slave_class = slave_info.slave_class;
		dev_eeprom.slave_info.slave_id = 0;
		dev_id_empty = true;
		dev_eeprom.sn_program_disable = 0;

	} else {
#if JA_DEBUG
	Serial.write("device SN existed\n");
#endif
		//force sn program disable
		if(!dev_eeprom.sn_program_disable){
			dev_eeprom.sn_program_disable = 0xAA;
			eeprom_write_byte((uint8_t*)(offsetof(ja_dev_eeprom_t,sn_program_disable)), 0);
			eeprom_write_byte((uint8_t*)(offsetof(ja_dev_eeprom_t,sn_program_disable)),
											dev_eeprom.sn_program_disable);

		}
	}
}

void JALedControllerDevice::syncJitter(void) {
	rx_jitter = dev_serial->get_rx_jitter();
	cmd_jitter = dev_serial->get_cmd_jitter();
	sync_jitter = dev_serial->get_sync_jitter();
	ack_jitter = dev_serial->get_sync_jitter();
}

void JALedControllerDevice::updateSn(char *dest, char *sn) {
  int i = SERIAL_ID_LENGTH;
  memcpy(dest, sn, i);
	checkHostMatched();
}


bool JALedControllerDevice::checkHostMatched() {
  int i = sizeof(host_sn);
  if(memcmp(host_sn, dev_eeprom.host_serial_id, i)) {
					device_status |= DEV_HOST_MATCHED;
					return true;
	} else {
					device_status &= ~DEV_HOST_MATCHED;
					return false;
	}
}

void JALedControllerDevice::ISRHooking() {
  	if (dev_serial->check_no_error()) {
    // No Parity error, read byte and store it in the buffer if there is
    // room
    unsigned char c = dev_serial->get_from_buf();
    ja_slave_info_t *info;
    ja_device_command_t  *cmd_buf = (ja_device_command_t *)temp_buf;
#if	JA_DEBUG		
    unsigned char t;
    byte_ascii(c,t);
    Serial.write(" ");
#endif
		switch(temp_count) {
		case 0:
			if (c == 0x55 || c == 0xAA) {
				temp_buf[temp_count] = c;
				temp_count ++;
			}
#if	JA_DEBUG		
			Serial.write("case 0\n");
#endif
			break;	
		case 1:
			if (c == 0x55 || c == 0xAA) {
				if ((temp_buf[temp_count -1] ^ c) == 0xFF) {
					temp_buf[temp_count] = c;
					temp_count ++;
				} else {
					temp_count = 0;
				}
			} else {
        temp_count = 0;
			}
#if	JA_DEBUG		
			Serial.write("case 1\n");
#endif
			break;
		case 2:
			temp_buf[temp_count] = c;
			temp_count ++;
			if (c & JA_READ) {
				rest_count = sizeof(cmd_buf->crc);
			}
			else {
        if ((c&0x07) <=4) {
				  rest_count = 1 << (c & 0x07);
        }

				//hard core CRC size;
				rest_count += sizeof(cmd_buf->crc);
						
			}
			//check byte count
#if	JA_DEBUG		
			Serial.write("case 2\n");
#endif
			break;
		case 3:
			temp_buf[temp_count] = c;
			temp_count ++;
#if	JA_DEBUG		
      Serial.write("case 3\n");
#endif
			break;
		case 4:
			temp_buf[temp_count] = c;
			temp_count ++;
#if	JA_DEBUG		
			Serial.write("case 4\n");
#endif
			break;
		case 5:
			temp_buf[temp_count] = c;
			temp_count ++;
			//check slave_info_t
      if (cmd_buf->cmd_header.cmd != CMD_RELEASE_BUS) {
			  info = (ja_slave_info_t *)(temp_buf +
              offsetof(ja_host_command_header_t, slave_info));
			  if ((info->slave_class != dev_eeprom.slave_info.slave_class) ||
				  	(info->slave_id != dev_eeprom.slave_info.slave_id)) {
				  temp_count = 0;
			  }
      }
#if	JA_DEBUG		
      Serial.write("case 5\n");
#endif
			break;
		default:
			if(rest_count) {
				temp_buf[temp_count] = c;
				temp_count ++;
				rest_count --;
#if	JA_DEBUG		
        Serial.write("default\n");
#endif
        if(!rest_count){
          byte_ascii(cmd_buf->cmd_header.cmd,t);
          if (cmd_buf->cmd_header.cmd == CMD_RELEASE_BUS){
            dev_serial->inc_sync_jitter();
#if	JA_DEBUG		
            Serial.write("sync jitter\n");
#endif
          }
          else if (cmd_buf->cmd_header.cmd == CMD_ACK){
            dev_serial->inc_ack_jitter();
#if	JA_DEBUG		
            Serial.write("ACK jitter\n");
#endif
          }
          else {
            dev_serial->inc_cmd_jitter();
#if	JA_DEBUG		
            Serial.write("CMD jitter\n");
#endif
          }
					//update rx_cmd_buf
          memcpy((void *)&rx_cmd_buf, temp_buf, BUFFER_SIZE);
          temp_count = 0;
        }
			}
			break;
		}

  } else {
    // Parity error, read byte but discard it
    dev_serial->get_from_buf();
  }
}

void JALedControllerDevice::start(void)
{

}




void JALedControllerDevice::cmdService(void)
{
  do {
    if (sync_jitter != dev_serial->get_sync_jitter()) {
      device_status |= DEV_RECEIVE_RELEASE_BUS;
      sync_jitter = dev_serial->get_sync_jitter();
    } else if (cmd_jitter != dev_serial->get_cmd_jitter()) {
      device_status |= DEV_RECEIVE_CMD;
      cmd_jitter = dev_serial->get_cmd_jitter();
    }
    switch(device_state) {
    case DEV_WAIT_FOR_SYNC:
      waitForSync();
      break;
    case DEV_TRY_REQUEST_ID:
      tryRequestId();
      break;
    case DEV_READY:
      dispatchCmd();
      break;
    //case DEV_TRY_REQUEST:
    //  tryRequest();
    //  break;
    //case DEV_TRY_PROCESS:
    //  tryProcessCmd();
    //  break;
    case DEV_DEAD:
      while(1) {
      }
    default:
      break;
    }
  //if device ready or wait for sync without any cmd/sync received.
  }while((device_state != DEV_READY && device_state != DEV_WAIT_FOR_SYNC) ||
      sync_jitter != dev_serial->get_sync_jitter() || cmd_jitter != dev_serial->get_cmd_jitter());
}



void JALedControllerDevice::processJADevCmd(void) {

      if(rx_cmd_buf.cmd_header.reg_index < sizeof (ja_dev_register_t)) {
        switch (rx_cmd_buf.cmd_header.cmd) {
        case CMD_WRITE_BYTE:
        case CMD_READ_BYTE:
        case CMD_WRITE_2_BYTE:
        case CMD_READ_2_BYTE:
        case CMD_WRITE_4_BYTE:
        case CMD_READ_4_BYTE:
        case CMD_WRITE_8_BYTE:
        case CMD_READ_8_BYTE:
        case CMD_WRITE_16_BYTE:
        case CMD_READ_16_BYTE:
          break;
        }
      }
      else
        processClassCmd();
}

void JALedControllerDevice::tryProcessCmd(void) {
  if(device_status & DEV_HOST_MATCHED) {
      device_status &= ~ DEV_RECEIVE_CMD;
      switch (rx_cmd_buf.cmd_header.cmd) {

      case CMD_UPDATE_SN_ENABLE:
      case CMD_UPDATE_SN_DISABLE:
      case CMD_FORCE_STOP:
        //device_state = DEV_TRY_PROCESS;
        
        break;
      default:
        processJADevCmd();
        break;
      }
      device_status &= ~ DEV_RECEIVE_RELEASE_BUS;
  } else {
      switch (rx_cmd_buf.cmd_header.cmd) {
      case CMD_UPDATE_SN_ENABLE:
      case CMD_UPDATE_SN_DISABLE:
      case CMD_FORCE_STOP:
        //device_state = DEV_TRY_PROCESS;
        break;
      default:
        break;
      }
  }
}

void JALedControllerDevice::waitForAck(unsigned int timeout)
{
  
}

void JALedControllerDevice::tryRequest(void) {
  unsigned int timeout = DEV_DEFAULT_TIMEOUT;
	//clear irq jitter before delay to check interrupt occured
  if(reportId){
    rx_jitter = dev_serial->get_rx_jitter();
    tx_cmd_buf.cmd_header.start_flag = DEV_START_FLAG;
    tx_cmd_buf.cmd_header.cmd = CMD_DEV_REPORT;
    tx_cmd_buf.cmd_header.reg_index = reportId;
    tx_cmd_buf.cmd_header.slave_info.slave_class = 
					dev_eeprom.slave_info.slave_class;
    tx_cmd_buf.cmd_header.slave_info.slave_id = 
					dev_eeprom.slave_info.slave_id;
	
    tx_cmd_buf.data[0] = reportData[0];

    rankDelay(device_rank);
    if(rx_jitter == dev_serial->get_rx_jitter()){
      //there are not any interrupt occured, it means bus was free
      sendCmd(4,0);
      waitForAck(timeout);
      //wait for ack;
      device_rank = DEV_DEFAULT_RANK;
      reportId = 0;
    } else {
      //report fail, premeept report rank for .
      device_rank --;
      if(device_rank < 0)
        device_rank = 0;
    }
    
  }
  
}

void JALedControllerDevice::forceStop(void) {
  
}

void JALedControllerDevice::functionStop(void) {
  
}

void JALedControllerDevice::dispatchCmd(void) {
  if (device_status & DEV_RECEIVE_RELEASE_BUS) {
    if(device_status & DEV_HOST_MATCHED) {
      //device_state = DEV_TRY_REQUEST;
      device_status &= ~ DEV_RECEIVE_RELEASE_BUS;
      tryRequest();
    }
    updateSn((char*)host_sn, (char *)&rx_cmd_buf.data);
  } else if (device_status & DEV_RECEIVE_CMD){
    device_status &= ~ DEV_RECEIVE_CMD;
    tryProcessCmd();
  }

  
}

void JALedControllerDevice::waitForSync(void) {
	
	if(device_status & DEV_RECEIVE_RELEASE_BUS) {
    Serial.write("recevied release bus\n");
		if (!(device_status & DEV_HOST_MATCHED)) {
      if (dev_id_empty || host_sn_empty || dev_sn_empty) {
        device_state = DEV_TRY_REQUEST_ID;
      } else {
        //set disable sn
        dev_eeprom.sn_program_disable = 0xAA;
        eeprom_write_byte((uint8_t*)(offsetof(ja_dev_eeprom_t,sn_program_disable)), 0);
        eeprom_write_byte((uint8_t*)(offsetof(ja_dev_eeprom_t,sn_program_disable)), dev_eeprom.sn_program_disable);
        device_state = DEV_READY;
      }
		} else {
			device_state = DEV_READY;
		}
		updateSn((char*)host_sn, (char *)&rx_cmd_buf.data);
    device_status &= ~(DEV_RECEIVE_RELEASE_BUS | DEV_INTERRUPT_OCCUR);
	}	
}

void JALedControllerDevice::beforeSendCmd(void) {

}
void JALedControllerDevice::afterSendCmd(void) {

}


void JALedControllerDevice::rankDelay(unsigned int i){
	unsigned long base,time;
	base = DEV_RANK_INTERVAL;
	base *= i;
	time = millis();
	base += ((time%DEV_RANK_MAX_RANDOM) *
				(DEV_RANK_INTERVAL/DEV_RANK_MAX_RANDOM));
	delay(base);
}

void JALedControllerDevice::sendCmd(size_t i, unsigned int flag){
	size_t size = sizeof(ja_host_command_header_t);
	char *src = (char *)&tx_cmd_buf;
	//hard core CRC size;
	size += (i + 2);
	beforeSendCmd();	
	dev_serial->write(src,size);
	afterSendCmd();
}

void JALedControllerDevice::programDevSn(void)
{
  unsigned int i;
  for (i = 0; i < SERIAL_ID_LENGTH; i++) {
    eeprom_write_byte((uint8_t*)i, 0),
    eeprom_write_byte((uint8_t*)i, dev_eeprom.serial_id[i]);
  }
}

void JALedControllerDevice::programHostSn(void)
{
  unsigned int i;
  for (i = 0; i < SERIAL_ID_LENGTH; i++) {
    eeprom_write_byte((uint8_t*)(i + offsetof(ja_dev_eeprom_t,host_serial_id)), 0);
    eeprom_write_byte((uint8_t*)(i + offsetof(ja_dev_eeprom_t,host_serial_id)),
        dev_eeprom.host_serial_id[i]);
  }

}


void JALedControllerDevice::programDevId(void)
{
  eeprom_write_byte((uint8_t*)offsetof(ja_dev_eeprom_t,slave_info.slave_id), 0);
  eeprom_write_byte((uint8_t*)offsetof(ja_dev_eeprom_t,slave_info.slave_id),
        dev_eeprom.slave_info.slave_id);
  eeprom_write_byte((uint8_t*)offsetof(ja_dev_eeprom_t,slave_info.slave_class), 0);
  eeprom_write_byte((uint8_t*)offsetof(ja_dev_eeprom_t,slave_info.slave_class),
        dev_eeprom.slave_info.slave_class);
}

bool JALedControllerDevice::waitForCmd(char cmd, unsigned int timeout)
{
  
}

void JALedControllerDevice::tryRequestId(void) {
	unsigned char empty_flag = 0;
  bool  isTimeout;
  unsigned int timeout = DEV_DEFAULT_TIMEOUT;
	//clear irq jitter before delay to check interrupt occured
	rx_jitter = dev_serial->get_rx_jitter();
	tx_cmd_buf.cmd_header.start_flag = DEV_START_FLAG;
	tx_cmd_buf.cmd_header.cmd = CMD_DEV_REQUEST_ID;
	tx_cmd_buf.cmd_header.slave_info.slave_class = 
					dev_eeprom.slave_info.slave_class;
	tx_cmd_buf.cmd_header.slave_info.slave_id = 
					dev_eeprom.slave_info.slave_id;
	if(dev_sn_empty)
		empty_flag |= DEV_SN_EMPTY;
	if(host_sn_empty)
		empty_flag |= HOST_SN_EMPTY;
	if(dev_id_empty)
		empty_flag |= DEV_ID_EMPTY;
	tx_cmd_buf.data[0] = empty_flag;

	rankDelay(DEV_REQUEST_ID_RANK);
	if(rx_jitter == dev_serial->get_rx_jitter()){
		//there are not any interrupt occured, it means bus was free
		sendCmd(1,0);
    waitForAck(timeout);
	}

  //enable one shot timer to check wait host set timeout;
  if(dev_sn_empty){
    isTimeout = waitForCmd(CMD_SET_DEV_SN,timeout);
    if(!isTimeout){
      updateSn((char *)&dev_eeprom.serial_id, (char *)&rx_cmd_buf.data);
      programDevSn();
      dev_sn_empty = 0;
    } else {
      device_state = DEV_WAIT_FOR_SYNC;
      return;
    }
  }
  if(dev_id_empty){
    isTimeout = waitForCmd(CMD_SET_DEV_ID,timeout);
    if(!isTimeout){
      dev_eeprom.slave_info.slave_id = rx_cmd_buf.data[0];
      programDevId();
      dev_id_empty = false;
    } else {
      device_state = DEV_WAIT_FOR_SYNC;
      return;
    }
  }
  if(host_sn_empty){
      programHostSn();
      host_sn_empty = 0;
      device_status &= ~DEV_HOST_MATCHED;
  }
	device_state = DEV_READY;
  return;
}

/*
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
*/
/*
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

*/
/*
char device_init() {
	device_state = DEV_WAIT_FOR_SYNC;
	device_status = 0;
	sync_from_eeprom();
	check_host_matched();
	return 0;
}

char device_start() {

	do {
		run_state_machine(0);
	}while(1);
	return 0;
}
*/



void  JALedControllerDevice::processSwitchReg(void)
{
  uint8_t bytes, direct, *buf, i;
  direct = rx_cmd_buf.cmd_header.cmd & JA_READ;
  bytes = 1 <<(rx_cmd_buf.cmd_header.cmd & JA_REG_BYTES);
  if (bytes == sizeof(ledSwitch)) {
    if (direct) {
      tx_cmd_buf.cmd_header.start_flag = DEV_START_FLAG;
      tx_cmd_buf.cmd_header.cmd = rx_cmd_buf.cmd_header.cmd;
      tx_cmd_buf.cmd_header.reg_index = rx_cmd_buf.cmd_header.reg_index;
      tx_cmd_buf.cmd_header.slave_info.slave_class = 
          dev_eeprom.slave_info.slave_class;
      tx_cmd_buf.cmd_header.slave_info.slave_id = 
					dev_eeprom.slave_info.slave_id;
      buf = (uint8_t *) &ledSwitch;
      for (i = 0; i < bytes; i++)
        tx_cmd_buf.data[i] = *(buf +i);
      sendCmd(bytes, 0);
    }
    else {
      buf = (uint8_t *) ledSwitch;
      for (i = 0; i < bytes; i++)
        *(buf +i) = tx_cmd_buf.data[i];
      if(ledSwitch)
        digitalWrite(powerPin, HIGH);
      else
        digitalWrite(powerPin, LOW);
    }
  }
}

void  JALedControllerDevice::getCurrentReg(void)
{
  uint8_t bytes, direct, *buf, i;
  Serial.write("getCurrentReg\n");
  direct = rx_cmd_buf.cmd_header.cmd & JA_READ;
  bytes = 1 <<(rx_cmd_buf.cmd_header.cmd & JA_REG_BYTES);
  if (bytes == sizeof(currentValue) & direct) {
      tx_cmd_buf.cmd_header.start_flag = DEV_START_FLAG;
      tx_cmd_buf.cmd_header.cmd = rx_cmd_buf.cmd_header.cmd;
      tx_cmd_buf.cmd_header.reg_index = rx_cmd_buf.cmd_header.reg_index;
      tx_cmd_buf.cmd_header.slave_info.slave_class = 
          dev_eeprom.slave_info.slave_class;
      tx_cmd_buf.cmd_header.slave_info.slave_id = 
					dev_eeprom.slave_info.slave_id;
      buf = (uint8_t *) &currentValue;
      for (i = 0; i < bytes; i++)
        tx_cmd_buf.data[i] = *(buf +i);
      sendCmd(bytes, 0);
  }
}

void  JALedControllerDevice::processClassCmd(void)
{
  uint8_t cmd;
  Serial.write("processClassCmd\n");
  switch (rx_cmd_buf.cmd_header.reg_index) {
    case offsetof(ja_led_controller_register_t, currentValue):
      getCurrentReg();
      break;
    case offsetof(ja_led_controller_register_t, ledSwtich):
      processSwitchReg();
      break;
  }

}




void	JALedControllerDevice::run(void)
{
  currentValue = (.0264 * analogRead(currentPin) - 13.51);
}

JALedControllerDevice::JALedControllerDevice(HardwareSerial *serial, uint8_t switchPin , uint8_t analogPin)
{
  slave_info.slave_class = LIGHT_CONTROLLER;
  currentPin = analogPin;
  powerPin = switchPin;
  pinMode(switchPin,OUTPUT);
  ledSwitch = 0;
  currentValue = 0;
	//temp solution need remove after
  dev_serial = serial;
  dev_serial->begin(9600);

}