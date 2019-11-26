var ant = require('ant');
var console = require('console');

var on_initialize = function() {
  console.log('on_initialize');
};

var on_start = function() {
  console.log('on_start');
};

var on_stop = function() {
  console.log('on_stop');
};

ant.runtime.setCurrentApp(on_initialize, on_start, on_stop);