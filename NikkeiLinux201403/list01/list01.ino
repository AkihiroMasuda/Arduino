#define LED_PIN (13) //LEDが繋がってるピン番号を定義

void setup() {
  pinMode(LED_PIN, OUTPUT); //指定ピンを出力モードに設定
}

void loop() {
  digitalWrite(LED_PIN, HIGH);   // LEDをON
  delay(1000);              // 1秒待つ
  digitalWrite(LED_PIN, LOW);    // LEDをOFF
  delay(1000);              // 1秒待つ
}

