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
import native
import logging
from datetime import datetime
import json

try:
    import antstream
except ImportError:
    raise Exception('Gateway API Dependency Error: not found Stream API')
try:
    import antruntime
except ImportError:
    raise Exception('Gateway API Dependency Error: not found Runtime API')

logger = logging.getLogger()
logger.setLevel(logging.INFO)
formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')

def shapeArrayToStr(shapeArray):
    shapeStr = ''
    isMultipleElements = isinstance(shapeArray[0],list)

    def elementArrayToStr(elementArray):
        elementStr = ''
        for i in range(len(elementArray)) :
            if i==0 :
                elementStr = elementStr + elementArray[i]
            else:
                elementStr = elementStr + ':' + elementArray[i]

        return elementStr
    
    if isMultipleElements :
        if len(shapeArray) ==0 :
            return None
        for i in range(len(shapeArray)):
            if i > 0 :
                shapeStr += ','
            shapeStr += elementArrayToStr(shapeArray[i])

        else:
            shapeStr += elementArrayToStr(shapeArray)
        
    return shapeStr


def typeArrayToStr(typeArray):
    typeStr = ''
    isMultipleElements = isinstance(typeArray[0],list)

    if(isMultipleElements):
        if (len(typeArray)==0):
            return None
        for i in range(len(typeArray)):
            if i > 0 :
                typeStr += ','
            typeStr += typeArray

        else:
            typeStr += typeArray
        
    return typeStr


nameArrayToStr = typeArrayToStr

def getAntRootDir():
    antRootDir = antruntime.getEnv('ANT_ROOT')
    if len(antRootDir) ==0 :
        return None
    
    return antRootDir

def getTaskPath(taskName):
    antRootDir = getAntRootDir()
    if (antRootDir == None):
        logger.info(f'ERROR: ANT_ROOT is not defined!')
        return None

    mlRootDir = str(antRootDir) + '/ml/'
    taskPath = str(mlRootDir) + 'mlelement_' + str(taskName) + '.py'
    if (os.path.isdir(taskPath)):

        logger.info('ERROR: cannot find file! ' + taskPath)
        return None

    return taskPath

class ANTML():
    def createImgClsImagenetElement(self,modelPath):
        mlElement = self.createMLElement(
        modelPath,
        [3, 224, 224, 1],
        'uint8',
        'input0',
        [1000, 1, 1, 1],
        'float32',
        'classes',
        'imgcls_imagenet')

        return mlElement

    def createObjDetCocoElement(self,modelPath,resolution,maxBoundingBoxes):
        mlElement = self.createMLElement(
        modelPath,
        [3, resolution, resolution, 1],
        'uint8',
        'data',
        [
            [1, 1, 1, 1],
            [10, 1, 1, 1],
            [10, 1, 1, 1],
            [4, 10, 1, 1]
        ],
        ['float32', 'float32', 'float32', 'float32'],
        ['num_objects', 'classes', 'scores', 'bboxes'],
        'objdet_coco'
        )
        return mlElement
        
    def createMLElement(
        self,
        modelPath,
        inputShapes,
        inputTypes,
        inputNames,
        outputShapes,
        outputTypes,
        outputNames,
        taskName):
        if modelPath.indexOf(' ') >=0 :
            logger.info(f'ERROR: Invalid modelPath! ' + str(modelPath))
            return None
        if inputTypes.indexOf(' ') >= 0 :
            logger.info(f'ERROR: Invalid inputTypes! ' + str(inputTypes))
            return None      
        if outputTypes.indexOf(' ') >= 0 :
            logger.info(f'ERROR: Invalid outputTypes! ' + str(outputTypes))
            return None
        if (antstream.isInitialized >= 0)== False:
            logger.info(f'ERROR: Invalid outputTypes! ' + str(outputTypes))
            return None
        
        if (taskName == None):
            taskName = 'imgcls_imagenet'

        taskPath = getTaskPath(taskName)
        if (taskPath == None):
            logger.info(f'ERROR: task does not exist: ' + str(taskPath))
            return None

        inputShapesStr = shapeArrayToStr(inputShapes)
        outputShapesStr = shapeArrayToStr(outputShapes)
        inputTypesStr = typeArrayToStr(inputTypes)
        outputTypesStr = typeArrayToStr(outputTypes)
        inputNamesStr = nameArrayToStr(inputNames)
        outputNamesStr = nameArrayToStr(outputNames)

        tensorFilter = antstream.createElement('tensor_filter')
        tensorFilter.setProperty('framework', 'python3')
        tensorFilter.setProperty('model', taskPath)
        tensorFilter.setProperty('input', inputShapesStr)
        tensorFilter.setProperty('inputtype', inputTypesStr)
        tensorFilter.setProperty('inputname', inputNamesStr)
        tensorFilter.setProperty('output', outputShapesStr)
        tensorFilter.setProperty('outputtype', outputTypesStr)
        tensorFilter.setProperty('outputname', outputNamesStr)

        custom = modelPath + ' ' + inputShapesStr + ' ' + inputTypesStr + ' ' + outputShapesStr + ' ' +outputTypesStr + ' ' + inputNamesStr + ' ' + outputNamesStr

        tensorFilter.setProperty('custom', custom)
        tensorFilter.modelPath = modelPath
        return tensorFilter

    def createMLFragmentElement(self,  modelPath, inputShapes,inputTypes,inputNames, taskName, numFragments,targetUri):
        if (modelPath.indexOf(' ') >= 0) :
            logger.info(f'ERROR: Invalid modelPath! ' + str(modelPath))
            return None
        if (inputTypes.indexOf(' ') >= 0) :
            logger.info('ERROR: Invalid inputTypes! ' + str(inputTypes))
            return None
        if (antstream.isInitialized()):
            logger.info('ERROR: Stream API is not initialized ')
            return None
        if (taskName == None) :
            # Default task: imgcls_imagenet
            taskName = 'imgcls_imagenet'

        taskPath = getTaskPath(taskName)
        if (taskPath == None):
            logger.info('ERROR: task does not exist: '+ str(taskPath))
            return None

        inputShapesStr = shapeArrayToStr(inputShapes)
        inputTypesStr = typeArrayToStr(inputTypes)
        inputNamesStr = nameArrayToStr(inputNames)

        tensorFilter = antstream.createElement('tensor_filter')
        tensorFilter.setProperty('framework', 'python3')
        tensorFilter.setProperty('model', taskPath)
        tensorFilter.setProperty('input', inputShapesStr)
        tensorFilter.setProperty('inputtype', inputTypesStr)
        tensorFilter.setProperty('inputname', inputNamesStr)
        tensorFilter.setProperty('output', '1:1:1:1')
        tensorFilter.setProperty('outputtype', 'int32')
        
        custom =modelPath + ' ' + inputShapesStr + ' ' + inputTypesStr + ' ' + inputNamesStr + ' ' + numFragments + ' ' + targetUri

        tensorFilter.setProperty('custom', custom)
        tensorFilter.modelPath = modelPath
        return tensorFilter

    def toFloatArray(self,buffer):
        return native.ant_ml_toFloatArray(buffer)

    def getMaxOfBuffer(self,buffer,type):
        return native.ant_ml_toFloatArray(buffer.type)

    def downloadModel(self,modelUrl, overwriteIfExists):
        if (overwriteIfExists == None):
            overwriteIfExists = False
        antRootDir = getAntRootDir()
        
        if (antRootDir == None) :
            logger.info('ERROR: task does not exist: ')
            return None
        
        fileName = modelUrl.substring(
            modelUrl.lastIndexOf('/') + 1,
            modelUrl.length)
        
        modelPath = fileName.substring(0, fileName.lastIndexOf('.'))
        modelRootDir = antRootDir + '/ml/'
        modelArchivePath = modelRootDir + fileName
        modelDirectoryPath = modelRootDir + modelPath

        logger.info('Download model from '+
                    str(modelUrl) +
                    '\n => ' +
                    str(modelArchivePath) +
                    '\n => ' +
                    str(modelDirectoryPath))
        #  Make model root directory if it does not exist
        if (os.path.isdir(modelRootDir)==False):
            os.makedirs(modelRootDir)

        # Check if the model archive file
        isDownload = True
        if (os.path.isfile(modelArchivePath)) :
            logger.info('Model archive file already exists. (' + modelArchivePath + ')')
        
            if (overwriteIfExists==False) :
                logger.info('Skip downloading the model archive file...')
                isDownload = False

        # Download model archive file
        if (isDownload):
            logger.info('Model download from ' + modelUrl + ' to ' + modelArchivePath)
            resultDownload = antruntime.downloadFileViaHTTP(modelUrl,modelArchivePath)
            
        if (resultDownload==False) :
            logger.info('Model download from ' + modelUrl + ' to ' + modelArchivePath)
            return None
        

        # Check if the model directory
        isArchive = True
        if (os.path.isfile(modelDirectoryPath)) :
            logger.info('Model directory already exists. (' + modelDirectoryPath + ')')

        if (os.path.isfile(overwriteIfExists) ):
            logger.info('Skip unarchiving the model archive file...')
            isArchive = False
            os.mkdir(modelDirectoryPath)


        #  Unarchive model archive file
        if (isArchive) :
            resultArchive = antruntime.unarchive(
            modelArchivePath,
            modelDirectoryPath
            )
        if (False==resultArchive) :
            logger.info('ERROR: archiving the model archive file failed')
            return None
        
        return modelDirectoryPath

        