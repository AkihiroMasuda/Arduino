#include <SoftwareSerial.h>

#define BLUETOOTH_RX_PIN	(7)
#define BLUETOOTH_TX_PIN	(12)
SoftwareSerial mySerial(BLUETOOTH_RX_PIN,BLUETOOTH_TX_PIN);

int count = 0;
char str[128];

void setup()
{

  Serial.begin(9600);
  mySerial.begin(9600); //BlueTooth用のシリアルポート
  
  count = 0;
}

void loop()
{
  //BlueToothポートへの書き込み
  sprintf(str, "cnt:%03d", count);
  mySerial.println(str);
  count++;

  //標準シリアルポートにデータ書き込まれたら、それをBlueToothの方へ転送する。
  if (Serial.available()){
    mySerial.write(Serial.read());
  }
  
  // BlueToothポートに書き込まれたら標準シリアルポートに転送。
  // 2012/09/04 現在では失敗。
  if (mySerial.available()){
    Serial.println("hogehoge");
    mySerial.println("read:");
    char c = 0;
    while((c = mySerial.read())!=-1){
      mySerial.write(c); //リードしないとバッファに溜まったままなので注意
    }
    mySerial.write("\n");
  }
  Serial.println("xx");
  
  ledLoop();
}

void ledLoop(){
  digitalWrite(13,HIGH);
  delay(200);
  digitalWrite(13,LOW);
  delay(200);
}  
