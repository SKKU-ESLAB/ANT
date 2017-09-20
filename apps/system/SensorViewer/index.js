var ant_api_dir = process.env.ANT_BIN_DIR + "/api/";
var appApi = require(ant_api_dir + "app-api");
var sensorApi = require(ant_api_dir + "sensor-api");
var NIL_MSG_TO_SENSOR_VIEWER = "1102";

// Get sensor list and size
var sensorList = sensorApi.GetSensorlist();
var sensorNum = sensorList.sensorNum;
var sensorData = {
  "BUTTON": -1,
  "ACC": -1,
  "MOTION": -1,
  "SOUND": -1,
  "LIGHT": -1,
  "VIBRATION": -1,
  "TEMP": -1
};
var i = 0;
for(i=0; i<sensorNum; i++){
  var sensorIndex = 'sensor' + (i+1);
  sensorData[sensorList[sensorIndex]] = 0;
}

// Report sensor data periodically
var repeat = setInterval(function() {
  sensorData.BUTTON =
    (sensorData.BUTTON >= 0) ? sensorApi.Get("BUTTON").BUTTON : -1;
  sensorData.ACC =
    (sensorData.ACC >= 0) ? sensorApi.Get("ACC").Z : -1;
  sensorData.MOTION =
    (sensorData.MOTION >= 0) ? sensorApi.Get("MOTION").MOTION : -1;
  sensorData.SOUND =
    (sensorData.SOUND >= 0) ? sensorApi.Get("SOUND").SOUND : -1;
  sensorData.LIGHT =
    (sensorData.LIGHT >= 0) ? sensorApi.Get("LIGHT").LIGHT : -1;
  sensorData.VIBRATION =
    (sensorData.VIBRATION >= 0) ? sensorApi.Get("VIBRATION").VIBRATION : -1;
  sensorData.TEMP =
    (sensorData.TEMP >= 0) ? sensorApi.Get("TEMP").TEMP : -1;

  var str = "{\"type\":\"" + NIL_MSG_TO_SENSOR_VIEWER
    + "\",\"Touch\":\"" + sensorData.BUTTON
    + "\",\"Accelerometer\":\"" + sensorData.ACC
    + "\",\"Motion\":\"" + sensorData.MOTION
    + "\",\"Sound\":\"" + sensorData.SOUND
    + "\",\"Light\":\"" + sensorData.LIGHT
    + "\",\"Vibration\":\"" + sensorData.VIBRATION
    + "\",\"Temperature\":\"" + sensorData.TEMP
    + "\"}";
  appApi.sendMsgToSensorViewer(str);
  console.log("Sent\n");
}, 500);
