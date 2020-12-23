# Release Note
## v3.0 (Lite)
Released Date: 2019.12.28
#### Lightweight JavaScript App Runtime and APIs
* Introduce IoT.js instead of node.js
  - Node.js adaptation layer will be introduced soon.
  - It reduces minimal memory footprint of ANT to 1.3MB. (It was 30MB in v2.0)
* Easier implementation of additional APIs based on Resource API
  - Remote UI API is made based on Resource API
* Simplified and Lightweight App Runtime
  - It was app core framework in v2.0

#### New ANT Companion App
* Introduce ANT Companion App instead of ANT Manager App
  - Simplified Android-side companion application

#### New Stream API and Compiler-based Machine Learning Runtime
* Add Stream API to customize end-to-end machine learning pipeline.
  - It was camera framework and machine learning framework in v2.0
* Introduce TVM instead of ArmCL, Caffe and FANN.

## v2.0 (Classic)
Released Date: 2019.12.13
* Introduce resource frameworks and REST API to ANT
* Add new frameworks: Gateway, Selective Connection
* Replace Caffe with ArmCL for machine learning framework
* Support for Odroid-XU3 and Odroid-XU4

## v1.0 (Classic)
Released Date: 2017.12.1
* **ANT API v1.0**: App, Machine Learning Prototype, Camera, Sensor, Communication
* **Target Boards**: Raspberry Pi 2, Raspberry Pi 3, Tegra TX1, Tegra TX2
* **Frameworks**: Appcore, Camera, Sensor
* **Common Libraries**: Communication, Message
* **Prototype of Machine Learning Framework**: Support ANN
* **Built-in(System) Apps**: CameraViewer, SensorViewer, ImageClassifier, MotionClassifier
* **Testing System** (Integration with Travis CI)
