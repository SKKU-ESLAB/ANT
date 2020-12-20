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

/**
 * ANT CoAP API
 */
function CoAP() {}

CoAP.prototype.createServer = function () {
  return new CoAPServer();
};

/**
 * CoAP Server API
 */
function CoAPServer() {}

CoAPServer.prototype.onEvent = function (eventType, handler) {
  native.coap_server_onEvent(eventType, handler);
};

CoAPServer.prototype.listen = function (callback) {
  native.coap_server_listen(callback);
};

/**
 * CoAP Response
 */
function CoAPResponse() {}

CoAPResponse.prototype.end = function (message) {
  native.coap_response_end(message);
};

CoAPResponse.prototype.get = function () {
  native.coap_response_get();
};

// CoAP Client API
CoAP.prototype.request = function (url, method) {
  return new CoAPRequest();
};

/**
 * CoAP Request
 */
function CoAPRequest() {}

CoAPRequest.prototype.onEvent = function (eventType, handler) {
  native.coap_request_onEvent(eventType, handler);
};

CoAPRequest.prototype.end = function () {
  native.coap_request_end();
};

module.exports = new LWM2M();
module.exports.LWM2M = LWM2M;
