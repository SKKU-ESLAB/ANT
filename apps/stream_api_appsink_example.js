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

// Streaming with AppSink Example
// It requires Companion, Resource, Remote UI, Stream API. (option2)

var ant = require('ant');
var console = require('console');

var settings = {};
settings.video_width = 224;
settings.video_height = 224;
settings.video_framerate = '30/1';
settings.video_format = 'BGR';
settings.video_sink_sync = false;
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
  setTimeout(function () {
    var pipeline = ant.stream.createPipeline('test');

    var source = ant.stream.createElement('videotestsrc');
    source.setProperty('pattern', 1);
    var filter = ant.stream.createElement('capsfilter');
    filter.setCapsProperty(
      'caps',
      'video/x-raw,width=' +
        settings.video_width +
        ',height=' +
        settings.video_height +
        ',framerate=' +
        settings.video_framerate +
        ',format=' +
        settings.video_format
    );
    var converter = ant.stream.createElement('videoconvert');
    var omxh264enc = ant.stream.createElement('omxh264enc');
    var rtph264pay = ant.stream.createElement('rtph264pay');
    rtph264pay.setProperty('pt', 06);
    rtph264pay.setProperty('config-interval', 1);
    var gdppay = ant.stream.createElement('gdppay');
    var sink = ant.stream.createElement('appsink');
    sink.setProperty('emit-signals', true);
    sink.connectSignal('new-sample', function (name, data) {
      console.log('New sample: name=' + name);
      console.log('Data: size=' + data.length);
    });

    pipeline.binAdd([
      source,
      filter,
      converter,
      omxh264enc,
      rtph264pay,
      gdppay,
      sink
    ]);
    pipeline.linkMany([
      source,
      filter,
      converter,
      omxh264enc,
      rtph264pay,
      gdppay,
      sink
    ]);
    pipeline.setState(pipeline.STATE_PLAYING);
    console.log(
      'Pipeline ready! (' +
        settings.my_ip_address +
        ':' +
        settings.my_port +
        ')'
    );
  }, 2000);
};

var onStop = function () {
  console.log('onStop');
  ant.stream.finalize();
};

ant.runtime.setCurrentApp(onInitialize, onStart, onStop);
