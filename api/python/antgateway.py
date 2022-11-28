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

import os.path
import logging
from datetime import datetime
import json
import antml
import antocf
import threading
import native

logger = logging.getLogger()
logger.setLevel(logging.INFO)
formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')

gDeviceUri = '/vs/d'
gDeviceType = 'ant.d.virtualsensor'
gDeviceName = 'VirtualSensorDevice'
gDeviceSpec = 'ant.1.0.0'
gDeviceDataModel = 'ant.res.1.0.0'

# OCF Resource Types of Virtual Sensor Resources
gVSUriRoot = 'ant.r.vs'
gVSUriMask = 'ant.r.vs.'
gVSInletUri = 'ant.r.vs.inlet'
gVSOutletUri = 'ant.r.vs.outlet'
gVSSettingUri = 'ant.r.vs.setting'

# OCF Resource Types of Gateway Manager Resources 
gGWUriRoot = 'ant.r.gw'
gGWVSMUri = 'ant.r.gw.vsm'

def getVSInletUri(name):
    return '/vs/' + name + '/inlet'

def getVSOutletUri(name):
    return '/vs/' + name + '/outlet'

def getVSSettingUri(name) :
  return '/vs/' + name + '/setting'


gGWVSMUri = '/gw/vsm'
gVSAdapter = None

def setInterval(func, sec):
    def func_wrapper():
        setInterval(func, sec)
        func()
    t = threading.Timer(sec, func_wrapper)
    t.start()
    return t


'''
/**
 *
 * In ANT framework, gateway provides sensor virtualization and
 * DNN partitioning. Sensor virtualization means unifying interfaces to access
 * physical sensors, soft sensors, local sensors, and remote sensors.
 * DNN partitioning means executing a DNN on multiple heterogeneous devices.
 * In real IoT systems, there are heterogeneous devices including gateway,
 * but the single device among them cannot provide sufficient computational
 * resources to DNNs. Therefore, DNN partitioning is required.
 *
 * There are two ways to implement gateway in ANT framework.
 * 1. ML fragment element (Based on gstreamer)
 * 2. Virtual sensor (Based on OCF standard)
 *
 * ML fragment element is an approach to extend the gstreamer to DNN
 * partitioning. In this way, to associate a DNN with a physical sensor, the
 * app developer must implement their own sensor sampling element that can be
 * compatible to gstreamer.
 *
 * Actually, many IoT devices does not use gstreamer-based approach, but use
 * OCF standard. Therefore, we additionally provide virtual sensor, an approach
 * based on OCF standard.
 *
 * Virtual sensor is an abstract layer that provide unified interface of
 * physical sensors, rule-based soft sensors, and DNN-based soft sensors
 * (a.k.a. deep sensors). It also supports DNN partitioning on the deep
 * sensors. To support that, ANT additionally provides DNN fragment engine
 * and gateway manager.
 */
 '''

class ANTGateway():
    
    def createImgClsImagenetElement(self,  modelPath, numFragments, targetUri):
        mlFragmentElement = antml.createMLFragmentElement(modelPath,[3, 224, 224, 1],'uint8','input','gateway_imgcls_imagenet',numFragments,targetUri)
     
        return mlFragmentElement
    
    def createDFE(self,modelName, numFragments) :
        if ( isinstance(modelName,str) == False) :
            logger.info('Invalid modelName: ' + str(modelName))
        if ( isinstance(numFragments,int) ):
            logger.info('Invalid numFragments:  ' + str(numFragments))

        dfe = self.DFE(modelName, numFragments)
        dfe.load()
        return dfe

    def getVSAdapter(self) :
        if gVSAdapter == None:
            gVSAdapter = VirtualSensorAdapter()
        
        return gVSAdapter

    


'''
/*
 * Virtual sensor (Based on OCF standard)
 *
 * There are many modules related to virtual sensor.
 *
 * - VirtualSensorAdapter : module to arbitrate user and OCF thread and to
 *                          organize virtual sensor system.
 *   - VirtualSensor : module to service virtual sensor through OCF resources
 *                     and several JavaScript handlers.
 *   - DFE (DNN Fragment Engine) : module to perform partitioned DNN inference
 *                                 (as now, it uses Tensorflow Lite.)
 *   - GatewayManager : module to manage virtual sensors and DNN partitioning.
 *                      This module must run on gateway device.
 *     - DFEScheduler : module to manage DNN partitioning. It monitors
 *                        the current status of DFEs running on several
 *                        devices and make commands to control DFE's
 *                        partitioning points.
 *     - VirtualSensorManager : module to manage virtual sensors. It discovers
 *                              virtual sensors via OCF discovery message and
 *                              associate the virtual sensors to form a sensor
 *                              data pipeline for the IoT applications.
 */

/**
 * Create a virtual sensor adapter. It is the second way to perform
 * gateway-centric DNN partitioning. It internally uses OCF standard.
 * @returns {Object} virtual sensor adapter object
 */

'''


class VirtualSensorAdapter():
    def __init__(self):
        self.mOCFAdapter = self.OCFAPI.getAdapter()
        self.mVirtualSensors = []
        self.mResources = []
        self.mGatewayManager = None
        self.mGatewayClient = None
        self.mOCFAdapter.onPrepareEventLoop(self.onPrepareOCFEventLoop)
        self.mOCFAdapter.onPrepareClient(self.onPrepareOCFClient)
        self.mOCFAdapter.onPrepareServer(self.onPrepareOCFServer)



    def onPrepareOCFEventLoop(self) :
        self.mOCFAdapter.setPlatform('ant')
        self.mOCFAdapter.addDevice(
            gDeviceUri,
            gDeviceType,
            gDeviceName,
            gDeviceSpec,
            gDeviceDataModel
            )

    def onPrepareOCFClient(self) :
        gwManager = self.getGWManager()
        if (gwManager != None) :
            gwManager.onOCFClientPrepared(self)
            
        gwClient = self.getGWClient()
        if (gwClient != None) :
            gwClient.onOCFClientPrepared(self)

    def onPrepareOCFServer(self):
        self.clearResources()
    # Add OCF resources of all the virtual sensors
        for i  in range(len(self.mVirtualSensors)):
            virtualSensor = self.mVirtualSensors[i]
            resources = virtualSensor.setup(self)
            for j in range(resources.length):
                self.addResource(resources[j])

    #Add OCF resources of gateway manager
        gwManager = self.getGWManager()
        if (gwManager != None) :
            resources = gwManager.setup(self)
            for i in range(len(resources)) :
                self.addResource(resources[j])
    def start(self):
        self.mOCFAdapter.start()
    def stop(self):
        self.mOCFAdapter.stop()
        self.mOCFAdapter.deinitialize()

    def createSensor(self,sensorName,sensorType,deviceType,observerHandler,generatorHandler,settingHandler):
        if (sensorName == None |  isinstance(sensorName,str) == False) :
            logger.info('VSA createSensor error: Invalid sensorName' + str(sensorName))
        if (sensorType == None | isinstance(sensorType,str) == False) :
            logger.info('VSA createSensor error: Invalid sensorType' + str(sensorType))
        if (deviceType == None | isinstance(sensorType,str) == False) :
            logger.info('VSA createSensor error: Invalid deviceType' + str(deviceType))
        if (observerHandler != None):
            logger.info('VSA observer error: Invalid observerHandler')
        if(generatorHandler == None):
            logger.info('VSA observer error: Invalid generatorHandler')
        if (settingHandler != None):
            logger.info('VSA observer error: Invalid settingHandler')

        virtualSensor = self.VirtualSensor(sensorName,sensorType, deviceType, observerHandler, generatorHandler,settingHandler)

        self.mVirtualSensors.push(virtualSensor)
        return virtualSensor

    def createDeepSensor(self, sensorName, sensorType, deviceType, modelName, numFragments):
        #TODO(RedCarrottt): add pre-processing handler, post-processing handler
        
        #Check arguments
        if (modelName == None |  isinstance(modelName,str)) :
            logger.info('VSA createDeepSensor error: Invalid modelName'+str(modelName))
        if (numFragments == None | isinstance(modelName,int)) :
            logger.info('VSA createDeepSensor error: Invalid numFragments'+str(numFragments))
            dfe = self.getANTGateway().createDFE(modelName, numFragments)
            hObserver = dfe.getObserverHandler()
            hGenerator = dfe.getGeneratorHandler()
            hSetting = dfe.getSettingHandler()
            return self.createSensor(
                sensorName,
                sensorType,
                deviceType,
                hObserver,
                hGenerator,
                hSetting)
    def createGWManager(self) :
        if (self.mGatewayManager == None) :
            self.mGatewayManager = self.GatewayManager()
        return self.getGWManager()

    def createGWClient(self,onPrepared) :
        if (self.mGatewayClient == None) :
            self.mGatewayClient = self.GatewayClient()
            gwClient = self.getGWClient()
            gwClient.setOnPrepared(onPrepared)
            return gwClient
    
    def getANTGateway(self) :
        return self.mANTGateway
    
    def getOCFAdapter(self) :
        return self.mOCFAdapter

    def getGWManager(self) :
        return self.mGatewayManager

    def getGWClient(self):
        return self.mGatewayClient

    def findSensorByName(self,name):
        for  i in range(len(self.mVirtualSensors)) :
            virtualSensor = self.mVirtualSensors[i]
            if (virtualSensor.getName() == name):
                return virtualSensor
    
        return None
        
    def findSensorByUri(self, uri):
        # offset of the tail of '/vs/'
        nameFrom = uri.indexOf('/', 1) + 1
        # offset of the tail of virtual sensor name
        nameLength = uri.indexOf('/', nameFrom) - nameFrom
        if (nameFrom < 0 | nameLength < 0):
            return None
        name = uri.substring(nameFrom, nameLength)
        if (name == None | name.length == 0):
            return None
        return self.findSensorByName(name)

    def getResource(self, uri) :
        for  i in range(len(self.mResources.length)):
            resource = self.mResources[i]
            if (resource.uri() == uri) :
                return resource
        return None

    def clearResources(self) :
        self.mResources = []

    def addResource(self, resource):
        if(resource != None):
            self.mResources.append(resource)
    
    class VirtualSensor():
        def __init__(self, name, sensorType, deviceType, observerHandler, generatorHandler, settingHandler) :
            self.mName = name
            self.mSensorType = sensorType
            self.mDeviceType = deviceType
            self.mObserverHandler = observerHandler
            self.mGeneratorHandler = generatorHandler
            self.mSettingHandler = settingHandler
            self.mInletResource = None
            self.mOutletResource = None
            self.mSettingResource = None
            self.mObservers = []
            self.mDFE = None
        
        def setup(self,vsAdapter) :
            oa = vsAdapter.getOCFAdapter()
            resources = []
            if (self.getObserverHandler() != None) :
                rInlet = self.setupInlet(oa)
                resources.append(rInlet)
            if (self.getGeneratorHandler() != None) :
                rOutlet = self.setupOutlet(oa)
                resources.push(rOutlet)
            if (self.getSettingHandler() != None) :
                rSetting = self.setupSetting(oa)
                resources.append(rSetting)
        
            return resources

        def setupInlet(self,oa) :
            device = oa.getDevice(0)
            uri = getVSInletUri(self.mName)
            self.mInletResource = self.OCFAPI.createResource(device,self.mName, uri, [gVSUriRoot, gVSInletUri, self.mDeviceType, self.mSensorType],[self.OCFAPI.OC_IF_RW])
            self.mInletResource.setDiscoverable(True)
            self.mInletResource.setPeriodicObservable(1)
            self.mInletResource.setHandler(self.OCFAPI.OC_POST, self.onPostInlet)
            self.mInletResource.setHandler(self.OCFAPI.OC_GET, self.onGetInlet)
            oa.addResource(self.mInletResource)
            return self.mInletResource
        
        def setupSetting(self,oa) :
            device = oa.getDevice(0)
            uri = getVSSettingUri(self.mName)
            self.mSettingResource = self.OCFAPI.createResource(
                device,
                self.mName,
                uri,
                [gVSUriRoot, gVSSettingUri, self.mDeviceType, self.mSensorType],
                [self.OCFAPI.OC_IF_RW])
            self.mSettingResource.setDiscoverable(True)
            self.mSettingResource.setHandler(self.OCFAPI.OC_POST, self.onPostSetting)
            oa.addResource(self.mSettingResource)
            return self.mSettingResource

        def addObserver(self, sensorType, deviceType, endpoint, uri, intervalMS) :
            oa = gVSAdapter.mOCFAdapter
            def onIncomingInletData(response) :
                inputData = {}
                if (response.payload != None) :
                    payloadObj = json.parse(response.payload)
                    inputData.jsObject = payloadObj.jsObject
                else :
                    inputData.jsObject = json.parse(response.payloadString)
                    inputData.buffer = response.payloadBuffer
                if (inputData.jsObject == None) :
                    logger.info('inlet error: jsObject is not defined')
                    return
                
                self.mObserverHandler(inputData)

            # Start observer
            intervalDesc = setInterval(oa.get(endpoint, uri, onIncomingInletData, None, None, True), intervalMS)

            # Add observer to the virtual sensor's observer list
            observer = {
            sensorType: sensorType,
            deviceType: deviceType,
            intervalDesc: intervalDesc}

            self.mObservers.append(observer)

        def removeObserver(self,sensorType, deviceType):
            # Find observer for the given sensor/device type
            foundObserver = None
            foundObserverIndex = -1
            for  i in range(len(self.mObservers)):
                observer = self.mObservers[i]
                if (observer.sensorType == sensorType & observer.deviceType == deviceType):
                    foundObserver = observer
                    foundObserverIndex = i
                    break

            # Stop observer
    
            if (foundObserver != None & foundObserverIndex >= 0) :
                self.clearInterval(foundObserver.intervalDesc)
                del self.mObservers[foundObserverIndex:foundObserverIndex+1]

        #/* Getters/setters */
        def getName(self):
            return self.mName
        
        def getUri(self):
            return '/vs/' + self.mName
        
        def getInletResource(self):
            return self.mInletResource
        
        def getOutletResource(self):
            return self.mOutletResource
        
        def getSettingResource(self):
            return self.mSettingResource

        def getObserverHandler(self):
            return self.mObserverHandler

        def getGeneratorHandler(self):
            return self.mGeneratorHandler

        def getSettingHandler(self):
            return self.mSettingHandler

        def getObservers(self) :
            return self.mObservers

def onPostInlet(virtualSensor,request):
    #POST inlet: Add observer

    
    #Parse OCF request
    requestPayloadString = request.payload_string
    requestPayload = json.parse(requestPayloadString)
    #command's type (add or remove)
    commandType = requestPayload.commandType
    #sensor's type (OCF resource type)
    sensorType = requestPayload.sensorType
    #device's type (OCF resource type)
    deviceType = requestPayload.deviceType
    #observation interval (milliseconds)
    intervalMS = requestPayload.intervalMS
    #default observation interval: 1 sec
    defaultIntervalMS = 1000


    #Check OCF request
    response = {}
    if (commandType == None | isinstance(commandType,str) == False) :
        response[str('result:')+'Failure'] ='Invalid commandType (' + str(commandType) + ')'

    if (sensorType == None | isinstance(sensorType,str) == False) :
        response[str('result:')+'Failure'] ='Invalid sensorType (' + str(sensorType) + ')'   

    if (deviceType == None | isinstance(deviceType,str) == False) :
        response[str('result:')+'Failure'] ='Invalid deviceType (' + str(deviceType) + ')'    
    
    if (sensorType == deviceType) :
        response[str('result:')+'Failure'] ='Invalid deviceType (' + str(sensorType) + ')'

    if (intervalMS == None) :
        intervalMS = defaultIntervalMS
    elif (isinstance(intervalMS,str) == False):
        response[str('result:')+'Failure'] ='Invalid intervalMS (' + str(intervalMS) + ')'

    if (commandType != 'add' & commandType != 'remove') :
        response[str('result:')+'Failure'] ='Invalid commandType (' + str(commandType) + ')'

    if (response.result != 'Failure') :
        response = onPostInletInternal(
            virtualSensor,
            commandType,
            sensorType,
            deviceType,
            intervalMS)
    #Send response
    oa = gVSAdapter.mOCFAdapter
    responsePayload = response
    oa.sendResponse(request, antocf.OC_STATUS_OK, responsePayload)

def onPostInletInternal(virtualSensor,commandType, sensorType, deviceType, intervalMS) :

    oa = gVSAdapter.mOCFAdapter
    
    def onDiscoveryAfterPostInlet(endpoint, uri, types, interfaceMask) :

        isFoundSensorType = False
        isFoundDeviceType = False
        for i in range(len(types)):
            if(types[i] == sensorType):
                isFoundSensorType = True
            elif (types[i] == deviceType):
                isFoundDeviceType = True
        if (isFoundSensorType &isFoundDeviceType):
            if (commandType == 'add'):
                virtualSensor.addObserver(
                    sensorType,
                    deviceType,
                    endpoint,
                    uri,
                    intervalMS)
            elif (commandType == 'remove'):
                virtualSensor.removeObserver(sensorType, deviceType)

    oa.discovery(gVSOutletUri, onDiscoveryAfterPostInlet)
    return {'result:'+ 'Success', 'reason:'+ 'None'}
 

def onGetInlet(request):
    virtualSensor = gVSAdapter.findSensorByUri(request.dest_uri)
    
    observers = virtualSensor.getObservers()
    
    oa = gVSAdapter.mOCFAdapter
    responsePayload = {
        str('name:')+ virtualSensor.getName(),
        str('sensorType:')+ virtualSensor.getSensorType(),
        str('deviceType:')+virtualSensor.getDeviceType(),
        str('observersJSON:')+ json.stringify(observers)
    }
    oa.sendResponse(request, antocf.OC_STATUS_OK, responsePayload)


def onGetOutlet(request):
    # GET outlet: Get DFE message
    virtualSensor = gVSAdapter.findSensorByUri(request.dest_uri)

     # Call custom generator handler
    result = virtualSensor.mGeneratorHandler()

    # Check resul
    oa = gVSAdapter.mOCFAdapter
    jsonObject = json.stringify(result.jsObject)
    buffer = result.buffer
    oa.sendResponseBuffer(request, antocf.OC_STATUS_OK, buffer, jsonObject)


def onPostSetting(request):
    virtualSensor = gVSAdapter.findSensorByUri(request.dest_uri)

    requestPayloadString = request.payload_string
    setting = json.parse(requestPayloadString)

#Call custom setting handler
    result = virtualSensor.mSettingHandler(setting)
    resultJSONString = json.stringify(result)

#Send response
    oa = gVSAdapter.mOCFAdapter
    responsePayload = {
      result: resultJSONString
    }
    oa.sendResponse(request, antocf.OC_STATUS_OK, responsePayload)


class DFE():
    def __init__(self,modelName, numFragments):
        self.modelName = modelName
        self.numFragments = numFragments
        self.interpreters = None
        self.recentInputData = None
        self.presentFragNum = numFragments - 1
        self.averageLatencyMS = 0.0

    def updateAverageLatency(self,latency):
        kMomentum= 0.9
        self.averageLatencyMS=self.selfaverageLatencyMS * kMomentum + latency * (1 - kMomentum)

    def loadAndPreprocessImag(self,imgPath):
        return native.ant_gateway_dfeLoadAndPreprocessImage(imgPath)

    def load(self):
        self.interpreters = native.ant_gateway_dfeLoad(
        self.modelName,
        self.numFragments
        )
    def execute(self,inputBuffer, startLayerNum, endLayerNum):
        if (isinstance(startLayerNum,int) ==False):
            logger.info('Invalid startLayerNum' + str(startLayerNum))
        if (isinstance(endLayerNum,int) == False) :
            logger.info('Invalid endLayerNum' + str(endLayerNum))
        
        return native.ant_gateway_dfeExecute(
            self.interpreters,
            inputBuffer,
            startLayerNum,
            endLayerNum)
    def getObserverHandler(self,inputData):
        def dfeObserverHandler(inputData):
             #TODO: custom DFE observer handler
                self.recentInputData = inputData
        return dfeObserverHandler

    def getGeneratorHandler(self) :
        def dfeGeneratorHandler():
            inputData=self.recentInputData
            buffer=inputData.buffer
            startTimeMS = datetime.now()
            self.execute(buffer, type(inputData),self.numFragments)
            endTimeMS = datetime.now()

            latencyMS = endTimeMS - startTimeMS

            self.updateAverageLatency(latencyMS)

        return dfeGeneratorHandler

    def getLoad (self):
        loadavg = os.path.listdir('/proc/loadavg')
        avgLoad1min =float(loadavg.slice[0:loadavg.indexOf(' ')])
        return avgLoad1min

    def getSettingHandler (self,fragNum):
        def dfeSettingHandler(setting):
            self.presentFragNum = setting.fragNum

            latencyMS = self.averageLatencyMS
            load = self.getLoad()
            result = {latencyMS: latencyMS, load: load}
            return result
    
        return dfeSettingHandler


class GatewayManager ():
    def __init__(self):
        self.mVirtualSensorManager = VirtualSensorManager()
        self.mDFEScheduler = DFEScheduler()

    def getVSManager(self):
        return self.mVirtualSensorManager

    def getDFEScheduler(self):
        return self.mDFEScheduler

    def setup(self,vsManager):
        resources =[]
        oa = vsManager.getOCFAdapter()

        vsManager = self.getVSManager()
        rVSManager = vsManager.setup(oa)
        resources.append(rVSManager)

        return resources
    
    def onOCFClientPrepared(self,vsAdapter):
        vsManager = self.getVSManager()
        if (vsManager != None) :
            vsManager.startDiscovery(vsAdapter)

        dfeScheduler = self.getDFEScheduler()
        if (dfeScheduler != None) :
            dfeScheduler.start()

class VirtualSensorManager():
    def __init__(self,mANTGateway,mResource = None,mInletList=None,mOutletList=None,mSettingList=None):
        self.mANTGateway = mANTGateway
        self.mResource = mResource
        self.mInletList = mInletList
        self.mOutletList = mOutletList
        self.mSettingList = mSettingList
    
    
    def findInlet(self, sensorType, deviceType) :
        self.findVSResource(self.mInletList, sensorType, deviceType)

    def findOutlet(self, sensorType, deviceType) :
        self.findVSResource(self.mOutletList, sensorType, deviceType)   

    def findSetting(self, sensorType, deviceType) :
        self.findVSResource(self.mSettingList, sensorType, deviceType)   

    def findVSResource(list,sensorType,deviceType) :
        for i in range(len(list)):
            entry = list[i]
            isSensorTypeFound = False
            isDeviceTypeFound = False
            for j in range(len(entry.types)):
                type = entry.types[j]
                if (type == sensorType) :
                    isSensorTypeFound = True
                if (type == deviceType) :
                    isDeviceTypeFound = True
        if (isSensorTypeFound & isDeviceTypeFound) :
            return entry
        return None

        
    def setup(self,oa):
        device = oa.getDevice(0)
        self.mInletResource = self.OCFAPI.createResource(
        device,
        'vsm',
        gGWVSMUri,
        [gGWUriRoot, gGWVSMUri],
        [self.OCFAPI.OC_IF_RW])
        self.mInletResource.setDiscoverable(True)
        self.mInletResource.setHandler(self.OCFAPI.OC_GET, self.onGetVirtualSensorManager)
        self.mInletResource.setHandler(self.OCFAPI.OC_POST, self.onPostVirtualSensorManager)
        oa.addResource(True.mInletResource)
        return True.mInletResource


    def startDiscovery(self,vsAdapter):
        def onDiscovery(self,endpoint, uri, types, interfaceMask):
            isFound = False
            for i in range(len(types)) :
                if (types[i] == gVSUriRoot) :
                    isFound = True
            if (isFound) :
                self.addResource(endpoint, uri, types)
        
        oa = vsAdapter.getOCFAdapter()
        oa.discoveryAll(onDiscovery)
        

    def onPostVirtualSensorManager(request):
        response = {str('result: ')+'None', str('reason:') +'None'}
        requestPayload = json.parse(request.payload_string)
        intervalMS = requestPayload.intervalMS
        sourceOutletJSON = requestPayload.sourceOutletJSON
        sinkInletJSON = requestPayload.sinkInletJSON

        if (sourceOutletJSON == None) :
            response = {
            str('result: ')+'Failure',
            str('reason:') +'Not found sourceOutlet'
            }
        if (sinkInletJSON == None) :
            response = {
            str('result: ')+ 'Failure',
            str('reason:')+'Not found sinkInlet'
            }
        sourceOutlet = json.parse(sourceOutletJSON)
        sinkInlet = json.parse(sinkInletJSON)


        foundSinkInlet = None
        if (response.result != 'Failure') :
            vsManager = gVSAdapter.getGWManager().getVSManager()
            foundInlets = vsManager.mInletList()
            for i in range(len(foundInlets.length)):
                foundInlet = foundInlets[i]
                types = foundInlet.type
                isFoundSensorType = False
                isFoundDeviceType = False
                for j in range(len(types)) :
                    type = types[j]
                    if (type == sinkInlet.sensorType):
                        isFoundSensorType = True
                    if (type == sinkInlet.deviceType):
                        isFoundDeviceType = True
                    
                if (isFoundSensorType & isFoundDeviceType) :
                    foundSinkInlet = foundInlet
                    break
        if (foundSinkInlet == None) :
            response = {
                str('result:') +'Failure',
                str('reason: ')+'Cannot find inlet resource ' + json.stringify(sinkInlet)}
        if (response.result != 'Failure') :
            response = {
            str('result:') +'Success',
            str('reason:') +'None',
            str('intervalMS:')+ intervalMS,
            str('sourceOutletJSON:')+ sourceOutletJSON,
            str('sinkInletJSON:')+ sinkInletJSON}

        oa = gVSAdapter.mOCFAdapter
        responsePayload = response
        oa.sendResponse(request, antocf.OC_STATUS_OK, responsePayload)

    # Send association command to sink inlet
        if (response.result != 'Failure') :
            def setTimeout() :
                requestPayload = {
                    str('sensorType:')+  sourceOutlet.sensorType,
                    str('deviceType:')+ sourceOutlet.deviceType,
                    str('intervalMS:')+ intervalMS
                }
                res = oa.post(
                    foundSinkInlet.endpoint,
                    foundSinkInlet.uri,
                    None,
                    '',
                    antocf.OC_LOW_QOS,
                    False,
                    requestPayload)
                
                if (res):
                    logger.info(
                        'Failed to post association message: ' +
                        json.stringify(requestPayload)
                    )


    def onGetVirtualSensorManager(self,request):
        inlets = []
        foundInlets = self.mInletList
        for i in range(len(foundInlets)):
            entry = {
                str('types:')+ foundInlets[i].types,
                str('uri:')+ foundInlets[i].uri
            }
            if foundInlets[i].observers != None:
                entry.observers = foundInlets[i].observers
            inlets.append(entry)

        outlets = []
        foundOutlets = self.mOutletList
        for i in range(len(foundOutlets)):
            entry = {
                str('types:')+ foundOutlets[i].types,
                str('uri:')+ foundOutlets[i].uri
            }
            outlets.append(entry)

        settings = []
        foundSettings = self.mSettingList
        for i in range(len(foundSettings)):
            entry = {
                str('types:')+ foundSettings[i].types,
                str('uri:')+ foundSettings[i].uri
            }
            settings.append(entry)

        oa = self.mOCFAdapter
        responsePayload = {
            str('inletsJSON:')+  json.dumps(inlets),
            str('outletsJSON:')+   json.dumps(outlets),
            str('settingsJSON:')+  json.dumps(settings)
        }
        oa.sendResponse(request, antocf.OCF_SUCCESS, responsePayload)

    def add_resource(self, endpoint, uri, types):
        vs_types = []
        let_type = None
        for type in types:
            if type.startswith(gVSUriMask):
                let_type = type
                continue
            elif type == gVSUriRoot:
                continue
        vs_types.append(type)
        found_entry = {'endpoint': endpoint, 'uri': uri, 'types': vs_types}
        if let_type == gVSInletUri:
            self.mInletList.append(found_entry)
        elif let_type == gVSOutletUri:
            self.mOutletList.append(found_entry)
        elif let_type == gVSSettingUri:
            self.mSettingList.append(found_entry)
        else:
            logger.info('Cannot identify resource: ' + str(uri))
            logger.info('\tTypes: ' + str(types))

    def observeInlet(self,inletEntry):
        def _onObserve(response) :
            payload = response.payload
            if (payload == None) :
                logger.info('Undefined response from observing inlet')
                return
            
            sensorType = payload.sensorType
            deviceType = payload.deviceType
            observers = json.parse(payload.observersJSON)
            
            inlet = self.findInlet(sensorType, deviceType)
            inlet.observers = observers
            
            oa = gVSAdapter.mOCFAdapter
            oa.observe(
                inletEntry.endpoint,
                inletEntry.uri,
                _onObserve,
                '',
                antocf.OC_LOW_QOS,
                False)


class DFEScheduler():
    def __init__(self):
        self.mResource = None

class GatewayClient():
    def __init__(self,vsAdapter):
        self.mResource = None
        self.mOnPrepared = None
        self.mFoundVSM = None
        self.mVSAdapter = vsAdapter
        self.kVSDefaultIntervalMS = 100

    def setOnPrepared(self,onPrepared):
        self.mOnPrepared = onPrepared
    
    def onOCFClientPrepared(self,vsAdapter):
        self.mVSAdapter = vsAdapter
        def _onDiscovery(endpoint, uri, types, interfaceMask) :
            for i in range(len(types)): 
                type = types[i]
                if (type == gGWVSMUri) :
                    self.mFoundVSM = {endpoint: endpoint, uri: uri}
        
        oa = self.vsAdapter.getOCFAdapter()
        oa.discovery(gGWUriRoot, self._onDiscovery)

        if (self.mOnPrepared != None) :
            self.mOnPrepared()

    
    def isPrepared(self):
        return self.mOnPrepared != None & self.mVSAdapter != None

    def getVirtualSensors(self,onResult):
        def _onGet(response):
            payloadObj = json.parse(response.payload)
            inlets = json.parse(payloadObj.inlets)
            outlets = json.parse(payloadObj.outlets)
            settings = json.parse(payloadObj.settings)

            if (onResult != None) :
                onResult(inlets, outlets, settings)
        if (self.isPrepared()==False) :
            logger.info( 'getVirtualSensors(): Gateway client not prepared yet')
            
            oa = self.mVSAdapter.getOCFAdapter()
            if (oa == None) :
                logger.info( 'getVirtualSensors(): Cannot get OCF Adapter')
            
            if (self.mFoundVSM != None) :
                oa.get(
                    self.mVSAdapter.endpoint,
                    self.mVSAdapter.uri,
                    _onGet,
                    None,
                    None,
                    False)
                return True
            else :
                # Virtual sensor manager is not found yet
                return False



    def associateVirtualSensors(self,sourceOutlet,sinkInlet,intervalMS,onResult):
        def _onPost(response):
            payloadObj = json.parse(response.payload)
            result = payloadObj.result
            reason = payloadObj.reason

            if (result == None) :
                onResult('Failure', 'Result from virtual sensor is undefined')
            elif (result == 'Failure') :           
                onResult(result, reason)
            elif (result == 'Success') :
                sourceOutlet = json.parse(payloadObj.sourceOutletJSON)
                sinkInlet = json.parse(payloadObj.sinkInletJSON)
                onResult(result, reason, sourceOutlet, sinkInlet)
            else :
                onResult(
                'Failure',
                'Result from virtual sensor is invalid: ' + response.payload)
                
        if (intervalMS == None) :
            intervalMS = self.kVSDefaultIntervalMS


        if (self.isPrepared()==False) :
            logger.info('getVirtualSensors(): Gateway client not prepared yet')
            
        oa = self.mVSAdapter.getOCFAdapter()
        if (oa == None) :
            logger.info( 'getVirtualSensors(): Cannot get OCF Adapter')

        if (self.mFoundVSM != None) :
            requestPayload = {'intervalMS:'+ intervalMS, 'sourceOutletJSON:' + json.stringify(sourceOutlet),'sinkInletJSON:'+ json.stringify(sinkInlet)}

            oa.post(
                self.mVSAdapter.endpoint,
                self.mVSAdapter.uri,
                _onPost,
                None,
                None,
                False,
                requestPayload
            )
            return True

        else:
            return False
