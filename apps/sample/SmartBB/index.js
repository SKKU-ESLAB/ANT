var ant_api_dir = process.env.ANT_BIN_DIR + "/api/";
var cameraApi = require(ant_api_dir + "camera-api");
var faceDetectionApi = require(ant_api_dir + "face-detection-api");
var appApi = require(ant_api_dir + "app-api");

var recObj = new cameraApi.ANTRecording();
var count = 10000;

var ant_data_dir = process.env.ANT_DATA_DIR;

faceDetectionApi.faceDetection(count, function(err, numberOfFace) {
  if(numberOfFace > 0) {
    // Face Detection Event Handling
    var filePath = ant_data_dir + "/BB[12345].mp4";
    var playSeconds = 10;
    var handler = function(err) {
      console.log("Face Detection Event: " + filePath);

      var notiPage = appApi.makeEventPage("Blackbox video was recorded");
      notiPage = appApi.addEventText(notiPage, 
          numberOfFace + " faces are detected.");
      notiPage = appApi.addEventText(notiPage, 
          "File path: " + filePath);
      appApi.sendEventPageWithNoti(notiPage);
    }

    console.log("Start recording...");
    recObj.recordingStart(0, filePath, playSeconds, handler);
  }
});
