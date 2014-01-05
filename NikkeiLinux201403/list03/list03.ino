#include <SoftwareSerial.h>

#define LED_PIN (13) //LEDピン番号定義
#define RX_PIN  (12) //RXピン番号定義
#define TX_PIN  (11) //TXピン番号定義
#define AIN_PIN (7)  //センサ入力ピン番号定義

SoftwareSerial mySerial(RX_PIN,TX_PIN); //シリアルポート設定

char str[128];           //送信用文字列バッファ
unsigned long nextTime;  //次回送信時刻
const unsigned long T = 1000; //送信間隔

void setup()
{
  //シリアルポート有効化。SBDBTのデフォルトのボーレート9600を設定
  mySerial.begin(9600); 
  
  // 次回送信時刻の更新
  nextTime = millis() + T;
}

void loop()
{
  // 現在時刻(=プログラムの実行開始から現在までの時間)を取得
  unsigned long curTime = millis();

  // 現在時刻が送信時刻を超えていたらデータ送信
  if (curTime > nextTime){
    
    // アナログ入力ピンの値を読み込む
    int analogInput = analogRead(AIN_PIN);
    
    // 送信用文字列を作成
    sprintf(str,"millis:%ld analogInput:%d",curTime, analogInput);

    // シリアルポートへ文字列送信
    mySerial.println(str);
    
    // スケッチ書き込み確認用にLED点滅
    digitalWrite(13,HIGH);
    delay(100);
    digitalWrite(13,LOW);
    
    // 次回送信時刻を更新
    nextTime += T;
  }
}

