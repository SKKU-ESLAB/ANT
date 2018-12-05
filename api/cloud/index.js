/* Copyright (c) 2017-2018 SKKU ESLAB, and contributors. All rights reserved.
 *
 * Contributor: Gyeonghwan Hong<redcarrottt@gmail.com>
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

'use strict';

const mqtt = require('mqtt');

function MQTTConnection() {
  // Callbacks
  this.subscribeCallbacks = {};
};

MQTTConnection.prototype.connect = function(connectionArgs, callback) {
  this.connectionArgs = connectionArgs;
  var connectionCallback = callback;
  this.client = mqtt.connect(mqttConnectionArgs);

  this.client.on('connect', success => {
    console.log('MQTT Connected!');
    if (!success) {
      console.log('Client not connected...');
    }

    if(connectionCallback != null) {
      connectionCallback(success);
    }
  });

  this.client.on('close', () => {
    console.log('MQTT Closed!');
    shouldBackoff = true;
  });

  this.client.on('error', err => {
    console.log('MQTT Error!', err);
  });

  // Subscribe
  this.client.on('message', (topic, message) => {
    var payloadString = Buffer.from(message, 'base64').toString('utf8');
    console.log('MQTT Subscribe: ' + payloadString);
    var subscribeCallback = subscribeCallbacks[topic];
    if(subscribeCallback != null) {
      subscribeCallback(topic, payloadString);
    }
  });
};

// Subscribe
MQTTConnection.prototype.subscribe = function(mqttTopic, callback, qos = 1) {
  this.subscribeCallbacks[mqttTopic] = callback;
  this.client.subscribe(mqttTopic, {qos: qos});
};

MQTTConnection.prototype.unsubscribe = function(mqttTopic) {
  this.subscribeCallbacks[mqttTopic] = null;
  this.client.unsubscribe(mqttTopic);
};

// Publish
MQTTConnection.prototype.publish = function(mqttTopic, message, qos = 1) {
  this.client.publish(mqttTopic, message, {qos: qos});
};

// Close
MQTTConnection.prototype.close = function() {
  this.client.end();
};

exports.connectMQTT = function(bridgeHostname, bridgePort,
    clientId, username, password, protocol, secureProtocol, callback = null) {
  var mqttConnectionArgs = {
    host: bridgeHostname,
    port: bridgePort,
    clientId: clientId,
    username: username,
    password: password,
    secureProtocol: secureProtocol,
    protocolId: 'MQTT',
    protocolVersion: 4,
    clean: true
  };
  var mqttConnection = new MQTTConnection();
  mqttConnection.connect(mqttConnectionArgs, callback);
  return mqttConnection;
};
