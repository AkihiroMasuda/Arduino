#include <SoftwareSerial.h>

#define LED_PIN (13) //LEDピン番号定義
#define RX_PIN  (12) //RXピン番号定義
#define TX_PIN  (11) //TXピン番号定義
#define AIN_PIN (7)  //センサ入力ピン番号定義

SoftwareSerial mySerial(RX_PIN,TX_PIN); //シリアルポート設定

char str[128];           //送信用文字列バッファ
unsigned long nextTime;  //次回送信時刻
const unsigned long T = 1000; //送信間隔[ms]

void setup()
{
  //シリアルポート有効化。SBDBTのデフォルトのボーレート9600を設定
  mySerial.begin(9600); 

  // 次回送信時刻の更新
  nextTime = millis() + T;

  // ピンモードの設定
  pinMode(LED_PIN, OUTPUT);
  pinMode(RX_PIN, INPUT);
  pinMode(TX_PIN, OUTPUT);
  pinMode(AIN_PIN, INPUT);

}

void loop()
{
  // 現在時刻(=プログラムの実行開始から現在までの時間)を取得
  unsigned long curTime = millis();

  // 現在時刻が送信時刻を超えていたら、ホイールの回転時間と速度を求めてデータ送信
  if (curTime > nextTime){

    // ホイールの回転時間[ms]を求める
    long period = calcRotationPeriod();
    if (period > 0){
      // 成功
      // 速度を算出
      // ホイール一回転で進む距離を回転時間で割って求める
      // ホイール一回転で進む距離はタイヤの円周と等しいとする
      const double diameter = 30; // タイヤの直径[mm]
      double velocity = diameter*PI/double(period)*3600; // 速度 [km/h]. 3600は[mm/μs]->[km/h]変換の係数

      // 送信用文字列を作成
      char strVel[6]; // 速度の文字列格納用
      dtostrf(velocity,5,2,strVel); //arduinoのsprintfは浮動小数点を文字列にする%fが使えない。代わりにdtostrfを使う。
      sprintf(str,"millis:%ld period:%ld[μs] velocity:%s[km/h]",curTime, period, strVel);
    }else{
      // 失敗
      // 送信用文字列を作成
      sprintf(str,"millis:%ld period:TimeOver velocity:Unknown",curTime);
    }

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

// アナログ入力値がLOW→HIGHに切り替わるまで待つ
// 成功すればtrueを返す。タイムオーバーで失敗すればfalseを返す。
boolean waitLowToHigh()
{
  const int thresholdHIGH = 120; //HIGHを判断する閾値。アナログ入力値がこの値以上になったらHIGHと判断
  const int thresholdLOW = 60; //LOWを判断する閾値。アナログ入力値がこの値以上になったらLOWと判断
  const unsigned long timeOverDuration = 3000000; //タイムオーバー間隔[μs]。この時間内にHIGH/LOW切替がなければエラーとみなす

  // １．LOWになるまで待つ
  unsigned long timeOver = micros() + timeOverDuration; //タイムオーバー時刻
  while(true){
    // タイムオーバーチェック
    if (micros() > timeOver){
      // タイムオーバー発生
      return false;
    }
    // アナログ入力値を見て閾値以下か確認
    if (analogRead(AIN_PIN) < thresholdLOW){
      // 十分にLOWになったとみなして抜ける
      break;
    }
  }

  // ２．HIGHになるまで待つ
  timeOver = micros() + timeOverDuration; //タイムオーバー時刻更新
  while(true){
    // タイムオーバーチェック
    if (micros() > timeOver){
      // タイムオーバー発生
      return false;
    }
    // アナログ入力値を見て閾値以上か確認
    if (analogRead(AIN_PIN) > thresholdHIGH){
      // 十分にHIGHになったとみなし、時刻を保存
      break;
    }
  }

  return true;
}

// ホイールが１回転する時間を求める。
// 正しく求められた時はその値[μs]を返す。
// タイムオーバーなどで失敗した時は-1を返す。
long calcRotationPeriod()
{
  // LOW→HIGHになるまで待つ
  if (waitLowToHigh() == false){
    return -1;
  }
  // LOW→HIGHになった時刻を保存
  unsigned long t1 = micros();
  micros();

  // もう一度LOW→HIGHになるまで待つ
  if (waitLowToHigh() == false){
    return -1;
  }
  // LOW→HIGHになった時刻を保存
  unsigned long t2 = micros();

  // 時刻t1, t2の差分が１回転した時間
  return t2 - t1;
}

