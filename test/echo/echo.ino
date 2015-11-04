
void setup()
{

char i;
char *dest,t;

pinMode(20, OUTPUT);
pinMode(21, OUTPUT);
digitalWrite(20,LOW);
digitalWrite(21,LOW);
Serial.begin(9600);
Serial1.begin(9600);
Serial2.begin(9600);
Serial.write("before clear\n");
      digitalWrite(21,HIGH);
    //Serial1.write(0x31);
    digitalWrite(21,LOW);
        digitalWrite(20,HIGH);
    Serial2.write(0x30);
    digitalWrite(20,LOW);

}

void loop()
{
  unsigned char c;

  if(Serial1.available())
  {
    Serial.write("Serial1 received\n");
    c = Serial1.read();
    digitalWrite(21,HIGH);
    Serial1.write(c);
    digitalWrite(21,LOW);
  }
   if(Serial2.available())
  {
    Serial.write("Serial2 received\n");
  }
}

