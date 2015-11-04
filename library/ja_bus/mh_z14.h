#ifndef _MH_Z14_H
#define _MH_Z14_H

#define MH_Z14_SENSOR_ID  0x1
#define MH_Z14_ZERO_CALIBRATION  0x87
#define MH_Z14_SPAN_CALIBRATION  0x88

#define MH_Z14_START_FLAG   0xFF

#define MHZ14_BUFFER_SIZE   0x10
#define MH_Z14_UPDATE_REGISTER 1
#define MH_Z14_DO_ZERO_CALIBRATION 2
#define MH_Z14_DO_SPAN_CALIBRATION 3

#pragma pack(push)
#pragma pack(1)
typedef struct _mh_z14_cmd {
  unsigned char startFlag;
  unsigned char  sensorNo;
  unsigned char  order;
	unsigned char	data[5];
	unsigned char checkSum;
}mh_z14_cmd_t;
#pragma pack(pop)


class mhz14Co2Meter
{
protected:
  HardwareSerial *devSerial;
  int co2Value;
  int tempValue;
  int mhz14Jitter;
  int tempJitter;
  unsigned char temp_buf[MHZ14_BUFFER_SIZE];
  int temp_count;
private:
  int  sendCmd(int);

public:
  int getCo2Value(void);
  int getTempValue(void);
  void co2SerialIrq(void);
  void  zeroCalibration(void);
  void  spanCalibration(void);
  void  updateRegister(void);
  mhz14Co2Meter(HardwareSerial *);

};


#endif  _MH_Z14_H