console.log("Start Code");

var addon = require('./build/Release/nil');


//addon.on("MOTION", function(value){
//	console.log("APP2 Work..." + value);
//});

//console.log("Return value : " + addon.Get("ACC"));
addon.On("test", function(value ){
	console.log("Event driven value :" + value);
} );

var repeat = setInterval(function(){
	console.log("current working interval func...");
//	console.log("Return value : " + addon.Get("ACC"));
	}, 5000);





