const int dig1 = 13;
const int dig2 = 1;
const int dig3 = 2;
const int dig4 = 4;

const int a = 12;
const int b = 10;
const int c =  8;
const int d =  6;
const int e =  5;
const int f = 11;
const int g =  9;
const int dp = 7;

const int PWM = 3;
 
// 全てのDIG(桁)をクリア
void clearDigit(){
    digitalWrite(dig1,LOW);
    digitalWrite(dig2,LOW);
    digitalWrite(dig3,LOW);
    digitalWrite(dig4,LOW);
}
// 全てのセグ(A-G,DP)をクリア
void clearSegment(){
    digitalWrite(a,HIGH);
    digitalWrite(b,HIGH);
    digitalWrite(c,HIGH);
    digitalWrite(d,HIGH);
    digitalWrite(e,HIGH);
    digitalWrite(f,HIGH);
    digitalWrite(g,HIGH);
    digitalWrite(dp,HIGH);
}
 
void setup() {
  // 全てのピンを出力にする
  for(int i =0; i <= 13; i++){
    if(i+1!=3) pinMode((i+1),OUTPUT);
  }
  
  // 全てのDIG(桁)をクリア
  clearDigit();
  
  // 全てのセグ(A-G,DP)をクリア
  clearSegment();
}
void disp(int dig,int num){
  clearSegment(); clearDigit(); 
  digitalWrite(dig,HIGH);
    switch (num) {
    case 0: // 0を表示
      digitalWrite(a, LOW);
      digitalWrite(b, LOW);
      digitalWrite(c, LOW);
      digitalWrite(d, LOW);
      digitalWrite(e, LOW);
      digitalWrite(f, LOW);
      digitalWrite(g, HIGH); // Gはオフ
      break;
    case 1: // 1を表示
      digitalWrite(a, HIGH);
      digitalWrite(b, LOW);
      digitalWrite(c, LOW);
      digitalWrite(d, HIGH);
      digitalWrite(e, HIGH);
      digitalWrite(f, HIGH);
      digitalWrite(g, HIGH);
      break;
    case 2: // 2を表示
      digitalWrite(a, LOW);
      digitalWrite(b, LOW);
      digitalWrite(c, HIGH);
      digitalWrite(d, LOW);
      digitalWrite(e, LOW);
      digitalWrite(f, HIGH);
      digitalWrite(g, LOW);
      break;
    case 3: // 3を表示
      digitalWrite(a, LOW);
      digitalWrite(b, LOW);
      digitalWrite(c, LOW);
      digitalWrite(d, LOW);
      digitalWrite(e, HIGH);
      digitalWrite(f, HIGH);
      digitalWrite(g, LOW);
      break;
    case 4: // 4を表示
      digitalWrite(a, HIGH);
      digitalWrite(b, LOW);
      digitalWrite(c, LOW);
      digitalWrite(d, HIGH);
      digitalWrite(e, HIGH);
      digitalWrite(f, LOW);
      digitalWrite(g, LOW);
      break;
    case 5: // 5を表示
      digitalWrite(a, LOW);
      digitalWrite(b, HIGH);
      digitalWrite(c, LOW);
      digitalWrite(d, LOW);
      digitalWrite(e, HIGH);
      digitalWrite(f, LOW);
      digitalWrite(g, LOW);
      break;
    case 6: // 6を表示
      digitalWrite(a, LOW);
      digitalWrite(b, HIGH);
      digitalWrite(c, LOW);
      digitalWrite(d, LOW);
      digitalWrite(e, LOW);
      digitalWrite(f, LOW);
      digitalWrite(g, LOW);
      break;
    case 7: // 7を表示
      digitalWrite(a, LOW);
      digitalWrite(b, LOW);
      digitalWrite(c, LOW);
      digitalWrite(d, HIGH);
      digitalWrite(e, HIGH);
      digitalWrite(f, HIGH);
      digitalWrite(g, HIGH);
      break;
    case 8: // 8を表示
      digitalWrite(a, LOW);
      digitalWrite(b, LOW);
      digitalWrite(c, LOW);
      digitalWrite(d, LOW);
      digitalWrite(e, LOW);
      digitalWrite(f, LOW);
      digitalWrite(g, LOW);
      break;
    case 9: // 9を表示
      digitalWrite(a, LOW);
      digitalWrite(b, LOW);
      digitalWrite(c, LOW);
      digitalWrite(d, LOW);
      digitalWrite(e, HIGH);
      digitalWrite(f, LOW);
      digitalWrite(g, LOW);
      break;
    case -1://dot
      digitalWrite(dp,LOW);
      break;
    default: // 0-9以外の数字が来た場合、全てオフにするか、エラー表示など
      break;
  }
  delayMicroseconds(50);    
}
void meter(int val){
  analogWrite(PWM,val);  
}
void loop() {
  disp(dig3,9);
  disp(dig3,-1);
  meter(255);
}