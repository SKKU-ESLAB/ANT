console.log('Code start!');

var sensorManager = require("../build/Release/sensor-api");

sensorManager.On("BUTTON", "NOTIFY", 1000, function(button){
  console.log("Notify callback came");
  
  console.log("Event occurred!");

})


setInterval(function(){},1000);
