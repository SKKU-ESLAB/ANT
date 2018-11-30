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
