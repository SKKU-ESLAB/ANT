const ps = require('ps-node');
// basicServer.js
const express = require('express');
var app = express();

app.get('/', function(req, res){
	ps.lookup({pargs: '-lx'}, function(err, resultList) {
		if (err) {
			throw new Error( err);
		}
		res.send(JSON.stringify(resultList));
	});
});
var server = app.listen(1337, function () {
	console.log("hello");
});

