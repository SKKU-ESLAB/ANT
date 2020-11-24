var ocf = require('ocf');
console.log('OCF server example app');

var oa = ocf.getAdapter();
oa.onPrepareEventLoop(function () {
  oa.setPlatform('ant');
  oa.addDevice('/oic/d', 'oic.d.light', 'Light', 'ocf.1.0.0', 'ocf.res.1.0.0');
});

oa.onPrepareServer(function () {
  console.log('onPrepareServer()');
  device = oa.getDevice(0);
  var lightRes = ocf.createResource(
    device,
    'lightbulb',
    '/light/1',
    ['oic.r.light'],
    [ocf.OC_IF_RW]
  );
  lightRes.setDiscoverable(true);
  lightRes.setPeriodicObservable(1);
  lightRes.setHandler(ocf.OC_GET, getLightHandler);
  lightRes.setHandler(ocf.OC_POST, postLightHandler);
  oa.addResource(lightRes);

  var thRes = ocf.createResource(
    device,
    'thermostat',
    '/temperature/1',
    ['oic.r.temperature'],
    [ocf.OC_IF_RW]
  );
  thRes.setDiscoverable(true);
  thRes.setPeriodicObservable(1);
  thRes.setHandler(ocf.OC_GET, getTempHandler);
  thRes.setHandler(ocf.OC_POST, postTempHandler);
  oa.addResource(thRes);
});

var gLightState = false;
function getLightHandler(request) {
  oa.repStartRootObject();
  oa.repSet('state', gLightState);
  oa.repEndRootObject();
  oa.sendResponse(request, ocf.OC_STATUS_OK);
}
var i = 0;
function postLightHandler(request) {
  var requestPayloadString = request.requestPayloadString;
  var requestPayload = JSON.parse(requestPayloadString);
  console.log(
    '(' +
      i++ +
      ') POST Light Request: state=' +
      requestPayload.state +
      ' (present:' +
      gLightState +
      ')'
  );

  gLightState = requestPayload.state;
  oa.sendResponse(request, ocf.OC_STATUS_OK);
}

var gTempState = 15;
function getTempHandler(request) {
  oa.repStartRootObject();
  oa.repSet('state', gTempState);
  oa.repEndRootObject();
  oa.sendResponse(request, ocf.OC_STATUS_OK);
}
var i = 0;
function postTempHandler(request) {
  var requestPayloadString = request.requestPayloadString;
  var requestPayload = JSON.parse(requestPayloadString);
  console.log(
    '(' +
      i++ +
      ') POST Temp Request: state=' +
      requestPayload.state +
      ' (present:' +
      gTempState +
      ')'
  );

  gTempState = requestPayload.state;
  oa.sendResponse(request, ocf.OC_STATUS_OK);
}

oa.start();
setTimeout(function () {
  console.log('150s elapsed');
  oa.stop();
  oa.deinitialize();
}, 150000);
