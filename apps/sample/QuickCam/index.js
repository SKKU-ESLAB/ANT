console.log("Application start");

var ant_api_dir = process.env.ANT_BIN_DIR + "/api/";
var api = require(ant_api_dir + "ant");
var appApi = api.app();

var cameraApi = require(ant_api_dir + "camera-api");
var sensorApi = require(ant_api_dir + "sensor-api");
var recordingObj = new cameraApi.ANTRecording();
var cameraNum = 0;

appApi.onLaunch(function() {
  var prev;
  var curr = 0;
  var cnt = 0;

  setInterval(function() {
    var touch = sensorApi.Get("BUTTON").BUTTON;
    curr = touch;
    console.log('TOUCH value: ' + touch);
    console.log('curr:'+curr+'/prev:'+prev+'/touch'+touch); if(prev != curr) {
      // Touch sensor event
      prev = curr;
      if(curr == 0) {
        console.log('Capture!');
        cnt++;
        var ant_data_dir = process.env.ANT_DATA_DIR;
        var fileName = ant_data_dir + '/quickCam' + cnt + '.jpeg';
        recordingObj.snapshot(cameraNum, fileName);
        var notiPage = appApi.makeEventPage("Snapshot Saved");
        notiPage = appApi.addEventText(notiPage, "Quick Cam Snapshot");
        notiPage = appApi.addEventImg(notiPage, fileName);

        // Polling the sensor Data (Crap or Touch Event)
        appApi.sendEventPageWithNoti(notiPage);
      }
      else
        console.log("Touch:"+curr);
    }

  },1000);
});

appApi.onTermination(function() {
});

appApi.appReady();
