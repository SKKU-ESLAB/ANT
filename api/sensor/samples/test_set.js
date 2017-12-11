console.log('Code start!');

var sensorManager = require("../build/Release/sensor-api");

console.log('value:' + sensorManager.Set("ACC", 3));
/*
setInterval(function(){
  console.log('the value is ' + sensorManager.Get("BUTTON").BUTTON);
},5000);
*/
