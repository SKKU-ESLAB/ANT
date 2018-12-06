/* Copyright (c) 2017 SKKU ESLAB, and contributors. All rights reserved.
 *
 * Contributor: Gyeonghwan Hong<redcarrottt@gmail.com>
 *              Hayun Lee<lhy920806@gmail.com>
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

#include <stdio.h>

#include "DNNInferenceRunner.h"
#include "ANTdbugLog.h"

MLDataUnit* DNNInferenceRunner::run(MLDataUnit* inputData) {
  int outputShape[] = {30};
  MLTensorLayout outputTensorLayout(1, outputShape, MLDataType::Char);

  if (this->mIsStreaming == false) {

    this->streamingStart();
    this->mIsStreaming = true;

    initClassifier();

    ANT_DBG_VERB("---------- ACL Daemon Run ----------");
  }


  /* Print the top N predictions. */
  this->mGraph.run();

  MLTensor* outputTensor = new MLTensor(outputTensorLayout);
  char outputStr[100] = "TEST TEST";
  //strncpy(outputStr, predictions[0].first.c_str()+10, 100);
  //strtok(outputStr, ",");

  sendDBusMsg(this->mConnection, "textOverlayStart", outputStr);
  outputTensor->assignData(outputStr);

  MLDataUnit* outputData = new MLDataUnit();
  outputData->insertTensor("output", outputTensor);
  return outputData;
}

// Get resource usage of inference runner
std::string DNNInferenceRunner::getResourceUsage() {
  std::string data("");
  // not implemented yet
  
  return data;
}

bool DNNInferenceRunner::initClassifier() {

  char *dataPath = getenv("ANT_DATA_DIR");
  if (!dataPath) {
    ANT_DBG_ERR("Cannot read ANT_DATA_DIR");
    return NULL;
  }
  string data_path = string(dataPath) + "/models/alexnet";
  string label_option = "--labels=" + data_path + "/labels.txt";

  std::cout << data_path << std::endl;
  printf("%s\n", const_cast<char*>(label_option.c_str()));

  char *argv[] = {"/etc/ant/bin/run_ant", const_cast<char*>(label_option.c_str()), NULL};
  int argc = 2;

  // Parse  arguments
  mCmdParser.parse(argc, argv);

  // Consume common parameters
  mCommonParams = consume_common_graph_parameters(mCommonOpts); 

  // Set default layout if needed
  if (!mCommonOpts.data_layout->is_set() && mCommonParams.target == Target::NEON)
  {
    mCommonParams.data_layout = DataLayout::NCHW;
  }

  // Checks
  ARM_COMPUTE_EXIT_ON_MSG(arm_compute::is_data_type_quantized_asymmetric(mCommonParams.data_type), "QASYMM8 not supported for this graph");

  // Print parameter values
  std::cout << mCommonParams << std::endl;

  // Create a preprocessor object
  const std::array<float, 3> mean_rgb{ { 122.68f, 116.67f, 104.01f } };
  std::unique_ptr<IPreprocessor> preprocessor = arm_compute::support::cpp14::make_unique<CaffePreproccessor>(mean_rgb);

  // Create input descriptor
  const TensorShape tensor_shape = permute_shape(TensorShape(227U, 227U, 3U, 1U), DataLayout::NCHW, mCommonParams.data_layout);
  TensorDescriptor input_descriptor = TensorDescriptor(tensor_shape, mCommonParams.data_type).set_layout(mCommonParams.data_layout);

  // Set weights trained layout
  const DataLayout weights_layout = DataLayout::NCHW;

  mGraph << mCommonParams.target
    << mCommonParams.fast_math_hint
    << InputLayer(input_descriptor, get_shm_input_accessor(mCommonParams, std::move(preprocessor), this))
    // Layer 1
    << ConvolutionLayer(
        11U, 11U, 96U,
        get_weights_accessor(data_path, "/cnn_data/alexnet_model/conv1_w.npy", weights_layout),
        get_weights_accessor(data_path, "/cnn_data/alexnet_model/conv1_b.npy"),
        PadStrideInfo(4, 4, 0, 0))
    .set_name("conv1")
    << ActivationLayer(ActivationLayerInfo(ActivationLayerInfo::ActivationFunction::RELU)).set_name("relu1")
    << NormalizationLayer(NormalizationLayerInfo(NormType::CROSS_MAP, 5, 0.0001f, 0.75f)).set_name("norm1")
    << PoolingLayer(PoolingLayerInfo(PoolingType::MAX, 3, PadStrideInfo(2, 2, 0, 0))).set_name("pool1")
    // Layer 2
    << ConvolutionLayer(
        5U, 5U, 256U,
        get_weights_accessor(data_path, "/cnn_data/alexnet_model/conv2_w.npy", weights_layout),
        get_weights_accessor(data_path, "/cnn_data/alexnet_model/conv2_b.npy"),
        PadStrideInfo(1, 1, 2, 2), 2)
    .set_name("conv2")
    << ActivationLayer(ActivationLayerInfo(ActivationLayerInfo::ActivationFunction::RELU)).set_name("relu2")
    << NormalizationLayer(NormalizationLayerInfo(NormType::CROSS_MAP, 5, 0.0001f, 0.75f)).set_name("norm2")
    << PoolingLayer(PoolingLayerInfo(PoolingType::MAX, 3, PadStrideInfo(2, 2, 0, 0))).set_name("pool2")
    // Layer 3
    << ConvolutionLayer(
        3U, 3U, 384U,
        get_weights_accessor(data_path, "/cnn_data/alexnet_model/conv3_w.npy", weights_layout),
        get_weights_accessor(data_path, "/cnn_data/alexnet_model/conv3_b.npy"),
        PadStrideInfo(1, 1, 1, 1))
    .set_name("conv3")
    << ActivationLayer(ActivationLayerInfo(ActivationLayerInfo::ActivationFunction::RELU)).set_name("relu3")
    // Layer 4
    << ConvolutionLayer(
        3U, 3U, 384U,
        get_weights_accessor(data_path, "/cnn_data/alexnet_model/conv4_w.npy", weights_layout),
        get_weights_accessor(data_path, "/cnn_data/alexnet_model/conv4_b.npy"),
        PadStrideInfo(1, 1, 1, 1), 2)
    .set_name("conv4")
    << ActivationLayer(ActivationLayerInfo(ActivationLayerInfo::ActivationFunction::RELU)).set_name("relu4")
    // Layer 5
    << ConvolutionLayer(
        3U, 3U, 256U,
        get_weights_accessor(data_path, "/cnn_data/alexnet_model/conv5_w.npy", weights_layout),
        get_weights_accessor(data_path, "/cnn_data/alexnet_model/conv5_b.npy"),
        PadStrideInfo(1, 1, 1, 1), 2)
    .set_name("conv5")
    << ActivationLayer(ActivationLayerInfo(ActivationLayerInfo::ActivationFunction::RELU)).set_name("relu5")
    << PoolingLayer(PoolingLayerInfo(PoolingType::MAX, 3, PadStrideInfo(2, 2, 0, 0))).set_name("pool5")
    // Layer 6
    << FullyConnectedLayer(
        4096U,
        get_weights_accessor(data_path, "/cnn_data/alexnet_model/fc6_w.npy", weights_layout),
        get_weights_accessor(data_path, "/cnn_data/alexnet_model/fc6_b.npy"))
    .set_name("fc6")
    << ActivationLayer(ActivationLayerInfo(ActivationLayerInfo::ActivationFunction::RELU)).set_name("relu6")
    // Layer 7
    << FullyConnectedLayer(
        4096U,
        get_weights_accessor(data_path, "/cnn_data/alexnet_model/fc7_w.npy", weights_layout),
        get_weights_accessor(data_path, "/cnn_data/alexnet_model/fc7_b.npy"))
    .set_name("fc7")
    << ActivationLayer(ActivationLayerInfo(ActivationLayerInfo::ActivationFunction::RELU)).set_name("relu7")
    // Layer 8
    << FullyConnectedLayer(
        1000U,
        get_weights_accessor(data_path, "/cnn_data/alexnet_model/fc8_w.npy", weights_layout),
        get_weights_accessor(data_path, "/cnn_data/alexnet_model/fc8_b.npy"))
    .set_name("fc8")
    // Softmax
    << SoftmaxLayer().set_name("prob")
    << OutputLayer(get_output_accessor(mCommonParams, 5));

  // Finalize graph
  GraphConfig config;
  config.num_threads = mCommonParams.threads;
  config.use_tuner   = mCommonParams.enable_tuner;
  config.tuner_file  = mCommonParams.tuner_file;

  this->mGraph.finalize(mCommonParams.target, config);
}

bool DNNInferenceRunner::initSemaphore()
{
  this->mSem = sem_open("ORG.ANT.CAMERA", 0, 0666, 0);
  if (this->mSem == SEM_FAILED) {
    ANT_DBG_ERR(strerror(errno));
    sem_unlink("ORG.ANT.CAMERA");
    return false;
  }
  return true;
}

bool DNNInferenceRunner::releaseSemaphore() {
  sem_close(this->mSem);
  return true;
}

bool DNNInferenceRunner::initSharedMemorySpace() {
  this->mShmId = shmget((key_t)SHM_KEY_FOR_BUFFER, 0, 0);
  if (this->mShmId == -1) {
    ANT_DBG_ERR(strerror(errno));
    return false;
  }
  this->mShmPtr = shmat(this->mShmId, (void*)0, 0666|IPC_CREAT);
  if (this->mShmPtr == (void*)-1) {
    ANT_DBG_ERR(strerror(errno));
  }
  ANT_DBG_VERB("Shared memory space init");
  return true;
}

bool DNNInferenceRunner::releaseSharedMemorySpace() {
  return true;
}

bool DNNInferenceRunner::initDBus() {
  dbus_error_init(&this->mError);
  this->mConnection = dbus_bus_get(DBUS_BUS_SYSTEM, &this->mError);
  if (!this->mConnection) {
    ANT_DBG_ERR("dbus_bus_get error");
    dbus_error_free(&this->mError);
    return false;
  }
  return true;
}

bool DNNInferenceRunner::streamingStart() {
  ANT_DBG_VERB("Streaming (CAMFW -> MLFW) start!!");
  this->initDBus();
  sendDBusMsg(this->mConnection, "copyShmStart", NULL);
  sleep(2);

  this->mBufferSize = CAMERA_DEFAULT_BUF_SIZE;

  mFrame = (unsigned char*)malloc(sizeof(unsigned char*) * this->mBufferSize);

  while (1) {
    if (initSharedMemorySpace())
      break;
  }

  if (!initSemaphore()) {
    ANT_DBG_ERR("Cannot init semaphore");
    return false;
  }

  return true;
}

static void sendDBusMsg(DBusConnection* conn, const char* msg, const char* text) {
  DBusMessage* message;
  message = dbus_message_new_signal("/org/ant/cameraManager", "org.ant.cameraManager", msg);

  unsigned pid = getpid();
  unsigned camera_num = 0;

  if (text == NULL) {
    dbus_message_append_args(message,
        DBUS_TYPE_UINT64, &pid,
        DBUS_TYPE_UINT64, &camera_num,
        DBUS_TYPE_INVALID);
  }
  else {
    dbus_message_append_args(message,
        DBUS_TYPE_UINT64, &pid,
        DBUS_TYPE_UINT64, &camera_num,
        DBUS_TYPE_STRING, &text,
        DBUS_TYPE_INVALID);
  }
  dbus_connection_send(conn, message, NULL);
  dbus_message_unref(message);
}

ShmAccessor::ShmAccessor(DNNInferenceRunner* runner, bool bgr, std::unique_ptr<IPreprocessor> preprocessor)
  : _runner(runner), _bgr(bgr), _preprocessor(std::move(preprocessor))
{
}

bool ShmAccessor::access_tensor(ITensor &tensor)
{
  if (_runner == nullptr) {
    ANT_DBG_ERR("DNNInferenceRunner is null prointer");
    return false;
  }

  sem_wait(_runner->getSem());
  memcpy(_runner->getFrame(), _runner->getShmPtr(), _runner->getBufferSize());
  sem_post(_runner->getSem());

  unsigned int _width = 227;
  unsigned int _height = 227;

  const DataLayout data_layout = tensor.info()->data_layout();
  const TensorShape tensor_shape = tensor.info()->tensor_shape();

  map(tensor, true);

  size_t stride_z = 0;

  Window window;
  if (data_layout == DataLayout::NCHW) {
    window.set(Window::DimX, Window::Dimension(0, _width, 1));
    window.set(Window::DimY, Window::Dimension(0, _height, 1));
    window.set(Window::DimZ, Window::Dimension(0, 1, 1));
    stride_z = tensor.info()->strides_in_bytes()[2];
  }
  else
  {
    window.set(Window::DimX, Window::Dimension(0, 1, 1));
    window.set(Window::DimY, Window::Dimension(0, _width, 1));
    window.set(Window::DimZ, Window::Dimension(0, _height, 1));
    stride_z = tensor.info()->strides_in_bytes()[0];
  }

  Iterator out(&tensor, window);

  unsigned char red   = 0;
  unsigned char green = 0;
  unsigned char blue  = 0;

  bool bgr = _bgr;
  _feeder = support::cpp14::make_unique<MemoryImageFeeder>(_runner->getFrame());

  execute_window_loop(window, [&](const Coordinates & id)
  {
    red   = _feeder->get();
    green = _feeder->get();
    blue  = _feeder->get();

    switch(tensor.info()->data_type())
    {
      case DataType::U8:
      {
        *(out.ptr() + 0 * stride_z) = bgr ? blue : red;
        *(out.ptr() + 1 * stride_z) = green;
        *(out.ptr() + 2 * stride_z) = bgr ? red : blue;
        break;
      }
      case DataType::F32:
      {
        *reinterpret_cast<float *>(out.ptr() + 0 * stride_z) = static_cast<float>(bgr ? blue : red);
        *reinterpret_cast<float *>(out.ptr() + 1 * stride_z) = static_cast<float>(green);
        *reinterpret_cast<float *>(out.ptr() + 2 * stride_z) = static_cast<float>(bgr ? red : blue);
        break;
      }
      default:
      {
        ARM_COMPUTE_ERROR("Unsupported data type");
      }
    }
  },
  out);

  _feeder = nullptr;

  unmap(tensor);

  if (_preprocessor)
  {
    _preprocessor->preprocess(tensor);
  }

  return true;
}
