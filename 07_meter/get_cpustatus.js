const http = require('http');
const { resolve } = require('path');


exports.get_stats = function get_stats() {
  const options = {
    hostname: 'localhost',
    port: 8085,
    path: `/data.json`,
    method: 'GET',
    headers: {
      'Content-Type': 'application/json',
    },
  };

  const req = http.request(options, res => {
    let data = '';

    // データがチャンクで受信されるたびに実行
    res.on('data', (chunk) => {
      data += chunk;
    });

    // 全てのデータが受信された後に実行
    res.on('end', () => {
      try {
        const jsonData = JSON.parse(data);
        console.log('受信したJSONデータ:', jsonData);
        console.log(`cputemp: ${jsonData.Children[0].Children[1].Children[1].Children[0].Value}`);
        console.log(`cpuusage: ${jsonData.Children[0].Children[1].Children[2].Children[0].Value}`);
        console.log(`memoryusage: ${jsonData.Children[0].Children[2].Children[0].Children[0].Value}`);
        const cpuusage = jsonData.Children[0].Children[1].Children[2].Children[0].Value;
        const memoryusage = jsonData.Children[0].Children[2].Children[0].Children[0].Value;
        resolve(cpuusage * 1000 + memoryusage);
      } catch (error) {
        console.error('JSONのパースに失敗しました:', error);
        console.error('受信した生データ:', data);
      }
    });
  });

  req.on('error', error => {
    console.error(error);
  });

  req.end();
}