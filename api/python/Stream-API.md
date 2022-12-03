
# ANT Stream API

ANT Python is now available on any ARM Cortex-A devices. In future, we will add support for Cortex-M or Cortex-R devices.


## Class
antStream(_mIsInitialized=False, pipelines=[])

Stream API

**Kind**: class

|Param|Type|Description|
|------|---|---|
|_mIsInitialized|bool|check stream object is Initialized|
|pipelines|list|piplined work|


## Method

### call_dbus_method(message)

This method callback native function, ant_stream_callDbusMethod with message.

If stream opject is not Initialized, this function return error message and return False

**Kind**: private class method

|Param|Type|Description|
|------|---|---|
|message|string|call Dbus message for native.ant_stream_callDbusMethod fuinciton |

### is_initialized()

This method return true if stream object is Initialized, or not return false.

**Kind**: class method

### finalize()

This function quit antStream object and MainLoop.

if MainLoop is quited, return true.

**Kind**: class method


### create_pipeline(pipelineName)

this method create Pipeline object by making instance of Pipline class.

**Kind**: class method

|Param|Type|Description|
|------|---|---|
|pipelineName|string|get pipline name for Stream API initialization|

### create_element(elementName)

this method create Element of  

**Kind**: class method

|Param|Type|Description|
|------|---|---|
|elementName|string|get elementName for Stream API initialization|




## Class
Pipeline(name, elementIndex)

Pipeline api for Stream API

**Kind**: class

|Param|Type|Description|
|------|---|---|
|name|string|name of Pipline instance|
|elementIndex|int|index of element|


## Method

### binAdd(element_or_elements)

get antstream object and test.

**Kind**: private class method

|Param|Type|Description|
|------|---|---|
|elementOrElements|list| check this element is working or not |

### set_state(state)

This method return true if stream object is Initialized, or not return false.

**Kind**: class method

### link_many(elements)

link prevElement of pipline. if link failed, return False.

**Kind**: class method

|Param|Type|Description|
|------|---|---|
|elements|list|get element of pipline|

### unref()

call callDbusMethod function

**Kind**: class method


## Class Element

Element class

**Kind**: class

|Param|Type|Description|
|------|---|---|
|name|string|name of Pipline instance|
|elementIndex|int|index of element|


## Method

### set_property(key, value)

get antstream object and test.

**Kind**: class method

|Param|Type|Description|
|------|---|---|
|key|str|key for Property, setting type |
|value|str|value for Property, setting type|


### set_caps_property(key, value)

this method check key and value's type are string.

if they are string type, callback callDbusMethod.

**Kind**: class method

|Param|Type|Description|
|------|---|---|
|key|str|key for Property, setting type |
|value|str|value for Property, setting type|


### connect_signal(detailedSignal, handler)

this method use ant_stream_elementConnectSignal function in native.

**Kind**: class method

|Param|Type|Description|
|------|---|---|
|detailedSignal|int|hander list's index number|
|handler|list|list of handler|


### link(destElement)

this method link ANTstream with elemant.

**Kind**: class method

|Param|Type|Description|
|------|---|---|
|destElement|<class '__main__.Element'>|destination element|

