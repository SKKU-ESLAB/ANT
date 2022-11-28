'''
 @licence
 Copyright (c) 2017-2020 SKKU ESLAB, and contributors. All rights reserved.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
 limitations under the License.
''' 

import native
import logging
from datetime import datetime
import json

logger = logging.getLogger()
logger.setLevel(logging.INFO)
formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')

def makeRequest(requestId, query, qos, endpoint, uri, userHandler):
  request = {}
  request.id = requestId
  request.query = query
  request.qos = qos
  request.endpoint = endpoint
  request.uri = uri
  request.userHandler = userHandler
  request.timestamp = datetime.datetime.now()
  return request

gOCFAdapterRequestId = 1
gObserveRequestList = []
gGetRequestList = []
gDeleteRequestList = []
gPostRequestList = []
gPutRequestList = []
gOnewayRequestLists = [
  gGetRequestList,
  gDeleteRequestList,
  gPostRequestList,
  gPutRequestList
]


def make_request(request_id, query, qos, endpoint, uri, user_handler):
    request = {}
    request['id'] = request_id
    request['query'] = query
    request['qos'] = qos
    request['endpoint'] = endpoint
    request['uri'] = uri
    request['userHandler'] = user_handler
    request['timestamp'] = datetime.datetime.now()
    return request

def oaObserveResponseHandler(request_id, response):
    return oaResponseHandler(request_id, response, gObserveRequestList, False)

def oaGetResponseHandler(request_id, response):
    return oaResponseHandler(request_id, response, gGetRequestList, True)

def oaDeleteResponseHandler(request_id, response):
    return oaResponseHandler(request_id, response, gDeleteRequestList, True)

def oaPostResponseHandler(request_id, response):
    return oaResponseHandler(request_id, response, gPostRequestList, True)

def oaPutResponseHandler(request_id, response):
    return oaResponseHandler(request_id, response, gPutRequestList, True)

def oaResponseHandler(requestId, response, requestList, isOneway):
    request = None
    for i in range(len(requestList)):
        item = requestList[i]
        if (item != None & item.id == requestId):
            request = item
            break
    if request != None:
        userHandler = request.userHandler
        if userHandler != None :
            userHandler(response)
            if isOneway :
                del requestList[requestId:requestId+1]


responseTimeoutMs = 2000
requestListCleanerFrequencyMS = 2000
requestListCleaner = None

def requestListCleanerFn() :
    now = datetime.now()
    for i in range(len(gOnewayRequestLists)):
        requestList =gOnewayRequestLists[i]
        for j in range(len(requestList)):
            request = requestList[j]
            if(request == None):
                continue
            if now - request.timestamp > responseTimeoutMs:
                del requestList[request:request+1]

def oaStartRequestListCleaner() :
    requestListCleaner =setInterval(requestListCleanerFn,requestListCleanerFrequencyMS)

def oaStopRequestListCleaner():
    clearInterval(requestListCleaner)


 # ANT OCF API's main object.
 # OCF only generates OCF adapter, and the OCF adapter takes charge of
 # the main role of OCF API.
 

class OCF:
    def __init__(self):
    # Interface constants
        self.OC_IF_BASELINE = 1 << 1
        self.OC_IF_LL = 1 << 2
        self.OC_IF_B = 1 << 3
        self.OC_IF_R = 1 << 4
        self.OC_IF_RW = 1 << 5
        self.OC_IF_A = 1 << 6
        self.OC_IF_S = 1 << 7
        self.OC_IF_CREATE = 1 << 8

    #Method constants
        self.OC_GET = 1
        self.OC_POST = 2
        self.OC_PUT = 3
        self.OC_DELETE = 4
    # QoS constants
        self.OC_HIGH_QOS = 0
        self.OC_LOW_QOS = 1
        
    # Status constants
        self.OC_STATUS_OK = 0
        self.OC_STATUS_CREATED = 1
        self.OC_STATUS_CHANGED = 2
        self.OC_STATUS_DELETED = 3
        self.OC_STATUS_NOT_MODIFIED = 4
        self.OC_STATUS_BAD_REQUEST = 5
        self.OC_STATUS_UNAUTHORIZED = 6
        self.OC_STATUS_BAD_OPTION = 7
        self.OC_STATUS_FORBIDDEN = 8
        self.OC_STATUS_NOT_FOUND = 9
        self.OC_STATUS_METHOD_NOT_ALLOWED = 10
        self.OC_STATUS_NOT_ACCEPTABLE = 11
        self.OC_STATUS_REQUEST_ENTITY_TOO_LARGE = 12
        self.OC_STATUS_UNSUPPORTED_MEDIA_TYPE = 13
        self.OC_STATUS_INTERNAL_SERVER_ERROR = 14
        self.OC_STATUS_NOT_IMPLEMENTED = 15
        self.OC_STATUS_BAD_GATEWAY = 16
        self.OC_STATUS_SERVICE_UNAVAILABLE = 17
        self.OC_STATUS_GATEWAY_TIMEOUT = 18
        self.OC_STATUS_PROXYING_NOT_SUPPORTED = 19

        self.sOCFAdapter = None
        self.sOCFResource = None


        
        self.KEY_BUFFER_VALUE
        self.KEY_STRING_VALUE




# OCF Adapter. It manages OCF thread's lifecycle.
 
    class OCFAdapter:
        def __init__(self):
            self._mfgName = ''
            self._devices = []
            self._resources = []
            self._nextDeviceId = 0
            self.KEY_BUFFER_VALUE = 'bufferValue'
            self.KEY_STRING_VALUE = 'stringValue'

        def initialize(self):
            native.ocf_adapter_initialize()

        def deinitialize(self):
            native.ocf_adapter_deinitialize()

        def setPlatform(self, mfgName):
            self._mfgName = mfgName
            native.ocf_adapter_setPlatform(mfgName)

        def getPlatform(self):
            return self._mfgName

        def addDevice(self, uri, resourceType, name, specVersion, dataModelVersion):
            device = OCFDevice(self._nextDeviceId,uri,resourceType,name,specVersion,dataModelVersion)
            self._devices.append(device)
            native.ocf_adapter_addDevice(uri,resourceType,name,specVersion,dataModelVersion)

        def getDevices(self):
            return self._devices

        def getDevice(self,i):
            return self._devices[i]
            
        def onPrepareEventLoop(self,handler):
            native.ocf_adapter_onPrepareEventLoop(handler)

        def onPrepareClient(self,handler):
            native.ocf_adapter_onPrepareClient(handler)

        def onPrepareServer(self,handler):
            native.ocf_adapter_onPrepareServer(handler)

        def start(self):
            oaStartRequestListCleaner()
            native.ocf_adapter_start()

        def stop(self):
            native.ocf_adapter_stop()
            oaStopRequestListCleaner()
        
        def addResource(self,resource):
            self._resources.append(resource)
            native.ocf_adapter_addResource(resource)

        def deleteResource(self,resource):
            resource.destroyer()
            index= self._resources.find(resource)
            self._resources.remove(index)

        def getResources(self):
            return self._resources

        def repStartRootObject(self):
            native.ocf_adapter_repStartRootObject()

        def repSet(self,key,value):
            if isinstance(value,bool) :
                native.ocf_adapter_repSetBoolean(key, value)
            elif isinstance(value,int):
                native.ocf_adapter_repSetInt(key, value)
            elif isinstance(value,float):               
                native.ocf_adapter_repSetDouble(key, value)
            elif isinstance(value,SyntaxError):          
                native.ocf_adapter_repSetString(key, value)
            elif (isinstance(value,object)):          
                native.ocf_adapter_repSetByteArray(key, value)
            else :
                raise TypeError

        def repSetBufferAndString(self,bufferValue,stringValue):
            if (isinstance(bufferValue,object)==False):
                  raise TypeError              
            
            if (isinstance(bufferValue,str)==False):
                  raise TypeError

            native.ocf_adapter_repSetByteArray(self.KEY_BUFFER_VALUE, self.bufferValue)
            self.repSet(self.KEY_STRING_VALUE, stringValue)

            
        def repEndRootObject(self):
            native.ocf_adapter_repEndRootObject()

        def sendResponse(self,ocfRequest,statusCode,responsePayload):
            if responsePayload != None:
                if isinstance(responsePayload,object) == False:
                    file_handler = logging.FileHandler('my.log')
                    logger.info(f'repSetBufferAndString(): Invalid stringValue type'+ str(type(self.bufferValue)))
                self.repStartRootObject()
            for key in responsePayload:
                if responsePayload.hasOwnProperty(key):
                    value = responsePayload[key]
                    self.repSet(key, value)
            self.repEndRootObject()

            native.ocf_adapter_sendResponse(ocfRequest, statusCode)

        def sendResponseBuffer(self, ocfRequest, statusCode, responsePayloadBuffer,responsePayloadString):
            if responsePayloadBuffer == None :
                logger.info(f'Undefined responsePayloadBuffer')
            if responsePayloadString == None :
                logger.info(f'Undefined responsePayloadString')   
            if isinstance(responsePayloadBuffer,object) == False:
                logger.info(f'Invalid responsePayloadBuffer:'+ str(type(responsePayloadBuffer)))                  
            if isinstance(responsePayloadString,str) == False:
                logger.info(f'Invalid responsePayloadString: '+ str(type(responsePayloadString)) + str(type(responsePayloadString)))
            self.repStartRootObject()
            self.repSetBufferAndString(responsePayloadBuffer, responsePayloadString)
            self.repEndRootObject()
            
            native.ocf_adapter_sendResponse(ocfRequest, statusCode)


        def stopDiscovery(self):
            return native.ocf_adapter_stopDiscovery()

        def isDiscovering(self):
            return native.ocf_adapter.isDiscovering()

        def discovery(self,resourceType, discoveryHandler):
            return native.ocf_adapter_discovery(resourceType, discoveryHandler)

        def discoveryAll(self,discoveryHandler) :
            return native.ocf_adapter_discovery(' ', discoveryHandler)

        def observe(self,endpoint,uri,userHandler,query,qos,isResponsePayloadBuffer):
            if query == None:
                query = ''
            if qos == None:
                qos = 0
            if isResponsePayloadBuffer == None:
                isResponsePayloadBuffer = False
            
            gOCFAdapterRequestId= gOCFAdapterRequestId+1
            requestId = gOCFAdapterRequestId
            request = makeRequest(requestId, query, qos, endpoint, uri, userHandler)
          
            result = native.ocf_adapter_observe(request,oaObserveResponseHandler,isResponsePayloadBuffer)
            
            if (result):
                gGetRequestList.append(request)

            return result

        def stopObserve(self,endpoint, uri) :
            requestId = None
            for i in range(len(gObserveRequestList)):
                request = gObserveRequestList[i]
                if request.endpoint == endpoint & request.uri == request.uri:
                    requestId = request.id
                    del gObserveRequestList[i:i+1]
                    break

            if requestId != None:
                result = native.ocf_adapter_stopObserve(requestId, endpoint, uri)
                return result
            else:
                logger.info(f'Error: cannot find observe request for '+ str(uri))
                return False

        def get(self,endpoint,uri,userHandler,query,qos,isResponsePayloadBuffer):
            if query == None:
                query = ''
            if qos == None:
                qos = 0
            if isResponsePayloadBuffer == None:
                isResponsePayloadBuffer = False
        
            gOCFAdapterRequestId= gOCFAdapterRequestId+1
            requestId = gOCFAdapterRequestId
            request = makeRequest(requestId, query, qos, endpoint, uri, userHandler)
          
            result = native.ocf_adapter_get(request,oaGetResponseHandler,isResponsePayloadBuffer)
            
            if (result):
                gGetRequestList.append(request)

            return result

        def delete(self, endpoint, uri, userHandler, query, qos, isResponsePayloadBuffer):
            if (query == None):
                query = ''
            if (qos == None):
                qos = 0
            if isResponsePayloadBuffer == None:
                isResponsePayloadBuffer = False
            
            gOCFAdapterRequestId= gOCFAdapterRequestId+1
            requestId = gOCFAdapterRequestId
            request = makeRequest(requestId, query, qos, endpoint, uri, userHandler)
          
            result = native.ocf_adapter_delete(request,oaDeleteResponseHandler,isResponsePayloadBuffer)
            
            if (result):
                gDeleteRequestList.append(request)

            return result

        
        def initPost(self,endpoint,uri,userHandler,query,qos,isResponsePayloadBuffer):
            if query == None:
                query = ''
            if qos == None:
                qos = 0

            if isResponsePayloadBuffer == None:
                isResponsePayloadBuffer= False
            
            gOCFAdapterRequestId= gOCFAdapterRequestId+1
            requestId = gOCFAdapterRequestId
            request = makeRequest(requestId, query, qos, endpoint, uri, userHandler)
          
            result = native.ocf_adapter_initPost(request,oaPostResponseHandler,isResponsePayloadBuffer)
            
            if (result):
                gDeleteRequestList.append(request)

            return result

        def initPut(self,endpoint,uri,userHandler,query,qos,isResponsePayloadBuffer):
            if query == None:
                query = ''
            if qos == None:
                qos = 0

            if isResponsePayloadBuffer == None:
                isResponsePayloadBuffer= False

            gOCFAdapterRequestId= gOCFAdapterRequestId+1
            requestId = gOCFAdapterRequestId
            request = makeRequest(requestId, query, qos, endpoint, uri, userHandler)
          
            result = native.ocf_adapter_initPut(request,oaPutResponseHandler,isResponsePayloadBuffer)
            
            if (result):
                gDeleteRequestList.append(request)

            return result

        def finishPost(self):
            result = native.ocf_adapter_post()
            return result

        def finishPut(self):
            result = native.ocf_adapter_put()
            return result

        def post(self,endpoint, uri, userHandler, query, qos, isResponsePayloadBuffer,requestPayload):
            if endpoint == None:
                return self.finishPost()

            if ((requestPayload == None) &  isinstance(requestPayload,object)==False) :
                logger.info(f'Invalid request payload: '+ json.stringify(requestPayload))

            isSuccess =self.initPost(endpoint,uri,userHandler,query,qos,isResponsePayloadBuffer)

            if isSuccess :
                if requestPayload != None:
                    self.repStartRootObject()
                    for key in requestPayload:
                        if requestPayload.hasOwnProperty(key):
                            value = requestPayload[key]
                            self.repSet(key, value)

                    self.repEndRootObject()

                isSuccess &= self.finishPost()

            return isSuccess


        def put(self,endpoint,uri,userHandler,query,qos,isResponsePayloadBuffer,requestPayload):

            if endpoint == None:
                return self.finishPut()
            
            if requestPayload == None & isinstance(requestPayload,object)==False :
                 logger.info(f'Invalid request payload: '+ json.stringify(requestPayload))

            isSuccess = self.initPut(endpoint,uri, userHandler, query, qos, isResponsePayloadBuffer)

            if isSuccess :
                if requestPayload != None:
                    self.repStartRootObject()
                    for key in requestPayload :
                        if requestPayload.hasOwnProperty(key):
                            value = requestPayload[key]
                            self.repSet(key, value)

                    self.repEndRootObject()
                 
                isSuccess &= self.finishPut()
            
            return isSuccess




    class OCFResource:
        def __init__(self,device, name, uri, types, interfaceMasks):
            self.device=device
            self.name=name
            self.uri=uri
            self.types=types
            self.interfaceMask = 0
            for i in range(len(interfaceMasks)):
                self.interfaceMask |= interfaceMasks[i]
            self.defaultInterfaceMask = interfaceMasks[0]
            self.isDiscoverable = None
            self.periodSec = None
            self.handlerIDMap = {}
            self.gOCFResourceHandlerId = 0

            native.ocf_resource_constructor(self)

        def destroyer(self):
            handlerIDs=[]
            for i  in range(len(self.handlerIDMap)):
                handlerIDs.put(self.handlerIDMap[i])

            native.ocf_resource_destroyer(self, handlerIDs)

        def setDiscoverable(self,isDiscoverable):
            self.isDiscoverable=isDiscoverable
            native.ocf_resource_setDiscoverable(self, isDiscoverable)

        def setPeriodicObservable(self,periodSec):
            self.periodSec = periodSec
            native.ocf_resource_setPeriodicObservable(self, periodSec)

        def setHandler(self,method,handler):
            handlerId = self.gOCFResourceHandlerId
            self.handlerIDMap[method] = handlerId
            native.ocf_resource_setHandler(self, handlerId, method, handler)

'''
  OCF Device
  @param {int} id OCF device's ID that is internally managed.
  @param {string} uri URI of the OCF device
  @param {string} resourceType OCF device's resource type
  @param {string} name the OCF device's name
  @param {string} specVersion version of the device's spec
  @param {string} dataModelVersion version of the device's data model
             self.gOCFResourceHandlerId=self.gOCFResourceHandlerId+1
'''
class OCFDevice:
    def __init__(self,id, uri, resourceType, name, specVersion, dataModelVersion):
        self.id = id
        self.uri = uri
        self.resourceType = resourceType
        self.name = name
        self.specVersion = specVersion
        self.dataModelVersion = dataModelVersion

    def getAdapter(self):
        if (self.sOCFAdapter == None) :
            self.sOCFAdapter = self.OCFAdapter()
            return self.sOCFAdapter

    def createResource(self, device, name, uri, types, interfaceMasks):
        OCFResource_ins= self.OCFResource(device, name, uri, types, interfaceMasks)
        return OCFResource_ins