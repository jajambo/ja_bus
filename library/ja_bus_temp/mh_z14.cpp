#include <HardwareSerial.h>
#include <mh_z14.h>

int mhz14Co2Meter::sendCmd(int cmd)
{
  tempJitter = mhz14Jitter;
  
  switch(cmd){
  case MH_Z14_UPDATE_REGISTER:
    temp_buf[0] = MH_Z14_START_FLAG;
    temp_buf[1] = MH_Z14_SENSOR_ID;
    temp_buf[2] = 0x86;
    temp_buf[3] = 0;
    temp_buf[4] = 0;
    temp_buf[5] = 0;
    temp_buf[6] = 0;
    temp_buf[7] = 0;
    temp_buf[8] = 0x79;
    break;
  case MH_Z14_DO_ZERO_CALIBRATION:
    temp_buf[0] = MH_Z14_START_FLAG;
    temp_buf[1] = 0x87;
    temp_buf[2] = 0x87;
    temp_buf[3] = 0;
    temp_buf[4] = 0;
    temp_buf[5] = 0;
    temp_buf[6] = 0;
    temp_buf[7] = 0;
    temp_buf[8] = 0xf2;
    break;
  case MH_Z14_DO_SPAN_CALIBRATION:
    temp_buf[0] = MH_Z14_START_FLAG;
    temp_buf[1] = 0x88;
    temp_buf[2] = 0x88;
    temp_buf[3] = 0;
    temp_buf[4] = 0;
    temp_buf[5] = 0;
    temp_buf[6] = 0;
    temp_buf[7] = 0;
    temp_buf[8] = 0xf0;
    break;

  }
  //wait for respond
  devSerial->write(temp_buf,9);
  while(tempJitter == mhz14Jitter) {
    
  }
  return 0;
}

int mhz14Co2Meter::getCo2Value(void)
{
    return co2Value;
}
int mhz14Co2Meter::getTempValue(void)
{
    return tempValue;
}

void  mhz14Co2Meter::zeroCalibration(void)
{
    sendCmd(MH_Z14_DO_ZERO_CALIBRATION);
  
}
void  mhz14Co2Meter::spanCalibration(void)
{
  sendCmd(MH_Z14_DO_SPAN_CALIBRATION);
}
void  mhz14Co2Meter::updateRegister(void)
{
  sendCmd(MH_Z14_UPDATE_REGISTER);
}

void mhz14Co2Meter::co2SerialIrq() {
  	if (devSerial->check_no_error()) {
    // No Parity error, read byte and store it in the buffer if there is
    // room
    unsigned char c = devSerial->get_from_buf();
    mh_z14_cmd_t  *cmd_buf = (mh_z14_cmd_t *)temp_buf;
#if	JA_DEBUG		
    unsigned char t;
    byte_ascii(c,t);
    Serial.write(" ");
#endif
		switch(temp_count) {
		case 0:
			if (c == 0xFF) {
				temp_buf[temp_count] = c;
				temp_count ++;
			}
#if	JA_DEBUG		
			Serial.write("case 0\n");
#endif
			break;	
		case 1:
			if (c == MH_Z14_SENSOR_ID || c == MH_Z14_ZERO_CALIBRATION || c == MH_Z14_SPAN_CALIBRATION) {
					temp_buf[temp_count] = c;
					temp_count ++;
			} else {
					temp_count = 0;
			}

#if	JA_DEBUG		
			Serial.write("case 1\n");
#endif
			break;
		case 2:
		case 3:
		case 4:
		case 5:
    case 6:
    case 7:
			temp_buf[temp_count] = c;
			temp_count ++;
#if	JA_DEBUG		
      Serial.write("case data\n");
#endif
    case 8:

      temp_count = 0;
      
      if (temp_buf[1] == MH_Z14_SENSOR_ID) {
        co2Value = temp_buf[2] << 8;
        co2Value += temp_buf[3];
        tempValue = temp_buf[4];
      }
      mhz14Jitter ++;
			break;
		default:
        temp_count = 0;
			break;
		}

  } else {
    // Parity error, read byte but discard it
    devSerial->get_from_buf();
  }
}

mhz14Co2Meter::mhz14Co2Meter(HardwareSerial * serial)
{
  devSerial = serial;
  devSerial->begin(9600);
  mhz14Jitter = 0;
  temp_count = 0;
}
