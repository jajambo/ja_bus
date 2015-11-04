#define byte_ascii(c,b) \
  b = (c/16 <=9)? c/16+0x30:c/16+0x40-9; \
  Serial.write(b);\
  b = (c%16 <=9)? c%16+0x30:c%16+0x40-9; \
  Serial.write(b)
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

Serial.write("before clear\n");
      digitalWrite(21,HIGH);
    //Serial1.write(0x31);
    digitalWrite(21,LOW);
        digitalWrite(20,HIGH);

    digitalWrite(20,LOW);

}

void loop()
{
  unsigned char c,b;

  if(Serial1.available())
  {
    
    c = Serial1.read();
    //digitalWrite(21,HIGH);
    Serial.write(c);
    byte_ascii(c,b);
    //digitalWrite(21,LOW);
  }
  if(Serial.available())
  {
   // Serial.write("test");
    c = Serial.read();
    //Serial.write(c);
    digitalWrite(21,HIGH);
    Serial1.write(c);
    digitalWrite(21,LOW);
  }
   
}

