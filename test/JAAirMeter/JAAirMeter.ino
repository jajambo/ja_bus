


#include <ja_air_meter.h>
#include <TimerFour.h>

JAAirMeterDevice *dev;

void Serial1_RX_Irq() {
  dev->ISRHooking();
}

void Serial2_RX_Irq() {
  dev->co2SerialIrq();
}

void isr(){
  dev->cmdService();
}
void setup()
{
  Serial.begin(9600);
  dev = new JAAirMeterDevice((HardwareSerial *) &Serial1);
  
  Timer4.initialize(JA_CMD_SERVICE_INTERVAL);
  Timer4.attachInterrupt(isr);
}
void loop() {
  dev->run();
  Serial1.write(0x11);
  Serial1.write(0x55);
  Serial1.write(0xAA);
  Serial1.write(0x7D);
  Serial1.write(0x55);
  Serial1.write(0x02);
  Serial1.write(0x01);
  Serial1.write(0xAA);
  Serial1.write(0xAA);
  Serial1.write(0xAA);
  Serial1.write(0xFF);
  Serial.write(0x11);
    Serial1.write(0x11);
  Serial1.write(0x55);
  Serial1.write(0xAA);
  Serial1.write(0x74);
  Serial1.write(0x55);
  Serial1.write(0x02);
  Serial1.write(0x01);
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
