// App Benchmark
// It requires no other APIs. (option0)

var ant = require('ant');
var console = require('console');

var onInitialize = function () {
  console.log('onInitialize');
};

var onStart = function () {
  console.log('onStart');
};

var onStop = function () {
  console.log('onStop');
};

ant.runtime.setCurrentApp(onInitialize, onStart, onStop);
