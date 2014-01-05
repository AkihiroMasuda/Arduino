#include <SoftwareSerial.h>

//#define BLUETOOTH_RX_PIN	(7)
//#define BLUETOOTH_TX_PIN	(12)
#define BLUETOOTH_RX_PIN	(12)
#define BLUETOOTH_TX_PIN	(11)
SoftwareSerial mySerial(BLUETOOTH_RX_PIN,BLUETOOTH_TX_PIN);

int count = 0;
char str[128];
unsigned long nextTime;
const unsigned long T = 1000;

void setup()
{

//  Serial.begin(9600);
  mySerial.begin(9600); //BlueTooth用のシリアルポート
  
  count = 0;
  nextTime = millis() + T;
}

boolean flg = false;
void loop()
{
  //BlueToothポートへの書き込み
//  sprintf(str, "XXX:%03d", count);

  unsigned long curTime = millis();
  if (curTime > nextTime){
    nextTime = curTime + T;
    
    sprintf(str, "ABCDEFGHIJKLMN");
    mySerial.println("ABCDEFGHIJKLMN");
    count++;
    
    ledLoop();
  }


  //標準シリアルポートにデータ書き込まれたら、それをBlueToothの方へ転送する。
//  if (Serial.available()){
//    mySerial.write(Serial.read());
//  }
  
  // BlueToothポートに書き込まれたら標準シリアルポートに転送。
  // 2012/09/04 現在では失敗。
//  if (mySerial.available()){
//    Serial.println("hogehoge");
//    mySerial.println("read:");
//    mySerial.println(mySerial.read()); //リードしないとバッファに溜まったままなので注意
//  }
//  Serial.println("xx");
  
}

void ledLoop(){
  digitalWrite(13,HIGH);
  delay(200);
  digitalWrite(13,LOW);
  delay(200);
}  
