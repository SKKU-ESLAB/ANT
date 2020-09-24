var ocf = require('ocf');

var oa = ocf.getAdapter();
oa.onInitialize(function () {
  oa.setPlatform("ant");
  oa.addDevice("/oic/d", "oic.d.light", "Light", "ocf.res.1.0.0", "");
  // this.setPlatform("ant");
  // this.addDevice("/oic/d", "ant.d.gateway", "Gateway", "ant.res.1.0.0", NULL, NULL);
});
oa.onPrepareServer(function () {
  console.log("on prepare server");
});

console.log("oa.start() start");
oa.start();
console.log("oa.start() end");
setTimeout(function () {
  console.log("10s elapsed");
  oa.stop();
}, 10000);