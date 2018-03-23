console.log('Code start!');

var sensorManager = require("../build/Release/sensor-api");
 var i,j;
i=0;

setInterval(function(){
  if(i==0){
    console.log('on:' + sensorManager.Set("LED", 1));
    i=1;
  } else {
    console.log('off: ' + sensorManager.Set("LED",0));
    i=0;
  }
}, 1000);
/*
setInterval(function(){
  console.log('the value is ' + sensorManager.Get("BUTTON").BUTTON);
},5000);
*/
