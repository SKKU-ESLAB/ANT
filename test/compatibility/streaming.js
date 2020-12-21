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

// ANT compatibility test 1: Streaming

var ant = require('ant');
var console = require('console');

var settings = {};

// "rpi", "nvidia", "test", or others("/dev/video0")
settings.sourceType = 'nvidia';

settings.isH264Enabled = false;
settings.isSourceFilterEnabled = false;
settings.isScaleEnabled = true;
settings.isConvertEnabled = true;
if (settings.sourceType == 'nvidia') {
  settings.isConvertEnabled = false;
}
settings.videoWidth = 224;
settings.videoHeight = 224;
settings.videoFormat = 'RGB';
settings.videoFramerate = '30/1';
settings.videoSinkSync = false;
settings.my_ip_address = ant.companion.getMyIPAddress('eth0');
settings.my_port = 5000;

var onInitialize = function () {
  console.log('onInitialize');
};

var onStart = function () {
  console.log('onStart');

  // Because ant.stream.initialize() is an async function without
  // finish callback, pipeline setting should be executed by setTimeout.
  ant.stream.initialize();
  console.log('Wait until stream thread is ready...');
  setTimeout(function () {
    var pipeline = ant.stream.createPipeline('test');
    var elements = [];

    // source
    var source;
    if (settings.sourceType == 'rpi') {
      source = ant.stream.createElement('rpicamsrc');
    } else if (settings.sourceType == 'test') {
      source = ant.stream.createElement('videotestsrc');
      source.setProperty('pattern', 1);
    } else if (settings.sourceType == 'nvidia') {
      source = ant.stream.createElement('nvcamerasrc');
      source.setProperty('fpsRange', '30.0 30.0');
    } else {
      source = ant.stream.createElement('v4l2src');
      source.setProperty('device', settings.sourceType);
    }
    elements.push(source);

    if (settings.sourceType == 'nvidia') {
      converter = ant.stream.createElement('nvvidconv');
      converter.setProperty('flip-method', 0);
      elements.push(converter);
    }

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
    elements.push(sourcefilter);

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
      elements.push(videoscale);
      elements.push(scalefilter);
    }

    // videoconvert and convertfilter
    var converter = ant.stream.createElement('videoconvert');
    elements.push(converter);
    if (settings.isConvertEnabled) {
      var convertfilter = ant.stream.createElement('capsfilter');
      convertfilter.setCapsProperty(
        'caps',
        'video/x-raw,format=' + settings.videoFormat
      );
      elements.push(convertfilter);
    }

    // h264 encoder
    if (settings.isH264Enabled) {
      var omxh264enc = ant.stream.createElement('omxh264enc');
      var rtph264pay = ant.stream.createElement('rtph264pay');
      rtph264pay.setProperty('pt', 06);
      rtph264pay.setProperty('config-interval', 1);
      elements.push(omxh264enc);
      elements.push(rtph264pay);
    }

    // gdppay
    var gdppay = ant.stream.createElement('gdppay');

    // tcpserversink
    var sink = ant.stream.createElement('tcpserversink');
    sink.setProperty('sync', settings.videoSinkSync);
    sink.setProperty('host', settings.my_ip_address);
    sink.setProperty('port', settings.my_port);
    elements.push(gdppay);
    elements.push(sink);

    pipeline.binAdd(elements);
    pipeline.linkMany(elements);
    pipeline.setState(pipeline.STATE_PLAYING);
    console.log(
      'Pipeline ready! (' +
        settings.my_ip_address +
        ':' +
        settings.my_port +
        ')'
    );

    // Remote pipeline
    var remotePipeline;
    if (settings.isH264Enabled) {
      remotePipeline =
        'tcpclientsrc host=' +
        settings.my_ip_address +
        ' port=' +
        settings.my_port +
        ' ! gdpdepay ! rtph264depay ! h264parse ! avdec_h264' +
        ' ! videoconvert ! autovideosink sync=false';
    } else {
      remotePipeline =
        'tcpclientsrc host=' +
        settings.my_ip_address +
        ' port=' +
        settings.my_port +
        ' ! gdpdepay ! videoconvert ! autovideosink sync=false';
    }
    ant.remoteui.setStreamingViewPipeline(remotePipeline);
    ant.remoteui.setStreamingViewLabelText('ON');
  }, 5000);

  setTimeout(function () {
    var totalPss = 0.0;
    var sampleCount = 0;
    console.log('Start memory benchmarking...');
    if (ant.stream !== undefined) {
      ant.stream.initialize();
    }
    var getSample = function () {
      totalPss += ant.runtime.getPssInKB();
      sampleCount++;
      if (sampleCount == 10) {
        console.log((totalPss / sampleCount).toFixed(2) + ' KB');
      } else {
        setTimeout(getSample, 1000);
      }
    };
    setTimeout(getSample, 1000);
  }, 15000);
};

var onStop = function () {
  console.log('onStop');
  ant.stream.finalize();
  ant.remoteui.setStreamingViewLabelText('-');
};

ant.runtime.setCurrentApp(onInitialize, onStart, onStop);
