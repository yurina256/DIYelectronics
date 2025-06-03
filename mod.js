const crypto = require('crypto');
const https = require('https');
const token = "537578c57217bd42328779b205066b11e416fcef63cf41ba6ed45734e56dfd8a1fbd04ba535355bb5164a338d4f941ed";
const secret = "2bbb70363d32482a18ed88e03024d622";
const deviceId = "DCDA0CAB5D4E";
exports.get_weight = function get_weight() {
  return new Promise((resolve, reject) => {
    const t = Date.now();
    const nonce = crypto.randomUUID();
    const data = token + t + nonce;

    const sign = crypto
      .createHmac('sha256', secret)
      .update(data)
      .digest('base64');
    console.log(sign);
    /*
    const body = JSON.stringify({
      "command": "turnOn",
      "parameter": "default",
      "commandType": "command"
    });
    */
    const options = {
      hostname: 'api.switch-bot.com',
      port: 443,
      path: `/v1.1/devices/${deviceId}/status`,
      method: 'GET',
      headers: {
        "Authorization": token,
        "sign": sign,
        "nonce": nonce,
        "t": t,
        'Content-Type': 'application/json'
      },
    };

    const req = https.request(options, res => {
      console.log(`statusCode: ${res.statusCode}`);
      let rawData = ''; // レスポンスボディの生データを蓄積する変数

      res.on('data', chunk => {
        rawData += chunk; // データチャンクを文字列として蓄積
      });

      res.on('end', () => {
        try {
          // レスポンスボディをJSONとしてパースする
          const parsedData = JSON.parse(rawData);
          weight_val = parsedData.body.weight;
          resolve(weight_val);
        } catch (e) {
          console.error('Error parsing JSON:', e.message); // JSONパースエラーのハンドリング
        }
      });
    });

    req.on('error', error => {
      console.error(error);
    });

    req.end();
  })
}

function hsvToRgb(hsv) {
    let h = hsv[0];
    let s = hsv[1] / 100; // 0-100% を 0-1 に正規化
    let v = hsv[2] / 100; // 0-100% を 0-1 に正規化

    let r, g, b;

    let i = Math.floor(h / 60);
    let f = h / 60 - i;
    let p = v * (1 - s);
    let q = v * (1 - f * s);
    let t = v * (1 - (1 - f) * s);

    switch (i % 6) {
        case 0: r = v, g = t, b = p; break;
        case 1: r = q, g = v, b = p; break;
        case 2: r = p, g = v, b = t; break;
        case 3: r = p, g = q, b = v; break;
        case 4: r = t, g = p, b = v; break;
        case 5: r = v, g = p, b = q; break;
    }

    return [Math.round(r * 255), Math.round(g * 255), Math.round(b * 255)];
}

exports.getRgbFromValue = function getRgbFromValue(value){
  if (value < 0 || value > 100) {
    throw new Error("val: out of range");
  }

  let h; // Hue (色相)
  const s = 100; // Saturation (彩度) - 常に100%
  const v = 100; // Value (明度) - 常に100%

  if (value <= 50) {
    // 0-50の範囲: Hueが240から120へ線形補間
    // 0 -> 240
    // 50 -> 120
    // 1単位あたり (240 - 120) / 50 = 120 / 50 = 2.4 減少
    h = 240 - (value * 2.4);
  } else {
    // 51-100の範囲: Hueが120から0へ線形補間
    // 50 -> 120
    // 100 -> 0
    // 1単位あたり (120 - 0) / 50 = 120 / 50 = 2.4 減少
    // valueから50を引いて、0-50の範囲に正規化
    h = 120 - ((value - 50) * 2.4);
  }

  // color-convertライブラリはHSVを [H, S, V] の配列で受け取る
  // H: 0-360, S: 0-100, V: 0-100
  const rgb = hsvToRgb([h, s, v]);

  return rgb;
}

/*
get_weight().then(weight => {
  console.log("取得した電力:", weight);
}).catch(error => {
  console.error("取得中にエラーが発生しました:", error);
});
*/