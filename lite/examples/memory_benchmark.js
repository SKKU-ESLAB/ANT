// Memory Benchmark

var ant = require('ant');
var console = require('console');

var on_initialize = function () {
  console.log('on_initialize');
};

var on_start = function () {
  console.log('on_start');
  var totalPss = 0.0;
  var sampleCount = 0;
  console.log('Start memory benchmarking...');
  if(ant.stream !== undefined) {
    ant.stream.initialize();
  }
  var getSample = function () {
    totalPss += ant.runtime.getPssInKB();
    sampleCount++;
    if (sampleCount == 10) {
      console.log((totalPss / sampleCount).toFixed(2) + " KB");
    } else {
      setTimeout(getSample, 1000);
    }
  };
  setTimeout(getSample, 1000);
};

var on_stop = function () {
  console.log('on_stop');
};

ant.runtime.setCurrentApp(on_initialize, on_start, on_stop);