var ocf = require('ocf');
console.log('OCF client example app');

var oa = ocf.getAdapter();
oa.onPrepareEventLoop(function () {
  oa.setPlatform('ant');
  oa.addDevice('/oic/d', 'oic.wk.d', 'Client', 'ocf.1.0.0', 'ocf.res.1.0.0');
});

var g_light_state = false;
oa.onPrepareClient(function () {
  oa.discovery('oic.r.light', onDiscovery);
});

var foundLightUri = undefined;

function onDiscovery(endpoint, uri, types, interface_mask) {
  var isFound = false;
  for (var i in types) {
    if (types[i] == 'oic.r.light') {
      isFound = true;
    }
  }
  if (isFound) {
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
  }
}
var interval;

function onObserveLight(response) {
  var payload = response.payload;
  var endpoint = response.endpoint;
  var uri = foundLightUri;

  console.log('GET from ' + uri + ': ' + payload);
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
}, 150000);
