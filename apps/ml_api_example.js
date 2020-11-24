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

var ant = require('ant');
var console = require('console');

var settings = {};
settings.ml = {};
settings.ml.modelName = 'sample-models/rpi3_mobilenet_full';
settings.ml.inputShape = [3, 224, 224, 1];
settings.ml.inputType = 'uint8';
settings.ml.outputShape = [1000, 1, 1, 1];
settings.ml.outputType = 'float32';
settings.ml.labelFileName = 'imagenet-simple-labels.json';

settings.sourceType = '/dev/video0'; // "rpi", "test", or others(v4l2src)
settings.isH264Enabled = false;
settings.isSourceFilterEnabled = false;
settings.isScaleEnabled = true;
settings.isConvertEnabled = true;
settings.videoWidth = 224;
settings.videoHeight = 224;
settings.videoFormat = 'BGR';
settings.videoFramerate = '30/1';
settings.videoSinkSync = false;
settings.myIpAddress = ant.companion.getMyIPAddress('eth0');
settings.myPort = 5000;

var onInitialize = function () {
  console.log('onInitialize');
};

var prepareLabel = function (labelFilepath) {
  var fs = require('fs');
  var fileContents = fs.readFileSync(labelFilepath).toString();
  var labels = JSON.parse(fileContents);
  return labels;
};

var onStart = function () {
  console.log('onStart');

  var labelFilepath = settings.ml.modelName + '/' + settings.ml.labelFileName;
  var labels = prepareLabel(labelFilepath);

  // Because ant.stream.initialize() is an async function without finish
  // callback, pipeline setting should be executed by setTimeout.
  ant.stream.initialize();
  console.log('Wait until stream thread is ready...');
  var baselinePssInKB = ant.runtime.getPSSInKB();
  console.log('Baseline Memory: ' + baselinePssInKB + ' KB');
  setTimeout(function () {
    var pipeline = ant.stream.createPipeline('test');
    var mainpipeElements = [];
    var subpipe1Elements = [];
    var subpipe2Elements = [];

    // source
    var source;
    if (settings.sourceType == 'rpi') {
      source = ant.stream.createElement('rpicamsrc');
    } else if (settings.sourceType == 'test') {
      source = ant.stream.createElement('videotestsrc');
      source.setProperty('pattern', 1);
    } else {
      source = ant.stream.createElement('v4l2src');
      source.setProperty('device', settings.sourceType);
    }
    mainpipeElements.push(source);

    // source filter
    var sourcefilter = ant.stream.createElement('capsfilter');
    if (settings.isSourceFilterEnabled) {
      sourcefilter.setCapsProperty(
        'caps',
        'video/x-raw,width=' +
          settings.videoWidth +
          ',height=' +
          settings.videoHeight +
          ',framerate=' +
          settings.videoFramerate +
          ',format=' +
          settings.videoFormat
      );
    } else {
      sourcefilter.setCapsProperty('caps', 'video/x-raw');
    }
    mainpipeElements.push(sourcefilter);

    // videoscale and scalefilter
    if (settings.isScaleEnabled) {
      var videoscale = ant.stream.createElement('videoscale');
      videoscale.setProperty('method', 0);
      videoscale.setProperty('add-borders', false);
      var scalefilter = ant.stream.createElement('capsfilter');
      scalefilter.setCapsProperty(
        'caps',
        'video/x-raw,width=' +
          settings.videoWidth +
          ',height=' +
          settings.videoHeight
      );
      mainpipeElements.push(videoscale);
      mainpipeElements.push(scalefilter);
    }

    // videoconvert and convertfilter
    var converter = ant.stream.createElement('videoconvert');
    mainpipeElements.push(converter);
    if (settings.isConvertEnabled) {
      var convertfilter = ant.stream.createElement('capsfilter');
      convertfilter.setCapsProperty(
        'caps',
        'video/x-raw,format=' + settings.videoFormat
      );
      mainpipeElements.push(convertfilter);
    }

    // tee
    var tee = ant.stream.createElement('tee');
    tee.setProperty('name', 't');
    mainpipeElements.push(tee);

    // sub-pipeline 1
    // queue
    var queue1 = ant.stream.createElement('queue');
    subpipe1Elements.push(queue1);

    // tensorConverter
    var tensorConverter = ant.stream.createElement('tensor_converter');
    subpipe1Elements.push(tensorConverter);

    // tensor_filter (ml element)
    var mlElement = ant.ml.createMLElement(
      settings.ml.modelName,
      settings.ml.inputShape,
      settings.ml.inputType,
      settings.ml.outputShape,
      settings.ml.outputType
    );
    subpipe1Elements.push(mlElement);

    var sink = ant.stream.createElement('appsink');
    sink.setProperty('emit-signals', true);
    var prevTimestamp = 0;
    var totalPssInKB = 0;
    var totalFrameLatency = 0.0;
    var sampleCount = 0;
    sink.connectSignal('new-sample', function (name, data) {
      var result = ant.ml.getMaxOfBuffer(data, settings.ml.outputType);
      var labelMessage = '';

      var pssInKB = ant.runtime.getPSSInKB() - baselinePssInKB;
      var frameLatency = -1;
      if (prevTimestamp != 0) {
        var nowTimestamp = new Date().valueOf();
        frameLatency = nowTimestamp - prevTimestamp;
        prevTimestamp = nowTimestamp;
      } else {
        prevTimestamp = new Date().valueOf();
      }

      if (result === undefined) {
        labelMessage = 'Label error';
      } else {
        sampleCount++;
        totalPssInKB += pssInKB;
        var averagePssInKB = totalPssInKB / sampleCount;
        labelMessage =
          '' +
          labels[result.max_index] +
          ' (' +
          Math.round(result.max_value * 10000) / 100 +
          '%)\n' +
          (averagePssInKB / 1024.0).toFixed(1) +
          ' MB';
        if (frameLatency > 0) {
          totalFrameLatency += frameLatency;
          var averageFrameLatency = totalFrameLatency / sampleCount;
          var averageFPS = 1000.0 / averageFrameLatency;
          labelMessage +=
            '\n' +
            averageFrameLatency.toFixed(2) +
            ' ms (' +
            averageFPS.toFixed(2) +
            ' FPS)';
        }
      }
      ant.remoteui.setStreamingViewLabelText(labelMessage);
    });
    subpipe1Elements.push(sink);

    // sub-pipeline 2
    // h264 encoder
    var queue2 = ant.stream.createElement('queue');
    subpipe2Elements.push(queue2);

    if (settings.isH264Enabled) {
      var omxh264enc = ant.stream.createElement('omxh264enc');
      var rtph264pay = ant.stream.createElement('rtph264pay');
      rtph264pay.setProperty('pt', 06);
      rtph264pay.setProperty('config-interval', 1);
      subpipe2Elements.push(omxh264enc);
      subpipe2Elements.push(rtph264pay);
    }

    // gdppay
    var gdppay = ant.stream.createElement('gdppay');

    // tcpserversink
    var sink = ant.stream.createElement('tcpserversink');
    sink.setProperty('sync', settings.videoSinkSync);
    sink.setProperty('host', settings.myIpAddress);
    sink.setProperty('port', settings.myPort);
    subpipe2Elements.push(gdppay);
    subpipe2Elements.push(sink);

    pipeline.binAdd(mainpipeElements);
    pipeline.binAdd(subpipe1Elements);
    pipeline.binAdd(subpipe2Elements);
    pipeline.linkMany(mainpipeElements);
    pipeline.linkMany([tee].concat(subpipe1Elements));
    pipeline.linkMany([tee].concat(subpipe2Elements));
    pipeline.setState(pipeline.STATE_PLAYING);

    console.log(
      'Pipeline ready! (' + settings.myIpAddress + ':' + settings.myPort + ')'
    );

    // Remote pipeline
    var remotePipeline;
    if (settings.isH264Enabled) {
      remotePipeline =
        'tcpclientsrc host=' +
        settings.myIpAddress +
        ' port=' +
        settings.myPort +
        ' ! gdpdepay ! rtph264depay ! h264parse ! avdec_h264 !' +
        ' videoconvert ! autovideosink sync=false';
    } else {
      remotePipeline =
        'tcpclientsrc host=' +
        settings.myIpAddress +
        ' port=' +
        settings.myPort +
        ' ! gdpdepay ! videoconvert ! autovideosink sync=false';
    }
    ant.remoteui.setStreamingViewPipeline(remotePipeline);
    ant.remoteui.setStreamingViewLabelText('Waiting for Inference...');
  }, 5000);
};

var onStop = function () {
  console.log('onStop');
  ant.stream.finalize();
  ant.remoteui.setStreamingViewLabelText('-');
};

ant.runtime.setCurrentApp(onInitialize, onStart, onStop);
