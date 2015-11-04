/*
 * IRRemote 紅外線遙控教學
 *   範例 1.2: 顯示紅外線協定種類，如 NEC, Sony SIRC, Philips RC5, Philips RC6 等協定
 */
#include <AlternativeIRremote.h>                    // 引用 IRRemote 函式庫

//on Protocol: Unknown encoding, irCode: B54BAA43,  bits: 32
//off Protocol: Unknown encoding, irCode: 84EDCFE5,  bits: 32
//(fan speed Low)Protocol: Unknown encoding, irCode: 9871F9A3,  bits: 32
//(fan speed mid)Protocol: Unknown encoding, irCode: C12A0E1D,  bits: 32
//(fan speed high)Protocol: Unknown encoding, irCode: C04FB96,  bits: 32
//(fan speed Auro)Protocol: Unknown encoding, irCode: B54BAA43,  bits: 32
//(冷氣)Protocol: Unknown encoding, irCode: B54BAA43,  bits: 32
//(除濕)Protocol: Unknown encoding, irCode: 6CBF64B5,  bits: 32
//(送風)Protocol: Unknown encoding, irCode: 9A41479D,  bits: 32
//(擺葉Protocol: Unknown encoding, irCode: 43AE08E1,  bits: 32
//(不擺)Protocol: Unknown encoding, irCode: B54BAA43,  bits: 32
//速冷Protocol: Unknown encoding, irCode: FEB92D27,  bits: 32
//Normal Protocol: Unknown encoding, irCode: B54BAA43,  bits: 32

//32 Protocol: Unknown encoding, irCode: A93C2655,  bits: 32
//31 Protocol: Unknown encoding, irCode: 6AFF2D0F,  bits: 32
//30 Protocol: Unknown encoding, irCode: A06558F,  bits: 32
//29 Protocol: Unknown encoding, irCode: 4320165F,  bits: 32
//28 Protocol: Unknown encoding, irCode: 7777D81F,  bits: 32
//27 Protocol: Unknown encoding, irCode: A7107137,  bits: 32
//26 Protocol: Unknown encoding, irCode: A238937,  bits: 32
//25 Protocol: Unknown encoding, irCode: B54BAA43,  bits: 32
//24 Protocol: Unknown encoding, irCode: DC66058B,  bits: 32
//23 Protocol: Unknown encoding, irCode: EDEB1E11,  bits: 32
//22 Protocol: Unknown encoding, irCode: 50FE3611,  bits: 32
//21 Protocol: Unknown encoding, irCode: 474FD617,  bits: 32
//20 Protocol: Unknown encoding, irCode: CF2E6D8F,  bits: 32
//19 Protocol: Unknown encoding, irCode: 24251CCB,  bits: 32
//18 Protocol: Unknown encoding, irCode: 9C5AF903,  bits: 32
//17 Protocol: Unknown encoding, irCode: 6C12F5D9,  bits: 32
//16 Protocol: Unknown encoding, irCode: CF260DD9,  bits: 32
//15 Protocol: Unknown encoding, irCode: 9176BFED,  bits: 32
//14 Protocol: Unknown encoding, irCode: A1688A2D,  bits: 32








const int irReceiverPin = 2;             // 紅外線接收器 OUTPUT 訊號接在 pin 2
static const uint8_t TKD2 = 2;
AlternativeIRrecv irrecv(irReceiverPin);            // 定義 IRrecv 物件來接收紅外線訊號
decode_results results;                  // 解碼結果將放在 decode_results 結構的 result 變數裏

void setup()
{
  Serial.begin(9600);                     // 開啟 Serial port, 通訊速率為 9600 bps
  irrecv.enableIRIn();                   // 啟動紅外線解碼

}

 

// 顯示紅外線協定種類

void showIRProtocol(decode_results *results)
{
  unsigned int i;
  Serial.print("Protocol: ");
  // 判斷紅外線協定種類
  switch(results->decode_type) {
   case NEC:
     Serial.print("NEC");
     break;
   case SONY:
     Serial.print("SONY");
     break;
   case RC5:
     Serial.print("RC5");
     break;
   case RC6:
     Serial.print("RC6");
     break;
   case DISH:
     Serial.print("DISH");
     break;
   case SHARP:
     Serial.print("Sharp");
     break;
   default:
     Serial.print("Unknown encoding"); 
  } 
 
  // 把紅外線編碼印到 Serial port
  Serial.print(", irCode: ");           
  Serial.print(results->value, HEX);    // 紅外線編碼
  Serial.print(",  bits: ");          
  Serial.println(results->bits);        // 紅外線編碼位元數
  Serial.print("RAW: rawlen:");
  Serial.print(results->rawlen);
  Serial.print("raw:");
  for(i=0; i<results->rawlen; i++)
  {
    Serial.print(results->rawbuf[i]);
    Serial.print(",");
  }
  Serial.print("\n");
}
 
void loop()
{
  if (irrecv.decode(&results)) {         // 解碼成功，收到一組紅外線訊號
    showIRProtocol(&results);            // 顯示紅外線協定種類
    irrecv.resume();                     // 繼續收下一組紅外線訊號       
  } 
}
