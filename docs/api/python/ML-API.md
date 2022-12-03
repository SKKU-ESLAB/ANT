# ANTml -API

ANT Python is now available on any ARM Cortex-A devices. In future, we will add support for Cortex-M or Cortex-R devices.


## Class
ANTML()

**Kind**: class

## Method

### create_img_cls_imagenet_element(modelPath)

this method create ML element object(dateset : imagenet)

**Kind**: class method

|Param|Type|Description|
|------|---|---|
|modelPath|string|location of model file|

### create_obj_det_coco_element(modelPath)

this method create ML element object(dateset : coco)

**Kind**: class method

|Param|Type|Description|
|------|---|---|
|modelPath|string|location of model file|

### create_ml_fragment_element(modelPath, inputShapes,inputTypes,inputNames, taskName, numFragments,targetUri)

this method create ml fragment element object

**Kind**: class method

|Param|Type|Description|
|------|---|---|
|modelPath|string|location of model file|
|inputShapes|int|input shape|
|inputTypes|string|input type|
|inputNames|string|input Names|
|taskName|string|name of workload|
|numFragments|string|num of fragment|
|targetUri|string|location of target|


### toFloatArray(buffer)

this method use native function ant_ml_to_float_array

**Kind**: class method

|Param|Type|Description|
|------|---|---|
|buffer|list|array buffer|


### download_model(modelUrl, overwriteIfExists)

this method download model from Url and check over write If Exists

|Param|Type|Description|
|------|---|---|
|modelUrl|string|model location|
|overwriteIfExists|bool|check there is a model|






