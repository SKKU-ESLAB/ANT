// Memory Benchmark

var ant = require('ant');
var console = require('console');

var onInitialize = function () {
  console.log('onInitialize');
};

var onStart = function () {
  console.log('onStart');
  var totalPss = 0.0;
  var sampleCount = 0;
  console.log('Start memory benchmarking...');
  if (ant.stream !== undefined) {
    ant.stream.initialize();
  }
  var getSample = function () {
    totalPss += ant.runtime.getPssInKB();
    sampleCount++;
    if (sampleCount == 10) {
      console.log((totalPss / sampleCount).toFixed(2) + ' KB');
    } else {
      setTimeout(getSample, 1000);
    }
  };
  setTimeout(getSample, 1000);
};

var onStop = function () {
  console.log('onStop');
};

ant.runtime.setCurrentApp(onInitialize, onStart, onStop);
