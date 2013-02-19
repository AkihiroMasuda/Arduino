#include <SoftwareSerial.h>


// メディアンフィルタ
BYTE medianFilter(BYTE* dat, BYTE size)
{
	//ソートする
//	bubbleSort(dat,size);

//	RB2 = 1;
	{
		BYTE i=0;
		BYTE j=0;
		BYTE t=0;
		BYTE k=0;
		for (i=0; i<size-1; i++){
			for(j=size-1; j>i; j--){
				if(dat[j]<dat[j-1]){
					t = dat[j];
					k = dat[j-1];
	//				dat[j] = dat[j-1];
					dat[j] = k;
					dat[j-1] = t;
				}
			}
		}	
	}
//	RB2 = 0;

	if (size%2 == 0){
		//データ数が偶数の時
		return (dat[size/2-1] + dat[size/2])>>1;
	}else{
		//データ数が奇数の時
		return dat[(size-1)/2];
	}
}

BYTE getAnalogData()
{
	int ADCValue;

	AD1CON1bits.SAMP = 0;  //AD変換開始
	while(!AD1CON1bits.DONE){};
	ADCValue = ADC1BUF0;
	AD1CON1bits.SAMP = 1;  //変換が終わったのでフラグ立てる

	return ADCValue>=0xFF ? 0xFF : ADCValue&0x00FF;
}

// 閾値以下、または以上になるまで監視する。
// 引数：flg   閾値以下を見つける場合は０、閾値以上の場合は１
// 戻り値： 正常：１
//       タイムアウト：０
int detectPitch(BYTE flg, BYTE *buf, BYTE curInd)
{
	BYTE tmp[5];
//	const BYTE shikii = 0x20;
//	const BYTE shikii = 146;
	const BYTE shikii = 253;

	buf[0] = getAnalogData();
	buf[1] = getAnalogData();
	buf[2] = getAnalogData();
	buf[3] = getAnalogData();
	buf[4] = getAnalogData();

	while(1){
		if (flgLimit){
			//タイムアウトエラー
			return 0;
		}

		int newdat = getAnalogData();
//		buf[curInd%5] = (BYTE)((newdat>>2)&0x00FF);
		buf[curInd%5] = (BYTE)(newdat);
		++curInd;
		if (curInd == 5){ curInd = 0;}
		my_memcpy(tmp,buf,5);
		BYTE median = medianFilter(tmp, 5);
		if (flg==0){
			if (median <= shikii){
				break;
			}
		}else{
			if (median > shikii){
				break;
			}
//			rs_outbyte(0x99);
		}
		
	}

	return 1;
}

// タイヤ一回転の時間を測定する。
// 戻り値：時間 [μｓ]
unsigned long calRotUS()
//float calRotUS()
{
	BYTE buf[5];
	BYTE curInd = 0;

/*
	//まずはバッファにデータを貯める
	buf[0] = getAnalogData();
	buf[1] = getAnalogData();
	buf[2] = getAnalogData();
	buf[3] = getAnalogData();
	buf[4] = getAnalogData();
*/
	/*デバッグ用*/
/*
	while(1){
		//最初に印が無いところを探す。
		detectPitch(0, buf, curInd);
	}
*/

	//タイムリミット用のタイマーをセット
	initTimerForTimeLimit();

	//最初に印が無いところを探す。
	if(!detectPitch(0, buf, curInd)){
		closeTimerForTimeLimit();
		return timeLimitCnt*16;
	}
	
	//印が通過するのを待つ。
	if(!detectPitch(1, buf, curInd)){
		closeTimerForTimeLimit();
		return timeLimitCnt*16;
	}
	
	//タイマーセット
	initTimerForVelCnt();
	
	//ちょっと待って見る。チャタリング対策。
	delay_ms(1);
	
	
	//次に印が無いところを通過するのを探す。
	if(!detectPitch(0, buf, curInd)){
		closeTimerForVelCnt();
		closeTimerForTimeLimit();
		return timeLimitCnt*16;
	}
	
	//ちょっと待って見る。チャタリング対策。
	delay_ms(1);
	
	//再び印が通過するのを待つ。
	if(!detectPitch(1, buf, curInd)){
		closeTimerForVelCnt();
		closeTimerForTimeLimit();
		return timeLimitCnt*16;
	}
	
	//タイマー読み込み
	unsigned long tmr = TMR4;
//	int cnt = tmrCnt;

	//タイマー停止
	closeTimerForVelCnt();
	closeTimerForTimeLimit();

/*
	while(1){
		DB_PORTB = 1;
	}
*/
//	float fs = 16; //16MIPS
//	return tmr*64/fs;  //カウント数＊分周比/fs [μｓ]
//	return tmr*4; //16MIPS, 分周比64なら、１カウント4μｓ
	return tmr*16; //16MIPS, 分周比256なら、１カウント16μｓ
}




unsigned long t_next;
const unsigned long T = 1; //50ms
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


int cnt = 0;
unsigned long tc_old =  0;
unsigned long dts[5];

void loop()
{
  if (isInit){
    isInit = false;
    delay(5000);
  }

//  unsigned long tc = millis();
  unsigned long tc = micros();
  unsigned long dt = tc - tc_old;
  tc_old = tc;
  if (cnt < 5){
    dts[cnt] = dt;  
    ++cnt;
//    delay(0);
  }else{
    cnt = 0;
    memset(sendText, 0, sizeof(sendText));
    sprintf(sendText, "dts : %ld,%ld,%ld,%ld,%ld\n", dts[0], dts[1], dts[2], dts[3], dts[4]);
    mySerial.println(sendText);
  }
  //LED点灯
//    digitalWrite(DPORT_LED,HIGH);
//    delay(130);
//    digitalWrite(DPORT_LED,LOW);
}

