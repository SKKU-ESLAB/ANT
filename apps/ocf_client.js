var ocf = require('ocf');
console.log('OCF client example app');

var oa = ocf.getAdapter();
oa.onInitialize(function() {
  console.log('onInitialize()');
  oa.setPlatform('ant');
  oa.addDevice('/oic/d', 'oic.wk.d', 'Client', 'ocf.1.0.0', 'ocf.res.1.0.0');
});

var g_light_state = false;
oa.onPrepareClient(function() {
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
  }
}

function onObserveLight(response) {
  var payload = response.payload;
  var endpoint = response.endpoint;
  var uri = foundLightUri;

  console.log('From ' + uri + ': ' + payload);

  oa.initPost(endpoint, uri, '', ocf.OC_HIGH_QOS, onPost);
  oa.repStartRootObject();
  oa.repSet('light', g_light_state);
  g_light_state = !g_light_state;
  oa.repEndRootObject();
  oa.post();
}

function onPost(response) {
  console.log('Sent post request!');
}

oa.start();
setTimeout(function() {
  console.log('10s elapsed');
  oa.stop();
}, 10000);