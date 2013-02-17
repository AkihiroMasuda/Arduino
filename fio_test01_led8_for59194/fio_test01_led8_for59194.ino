int port = 8;

void setup()
{
  pinMode(port, OUTPUT);
}

void loop()
{
  digitalWrite(port,HIGH);
  delay(20);
  digitalWrite(port,LOW);
  delay(20);
  
}
