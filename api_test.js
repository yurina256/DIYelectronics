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
const body = JSON.stringify({
  "command": "turnOn",
  "parameter": "default",
  "commandType": "command"
});
const deviceId = "DC:DA:0C:AB:5D:4C";
const options = {
  hostname: 'api.switch-bot.com',
  port: 443,
  path: `/v1.1/devices/${deviceId}/commands`,
  method: 'POST',
  headers: {
    "Authorization": token,
    "sign": sign,
    "nonce": nonce,
    "t": t,
    'Content-Type': 'application/json',
    'Content-Length': body.length,
  },
};

const req = https.request(options, res => {
  console.log(`statusCode: ${res.statusCode}`);
  res.on('data', d => {
    process.stdout.write(d);
  });
});

req.on('error', error => {
  console.error(error);
});

req.write(body);
req.end();