var ant = require('ant');
var console = require('console');

var settings = {};
settings.ml = {};
settings.ml.model_name = "sample-models/rpi3_mobilenet_full";
settings.ml.input_shape = [3, 224, 224, 1];
settings.ml.input_type = "uint8";
settings.ml.output_shape = [1000, 1, 1, 1];
settings.ml.output_type = "float32";
settings.ml.label_file_name = "imagenet-simple-labels.json";

settings.source_type = "/dev/video0"; // "rpi", "test", or others(v4l2src)
settings.is_h264_enabled = false;
settings.is_source_filter_enabled = false;
settings.is_scale_enabled = true;
settings.is_convert_enabled = true;
settings.video_width = 224;
settings.video_height = 224;
settings.video_format = "BGR";
settings.video_framerate = "30/1";
settings.video_sink_sync = false;
settings.my_ip_address = ant.companion.getMyIPAddress("eth0");
settings.my_port = 5000;

var on_initialize = function () {
  console.log('on_initialize');
};

var prepare_label = function (label_filepath) {
  var fs = require('fs');
  var file_contents = fs.readFileSync(label_filepath).toString();
  var labels = JSON.parse(file_contents);
  return labels;
};

var on_start = function () {
  console.log('on_start');

  var label_filepath = settings.ml.model_name + '/' + settings.ml.label_file_name;
  var labels = prepare_label(label_filepath);

  // Because ant.stream.initialize() is an async function without finish callback,
  // pipeline setting should be executed by setTimeout.
  ant.stream.initialize();
  console.log('Wait until stream thread is ready...');
  var baselinePssInKB = ant.runtime.getPSSInKB();
  console.log("Baseline Memory: " + baselinePssInKB + " KB");
  setTimeout(function () {
    var pipeline = ant.stream.createPipeline("test");
    var mainpipe_elements = [];
    var subpipe1_elements = [];
    var subpipe2_elements = [];

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
    mainpipe_elements.push(source);

    // source filter
    var sourcefilter = ant.stream.createElement("capsfilter");
    if (settings.is_source_filter_enabled) {
      sourcefilter.setCapsProperty("caps", "video/x-raw,width=" + settings.video_width
        + ",height=" + settings.video_height + ",framerate=" + settings.video_framerate
        + ",format=" + settings.video_format);
    } else {
      sourcefilter.setCapsProperty("caps", "video/x-raw");
    }
    mainpipe_elements.push(sourcefilter);

    // videoscale and scalefilter
    if (settings.is_scale_enabled) {
      var videoscale = ant.stream.createElement("videoscale");
      videoscale.setProperty("method", 0);
      videoscale.setProperty("add-borders", false);
      var scalefilter = ant.stream.createElement("capsfilter");
      scalefilter.setCapsProperty("caps", "video/x-raw,width=" + settings.video_width
        + ",height=" + settings.video_height);
      mainpipe_elements.push(videoscale);
      mainpipe_elements.push(scalefilter);
    }

    // videoconvert and convertfilter
    var converter = ant.stream.createElement("videoconvert");
    mainpipe_elements.push(converter);
    if (settings.is_convert_enabled) {
      var convertfilter = ant.stream.createElement("capsfilter");
      convertfilter.setCapsProperty("caps", "video/x-raw,format=" + settings.video_format);
      mainpipe_elements.push(convertfilter);
    }

    // tee
    var tee = ant.stream.createElement("tee");
    tee.setProperty("name", "t");
    mainpipe_elements.push(tee);

    // sub-pipeline 1
    // queue
    var queue1 = ant.stream.createElement("queue");
    subpipe1_elements.push(queue1);

    // tensor_converter
    var tensor_converter = ant.stream.createElement("tensor_converter");
    subpipe1_elements.push(tensor_converter);

    // tensor_filter (ml element)
    var ml_element = ant.ml.createMLElement(settings.ml.model_name,
      settings.ml.input_shape, settings.ml.input_type,
      settings.ml.output_shape, settings.ml.output_type);
    subpipe1_elements.push(ml_element);

    var sink = ant.stream.createElement("appsink");
    sink.setProperty("emit-signals", true);
    var prevTimestamp = 0;
    var totalPssInKB = 0;
    var totalFrameLatency = 0.0;
    var sampleCount = 0;
    sink.connectSignal("new-sample", function (name, data) {
      var result = ant.ml.getMaxOfBuffer(data, settings.ml.output_type);
      var label_message = "";

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
        label_message = "Label error";
      } else {
        sampleCount++;
        totalPssInKB += pssInKB;
        var averagePssInKB = totalPssInKB / sampleCount;
        label_message = "" + labels[result.max_index]
          + " (" + Math.round(result.max_value * 10000) / 100 + "%)\n"
          + (averagePssInKB / 1024.0).toFixed(1) + " MB";
        if (frameLatency > 0) {
          totalFrameLatency += frameLatency;
          var averageFrameLatency = totalFrameLatency / sampleCount;
          var averageFPS = 1000.0 / averageFrameLatency;
          label_message += "\n" + averageFrameLatency.toFixed(2) + " ms (" + averageFPS.toFixed(2) + " FPS)";
        }
      }
      ant.remoteui.setStreamingViewLabelText(label_message);
    });
    subpipe1_elements.push(sink);

    // sub-pipeline 2
    // h264 encoder
    var queue2 = ant.stream.createElement("queue");
    subpipe2_elements.push(queue2);

    if (settings.is_h264_enabled) {
      var omxh264enc = ant.stream.createElement("omxh264enc");
      var rtph264pay = ant.stream.createElement("rtph264pay");
      rtph264pay.setProperty("pt", 06);
      rtph264pay.setProperty("config-interval", 1);
      subpipe2_elements.push(omxh264enc);
      subpipe2_elements.push(rtph264pay);
    }

    // gdppay
    var gdppay = ant.stream.createElement("gdppay");

    // tcpserversink
    var sink = ant.stream.createElement("tcpserversink");
    sink.setProperty("sync", settings.video_sink_sync);
    sink.setProperty("host", settings.my_ip_address);
    sink.setProperty("port", settings.my_port);
    subpipe2_elements.push(gdppay);
    subpipe2_elements.push(sink);

    pipeline.binAdd(mainpipe_elements);
    pipeline.binAdd(subpipe1_elements);
    pipeline.binAdd(subpipe2_elements);
    pipeline.linkMany(mainpipe_elements);
    pipeline.linkMany([tee].concat(subpipe1_elements));
    pipeline.linkMany([tee].concat(subpipe2_elements));
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
    ant.remoteui.setStreamingViewLabelText("Waiting for Inference...");
  }, 5000);
};

var on_stop = function () {
  console.log('on_stop');
  ant.stream.finalize();
  ant.remoteui.setStreamingViewLabelText("-");
};

ant.runtime.setCurrentApp(on_initialize, on_start, on_stop);