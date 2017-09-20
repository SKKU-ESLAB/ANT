var addon = require('./build/Release/ANTRecording');
var cameraAPI = new addon.ANTRecording();
var ant_data_dir = process.env.ANT_DATA_DIR;
require('date-utils');
var count=0;
var dt;
var d;

dt = new Date();
d = dt.toFormat('YYYY-MM-DD HH24:MI:SS');


cameraAPI.RecordingStart(ant_data_dir + "/some.mp4", 10, function(err){
				console.log('Success');
		});


var repeat = setInterval(function(){
		count++;
//			recObj.start(ant_data_dir+'['+d+']['+count+'].jpeg', 1, function(err, data){
//			console.log('count : '+ count);
	//	}); 
},	5000);
