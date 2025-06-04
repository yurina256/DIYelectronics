#include <Adafruit_NeoPixel.h>  //ライブラリAdafruit NeoPixelをインクルード
#define LED_PIN 6
#define LED_COUNT 4

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int receivedInt = 0; 
int wheelpos = 0;
int wheelpos_next = 0;

void setup() {
  Serial.begin(9600); // シリアル通信を開始 (ボーレートはNode.js側と合わせる)
  strip.begin();  
  strip.show();
  strip.setBrightness(50);
}

void loop() {
  if (Serial.available() > 0) { // シリアルデータが利用可能かチェック
    receivedInt = Serial.parseInt();//0-100
    while (Serial.available()) {
      Serial.read();
    }
    wheelpos_next = 5*receivedInt;
    Serial.print("wheelpos_next: ");
    Serial.println(wheelpos_next);
  }
  if(wheelpos < wheelpos_next) wheelpos += 1;
  if(wheelpos > wheelpos_next) wheelpos -= 1;
  for (int i=0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i,getHtmlColorFromValue(wheelpos));
    strip.show();
  }
  Serial.println(wheelpos);
  delay(100);    
}
uint32_t Wheel(uint16_t WheelPos) //0-512
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
int hsvToHtmlColorInternal(double h, double s, double v) {
    s /= 100.0; // 0-100% を 0-1 に正規化
    v /= 100.0; // 0-100% を 0-1 に正規化

    double r_double, g_double, b_double;

    int i = static_cast<int>(std::floor(h / 60.0));
    double f = h / 60.0 - i;
    double p = v * (1.0 - s);
    double q = v * (1.0 - f * s);
    double t = v * (1.0 - (1.0 - f) * s);

    switch (i % 6) {
        case 0: r_double = v; g_double = t; b_double = p; break;
        case 1: r_double = q; g_double = v; b_double = p; break;
        case 2: r_double = p; g_double = v; b_double = t; break;
        case 3: r_double = p; g_double = q; b_double = v; break;
        case 4: r_double = t; g_double = p; b_double = v; break;
        case 5: r_double = v; g_double = p; b_double = q; break;
        default: r_double = 0; g_double = 0; b_double = 0; // エラー時のデフォルト値
    }

    // RGB値を0-255の範囲に変換し、丸める
    int R = static_cast<int>(std::round(r_double * 255.0));
    int G = static_cast<int>(std::round(g_double * 255.0));
    int B = static_cast<int>(std::round(b_double * 255.0));

    // 念のため0-255の範囲にクランプ
    // C++17以降なら std::clamp を使うのがベスト: R = std::clamp(R, 0, 255);
    R = std::max(0, std::min(255, R));
    G = std::max(0, std::min(255, G));
    B = std::max(0, std::min(255, B));

    // R, G, B成分をHTMLカラーコード形式の整数に結合 (0xRRGGBB)
    return (R << 16) | (G << 8) | B;
}

/**
 * @brief 0-512の入力値からHTMLカラーコード形式の整数値RGBを計算する関数
 *
 * 入力値0: (240,100,100) 青
 * 入力値256: (120,100,100) 緑
 * 入力値512: (0,100,100) 赤
 *
 * @param value 0から512までの整数値
 * @return HTMLカラーコード形式の10進数整数値 (0xRRGGBB)。エラー時は-1を返す。
 */
int getHtmlColorFromValue(int value) {
    // 入力値のバリデーション
    if (value < 0 || value > 512) {
        return -1; // 不正な値を示す
    }

    double h; // Hue (色相)
    const double s = 100.0; // Saturation (彩度) - 常に100%
    const double v = 100.0; // Value (明度) - 常に100%

    if (value <= 256) {
        // 0-256の範囲: Hueが240から120へ線形補間
        // 変化量: 240 - 120 = 120
        // 1単位あたりの変化: 120 / 256
        h = 240.0 - (static_cast<double>(value) * (120.0 / 256.0));
    } else {
        // 257-512の範囲: Hueが120から0へ線形補間
        // 変化量: 120 - 0 = 120
        // 1単位あたりの変化: 120 / 256
        h = 120.0 - ((static_cast<double>(value) - 256.0) * (120.0 / 256.0));
    }

    // HSV値を直接 hsvToHtmlColorInternal 関数に渡す
    return hsvToHtmlColorInternal(h, s, v);
}
