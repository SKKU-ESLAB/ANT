var comm = require('./build/Release/NIL_Comm');

function mainCb(str, stat) {
	console.log("Incoming Message:"+str)
	var strArray=str.split(':');
	if(strArray[0] == "Get")
	{
		if(strArray[1] == "Temperature")
			comm.SendMsg("Temp:");
		else if(strArray[1] == "Sound")
			comm.SendMsg("Sound:");
		else
			comm.SendMsg("No sensor exists:"+strArray[1]);

		//Get Sensor Data as a string Value
	}
	else
		comm.SendMsg("Correct Format = GET:[Sensor_Name]")

}

function myLoop (i){
	setTimeout(function(){
			if(--i) myLoop(i);
			}, 3000)
}


comm.OpenChannel('Test Interface', mainCb)

/* 아래 코드가 Touch 왔을 때 불리면 됩니다. */
//comm.SendMsg("Touch!")
