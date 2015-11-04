#include <ja_ir_commander.h>

void  JAIrCommanderDevice::processClassCmd(void)
{
  
}




void	JAIrCommanderDevice::run(void)
{
}

JAIrCommanderDevice::JAIrCommanderDevice(HardwareSerial *serial)
{
  slave_info.slave_class = IR_COMMANDER;
	//temp solution need remove after
  dev_serial = serial;
  dev_serial->begin(9600);

}