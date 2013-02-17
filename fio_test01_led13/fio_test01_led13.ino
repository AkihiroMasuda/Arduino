int PIN = 13; //PB5を光らす
//int PIN = 8; //自作加速度センサー基板用。PB0を光らす

void setup()
{
  pinMode(PIN, OUTPUT);
//  pinMode(8, OUTPUT);
}

void loop()
{
  digitalWrite(PIN,HIGH);
//  digitalWrite(8,HIGH);
  delay(350);
  digitalWrite(PIN,LOW);
//  digitalWrite(8,LOW);
  delay(150);
  
}
