#define LEDPIN (13) //LEDが繋がってるピン番号を定義

void setup() {
  pinMode(LEDPIN, OUTPUT); //指定ピンを出力モードに設定
}

void loop() {
  digitalWrite(LEDPIN, HIGH);   // LEDをON
  delay(1000);              // 1000秒待つ
  digitalWrite(LEDPIN, LOW);    // LEDをOFF
  delay(1000);              // 1000秒待つ
}

