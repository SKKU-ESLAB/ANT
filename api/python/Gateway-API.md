<a name="ANTGateway"></a>

## ANTGateway()
ANT Gateway API

 In ANT framework, gateway provides sensor virtualization and  DNN partitioning. Sensor virtualization means unifying interfaces to access  physical sensors, soft sensors, local sensors, and remote sensors. DNN partitioning means executing a DNN on multiple heterogeneous devices. In real IoT systems, there are heterogeneous devices including gateway, but the single device among them cannot provide sufficient computational resources to DNNs. Therefore, DNN partitioning is required.
 
 There are two ways to implement gateway in ANT framework.
 1. ML fragment element (Based on gstreamer)
 2. Virtual sensor (Based on OCF standard)
 
 ML fragment element is an approach to extend the gstreamer to DNN
 partitioning. In this way, to associate a DNN with a physical sensor, the
 app developer must implement their own sensor sampling element that can be
 compatible to gstreamer.
 
 Actually, many IoT devices does not use gstreamer-based approach, but use
 OCF standard. Therefore, we additionally provide virtual sensor, an approach
 based on OCF standard.
 
 Virtual sensor is an abstract layer that provide unified interface of
 physical sensors, rule-based soft sensors, and DNN-based soft sensors
 (a.k.a. deep sensors). It also supports DNN partitioning on the deep
 sensors. To support that, ANT additionally provides DNN fragment engine
 and gateway manager.

**Kind**: class

## createImgClsImagenetElement(modelpath, numfragments, targetUri)

load model from path

**Kind**: class method

## createDFE(modelname, number of fragments)

load model inference function for model

**Kind**: class method

## getVSAdapter()

load virtual sensor

**Kind**: class method

## VirtualSensorAdapter()

module to arbitrate user and OCF thread and to                         organize virtual sensor system.

**Kind**: class

## onPrepareOCFServer()

connect all OCFsensors, gateway maneger

**Kind**: class method

## onPrepareOCFClient()

set gateway maneger and connect to gateway client

**Kind**: class method

## createSensor(sesorname, sensorType,deviceType, observerHandler, generatorHandler, settingHandler)

set virtual sensor

**Kind**: class method

## findSensorByName(name)

search sensor by name

**Kind**: class method

## findSensorByUri(name)

search sensor by Uri

**Kind**: class method

## createGWManeger()

create gateway maneger

**Kind**: class method

## createGWClient()

create gateway client

**Kind**: class method

## VirtualSensor()

module to service virtual sensor through OCF resources and several JavaScript handlers

**Kind**: class

## DFE()

module to perform partitioned DNN inference

**Kind**: class

## updateAverageLatency(latency)

load new average latency

**Kind**: class method

## loadAndPreprocessImag(img_path)

load image

**Kind**: class method

## execute(inputbuff, startlayernum, endlayernum)

run dnn model with offloading

**Kind**: class method

## GatewayManager()

module to manage virtual sensors and DNN partitioning. This module must run on gateway device.

**Kind**: class

## DFEScheduler()

module to manage DNN partitioning. It monitors the current status of DFEs running on several devices and make commands to control DFE's partitioning points

**Kind**: class

## VirtualSensorManager()

module to manage virtual sensors. It discovers virtual sensors via OCF discovery message and associate the virtual sensors to form a sensor data pipeline for the IoT applications.

**Kind**: class

## GetVirtualSensors()

print all Virtualsensors

**Kind**: class method



