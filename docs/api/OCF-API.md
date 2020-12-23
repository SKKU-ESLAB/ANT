## Functions

<dl>
<dt><a href="#OCF">OCF()</a></dt>
<dd><p>ANT OCF API</p>
</dd>
<dt><a href="#OCFAdapter">OCFAdapter()</a></dt>
<dd><p>OCF Adapter</p>
</dd>
<dt><a href="#OCFDevice">OCFDevice(id, uri, resourceType, name, specVersion, dataModelVersion)</a></dt>
<dd><p>OCF Device</p>
</dd>
<dt><a href="#OCFResource">OCFResource(device, name, uri, types, interfaceMasks)</a></dt>
<dd><p>OCF Resource</p>
</dd>
</dl>

<a name="OCF"></a>

## OCF()
ANT OCF API

**Kind**: global function  
<a name="OCFAdapter"></a>

## OCFAdapter()
OCF Adapter

**Kind**: global function  

* [OCFAdapter()](#OCFAdapter)
    * [.setPlatform(mfgName)](#OCFAdapter+setPlatform)
    * [.getPlatform()](#OCFAdapter+getPlatform) ⇒ <code>string</code>
    * [.addDevice(uri, resourceType, name, specVersion, dataModelVersion)](#OCFAdapter+addDevice)
    * [.getDevices()](#OCFAdapter+getDevices) ⇒ [<code>Array.&lt;OCFDevice&gt;</code>](#OCFDevice)
    * [.getDevice(i)](#OCFAdapter+getDevice) ⇒ [<code>OCFDevice</code>](#OCFDevice)
    * [.onPrepareEventLoop(handler)](#OCFAdapter+onPrepareEventLoop)
    * [.onPrepareClient(handler)](#OCFAdapter+onPrepareClient)
    * [.onPrepareServer(handler)](#OCFAdapter+onPrepareServer)
    * [.start()](#OCFAdapter+start)
    * [.stop()](#OCFAdapter+stop)
    * [.addResource(resource)](#OCFAdapter+addResource)
    * [.deleteResource(resource)](#OCFAdapter+deleteResource)
    * [.getResources()](#OCFAdapter+getResources) ⇒ [<code>OCFResource</code>](#OCFResource)
    * [.repStartRootObject()](#OCFAdapter+repStartRootObject)
    * [.repSet(key, value)](#OCFAdapter+repSet)
    * [.repEndRootObject()](#OCFAdapter+repEndRootObject)
    * [.sendResponse(ocfRequest, statusCode)](#OCFAdapter+sendResponse)
    * [.stopDiscovery()](#OCFAdapter+stopDiscovery) ⇒ <code>Boolean</code>
    * [.isDiscovering()](#OCFAdapter+isDiscovering) ⇒ <code>Boolean</code>
    * [.discovery(resourceType, discoveryHandler)](#OCFAdapter+discovery) ⇒ <code>Boolean</code>
    * [.discoveryAll(discoveryHandler)](#OCFAdapter+discoveryAll) ⇒ <code>Boolean</code>
    * [.observe(endpoint, uri, userHandler, query, qos)](#OCFAdapter+observe) ⇒ <code>Boolean</code>
    * [.stopObserve(endpoint, uri)](#OCFAdapter+stopObserve) ⇒ <code>Boolean</code>
    * [.get(endpoint, uri, userHandler, query, qos)](#OCFAdapter+get) ⇒ <code>Boolean</code>
    * [.delete(endpoint, uri, userHandler, query, qos)](#OCFAdapter+delete) ⇒ <code>Boolean</code>
    * [.initPost(endpoint, uri, userHandler, query, qos)](#OCFAdapter+initPost) ⇒ <code>Boolean</code>
    * [.initPut(endpoint, uri, userHandler, query, qos)](#OCFAdapter+initPut) ⇒ <code>Boolean</code>
    * [.post()](#OCFAdapter+post) ⇒ <code>Boolean</code>
    * [.put()](#OCFAdapter+put) ⇒ <code>Boolean</code>

<a name="OCFAdapter+setPlatform"></a>

### ocfAdapter.setPlatform(mfgName)
OCFAdapter.setPlatform

**Kind**: instance method of [<code>OCFAdapter</code>](#OCFAdapter)  

| Param | Type | Description |
| --- | --- | --- |
| mfgName | <code>String</code> | Manufacturer's name |

<a name="OCFAdapter+getPlatform"></a>

### ocfAdapter.getPlatform() ⇒ <code>string</code>
OCFAdapter.getPlatform

**Kind**: instance method of [<code>OCFAdapter</code>](#OCFAdapter)  
**Returns**: <code>string</code> - platformName  
<a name="OCFAdapter+addDevice"></a>

### ocfAdapter.addDevice(uri, resourceType, name, specVersion, dataModelVersion)
OCFAdapter.addDevice

**Kind**: instance method of [<code>OCFAdapter</code>](#OCFAdapter)  

| Param | Type | Description |
| --- | --- | --- |
| uri | <code>String</code> | id of device to be serviced |
| resourceType | <code>String</code> | resource type of device to be serviced |
| name | <code>String</code> | name of device to be serviced |
| specVersion | <code>String</code> | specification of device to be serviced |
| dataModelVersion | <code>String</code> | data model version of device to be serviced Set the device to be serviced by the OCFAdapter. One OCFAdapter can service multiple devices. |

<a name="OCFAdapter+getDevices"></a>

### ocfAdapter.getDevices() ⇒ [<code>Array.&lt;OCFDevice&gt;</code>](#OCFDevice)
OCFAdapter.getDevices

**Kind**: instance method of [<code>OCFAdapter</code>](#OCFAdapter)  
**Returns**: [<code>Array.&lt;OCFDevice&gt;</code>](#OCFDevice) - devices  
<a name="OCFAdapter+getDevice"></a>

### ocfAdapter.getDevice(i) ⇒ [<code>OCFDevice</code>](#OCFDevice)
OCFAdapter.getDevice

**Kind**: instance method of [<code>OCFAdapter</code>](#OCFAdapter)  
**Returns**: [<code>OCFDevice</code>](#OCFDevice) - device  

| Param | Type |
| --- | --- |
| i | <code>Integer</code> | 

<a name="OCFAdapter+onPrepareEventLoop"></a>

### ocfAdapter.onPrepareEventLoop(handler)
OCFAdapter.prototype.onPrepareEventLoop

**Kind**: instance method of [<code>OCFAdapter</code>](#OCFAdapter)  

| Param | Type |
| --- | --- |
| handler | <code>function</code> | 

<a name="OCFAdapter+onPrepareClient"></a>

### ocfAdapter.onPrepareClient(handler)
OCFAdapter.onPrepareClient

**Kind**: instance method of [<code>OCFAdapter</code>](#OCFAdapter)  

| Param | Type | Description |
| --- | --- | --- |
| handler | <code>function</code> | called when all preparations for executing client functions in the OCF thread are complete |

<a name="OCFAdapter+onPrepareServer"></a>

### ocfAdapter.onPrepareServer(handler)
OCFAdapter.onPrepareServer

**Kind**: instance method of [<code>OCFAdapter</code>](#OCFAdapter)  

| Param | Type | Description |
| --- | --- | --- |
| handler | <code>function</code> | called when all preparations for executing server functions in OCF thread are complete |

<a name="OCFAdapter+start"></a>

### ocfAdapter.start()
OCFAdapter.startRun the OCF thread. This function must be called after OCFAdapter.initialize() is called.You can use the OCF Server API and OCF Client API only while the OCF thread is running.If you need to know exactly when you can use OCF Server API and OCF Client API,you can use the handlers of OCFAdapter.onPrepareServer() and OCFAdapter.onPrepareClient().

**Kind**: instance method of [<code>OCFAdapter</code>](#OCFAdapter)  
<a name="OCFAdapter+stop"></a>

### ocfAdapter.stop()
OCFAdapter.stopStop the OCF thread.

**Kind**: instance method of [<code>OCFAdapter</code>](#OCFAdapter)  
<a name="OCFAdapter+addResource"></a>

### ocfAdapter.addResource(resource)
OCFAdapter.addResource

**Kind**: instance method of [<code>OCFAdapter</code>](#OCFAdapter)  

| Param | Type |
| --- | --- |
| resource | [<code>OCFResource</code>](#OCFResource) | 

<a name="OCFAdapter+deleteResource"></a>

### ocfAdapter.deleteResource(resource)
OCFAdapter.deleteResource

**Kind**: instance method of [<code>OCFAdapter</code>](#OCFAdapter)  

| Param | Type |
| --- | --- |
| resource | [<code>OCFResource</code>](#OCFResource) | 

<a name="OCFAdapter+getResources"></a>

### ocfAdapter.getResources() ⇒ [<code>OCFResource</code>](#OCFResource)
OCFAdapter.getResources

**Kind**: instance method of [<code>OCFAdapter</code>](#OCFAdapter)  
**Returns**: [<code>OCFResource</code>](#OCFResource) - resource  
<a name="OCFAdapter+repStartRootObject"></a>

### ocfAdapter.repStartRootObject()
OCFAdapter.repStartRootObjectLet the OCF thread start writing the OCRepresentation.

**Kind**: instance method of [<code>OCFAdapter</code>](#OCFAdapter)  
<a name="OCFAdapter+repSet"></a>

### ocfAdapter.repSet(key, value)
OCFAdapter.repSet

**Kind**: instance method of [<code>OCFAdapter</code>](#OCFAdapter)  

| Param | Type | Description |
| --- | --- | --- |
| key | <code>String</code> |  |
| value | <code>Boolean</code> \| <code>Number</code> \| <code>String</code> | The value is stored in a specific key among OCRepresentations being created by OCF thread. In this function, various types of data including Boolean, Number, and String can be used as value. |

<a name="OCFAdapter+repEndRootObject"></a>

### ocfAdapter.repEndRootObject()
OCFAdapter.repEndRootObjectFinish writing OCRepresentation of OCF thread.

**Kind**: instance method of [<code>OCFAdapter</code>](#OCFAdapter)  
<a name="OCFAdapter+sendResponse"></a>

### ocfAdapter.sendResponse(ocfRequest, statusCode)
OCFAdapter.sendResponse

**Kind**: instance method of [<code>OCFAdapter</code>](#OCFAdapter)  

| Param | Type | Description |
| --- | --- | --- |
| ocfRequest | <code>OCFRequest</code> | target of response |
| statusCode | <code>Number</code> | Response status code value Sends a response with a specific status code to a request from another device. |

<a name="OCFAdapter+stopDiscovery"></a>

### ocfAdapter.stopDiscovery() ⇒ <code>Boolean</code>
OCFAdapter.stopDiscovery

**Kind**: instance method of [<code>OCFAdapter</code>](#OCFAdapter)  
**Returns**: <code>Boolean</code> - isSuccess  
<a name="OCFAdapter+isDiscovering"></a>

### ocfAdapter.isDiscovering() ⇒ <code>Boolean</code>
OCFAdapter.isDiscovering

**Kind**: instance method of [<code>OCFAdapter</code>](#OCFAdapter)  
**Returns**: <code>Boolean</code> - isDiscovering  
<a name="OCFAdapter+discovery"></a>

### ocfAdapter.discovery(resourceType, discoveryHandler) ⇒ <code>Boolean</code>
OCFAdapter.discovery

**Kind**: instance method of [<code>OCFAdapter</code>](#OCFAdapter)  
**Returns**: <code>Boolean</code> - isSuccess  

| Param | Type | Description |
| --- | --- | --- |
| resourceType | <code>String</code> | Type of resource to find on the network |
| discoveryHandler | <code>function</code> | called whenever one OCFResource is discovered |

<a name="OCFAdapter+discoveryAll"></a>

### ocfAdapter.discoveryAll(discoveryHandler) ⇒ <code>Boolean</code>
OCFAdapter.discoveryAll

**Kind**: instance method of [<code>OCFAdapter</code>](#OCFAdapter)  
**Returns**: <code>Boolean</code> - isSuccessSearch all resources regardless of any type on the network.  

| Param | Type | Description |
| --- | --- | --- |
| discoveryHandler | <code>function</code> | Handler function for discovery response |

<a name="OCFAdapter+observe"></a>

### ocfAdapter.observe(endpoint, uri, userHandler, query, qos) ⇒ <code>Boolean</code>
OCFAdapter.observe

**Kind**: instance method of [<code>OCFAdapter</code>](#OCFAdapter)  
**Returns**: <code>Boolean</code> - isSuccess  

| Param | Type |
| --- | --- |
| endpoint | <code>OCFEndpoint</code> | 
| uri | <code>String</code> | 
| userHandler | <code>function</code> | 
| query | <code>String</code> | 
| qos | <code>Integer</code> | 

<a name="OCFAdapter+stopObserve"></a>

### ocfAdapter.stopObserve(endpoint, uri) ⇒ <code>Boolean</code>
OCFAdapter.stopObserve

**Kind**: instance method of [<code>OCFAdapter</code>](#OCFAdapter)  
**Returns**: <code>Boolean</code> - isSuccess  

| Param | Type |
| --- | --- |
| endpoint | <code>OCFEndpoint</code> | 
| uri | <code>String</code> | 

<a name="OCFAdapter+get"></a>

### ocfAdapter.get(endpoint, uri, userHandler, query, qos) ⇒ <code>Boolean</code>
OCFAdapter.get

**Kind**: instance method of [<code>OCFAdapter</code>](#OCFAdapter)  
**Returns**: <code>Boolean</code> - isSuccess  

| Param | Type |
| --- | --- |
| endpoint | <code>OCFEndpoint</code> | 
| uri | <code>String</code> | 
| userHandler | <code>function</code> | 
| query | <code>String</code> | 
| qos | <code>Integer</code> | 

<a name="OCFAdapter+delete"></a>

### ocfAdapter.delete(endpoint, uri, userHandler, query, qos) ⇒ <code>Boolean</code>
OCFAdapter.delete

**Kind**: instance method of [<code>OCFAdapter</code>](#OCFAdapter)  
**Returns**: <code>Boolean</code> - isSuccess  

| Param | Type |
| --- | --- |
| endpoint | <code>OCFEndpoint</code> | 
| uri | <code>String</code> | 
| userHandler | <code>function</code> | 
| query | <code>String</code> | 
| qos | <code>Integer</code> | 

<a name="OCFAdapter+initPost"></a>

### ocfAdapter.initPost(endpoint, uri, userHandler, query, qos) ⇒ <code>Boolean</code>
OCFAdapter.initPost

**Kind**: instance method of [<code>OCFAdapter</code>](#OCFAdapter)  
**Returns**: <code>Boolean</code> - isSuccess  

| Param | Type |
| --- | --- |
| endpoint | <code>OCFEndpoint</code> | 
| uri | <code>String</code> | 
| userHandler | <code>function</code> | 
| query | <code>String</code> | 
| qos | <code>Integer</code> | 

<a name="OCFAdapter+initPut"></a>

### ocfAdapter.initPut(endpoint, uri, userHandler, query, qos) ⇒ <code>Boolean</code>
OCFAdapter.initPut

**Kind**: instance method of [<code>OCFAdapter</code>](#OCFAdapter)  
**Returns**: <code>Boolean</code> - isSuccess  

| Param | Type |
| --- | --- |
| endpoint | <code>OCFEndpoint</code> | 
| uri | <code>String</code> | 
| userHandler | <code>function</code> | 
| query | <code>String</code> | 
| qos | <code>Integer</code> | 

<a name="OCFAdapter+post"></a>

### ocfAdapter.post() ⇒ <code>Boolean</code>
OCFAdapter.post

**Kind**: instance method of [<code>OCFAdapter</code>](#OCFAdapter)  
**Returns**: <code>Boolean</code> - isSuccess  
<a name="OCFAdapter+put"></a>

### ocfAdapter.put() ⇒ <code>Boolean</code>
OCFAdapter.put

**Kind**: instance method of [<code>OCFAdapter</code>](#OCFAdapter)  
**Returns**: <code>Boolean</code> - isSuccess  
<a name="OCFDevice"></a>

## OCFDevice(id, uri, resourceType, name, specVersion, dataModelVersion)
OCF Device

**Kind**: global function  

| Param | Type | Description |
| --- | --- | --- |
| id | <code>int</code> | OCF device's ID that is internally managed. |
| uri | <code>string</code> | URI of the OCF device |
| resourceType | <code>string</code> | OCF device's resource type |
| name | <code>string</code> | the OCF device's name |
| specVersion | <code>string</code> | version of the device's spec |
| dataModelVersion | <code>string</code> | version of the device's data model |

<a name="OCFResource"></a>

## OCFResource(device, name, uri, types, interfaceMasks)
OCF Resource

**Kind**: global function  

| Param | Type | Description |
| --- | --- | --- |
| device | <code>object</code> | OCF device that serves this resource |
| name | <code>string</code> | the resource's name |
| uri | <code>string</code> | URI of the resource |
| types | <code>array</code> | the array of resource's types. each element's type is string. |
| interfaceMasks | <code>interfaceMasks</code> | the array of interface masks. each element's type is string. |


* [OCFResource(device, name, uri, types, interfaceMasks)](#OCFResource)
    * [.destroyer()](#OCFResource+destroyer)
    * [.setDiscoverable(isDiscoverable)](#OCFResource+setDiscoverable)
    * [.setPeriodicObservable(periodSec)](#OCFResource+setPeriodicObservable)
    * [.setHandler(method, handler)](#OCFResource+setHandler)

<a name="OCFResource+destroyer"></a>

### ocfResource.destroyer()
OCFResource.destroyer

**Kind**: instance method of [<code>OCFResource</code>](#OCFResource)  
<a name="OCFResource+setDiscoverable"></a>

### ocfResource.setDiscoverable(isDiscoverable)
OCFResource.setDiscoverable

**Kind**: instance method of [<code>OCFResource</code>](#OCFResource)  

| Param | Type |
| --- | --- |
| isDiscoverable | <code>Boolean</code> | 

<a name="OCFResource+setPeriodicObservable"></a>

### ocfResource.setPeriodicObservable(periodSec)
OCFResource.setPeriodicObservable

**Kind**: instance method of [<code>OCFResource</code>](#OCFResource)  

| Param | Type |
| --- | --- |
| periodSec | <code>Integer</code> | 

<a name="OCFResource+setHandler"></a>

### ocfResource.setHandler(method, handler)
OCFResource.setHandler

**Kind**: instance method of [<code>OCFResource</code>](#OCFResource)  

| Param | Type |
| --- | --- |
| method | <code>Number</code> | 
| handler | <code>function</code> | 

