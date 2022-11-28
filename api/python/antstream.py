import native
import logging
import os.path

logger = logging.getLogger()
logger.setLevel(logging.INFO)
formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')


class antStream:
    def __init__(self,_mIsInitialized=False, pipelines=[]):
        self._mIsInitialized = _mIsInitialized
        self.pipelines = pipelines

    def callDbusMethod(self,message):
        if(self._mIsInitialized==False):
            raise AntError('ERROR: Stream API is not initialized')
            return False
        return native.ant_stream_callDbusMethod(message)
            
    def isInitialized(self):
        return self._mIsInitialized

    def initialize(self):
        self._mIsInitialized=True
        native.ant_stream_initializeStream()

    def finalize(self):
        antStream = self
        def quitMainLoop():
            result=bool(antStream.callDbusMethod('streamapi_quitMainLoop'))
            if result:
                self._mIsInitialized = False
            return result
        for i in range(len(self.pipelines)):
            self.pipelines[i].setState(self.pipelines[i].STATE_NULL)
            self.pipelines[i].unref()
            self.pipelines.splice(i, 1)
        logger.info(f'quitMainLoop()')
        quitMainLoop()
        logger.info(f'end()')

    def createPipeline(self,pipelineName):
        if(self._mIsInitialized == False):
            logger.info(f'ERROR: Stream API is not initialized')
            return None

        elementIndex = int(self.callDbusMethod('streamapi_createPipeline\n' + pipelineName))

        pipeline = Pipeline(pipelineName, elementIndex)
        self.pipelines.append(pipeline)
        return pipeline
    
    def createElement(elementName) :
        if(self._mIsInitialized == False):
            logger.info(f'ERROR: Stream API is not initialized')
            return None
        
        elementIndex = int(self.callDbusMethod('streamapi_createElement\n' + elementName))

        return Element(elementName, elementIndex)

    
class Pipeline():
    def __init__(self,name, elementIndex):
        self._elementIndex = elementIndex
        self.name = name
        self.elements = []

        self.STATE_VOID_PENDING = 0
        self.STATE_NULL = 1
        self.STATE_READY = 2
        self.STATE_PAUSED = 3
        self.STATE_PLAYING = 4

    def binAdd(self,elementOrElements):
        ANTStream = antStream()

        if(ANTStream.isInitialized()==False):
            logger.info(f' ERROR: Stream API is not initialized')
            return False

        if (isinstance(elementOrElements,list)):
            elements = elementOrElements
            for i in range(len(elements)):
                result = self.binAdd(elements[i])
                if (result==False):
                    logger.info('ERROR: Failed to add to bin at ' + i)
                    return False
            return True
        elif (str(type(elementOrElements)) ==  "<class '__main__.Element'>"):
            element = elementOrElements
            self.elements.append(element)
            result = result
        
        else:
            logger.info(f' ERROR: Stream API is not initialized')
            return False

    def setState(self,state):
        ANTStream = antStream()
        if (ANTStream.isInitialized()==False):
            logger.info(f' ERROR: Stream API is not initialized') 
            return False
        if (isinstance(state,int)):
            logger.info(f'ERROR: Invalid state! ' + str(state))
            return False
        result = int(ANTStream.callDbusMethod('pipeline_setState\n' + str(self._elementIndex) + '\n' + str(state)))

        return result

    def linkMany(self,elements) :
        ANTStream = antStream()
        if(ANTStream.isInitialized()==False):
            logger.info(f' ERROR: Stream API is not initialized')
            return False
        prevElement = None

        for i in range(len(elements)):
            if result==False :
                logger.info('ERROR: Failed to link element at ' + str((i - 1)) + ' with one at ' + str(i))
                return False
            
            prevElement = elements[i]

        return True
        
    def unref(self):
        ANTStream = antStream()
        result = bool(ANTStream.callDbusMethod('pipeline_unref\n' + str(self._elementIndex)))
        return result

class Element:
    def __init__(self,name,elementIndex):
        self._elementIndex = elementIndex
        self.name = name
        self.properties = {}
        self.handlers = {}
        self.srcElement = None
        self.sinkElement = None
    def setProperty(self,key,value):
        ANTStream = antStream()
        if(ANTStream.isInitialized()==False):
            logger.info(f' ERROR: Stream API is not initialized')
            return False
        elif (isinstance(key,str) == False):
            logger.info(f' ERROR: Invalid key: ' + str(key))
            return False
        elif ((isinstance(value,str) == False& isinstance(value,int) == False)&isinstance(value,bool) == False):
            logger.info(f' ERROR: Invalid value:  ' + str(value)) 
            return False

        type = 0
        if (isinstance(value,bool)):
            type = 0
        elif (isinstance(value,str)):
            type = 1
        elif (isinstance(value,int)):
            type = 2
        else :
            type = 3

        result = bool(ANTStream.callDbusMethod('element_setProperty\n'+ str(self._elementIndex) +'\n' + str(key)+'\n' + str(type) +'\n' +str(value)))

        self.properties[key] = value
        return result

    def setCapsProperty(self,key,value) :
        ANTStream = antStream()
        if (ANTStream.isInitialized()==False):
            logger.info(f' ERROR: Stream API is not initialized')
            return False
        elif(isinstance(key,str)==False):
            logger.info(f'ERROR: Invalid key: ' + str(key))           
            return False
        elif(isinstance(value,str)==False):
            logger.info(f'ERROR: Invalid key: ' + str(value))           
            return False

        result=bool(ANTStream.callDbusMethod('element_setCapsProperty\n' + self._elementIndex + '\n' + str(key) + '\n' + str(value)))

        self.properties[key] = value

        return result

    def connectSignal(self,detailedSignal, handler):
        ANTStream = antStream()
        if (ANTStream.isInitialized()==False):
            logger.info(f' ERROR: Stream API is not initialized')
            return False        
        elif(isinstance(key,str)==False):
            logger.info(f'ERROR: Invalid detailedSignal: ' + str(key))           
            return False
        elif(self.handlers[detailedSignal] != None):
            logger.info(f'ERROR: Handler already exists for  ' + str(detailedSignal))           
            return False

        result = native.ant_stream_elementConnectSignal(self._elementIndex,detailedSignal,handler)

        if(result):
            self.handlers[detailedSignal] = handler
        
        return result

    def link(self,destElement):
        ANTStream = antStream()
        if (destElement == None | (str(type(destElement)) !=  "<class '__main__.Element'>")):
            logger.info(f'ERROR: Invalid sink element')
            return False
        self.sinkElement = destElement
        destElement.srcElement = self

        result = bool(ANTStream.callDbusMethod('element_link\n'+ str(self._elementIndex) +'\n' + str(destElement._elementIndex)))

        return result
        
