var ocf = require('ocf');
console.log('OCF server example app');

var oa = ocf.getAdapter();
oa.onPrepareEventLoop(function() {
  oa.setPlatform('ant');
  oa.addDevice('/oic/d', 'oic.d.light', 'Light', 'ocf.1.0.0', 'ocf.res.1.0.0');
});

var g_light_state = false;
oa.onPrepareServer(function() {
  console.log('onPrepareServer()');
  device = oa.getDevice(0);
  var lightRes = ocf.createResource(
      device, 'lightbulb', '/light/1', ['oic.r.light'], [ocf.OC_IF_RW]);
  lightRes.setDiscoverable(true);
  lightRes.setPeriodicObservable(1);
  lightRes.setHandler(ocf.OC_GET, getLightHandler);
  lightRes.setHandler(ocf.OC_POST, postLightHandler);
  oa.addResource(lightRes);
});

function getLightHandler(request) {
  oa.repStartRootObject();
  oa.repSet('state', g_light_state);
  oa.repEndRootObject();
  oa.sendResponse(request, ocf.OC_STATUS_OK);
}

var i = 0;
function postLightHandler(request) {
  var request_payload_string = request.request_payload_string;
  request_payload = JSON.parse(request_payload_string);
  console.log(
      '(' + i++ + ') POST Request: state=' + request_payload.state +
      ' (present:' + g_light_state + ')');

  g_light_state = request_payload.state;
  oa.sendResponse(request, ocf.OC_STATUS_OK);
}

oa.start();
setTimeout(function() {
  console.log('15s elapsed');
  oa.stop();
  oa.deinitialize();
}, 15000);