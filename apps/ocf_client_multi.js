var ocf = require('ocf');
console.log('OCF client example app');

var oa = ocf.getAdapter();
oa.onPrepareEventLoop(function () {
  oa.setPlatform('ant');
  oa.addDevice('/oic/d', 'oic.wk.d', 'Client', 'ocf.1.0.0', 'ocf.res.1.0.0');
});

var g_light_state = false;
var g_temp_state = 15;
oa.onPrepareClient(function () {
  oa.discoveryAll(onDiscovery);
});

var foundLightUri = undefined;
var foundTempUri = undefined;

function onDiscovery(endpoint, uri, types, interface_mask) {
  for (var i in types) {
    console.log('Discovered: ' + types[i]);
    if (types[i] == 'oic.r.light') {
      foundLightUri = uri;
      oa.observe(endpoint, uri, onObserveLight);
      interval = setInterval(function () {
        var res = oa.initPost(endpoint, uri, onPost, '', ocf.OC_LOW_QOS);
        if (res) {
          oa.repStartRootObject();
          oa.repSet('state', g_light_state);
          g_light_state = !g_light_state;
          oa.repEndRootObject();
          oa.post();
        }
      }, 1000);
    } else if (types[i] == 'oic.r.temperature') {
      foundTempUri = uri;
      oa.observe(endpoint, uri, onObserveTemp);
      interval2 = setInterval(function () {
        var res = oa.initPost(endpoint, uri, onPost, '', ocf.OC_LOW_QOS);
        if (res) {
          oa.repStartRootObject();
          oa.repSet('state', g_temp_state);
          g_temp_state = (g_temp_state + 1) % 30;
          oa.repEndRootObject();
          oa.post();
        }
      }, 2000);
    }
  }
  console.log(' ');
}
var interval;
var interval2;

function onObserveLight(response) {
  var payload = response.payload;
  var endpoint = response.endpoint;
  var uri = foundLightUri;

  console.log('GET from ' + uri);
}

function onObserveTemp(response) {
  var payload = response.payload;
  var endpoint = response.endpoint;
  var uri = foundTempUri;

  console.log('GET from ' + uri);
}

var i = 0;
function onPost(response) {
  console.log('(' + i++ + ') POST request sent!');
}

oa.start();
setTimeout(function () {
  console.log('150s elapsed');
  oa.stop();
  oa.deinitialize();
  clearInterval(interval);
  clearInterval(interval2);
}, 150000);
