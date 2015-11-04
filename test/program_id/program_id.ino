#include <ja_bus_protocol.h>
#include <ja_device.h>
#include <EEPROM.h>
ja_dev_eeprom_t eeprom;
char dev_sn[16] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,
		0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10};
char host_sn[16] = {0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,
		0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,0x20};
ja_slave_info_t slave_info;
char capability[CAPABILITY_LENGTH] = {0x30,0x31,0x32,0x33};

void setup()
{

char i;
char *dest,t;
slave_info.slave_class = 0x02;
slave_info.slave_id = 0x01;
Serial.begin(9600);
Serial.write("before clear\n");

dest = (char *) &eeprom;
	for(i=0; i<sizeof(eeprom); i++){
		*dest = EEPROM.read(i);
		dest ++;
	}
dest = (char *) &eeprom;
	for(i=0; i<sizeof(eeprom); i++){
		byte_ascii(*dest,t);
		Serial.write("\n");
		dest ++;
	}

dest = (char *) &eeprom;
	for(i=0; i<sizeof(eeprom); i++){
		EEPROM.write(i,0);
	}
	for(i=0; i<sizeof(eeprom); i++){
		if(i<16)
			EEPROM.write(i,dev_sn[i]);
		else if(i>=16 && i<32)
			EEPROM.write(i,host_sn[i-16]);
		else if(i==32)
			EEPROM.write(i,slave_info.slave_class);
		else if(i==33)
			EEPROM.write(i,slave_info.slave_id);
		else
			EEPROM.write(i, capability[i-34]);
	}
	
	

Serial.write("after clear\n");

dest = (char *) &eeprom;
	for(i=0; i<sizeof(eeprom); i++){
		*dest = eeprom_read_byte((uint8_t *)i);
		dest ++;
	}

	
dest = (char *) &eeprom;
	for(i=0; i<sizeof(eeprom); i++){
		byte_ascii(*dest,t);
		Serial.write("\n");
		dest ++;
	}
		
}

void loop()
{
}

