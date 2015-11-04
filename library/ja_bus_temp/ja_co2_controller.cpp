#include <ja_co2_controller.h>

void  JACo2ControllerDevice::processClassCmd(void)
{
  
}




void	JACo2ControllerDevice::run(void)
{
}

JACo2ControllerDevice::JACo2ControllerDevice(HardwareSerial *serial)
{
  slave_info.slave_class = CO2_CONTROLLER;
	//temp solution need remove after
  dev_serial = serial;
  dev_serial->begin(9600);

}