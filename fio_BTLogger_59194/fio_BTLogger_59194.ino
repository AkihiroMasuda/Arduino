// 自作基板59194向け
// アナログデータBlueTooth転送
#include <SoftwareSerial.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>

unsigned long t_next;
const unsigned long T = 10; //50ms
const int DPORT_RX = 4; //シリアル通信用 ポート
const int DPORT_TX = 3; //シリアル通信用 ポート
const int APORT_X = 0;
const int APORT_Y = 1;
const int APORT_Z = 2;
const int DPORT_LED = 8;
char sendText[256]; //シリアル通信で送る文字

SoftwareSerial mySerial(DPORT_RX, DPORT_TX);

boolean isInit = true;

void setup()
{
  pinMode(DPORT_LED, OUTPUT);
  t_next = millis();
  mySerial.begin(9600);
//  mySerial.begin(19200);
}

byte gSendDat[sizeof(unsigned long)+1+3];

void loop()
{
  if (isInit){
    isInit = false;
    delay(5000);
  }
  
  unsigned long tc = millis();
  if (t_next <= tc){
    //T秒経過した
    t_next = tc + T;
   
    //アナログポート読み込み
    int val[] = {analogRead(APORT_X), analogRead(APORT_Y), analogRead(APORT_Z)};    // アナログピンを読み取る
   
    if (mySerial.available()){
      val[0] = mySerial.read();
      memset(sendText, 0, sizeof(sendText));
      sprintf(sendText, "%ld,%d,%d,%d\n", tc, val[0], val[1], val[2]);
      mySerial.println(sendText);
    }else{
      byte *p = gSendDat;
      *p = 33; ++p;
//      memcpy(p, &tc, sizeof(tc));

//      *p = sizeof(tc); ++p;
//      *p = 0x44; ++p;
//      *p = 0x55; ++p;
//      *p = 0x66; ++p;
///      tc = 0x12345678;
      //エンディアンで頭悩ませたくないので、とりあえず強制的にBigEndian化
      *p = (tc&0xFF000000)>>24; ++p;
      *p = (tc&0x00FF0000)>>16; ++p;
      *p = (tc&0x0000FF00)>>8; ++p;
      *p = (tc&0x000000FF); ++p;
      *p = 0x11; ++p; //ダミー アナログデータのつもり。速度測定用に仮データをセット
      *p = 0x22; ++p;
      *p = 0x33; ++p;
      

      //シリアル通信で送信
//      memset(sendText, 0, sizeof(sendText));
//      sprintf(sendText, "%ld,%d,%d,%d\n", tc, val[0], val[1], val[2]);
//      sprintf(sendText, "%ld\n", tc);
//      mySerial.println(sendText);
  //    mySerial.write("\n");
      mySerial.write(gSendDat, sizeof(gSendDat));
    }

    //LED点灯
    digitalWrite(DPORT_LED,HIGH);
    delay(1);
    digitalWrite(DPORT_LED,LOW);

  }
}

