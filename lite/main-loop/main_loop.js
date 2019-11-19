var console = require('console');
// var http = require('http');

var ant = require('ant');

function main_loop() {
  console.log('main loop start');

  console.log(ant.runtime.setCurrentApp());

  var server = http.createServer();
  server.on('request', function (request, response) {
    console.log('Request for path: ' + request.url);

    var message = 'Okay!';

    response.setHeader('Content-Type', 'text/html');
    response.setHeader('Content-Length', message.length);
    response.writeHead(200);
    response.write(message);
    response.end();
  });

  var port = 8001;
  server.listen(port, function() {
    console.log("ANT core listening on port: " + port);
  });
}

main_loop();