#include <SoftwareSerial.h>

unsigned long t_next;
const unsigned long T = 500; //50ms
const int DPORT_RX = 11; //シリアル通信用 ポート
const int DPORT_TX = 12; //シリアル通信用 ポート
const int APORT_X = 4;
const int APORT_Y = 5;
const int APORT_Z = 7;
char sendText[256]; //シリアル通信で送る文字

SoftwareSerial mySerial(DPORT_RX, DPORT_TX);

void setup()
{
  t_next = millis();
  mySerial.begin(9600);
}


void loop()
{
  unsigned long tc = millis();
  if (t_next < tc){
    //T秒経過した
    t_next = tc + T;
   
    //アナログポート読み込み
    int val[] = {analogRead(APORT_X), analogRead(APORT_Y), analogRead(APORT_Z)};    // アナログピンを読み取る
   
    //シリアル通信で送信
    memset(sendText, 0, sizeof(sendText));
    sprintf(sendText, "%ld,%d,%d,%d\n", tc, val[0], val[1], val[2]);
    mySerial.println(sendText);
//    mySerial.write("\n");
  }
}

