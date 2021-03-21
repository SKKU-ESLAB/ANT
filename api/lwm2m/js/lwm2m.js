/* @licence
/* Copyright (c) 2017-2020 SKKU ESLAB, and contributors. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// var console = require('console');
// var assert = require('assert');

// var CoAP = require('coap');

/**
 * ANT LWM2M API
 */
function LWM2M() {}

LWM2M.prototype.createClient = function () {
  return new LWL2MClient();
};

LWM2M.prototype.createBootstrapServer = function () {
  return new LWM2MBootstrapServer();
};

LWM2M.prototype.server = LWM2M.prototype.createBootStrapServer();

// LWM2M client API
LWM2M.prototype.createSmartObject = function () {
  return new LWM2MObject();
};

/**
 * LWM2M Client API
 */
function LWM2MClient() {}

LWM2MClient.prototype.register = function (ip, port, opts, callback) {
  var ssid = this.configure(ip, port, opts);
  // this._register(ip, port, ssid, callback);
};

LWM2MClient.prototype.on = function () {};

// LWM2MClient.prototype._register = function (ip, port, ssid, callback) {
//   var self = this,
//     reqObj = {
//       hostname: ip,
//       port: port,
//       pathname: '/rd',
//       payload: helper.checkAndBuildObjList(this, false, {
//         ct: 'application/json',
//         hb: true
//       }),
//       method: 'POST',
//       options: {'Content-Format': 'application/link-format'}
//     },
//     agent = this._createAgent(),
//     resetCount = 0,
//     msg;

//   function setListenerStart(port, msg) {
//     if (!agent._sock) {
//       startListener(self, port, function (err) {
//         if (err) {
//           invokeCbNextTick(err, null, callback);
//         } else {
//           self._sleep = false;
//           invokeCbNextTick(null, msg, callback);
//           self.emit('registered');
//         }
//       });
//     } else {
//       if (resetCount < 10) {
//         resetCount += 1;
//         setTimeout(function () {
//           return setListenerStart(port, msg);
//         }, 10);
//       } else {
//         invokeCbNextTick(
//           new Error('Socket can not be create.'),
//           null,
//           callback
//         );
//       }
//     }
//   }

//   // [TODO] server is exist
//   this._updateNetInfo(function () {
//     reqObj.query =
//       'ep=' +
//       self.clientName +
//       '&lt=' +
//       self.lifetime +
//       '&lwm2m=' +
//       self.version +
//       '&mac=' +
//       self.mac;
//     self.request(reqObj, agent, function (err, rsp) {
//       if (err) {
//         invokeCbNextTick(err, null, callback);
//       } else {
//         msg = {status: rsp.code};

//         if (rsp.code === RSP.created) {
//           self.serversInfo[ssid] = {
//             shortServerId: ssid,
//             ip: rsp.rsinfo.address,
//             port: rsp.rsinfo.port,
//             locationPath: '/rd/' + rsp.headers['Location-Path'],
//             registered: true,
//             lfsecs: 0,
//             repAttrs: {},
//             reporters: {},
//             hbPacemaker: null,
//             hbStream: {stream: null, port: null, finishCb: null}
//           };

//           self.ip = rsp.outSocket.ip;
//           self.port = rsp.outSocket.port;
//           setListenerStart(rsp.outSocket.port, msg);
//         } else {
//           invokeCbNextTick(null, msg, callback);
//         }
//       }
//     });
//   });
// };

/**
 * Smart Object: IPSO smart objects that are provided by LWM2M client
 */
function SmartObject() {}

SmartObject.prototype.init = function (oid, iid, resrcs, setup) {
  native.lwm2m_smartobject_init(oid, iid, resrcs, setup);
};

/**
 * LWM2M Bootstrap Server API
 */
function LWM2MBootstrapServer() {}

LWM2MBootstrapServer.prototype.on = function (eventType, handler) {
  native.lwm2m_boostrapserver_on(eventType, handler);
};

LWM2MBootstrapServer.prototype.configure = function (serverConfig) {
  native.lwm2m_boostrapserver_configure(serverConfig);
};

LWM2MBootstrapServer.prototype.start = function () {
  native.lwm2m_boostrapserver_start();
};

module.exports = new LWM2M();
module.exports.LWM2M = LWM2M;
