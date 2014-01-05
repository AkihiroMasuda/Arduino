#define LEDPIN 13

void setup() {
  pinMode(LEDPIN, OUTPUT); //
}

void loop() {
  digitalWrite(LEDPIN, HIGH);   // LEDをON
  delay(1000);              // 1000秒待つ
  digitalWrite(LEDPIN, LOW);    // LEDをOFF
  delay(1000);              // 1000秒待つ
}

