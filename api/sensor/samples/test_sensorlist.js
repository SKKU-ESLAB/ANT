console.log('Code start!');

var sensorManager = require("./build/Release/sensor-api");
var sensorList = sensorManager.GetSensorlist();

// Get sensor num 
console.log('sensor num : ' + sensorList.sensorNum);

// print sensor list
var sensorNum = sensorList.sensorNum;
var i = 0;
for(i=0; i<sensorNum; i++){
  var sensorIndex = 'sensor' + (i+1);
  console.log('sensor' + (i+1) + ': ' +  sensorList[sensorIndex]);

}

// You can check the sensor names of the sensor list
