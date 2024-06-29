const http = require('http');
const fs = require('fs');
const path = require('path');

const port = 8080;

const server = http.createServer((req, res) => {
  let filePath = './website' + req.url;
  if (filePath === './website/') {
    filePath = './website/index.html'; // 默认为首页
  }

  const extname = path.extname(filePath);
  let contentType = 'text/html';

  // 根据文件扩展名设置 Content-Type
  switch (extname) {
    case '.js':
      contentType = 'text/javascript';
      break;
    case '.css':
      contentType = 'text/css';
      break;
    case '.json':
      contentType = 'application/json';
      break;
    case '.png':
      contentType = 'image/png';
      break;
    case '.jpg':
    case '.jpeg':
      contentType = 'image/jpeg';
      break;
    case '.gif':
      contentType = 'image/gif';
      break;
    default:
      contentType = 'text/html';
  }

  fs.readFile(filePath, (err, content) => {
    if (err) {
      if (err.code == 'ENOENT') {
        // 文件不存在，返回404
        res.writeHead(404);
        res.end('404 Not Found');
      } else {
        // 其他错误，返回500
        res.writeHead(500);
        res.end('500 Internal Server Error: ' + err.code);
      }
    } else {
      // 成功读取文件，返回文件内容
      res.writeHead(200, { 'Content-Type': contentType });
      res.end(content, 'utf-8');
    }
  });
});

server.listen(port, () => {
  console.log(`Server is running on http://8.137.120.53:${port}`);
});
