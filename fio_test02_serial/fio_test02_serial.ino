#include <SoftwareSerial.h>

void setup()
{
  Serial.begin(9600);
  Serial.println("abc");
}

void loop()
{
  digitalWrite(13,HIGH);
  delay(200);
  digitalWrite(13,LOW);
  delay(1000);
  Serial.println("a");
  
}
