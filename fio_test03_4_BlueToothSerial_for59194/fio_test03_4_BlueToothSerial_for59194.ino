#include <SoftwareSerial.h>

unsigned long t_next;
const unsigned long T = 100; //50ms
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


void loop()
{
  if (isInit){
    isInit = false;
    delay(5000);
  }
  
  unsigned long tc = millis();
  if (t_next < tc){
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
      //シリアル通信で送信
      memset(sendText, 0, sizeof(sendText));
      sprintf(sendText, "%ld,%d,%d,%d\n", tc, val[0], val[1], val[2]);
//      sprintf(sendText, "%ld\n", tc);
      mySerial.println(sendText);
  //    mySerial.write("\n");
      
    }

  
    //LED点灯
    digitalWrite(DPORT_LED,HIGH);
    delay(30);
    digitalWrite(DPORT_LED,LOW);

  }
}

