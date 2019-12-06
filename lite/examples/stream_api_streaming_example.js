var ant = require('ant');
var console = require('console');

var settings = {};
settings.video_width = 480;
settings.video_height = 360;
settings.video_framerate = "30/1";
settings.video_format = "RGB";
settings.video_sink_sync = false;
settings.my_ip_address = "192.168.0.33";
settings.my_port = 5000;

var on_initialize = function () {
  console.log('on_initialize');
};

var on_start = function () {
  console.log('on_start');

  // Because ant.stream.initialize() is an async function without finish callback,
  // pipeline setting should be executed by setTimeout.
  ant.stream.initialize();
  setTimeout(function () {
    var pipeline = ant.stream.createPipeline("test");

    var source = ant.stream.createElement("videotestsrc");
    source.setProperty("pattern", 1);
    var filter = ant.stream.createElement("capsfilter");
    filter.setCapsProperty("caps", "video/x-raw,width=" + settings.video_width
      + ",height=" + settings.video_height + ",framerate=" + settings.video_framerate
      + ",format=" + settings.video_format);
    var converter = ant.stream.createElement("videoconvert");
    var omxh264enc = ant.stream.createElement("omxh264enc");
    var rtph264pay = ant.stream.createElement("rtph264pay");
    rtph264pay.setProperty("pt", 06);
    rtph264pay.setProperty("config-interval", 1);
    var gdppay = ant.stream.createElement("gdppay");
    var sink = ant.stream.createElement("tcpserversink");
    sink.setProperty("sync", settings.video_sink_sync);
    sink.setProperty("host", settings.my_ip_address);
    sink.setProperty("port", settings.my_port);

    pipeline.binAdd([source, filter, converter, omxh264enc, rtph264pay, gdppay, sink]);
    pipeline.linkMany([source, filter, converter, omxh264enc, rtph264pay, gdppay, sink]);
    pipeline.setState(pipeline.STATE_PLAYING);
    console.log("Pipeline ready! (" + settings.my_ip_address + ":" + settings.my_port + ")");
  }, 2000);
};

var on_stop = function () {
  console.log('on_stop');
  ant.stream.finalize();
};

ant.runtime.setCurrentApp(on_initialize, on_start, on_stop);