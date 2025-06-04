const { SerialPort, ReadlineParser } = require('serialport');
const mod = require("./mod");
// Arduinoが接続されているシリアルポートのパス
// Windows: 'COMx' (例: 'COM3')
// macOS/Linux: '/dev/tty.usbmodemXXXX' または '/dev/ttyUSBx', '/dev/ttyACM0' など
// 適切なポートパスに置き換えてください。
const portPath = 'COM22'; // ここをあなたのArduinoのポートに合わせる
const baudRate = 9600; // Arduinoのスケッチと合わせる

const port = new SerialPort({
  path: portPath,
  baudRate: baudRate,
  autoOpen: false // 自動でポートを開かないようにする
});

port.open((err) => {
  if (err) {
    return console.error('シリアルポートを開けませんでした:', err.message);
  }
  console.log('シリアルポートが開かれました');

  let toggle = 0;
  setInterval(() => {
    mod.get_weight().then(weight => {
      console.log(weight);
      /* arduino側で補完するには結局向こうで変換する
      var tmp = mod.getRgbFromValue(Math.min(100,weight/4));
      const R = tmp[0];
      const G = tmp[1];
      const B = tmp[2];
      console.log(tmp);
      dataToSend = R*65536 + G*256 + B;
      //console.log(dataToSend);
      */
      dataToSend = Math.floor(Math.max(0,Math.min(100,weight/4)));
      port.write(dataToSend + '\n', (err) => { // 改行を追加してArduinoで読み取りやすくする
        if (err) {
          return console.error('データの書き込みエラー:', err.message);
        }
        console.log('送信データ:', dataToSend);
      });
      toggle = 1 - toggle; // トグル
    })
  }, 10000); // 2秒ごとに送信

});

port.on('error', (err) => {
  console.error('シリアルポートエラー:', err.message);
});

port.on('data', (data) => {
  console.log('Arduinoからの応答:', data.toString().trim());
});

port.on('close', () => {
  console.log('シリアルポートが閉じられました');
});

// エラーハンドリング
process.on('SIGINT', () => {
  console.log('アプリケーションを終了します');
  port.close(() => {
    process.exit();
  });
});