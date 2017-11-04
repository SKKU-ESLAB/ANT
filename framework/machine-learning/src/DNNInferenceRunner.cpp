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

/*
 * Caffe LICENSE
 *
 * COPYRIGHT
 *
 * All contributions by the University of California:
 * Copyright (c) 2014-2017 The Regents of the University of California (Regents)
 * All rights reserved.
 *
 * All other contributions:
 * Copyright (c) 2014-2017, the respective contributors
 * All rights reserved.
 *
 * Caffe uses a shared copyright model: each contributor holds copyright over
 * their contributions to Caffe. The project versioning records all such
 * contribution and copyright details. If a contributor wants to further mark
 * their specific copyright on a particular contribution, they should indicate
 * their copyright solely in the commit message of the change when it is
 * committed.
 */


#include <stdio.h>

#include "DNNInferenceRunner.h"
#include "ANTdbugLog.h"

MLDataUnit* DNNInferenceRunner::run(MLDataUnit* inputData) {
  int outputShape[] = {30};
  MLTensorLayout outputTensorLayout(1, outputShape, MLDataType::Char);

  if (this->mIsStreaming == false) {
    ::google::InitGoogleLogging("DNNInferenceRunner");

    char *dataPath = getenv("ANT_DATA_DIR");
    if (!dataPath) {
      ANT_DBG_ERR("Cannot read ANT_DATA_DIR");
      return NULL;
    }
    string dataPathStr = string(dataPath);

    string model_file   = dataPathStr + "/models/squeezenet_v1.1_deploy.prototxt";
    string trained_file = dataPathStr + "/models/squeezenet_v1.1.caffemodel";
    string mean_file    = dataPathStr + "/models/ilsvrc12_imagenet_mean.binaryproto";
    string label_file   = dataPathStr + "/models/ilsvrc12_synset_words.txt";

    std::cout << model_file << std::endl;

    initClassifier(model_file, trained_file, mean_file, label_file);

    this->streamingStart();
    this->mIsStreaming = true;
    ANT_DBG_VERB("---------- Caffe Daemon Run ----------");
  }

  IplImage *image = this->retrieveFrame();

  cv::Mat img = cv::cvarrToMat(image);

  CHECK(!img.empty()) << "Unable to decode image ";
  std::vector<Prediction> predictions = this->classify(img);

  /* Print the top N predictions. */
  for (size_t i = 0; i < predictions.size(); ++i) {
    Prediction p = predictions[i];
    std::cout << std::fixed << std::setprecision(4) << p.second << " - \""
      << p.first << "\"" << std::endl;
  }

  MLTensor* outputTensor = new MLTensor(outputTensorLayout);
  char outputStr[100];
  strncpy(outputStr, predictions[0].first.c_str()+10, 100);
  strtok(outputStr, ",");

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

bool DNNInferenceRunner::initClassifier(const string& model_file,
                       const string& trained_file,
                       const string& mean_file,
                       const string& label_file) {
#ifdef CPU_ONLY
  Caffe::set_mode(Caffe::CPU);
#else
  Caffe::set_mode(Caffe::GPU);
#endif

  /* Load the network. */
  this->mNet.reset(new Net<float>(model_file, TEST));
  this->mNet->CopyTrainedLayersFrom(trained_file);

  CHECK_EQ(this->mNet->num_inputs(), 1) << "Network should have exactly one input.";
  CHECK_EQ(this->mNet->num_outputs(), 1) << "Network should have exactly one output.";

  Blob<float>* input_layer = this->mNet->input_blobs()[0];
  this->mNumChannels = input_layer->channels();
  CHECK(this->mNumChannels == 3 || this->mNumChannels == 1)
    << "Input layer should have 1 or 3 channels.";
  this->mInputGeometry = cv::Size(input_layer->width(), input_layer->height());

  /* Load the binaryproto mean file. */
  this->setMean(mean_file);

  /* Load labels. */
  std::ifstream labels(label_file.c_str());
  CHECK(labels) << "Unable to open labels file " << label_file;
  string line;
  while (std::getline(labels, line))
    this->mLabels.push_back(string(line));

  Blob<float>* output_layer = this->mNet->output_blobs()[0];
  CHECK_EQ(this->mLabels.size(), output_layer->channels())
    << "Number of labels is different from the output layer dimension.";
}

static bool pairCompare(const std::pair<float, int>& lhs,
                        const std::pair<float, int>& rhs) {
  return lhs.first > rhs.first;
}

/* Return the indices of the top N values of vector v. */
static std::vector<int> argmax(const std::vector<float>& v, int N) {
  std::vector<std::pair<float, int> > pairs;
  for (size_t i = 0; i < v.size(); ++i)
    pairs.push_back(std::make_pair(v[i], i));
  std::partial_sort(pairs.begin(), pairs.begin() + N, pairs.end(), pairCompare);

  std::vector<int> result;
  for (int i = 0; i < N; ++i)
    result.push_back(pairs[i].second);
  return result;
}

/* Return the top N predictions. */
std::vector<Prediction> DNNInferenceRunner::classify(const cv::Mat& img, int N) {
  std::vector<float> output = this->predict(img);

  N = std::min<int>(this->mLabels.size(), N);
  std::vector<int> maxN = argmax(output, N);
  std::vector<Prediction> predictions;
  for (int i = 0; i < N; ++i) {
    int idx = maxN[i];
    predictions.push_back(std::make_pair(this->mLabels[idx], output[idx]));
  }

  return predictions;
}

/* Load the mean file in binaryproto format. */
void DNNInferenceRunner::setMean(const string& mean_file) {
  BlobProto blob_proto;
  ReadProtoFromBinaryFileOrDie(mean_file.c_str(), &blob_proto);

  /* Convert from BlobProto to Blob<float> */
  Blob<float> mean_blob;
  mean_blob.FromProto(blob_proto);
  CHECK_EQ(mean_blob.channels(), this->mNumChannels)
    << "Number of channels of mean file doesn't match input layer.";

  /* The format of the mean file is planar 32-bit float BGR or grayscale. */
  std::vector<cv::Mat> channels;
  float* data = mean_blob.mutable_cpu_data();
  for (int i = 0; i < this->mNumChannels; ++i) {
    /* Extract an individual channel. */
    cv::Mat channel(mean_blob.height(), mean_blob.width(), CV_32FC1, data);
    channels.push_back(channel);
    data += mean_blob.height() * mean_blob.width();
  }

  /* Merge the separate channels into a single image. */
  cv::Mat mean;
  cv::merge(channels, mean);

  /* Compute the global mean pixel value and create a mean image
   * filled with this value. */
  cv::Scalar channel_mean = cv::mean(mean);
  this->mMean = cv::Mat(this->mInputGeometry, mean.type(), channel_mean);
}

std::vector<float> DNNInferenceRunner::predict(const cv::Mat& img) {
  Blob<float>* input_layer = this->mNet->input_blobs()[0];
  input_layer->Reshape(1, this->mNumChannels,
                       this->mInputGeometry.height, this->mInputGeometry.width);
  /* Forward dimension change to all layers. */
  this->mNet->Reshape();

  std::vector<cv::Mat> input_channels;
  this->wrapInputLayer(&input_channels);

  this->preprocess(img, &input_channels);

  this->mNet->Forward();

  /* Copy the output layer to a std::vector */
  Blob<float>* output_layer = this->mNet->output_blobs()[0];
  const float* begin = output_layer->cpu_data();
  const float* end = begin + output_layer->channels();
  return std::vector<float>(begin, end);
}

/* Wrap the input layer of the network in separate cv::Mat objects
 * (one per channel). This way we save one memcpy operation and we
 * don't need to rely on cudaMemcpy2D. The last preprocessing
 * operation will write the separate channels directly to the input
 * layer. */
void DNNInferenceRunner::wrapInputLayer(std::vector<cv::Mat>* input_channels) {
  Blob<float>* input_layer = this->mNet->input_blobs()[0];

  int width = input_layer->width();
  int height = input_layer->height();
  float* input_data = input_layer->mutable_cpu_data();
  for (int i = 0; i < input_layer->channels(); ++i) {
    cv::Mat channel(height, width, CV_32FC1, input_data);
    input_channels->push_back(channel);
    input_data += width * height;
  }
}

void DNNInferenceRunner::preprocess(const cv::Mat& img,
                            std::vector<cv::Mat>* input_channels) {
  /* Convert the input image to the input image format of the network. */
  cv::Mat sample;
  if (img.channels() == 3 && this->mNumChannels == 1)
    cv::cvtColor(img, sample, cv::COLOR_BGR2GRAY);
  else if (img.channels() == 4 && this->mNumChannels == 1)
    cv::cvtColor(img, sample, cv::COLOR_BGRA2GRAY);
  else if (img.channels() == 4 && this->mNumChannels == 3)
    cv::cvtColor(img, sample, cv::COLOR_BGRA2BGR);
  else if (img.channels() == 1 && this->mNumChannels == 3)
    cv::cvtColor(img, sample, cv::COLOR_GRAY2BGR);
  else
    sample = img;

  cv::Mat sample_resized;
  if (sample.size() != this->mInputGeometry)
    cv::resize(sample, sample_resized, this->mInputGeometry);
  else
    sample_resized = sample;

  cv::Mat sample_float;
  if (this->mNumChannels == 3)
    sample_resized.convertTo(sample_float, CV_32FC3);
  else
    sample_resized.convertTo(sample_float, CV_32FC1);

  cv::Mat sample_normalized;
  cv::subtract(sample_float, this->mMean, sample_normalized);

  /* This operation will write the separate BGR planes directly to the
   * input layer of the network because it is wrapped by the cv::Mat
   * objects in input_channels. */
  cv::split(sample_normalized, *input_channels);

  CHECK(reinterpret_cast<float*>(input_channels->at(0).data)
        == this->mNet->input_blobs()[0]->cpu_data())
    << "Input channels are not wrapping the input layer of the network.";
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

IplImage* DNNInferenceRunner::retrieveFrame() {
  if (this->mShmPtr) {
    sem_wait(this->mSem);
    memcpy((char*)this->mFrame.imageData, (char*)this->mShmPtr,
        CAMERA_DEFAULT_BUF_SIZE);
    sem_post(this->mSem);
  }
  return &this->mFrame;
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

  cvInitImageHeader(&this->mFrame, cvSize(640, 480), IPL_DEPTH_8U, 3, IPL_ORIGIN_BL, 8);
  this->mFrame.imageData = (char *)cvAlloc(this->mFrame.imageSize);
  if (!this->mFrame.imageData) {
    ANT_DBG_ERR("cvAlloc error");
    return false;
  }

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
