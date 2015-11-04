#include <ja_led_controller.h>
#include <Arduino.h>
#include <stddef.h>
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