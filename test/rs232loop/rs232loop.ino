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
Serial.print("float size ");
byte_ascii(sizeof(float),i);
Serial.write("\n");
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
  //digitalWrite(21,HIGH);
 //   Serial1.write(0xAA);
  //      byte_ascii(c,b);
   // digitalWrite(21,LOW);
    //delay(10000);
  if(Serial1.available())
  {
  //  
    c = Serial1.read();

    digitalWrite(21,HIGH);
    Serial1.write(c);
      //  byte_ascii(c,b);
      delay(1);
    digitalWrite(21,LOW);
 }
   
}

