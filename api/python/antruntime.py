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

logger = logging.getLogger()
logger.setLevel(logging.INFO)
formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')


RESULT_SUCCESS = 'Success'
RESULT_FAILED = 'Failed'

class ANTRuntime():
    def __init__(self,_mCurrentApp=None):
        self._mCurrentApp=_mCurrentApp

    def _removeCurrentApp(self):
        self._mCurrentApp = None
    
    def setCurrentApp(self,onInitialize, onStart, onStop):
        if(onInitialize == None | onStart == None | onStop == None):
            return RESULT_FAILED
        self._mCurrentApp = App(onInitialize, onStart, onStop)
        return RESULT_SUCCESS

    def getCurrentApp(self):
        return self._mCurrentApp

    def getPssInKB(self):
        return native.ant_runtime_getPssInKB()
    
    def getEnv(self,envKey):
        return native.ant_runtime_getEnv(envKey)
    
    def downloadFileViaHTTP(self,url, downloadPath):
        return native.ant_runtime_downloadFileViaHTTP(url, downloadPath)

    def unarchive(self,archiveFilePath, targetDirPath):
        if os.path.exists(archiveFilePath):
            logger.info(f'Error: cannot find archive file! ' + str(archiveFilePath))
            return False
        if os.path.exists(targetDirPath) :
            logger.info(f'Error: cannot find target directory! ' + str(targetDirPath))
            return False
        
        return native.ant_runtime_unarchive(archiveFilePath, targetDirPath)
    
    def disableStdoutBuffering(self):
        return native.ant_runtime_disableStdoutBuffering()


class App :
    def __init__(self,onInitialize,onStart,onStop):
        self.state = self.STATE.IDLE
        self.onInitialize = onInitialize
        self.onStart = onStart
        self.onStop = onStop

        self.onInitialize()
        self.STATE = {}
        self.STATE.IDLE=0
        self.STATE.RUNNING = 1

    def start(self):
        if self.state == self.STATE.RUNNING:
            return RESULT_FAILED
        
        self.state = self.STATE.RUNNING
        self.onStart()
        return RESULT_SUCCESS

    def stop(self):
        if self.state == self.STATE.IDLE:
            return RESULT_FAILED
        self.state=self.STATE.IDLE
        self.onStop()
        return RESULT_SUCCESS
    
    def getState(self):
        if self.state == self.STATE.IDLE:
            return 'Idle'
        elif self.state == self.STATE.RUNNING:
            return 'Running'      
        else:
            return 'Unknown'     

    def getInfo(self):
        appInfo =self.getState()
        return appInfo        


