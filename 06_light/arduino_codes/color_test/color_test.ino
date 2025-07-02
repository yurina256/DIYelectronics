#include <Adafruit_NeoPixel.h>  //ライブラリAdafruit NeoPixelをインクルード
#define LED_PIN 6
#define LED_COUNT 4

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int receivedInt = 0; 
int wheelpos = 512;
int wheelpos_next = 512;

void setup() {
  Serial.begin(9600); // シリアル通信を開始 (ボーレートはNode.js側と合わせる)
  strip.begin();  
  strip.show();
  strip.setBrightness(50);
}

void loop() {
    for(int j=0;j<=512;j++){
        for (int i=0; i < strip.numPixels(); i++) {
            strip.setPixelColor(i,Wheel(j));
            strip.show();
        }
        Serial.println(wheelpos);
        delay(100);    
    }
}
uint32_t Wheel(uint16_t WheelPos)
{
  byte r, g, b;
  switch(WheelPos / 256)
  {
    case 0:
      r = 127 - WheelPos % 256;   //Red 下がる
      g = WheelPos % 256;      // Green 上がる
      b = 0;                  //blue オフ
      break; 
    case 1:
      g = 127 - WheelPos % 256;  //green 下がる
      b = WheelPos % 256;      //blue 上がる
      r = 0;                  //red オフ
      break; 
    case 2:
      b = 127 - WheelPos % 256;  //blue 下がる 
      r = WheelPos % 256;      //red 上がる
      g = 0;                  //green オフ
      break; 
  }
  //RGBの色（のデータ型　の変数）を返す
  return(strip.Color(r,g,b));
}
