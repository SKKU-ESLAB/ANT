var addon = require('./build/Release/ANTFaceDetection');

var count = 50;


addon.faceDetection(count, function(err, numberOfFace){
		if(numberOfFace > 0){
			console.log('Face Detection !!! ');
			console.log('Number of Face : ' + numberOfFace +'\n');
			//이부분에 noti 관련 api로 noti 알려
		}	
		else
		{
			console.log('Not Detection !!! \n');
		}
	});

