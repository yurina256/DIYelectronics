const { SerialPort, ReadlineParser } = require('serialport');
const getcpu = require("./get_cpustatus");
// Arduinoが接続されているシリアルポートのパス
// Windows: 'COMx' (例: 'COM3')
// macOS/Linux: '/dev/tty.usbmodemXXXX' または '/dev/ttyUSBx', '/dev/ttyACM0' など
// 適切なポートパスに置き換えてください。
const portPath = 'COM3'; // ここをあなたのArduinoのポートに合わせる
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
    getcpu.get_cpustatus().then(val => {
      console.log(val);
      /* こっちも結局向こうで変換する
      */
      port.write(val + '\n', (err) => { // 改行を追加してArduinoで読み取りやすくする
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