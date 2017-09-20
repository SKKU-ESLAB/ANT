console.log('Code start!');

var sensorManager = require("../build/Release/sensor-api");

sensorManager.On("BUTTON", "PERIODIC", 1000, function(button){
  console.log("callback came");

  console.log("data: " + button.BUTTON);
})


setInterval(function(){},1000);
