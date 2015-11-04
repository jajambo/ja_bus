#include <HardwareSerial.h>
#include <ja_bus_protocol.h>
#include <ja_device.h>

#define byte_ascii(c,b) \
  b = (c/16 <=9)? c/16+0x30:c/16+0x40-9; \
  Serial.write(b);\
  b = (c%16 <=9)? c%16+0x30:c%16+0x40-9; \
  Serial.write(b)
  
#define BUFFER_SIZE 32
unsigned char temp_buf[BUFFER_SIZE];

unsigned char temp_count = 0;
unsigned char	rest_count = 0;
unsigned int head, tail, start_offset = 0;
ja_slave_info_t *info,dev_info;
ja_device_command_t *cmd_buf =
  (ja_device_command_t *)temp_buf;


void Serial1_RX_Irq() {
	if (Serial1.check_no_error()) {
    // No Parity error, read byte and store it in the buffer if there is
    // room
    unsigned char c = Serial1.get_from_buf();
    unsigned char t;
		unsigned int i = (tail + 1) % BUFFER_SIZE;
    byte_ascii(c,t);

    Serial.write(" ");
		switch(temp_count) {

		case 0:
			if (c == 0x55 || c == 0xAA) {
				start_offset = tail;
				temp_buf[temp_count] = c;
				temp_count ++;
			}
			Serial.write("case 0\n");
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
			Serial.write("case 1\n");
			break;

		case 2:
			temp_buf[temp_count] = c;
			temp_count ++;
			Serial.write("case 2\n");
			break;

		case 3:
			temp_buf[temp_count] = c;
			temp_count ++;
			//check slave_info_t
			info = (ja_slave_info_t *) (temp_buf + 2);
			if ((info->slave_class != dev_info.slave_class) ||
					(info->slave_id != dev_info.slave_id)) {
				temp_count = 0;
			}
			Serial.write("case 3\n");
			break;

		case 4:
			temp_buf[temp_count] = c;
			temp_count ++;
			if (c & JA_READ) {
				rest_count = 1;
			}
			else {
        if ((c&0x07) <=4) {
				  rest_count = 1 << (c & 0x07);
				  rest_count +=	3;
        } else
          rest_count += 3;
						
			}
			//check byte count
			Serial.write("case 4\n");
			break;

		default:
			if(rest_count) {
				temp_buf[temp_count] = c;
				temp_count ++;
				rest_count --;
        Serial.write("default\n");
        if(!rest_count){
          byte_ascii(cmd_buf->cmd_header.cmd,t);
          if (cmd_buf->cmd_header.cmd == CMD_RELEASE_BUS){
            Serial1.inc_sync_jitter();
            Serial.write("sync jitter\n");
          }
          else if (cmd_buf->cmd_header.cmd == CMD_ACK){
            Serial1.inc_ack_jitter();
            Serial.write("ACK jitter\n");
          }
          else {
            Serial1.inc_cmd_jitter();
            Serial.write("CMD jitter\n");
          }
          temp_count = 0;
        }
			}

			break;
		}

  } else {
    // Parity error, read byte but discard it
    Serial1.get_from_buf();
  }

}
void setup() {
	dev_info.slave_class = 0x02;
	dev_info.slave_id = 0x01;
	Serial.begin(9600);
	Serial1.begin(9600);

}

void loop() {
  Serial1.write(0x11);
  Serial1.write(0x55);
  Serial1.write(0xAA);
  Serial1.write(0x02);
  Serial1.write(0x01);
  Serial1.write(0x7D);
  Serial1.write(0x55);
  Serial1.write(0xAA);
  Serial1.write(0xAA);
  Serial1.write(0xAA);
  Serial1.write(0xFF);
  Serial.write(0x11);
    Serial1.write(0x11);
  Serial1.write(0x55);
  Serial1.write(0xAA);
  Serial1.write(0x02);
  Serial1.write(0x01);
  Serial1.write(0x74);
  Serial1.write(0x55);
  Serial1.write(0xAA);
  Serial1.write(0xAA);
  Serial1.write(0xAA);
  Serial1.write(0xFF);
    Serial1.write(0xAA);
  Serial1.write(0xAA);
  Serial1.write(0xAA);
  Serial1.write(0xFF);
    Serial1.write(0xAA);
  Serial1.write(0xAA);
  Serial1.write(0xAA);
  Serial1.write(0xFF);
  Serial1.write(0xAA);
  Serial1.write(0xAA);
  Serial1.write(0xAA);
  Serial1.write(0xFF);
  Serial1.write(0xAA);
  Serial1.write(0xAA);
  Serial1.write(0xAA);
  Serial1.write(0xFF);
  while(1);

}
