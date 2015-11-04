#include <ja_air_meter.h>

void  JAAirMeterDevice::processClassCmd(void)
{
  
}

void  JAAirMeterDevice::co2SerialIrq(void)
{
  mhz14->co2SerialIrq();
}


void	JAAirMeterDevice::run(void)
{
  mhz14->updateRegister();
}

JAAirMeterDevice::JAAirMeterDevice(HardwareSerial *serial)
{
  slave_info.slave_class = AIR_METER;
	//temp solution need remove after
  dev_serial = serial;
  dev_serial->begin(9600);
	

  mhz14 = new mhz14Co2Meter((HardwareSerial *)&Serial2);
}