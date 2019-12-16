var console = require('console');

function ANTML() { }

ANTML.createMLElement = function (model_name, input_shape, input_type, output_shape, output_type) {
  if (model_name.indexOf(" ") >= 0) {
    console.error("Invalid model_name! " + model_name);
    return undefined;
  }
  if (input_shape.indexOf(" ") >= 0) {
    console.error("Invalid input_shape! " + input_shape);
    return undefined;
  }
  if (input_type.indexOf(" ") >= 0) {
    console.error("Invalid input_type! " + input_type);
    return undefined;
  }
  if (output_shape.indexOf(" ") >= 0) {
    console.error("Invalid output_shape! " + output_shape);
    return undefined;
  }
  if (output_type.indexOf(" ") >= 0) {
    console.error("Invalid output_type! " + output_type);
    return undefined;
  }
  if (!StreamAPI.isInitialized()) {
    console.error("ERROR: Stream API is not initialized");
    return;
  }

  function shape_array_to_str(shape_array) {
    var shape_str = "";
    for (var i in shape_array) {
      if (i == 0) {
        shape_str = shape_str + shape_array[i];
      } else {
        shape_str = shape_str + ":" + shape_array[i];
      }
    }
    return shape_str;
  }

  var input_shape_str = shape_array_to_str(input_shape);
  var output_shape_str = shape_array_to_str(output_shape);

  var tensor_filter = StreamAPI.createElement("tensor_filter");
  tensor_filter.setProperty("framework", "python3");
  tensor_filter.setProperty("model", "./ml/tvm_nnstreamer.py");
  tensor_filter.setProperty("input", input_shape_str);
  tensor_filter.setProperty("inputtype", input_type);
  tensor_filter.setProperty("output", output_shape_str);
  tensor_filter.setProperty("outputtype", output_type);
  var custom = model_name + " " + input_shape_str + " " + input_type
    + " " + output_shape_str + " " + output_type;
  tensor_filter.setProperty("custom", custom);
  return tensor_filter;
};

ANTML.getMaxOfBuffer = function (buffer, type) {
  return native.ml_getMaxOfBuffer(buffer, type);
}

ANTML.connectCompressionServer = function (ipAddress) {
  return new CompressionServer(ipAddress);
};

function CompressionServer(ipAddress) {
  this.ipAddress = ipAddress;
}

CompressionServer.prototype.downloadModel = function (modelId) {
  if (this.ipAddress === undefined)
    return undefined;
  // TODO: implement details
};
CompressionServer.prototype.downloadKernel = function (kernelId) {
  if (this.ipAddress === undefined)
    return undefined;
  // TODO: implement details
};

module.exports = new ANTML();
module.exports.ANTML = ANTML;