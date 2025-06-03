const SerialPort = require('serialport');
const port = new SerialPort('/dev/cu.usbmodem14141', {
  parser: SerialPort.parsers.readline('\n'),
  baudrate: 9600
});

port.on('open', function () {
  console.log('Serial open.');
  signeture();
  setInterval(write, 1000, 'OK\n');
});

port.on('data', function (data) {
  console.log('Data: ' + data);
});

function write(data) {
    console.log('Write: ' + data);
    port.write(new Buffer(data), function(err, results) {
      if(err) {
        console.log('Err: ' + err);
        console.log('Results: ' + results);
      }
  });
}

signeture();
function signeture(){
  const crypto = require('crypto');
  const https = require('https');

  const token = "537578c57217bd42328779b205066b11e416fcef63cf41ba6ed45734e56dfd8a1fbd04ba535355bb5164a338d4f941ed";
  const secret = "2bbb70363d32482a18ed88e03024d622";
  const t = Date.now();
  const nonce = crypto.randomUUID();
  const data = token + t + nonce;
  const sign = crypto
    .createHmac('sha256', secret)
    .update(data)
    .digest('base64');
  console.log(sign);
}