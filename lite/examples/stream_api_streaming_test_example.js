// Streaming Example
//  - Video Source => TCP Network (video) => Smartphone Streaming View


var ant = require('ant');
var console = require('console');

var settings = {};
settings.source_type = "/dev/video0"; // "rpi", "test", or others(v4l2src)
settings.is_h264_enabled = false;
settings.is_source_filter_enabled = false;
settings.is_scale_enabled = true;
settings.is_convert_enabled = true;
settings.video_width = 224;
settings.video_height = 224;
settings.video_format = "RGB";
settings.video_framerate = "30/1";
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
  console.log('Wait until stream thread is ready...');
  setTimeout(function () {
    var pipeline = ant.stream.createPipeline("test");
    var elements = [];

    // source
    var source;
    if (settings.source_type == "rpi") {
      source = ant.stream.createElement("rpicamsrc");
    } else if (settings.source_type == "test") {
      source = ant.stream.createElement("videotestsrc");
      source.setProperty("pattern", 1);
    } else {
      source = ant.stream.createElement("v4l2src");
      source.setProperty("device", settings.source_type);
    }
    elements.push(source);

    // source filter
    var sourcefilter = ant.stream.createElement("capsfilter");
    if (settings.is_source_filter_enabled) {
      sourcefilter.setCapsProperty("caps", "video/x-raw,width=" + settings.video_width
        + ",height=" + settings.video_height + ",framerate=" + settings.video_framerate
        + ",format=" + settings.video_format);
    } else {
      sourcefilter.setCapsProperty("caps", "video/x-raw");
    }
    elements.push(sourcefilter);

    // videoscale and scalefilter
    if (settings.is_scale_enabled) {
      var videoscale = ant.stream.createElement("videoscale");
      videoscale.setProperty("method", 0);
      videoscale.setProperty("add-borders", false);
      var scalefilter = ant.stream.createElement("capsfilter");
      scalefilter.setCapsProperty("caps", "video/x-raw,width=" + settings.video_width
        + ",height=" + settings.video_height);
      elements.push(videoscale);
      elements.push(scalefilter);
    }

    // videoconvert and convertfilter
    var converter = ant.stream.createElement("videoconvert");
    elements.push(converter);
    if (settings.is_convert_enabled) {
      var convertfilter = ant.stream.createElement("capsfilter");
      convertfilter.setCapsProperty("caps", "video/x-raw,format=" + settings.video_format);
      elements.push(convertfilter);
    }

    // h264 encoder
    if (settings.is_h264_enabled) {
      var omxh264enc = ant.stream.createElement("omxh264enc");
      var rtph264pay = ant.stream.createElement("rtph264pay");
      rtph264pay.setProperty("pt", 06);
      rtph264pay.setProperty("config-interval", 1);
      elements.push(omxh264enc);
      elements.push(rtph264pay);
    }

    // gdppay
    var gdppay = ant.stream.createElement("gdppay");

    // tcpserversink
    var sink = ant.stream.createElement("tcpserversink");
    sink.setProperty("sync", settings.video_sink_sync);
    sink.setProperty("host", settings.my_ip_address);
    sink.setProperty("port", settings.my_port);
    elements.push(gdppay);
    elements.push(sink);

    pipeline.binAdd(elements);
    pipeline.linkMany(elements);
    pipeline.setState(pipeline.STATE_PLAYING);
    console.log("Pipeline ready! (" + settings.my_ip_address + ":" + settings.my_port + ")");

    // Remote pipeline
    var remote_pipeline;
    if (settings.is_h264_enabled) {
      remote_pipeline = "tcpclientsrc host=" + settings.my_ip_address + " port=" + settings.my_port
        + " ! gdpdepay ! rtph264depay ! h264parse ! avdec_h264 ! videoconvert ! autovideosink sync=false";
    } else {
      remote_pipeline = "tcpclientsrc host=" + settings.my_ip_address + " port=" + settings.my_port
        + " ! gdpdepay ! videoconvert ! autovideosink sync=false";
    }
    ant.remoteui.setStreamingViewPipeline(remote_pipeline);
    ant.remoteui.setStreamingViewLabelText("ON");
  }, 5000);
};

var on_stop = function () {
  console.log('on_stop');
  ant.stream.finalize();
  ant.remoteui.setStreamingViewLabelText("-");
};

ant.runtime.setCurrentApp(on_initialize, on_start, on_stop);