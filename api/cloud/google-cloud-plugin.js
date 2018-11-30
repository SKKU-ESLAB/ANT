var ant_api_dir = process.env.ANT_BIN_DIR + "/api/";
const cloudApi = require(ant_api_dir + "cloud-api");

const fs = require('fs');
const jwt = require('jsonwebtoken');
const assert = require('assert');

'use strict';

function GoogleCloudConnection() {
  this.isConnected = false;
};

function createJwt(projectId, privateKeyFile, algorithm) {
  // Create a JWT to authenticate this device. The device will be disconnected
  // after the token expires, and will have to reconnect with a new token. The
  // audience field should always be set to the GCP project id.
  const token = {
    iat: parseInt(Date.now() / 1000),
    exp: parseInt(Date.now() / 1000) + 20 * 60, // 20 minutes
    aud: projectId,
  };
  const privateKey = fs.readFileSync(privateKeyFile);
  return jwt.sign(token, privateKey, {algorithm: algorithm});
}

GoogleCloudConnection.prototype.connectSecure = function(callback) {
  var cloudConnection = this;
  this.iatTime = parseInt(Date.now() / 1000);
  this.mqttConnection = cloudApi.connectMQTT(
      this.hostname, this.port,
      this.clientId, this.username,
      createJwt(this.projectId, this.privateKeyFile, this.algorithm),
      this.protocol, this.secureProtocol, callback
      );

  // Schedule refreshing token
  this.setTimeout(function() {
    cloudConnection.iatTime = parseInt(Date.now() / 1000);
    console.log(`\tRefreshing token after ${cloudConnection.tokenExpMins} min.`);

    client.end();
    cloudConnection.mqttConnection = cloudApi.connectMQTT(
        this.hostname, this.port,
        this.clientId, this.username,
        createJwt(this.projectId, this.privateKeyFile, this.algorithm),
        this.protocol, this.secureProtocol, callback
        );
    // TODO: setting callbacks for MQTT events
  }, this.tokenExpMins * 60);
};

GoogleCloudConnection.prototype.setCloudInfo = function(cloudInfo) {
  this.hostname = (cloudInfo.hostname == null) ? "mqtt.googleapis.com": cloudInfo.hostname;
  this.port = (cloudInfo.port == null) ? 8883 : cloudInfo.port;
  this.cloudRegion = (cloudInfo.cloudRegion == null) ? "asia-east1": cloudInfo.cloudRegion;

  this.username = (cloudInfo.username == null) ? "unused" : cloudInfo.username;

  assert(cloudInfo.projectId != null);
  this.projectId = cloudInfo.projectId;

  assert(cloudInfo.privateKeyFile != null);
  this.privateKeyFile = cloudInfo.privateKeyFile;

  assert(cloudInfo.registryId != null);
  this.registryId = cloudInfo.registryId;

  assert(cloudInfo.deviceId != null);
  this.deviceId = cloudInfo.deviceId;

  this.tokenExpMins = (cloudInfo.tokenExpMins == null) ? 20 : cloudInfo.tokenExpMins;
  
  this.algorithm = (cloudInfo.algorithm == null) ? "RS256": cloudInfo.algorithm;

  this.protocol = (cloudInfo.protocol == null) ? "mqtts" : cloudInfo.protocol;
  this.secureProtocol = (cloudInfo.secureProtocol == null) ? "TLSv1_2_method" : cloudInfo.secureProtocol;

  this.clientId = `projects/${this.projectId}/locations/${
    this.cloudRegion
  }/registries/${this.registryId}/devices/${this.deviceId}`;

  this.mqttTopic = `/devices/${this.deviceId}/${this.messageType}`;
};

GoogleCloudConnection.prototype.close = function() {
  this.isConnected = false;
  this.mqttConnection.close();
};

GoogleCloudConnection.prototype.subscribe = function(messageType, callback) {
  const mqttTopic = `/devices/${argv.deviceId}/${messageType}`;
  this.mqttConnection.subscribe(mqttTopic, callback);
};

GoogleCloudConnection.prototype.subscribeEvents = function(callback) {
  this.subscribe("events", callback);
};

GoogleCloudConnection.prototype.subscribeState = function(callback) {
  this.subscribe("state", callback);
};

GoogleCloudConnection.prototype.subscribeConfig = function(callback) {
  this.subscribe("config", callback);
};

GoogleCloudConnection.prototype.publish = function(messagetype, message) {
  const mqttTopic = `/devices/${argv.deviceId}/${messageType}`;
  this.mqttConnection.publish(mqttTopic, message);
}

GoogleCloudConnection.prototype.publishEvents = function(message) {
  this.publish("events", message);
}

GoogleCloudConnection.prototype.publishState = function(message) {
  this.publish("state", message);
}

GoogleCloudConnection.prototype.publishConfig = function(message) {
  this.publish("config", message);
}

exports.connectSecure = function(cloudInfo) {
  var connection = new GoogleCloudConnection();
  connection.setCloudInfo(cloudInfo);
  connection.connectSecure(function() {
    connection.isConnected = true;
  });
  return connection;
};
