


#include <ja_led_controller.h>
#include <TimerFour.h>

JAAirMeterDevice *dev;

void Serial1_RX_Irq() {
  dev->ISRHooking();
}


void isr(){
  dev->cmdService();
}

void setup()
{
  //Serial.begin(9600);
  dev = new JAAirMeterDevice((HardwareSerial *) &Serial1);
  
  Timer4.initialize(JA_CMD_SERVICE_INTERVAL);
  Timer4.attachInterrupt(isr);
}
void loop() {
  dev->run();

}
