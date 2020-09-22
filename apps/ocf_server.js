var ocf = require('ocf');

var oa = ocf.getAdapter();
oa.onPrepareServer(function() {
  console.log("on prepare server");
});

console.log("oa.start() start");
oa.start();
console.log("oa.start() end");
setTimeout(function() {
  console.log("100s elapsed");
}, 100000);