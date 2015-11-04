#include <ja_pump_controller.h>

void  JAPumpControllerDevice::processClassCmd(void)
{
  
}




void	JAPumpControllerDevice::run(void)
{
}

JAPumpControllerDevice::JAPumpControllerDevice(HardwareSerial *serial)
{
  slave_info.slave_class = PUMP_CONTROLLER;
	//temp solution need remove after
  dev_serial = serial;
  dev_serial->begin(9600);

}