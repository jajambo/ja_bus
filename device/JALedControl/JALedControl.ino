


#include <ja_led_controller.h>
#include <TimerFour.h>

JALedControllerDevice *dev;

void Serial1_RX_Irq() {
  dev->ISRHooking();
}

void isr(){
  dev->cmdService();
}
void setup()
{
  
  Serial.begin(9600);
  
  dev = new JALedControllerDevice((HardwareSerial *) &Serial1, 1, 8, A0);
  
  Timer4.initialize(JA_CMD_SERVICE_INTERVAL);
  Timer4.attachInterrupt(isr);
}
void loop() {
  dev->run();
 // while(1);
}
