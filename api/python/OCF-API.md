# ANT-OCF API

## Functions

### make_request(requestId, query, qos, endpoint, uri, userHandler)

make request for end-point 

**Kind**:function
**Return**:dict - dictionary contains Request for end-point

|Param|Type|
|------|---|
|requestId|int|
|query|string|
|qos|int|
|endpoint|OCFEndpoint|
|uri|string|
|userHandler|function|

### oaObserveResponseHandler(request_id, response)

observe respones hanlder

**Kind**:function
**Return**:handler - function for handler to observe request 

|Param|Type|
|------|---|
|request_id|int|
|response|int|

### oaGetResponseHandler(request_id, response)

get handler to response

**Kind**:function
**Return**:handler - function for handler to response request_id

|Param|Type|
|------|---|
|request_id|int|
|response|int|

### onDeleteResponseHandler(request_id, response)

delete handler about request_id

**Kind**:function
**Return**:bool - isSuccess

|Param|Type|
|------|---|
|request_id|int|
|response|int|


### oaPostResponseHandler(request_id, response:

post handler to responce 

**Kind**:function
**Return**:bool - isSuccess

|Param|Type|
|------|---|
|request_id|int|
|response|int|

### oaPutResponseHandler(request_id, response)

put response handler

**Kind**:function
**Return**:bool - isSuccess

|Param|Type|
|------|---|
|request_id|int|
|response|int|

### oaResponseHandler(requestId, reponse, requestList, isOneway)

add response handler

**Kind**:function
**Return**:bool - isSuccess

|Param|Type|
|------|---|
|requestId|int|
|response|int|
|requestList|list|
|isOneway|bool|

# Class
OCF()

**Kind**: class

## Method

# Class
OCF Adapter

It manages OCF thread's lifecycle.

**Kind**: class

## Method

### __init__()

create OCF Element

**Kind**:class method

### initialize()

initialize OCF Element

**Kind**:class method

### deinitialize()

de-initilaize OCF Element

**Kind**:class method

### setPlaform(mfgName)

**Kind**:class method

|Param|Type|Description|
|------|---|---|
|mfgName|string|Manufacturer's name|

### getPlaform()

get Plaform

**Kind**:class method
**Return**:string-platformName
### addDevice(uri, resourceType, name, specVersion, dataModelVersion)

add device

**Kind**:class method

|Param|Type|Description|
|------|---|---|
|uri|string|id of device to be serviced|
|resourceType|string|resource type of device to be serviced|
|name|string|name of device to be serviced|
|specVersion|string|specification of device to be serviced|
|dataModelVersion|string|data model version of device to be serviced Set the device to be serviced by the OCFAdapter. One OCFAdapter can service multiple devices.|

### onPrepareEventLoop(handler)

prepare Event Loop

**Kind**:class method

|Param|Type|
|------|---|
|handler|function|

###  onPrepareClient(handler):

prepare Cilent

**Kind**:class method

|Param|Type|Description|
|------|---|---|
|handler|function|called when all preparations for executing client functions in the OCF thread are complete|

### onPrepareServer(handler)

prepare Server

**Kind**:class method

|Param|Type|Description|
|------|---|---|
|handler|function|called when all preparations for executing server functions in OCF thread are complete|

### start()

OCFAdapter.start Run the OCF thread. This function must be called after OCFAdapter.initialize() is called. You can use the OCF Server API and OCF Client API only while the OCF thread is running. If you need to know exactly when you can use OCF Server API and OCF Client API, you can use the handlers of OCFAdapter.onPrepareServer() and OCFAdapter.onPrepareClient().

**Kind**:class method

### stop()

OCFAdapter.stop Stop the OCF thread

**Kind**:class method

### addResource()

add Resource

**Kind**:class method

|Param|Type|
|------|---|
|resource|OCFResource|

### deleteResource(resource)

delete Resource

**Kind**:class method

|Param|Type|
|------|---|
|resource|OCFResource|

### getResource()

get Resource

**Kind**:class method
**Return**:Resource

### repStartRootObject()

OCFAdapter.repStartRootObject Let the OCF thread start writing the OCRepresentation.

**Kind**:class method

### repSet(key, value)

OCFAdapter.repSet

**Kind**:class method

|Param|Type|Description|
|------|---|---|
|resource|OCFResource|
|value|Boolean \| Number \|string|The value is stored in a specific key among OCRepresentations being created by OCF thread. In this function, various types of data including Boolean, Number, and string can be used as value.

### repSetBufferAndstring(bufferValue, stringValue)

set Buffer and string for repset

|Param|Type|
|------|---|
|bufferValue|buffer|
|stringValue|string|

**Kind**:class method

### ocfAdapter.repEndRootObject()

OCFAdapter.repEndRootObject Finish writing OCRepresentation of OCF thread.

**Kind**:class method
**Return**:OCFAdapter

### sendResponse(ocfRequest, statusCode, responsePayload)

send Response

**Kind**:class method

|Param|Type|Description|
|------|---|---|
|ocfRequest|OCFRequest|target of response|
|statusCode|int|Response status code value Sends a response with a specific status code to a request from another device.|
|responsePayload|Dict|payload for Response|

### sendResponseBuffer(ocfRequest, statusCode, responsePayloadBuffer,responsePayloadstring)

send Response Buffer

**Kind**:class method

|Param|Type|Description|
|------|---|---|
|ocfRequest|OCFRequest|target of response|
|statusCode|int|Response status code value Sends a response with a specific status code to a request from another device.|
|responsePayloadBuffer|Buffer|response for Buffer to payload|
|responsePayloadstring|string|playload name to response|

### stopDiscovery()

stop discovery

**Kind**:class method
**Return**:bool - status

### isDiscovering()

check the enability to discovery

**Kind**:class method
**Return**:bool - status

### discovery(resourceType, discoveryHandler)

discovery depends on ResourceType

**Kind**:class method
**Return**:bool - isSuccess

|Param|Type|Description|
|------|---|---|
|resourceType|string|Type of resource to find on the network|
|discoveryHandler|function|called whenever one OCFResource is discovered|

### discoveryAll(discoveryHandler)

discovery all ResourceType

**Kind**:class method
**Return**:bool - isSuccess Search all resources regardless of any type on the networks.

### observe(endpoint,uri,userHandler,query,qos,isResponsePayloadBuffer)

observe endpoint

**Kind**:class method
**Return**:bool - isSuccess

|Param|Type|
|------|---|
|endpoint|OCFEndpoint|
|uri|string|
|userHandler|function|
|query|string|
|qos|int|
|isResponsePayloadBuffer|Buffer|

### stopObserve(endpoint, uri)

stop observe end-point

**Kind**:class method
**Return**:bool - isSuccess

|Param|Type|
|------|---|
|endpoint|OCFEndpoint|
|uri|string|
|isResponsePayloadBuffer|Buffer|

### get(endpoint,uri,userHandler,query,qos,isResponsePayloadBuffer)

get information of end-point

**Kind**:class method
**Return**:bool - isSuccess

|Param|Type|
|------|---|
|endpoint|OCFEndpoint|
|uri|string|
|userHandler|function|
|query|string|
|qos|int|
|isResponsePayloadBuffer|Buffer|

### delete(endpoint, uri, userHandler, query, qos, isResponsePayloadBuffer)

delete endpoint

**Kind**:class method
**Return**:bool - isSuccess

|Param|Type|
|------|---|
|endpoint|OCFEndpoint|
|uri|string|
|userHandler|function|
|query|string|
|qos|int|
|isResponsePayloadBuffer|Buffer|

### initPost(endpoint,uri,userHandler,query,qos,isResponsePayloadBuffer)

initialize post for end-point

**Kind**:class method
**Return**:bool - isSuccess

|Param|Type|
|------|---|
|endpoint|OCFEndpoint|
|uri|string|
|userHandler|function|
|query|string|
|qos|int|
|isResponsePayloadBuffer|Buffer|

### initPut(self,endpoint,uri,userHandler,query,qos,isResponsePayloadBuffer)

initialize put for end-point

**Kind**:class method
**Return**:bool - isSuccess

|Param|Type|
|------|---|
|endpoint|OCFEndpoint|
|uri|string|
|userHandler|function|
|query|string|
|qos|int|
|isResponsePayloadBuffer|Buffer|

### finishPost()

finish post

**Kind**:class method

### finishPut()

finish put

**Kind**:class method

### post(endpoint, uri, userHandler, query, qos, isResponsePayloadBuffer,requestPayload)

post end-point

**Kind**:class method
**Return**:bool - isSuccess

|Param|Type|
|------|---|
|endpoint|OCFEndpoint|
|uri|string|
|userHandler|function|
|query|string|
|qos|int|
|isResponsePayloadBuffer|Buffer|

### put(endpoint, uri, userHandler, query, qos, isResponsePayloadBuffer,requestPayload)

put end-point

**Kind**:class method
**Return**:bool - isSuccess

|Param|Type|
|------|---|
|endpoint|OCFEndpoint|
|uri|string|
|userHandler|function|
|query|string|
|qos|int|
|isResponsePayloadBuffer|Buffer|

# Class
OCFResource()

**Kind**:Class

##Method

### __init__(device, name, uri, types, interfaceMasks)

initialize OCFResource Element

**Kind**:class method

|Param|Type|Description|
|------|---|---|
|device|object|OCF device that serves this resource|
|name|string|the resource's name|
|uri|string|URI of the resource|
|types|array|the array of resource's types. each element's type is string.|
|interfaceMasks|interfaceMasks|the array of interface masks. each element's type is string.|

### destroyer():

destory OCFResource

**Kind**:class method

### setDiscoverable(isDiscoverable)

set discoverability on OCF Resource

**Kind**:class method

|Param|Type|
|------|---|
|isDiscoverable|bool|

### setPeriodicObservable(periodSec)

set period time (seconds) to observe OCF Resource

**Kind**:class method

|Param|Type|
|------|---|
|periodSec|int|

# Class

OCFDevice()

**Kind**:Class

## Method

### __init__(id, uri, resourceType, name, specVersion, dataModelVersion)

initialize OCF Device

**Kind**:class method

|Param|Type|Description|
|------|---|---|
|id|int|OCF device's ID that is internally managed.|
|uri|string|URI of the OCF device|
|resourceType|string|OCF device's resource type|
|name|string|the OCF device's name|
|specVersion|string|version of the device's spec|
|dataModeVersion|string|version of the device's data model

### getAdapter()

get adapter attached on OCF Device

**Kind**:class method
**Return**:OCFAdapter - OCFAdapter on attached on OCF Device

### createResource(device, name, uri, types, interfaceMasks)

create Resource

**Kind**:class method
**Return**:OCFDevice - OCFDevice named given parameters

|Param|Type|Description|
|------|---|---|
|device|object|OCF device that serves this resource|
|name|string|the resource's name|
|uri|string|URI of the resource|
|types|array|the array of resource's types. each element's type is string.|
|interfaceMasks|interfaceMasks|the array of interface masks. each element's type is string.|