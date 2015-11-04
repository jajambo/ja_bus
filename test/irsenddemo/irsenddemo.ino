/*
 * IRremote: IRsendDemo - demonstrates sending IR codes with IRsend
 * An IR LED must be connected to Arduino PWM pin 3.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 */

#include <AlternativeIRremote.h>
#include <AlternativeIRremoteInt.h>

AlternativeIRsend irsend;
unsigned int t25_on[75]={179,95,10,33,12,11,12,33,12,32,11,11,12,11,12,33,12,12,11,12,10,33,12,12,11,12,11,33,11,12,11,12,11,14,10,12,12,11,12,11,12,11,11,33,12,33,11,11,11,13,12,32,13,11,11,32,12,11,11,13,10,34,11,12,10,13,12,11,11,13,11,12,11,12,11};

unsigned int t25_off[75]={180,94,11,32,11,12,12,33,11,32,12,11,11,12,11,34,12,12,12,10,11,34,12,11,11,12,12,32,11,12,11,12,11,14,11,12,10,12,12,11,12,11,11,12,11,34,11,12,11,12,11,33,12,12,12,32,11,11,11,12,12,33,11,12,12,12,11,11,11,12,11,13,11,12,11};


void setup()
{
  Serial.begin(9600);
  Serial.print(TIMER_PWM_PIN);
}

void loop() {
  unsigned char c;
  if (Serial.available()) {
    c = Serial.read();
    switch(c){
      case 'n':
      case 'N':
        Serial.print("send on\n");
        irsend.sendRaw(t25_on, 75, 38);
        break;
      case 'f':
      case 'F':
      Serial.print("send off\n");
        irsend.sendRaw(t25_off, 76, 38);
        break;
    }
  }
}
