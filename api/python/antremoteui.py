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

try:
    import antresource as ResourceAPI
    import json 
except ModuleNotFoundError as e:
    raise ModuleNotFoundError(
        'Remote UI API Dependency Error: not found Resource API')


class ANTRemoteUI:
    def __init__(self):
        pass

    def setStreamingViewPipeline(self, pipeline, handler):
        ResourceAPI.requestPost(
            '/remoteui/streamingview/pipeline',
            pipeline,
            handler
        )

    def setStreamingViewLabelText(self, labelText, handler):
        ResourceAPI.requestPost(
            '/remoteui/streamingview/labelText',
            labelText,
            handler
        )

    def setStreamingViewBoundingBoxes(self, boundingBoxes, handler):
        boundingBoxesJSON = json.loads(boundingBoxes)
        ResourceAPI.requestPost(
            '/remoteui/streamingview/boundingBoxes',
            boundingBoxesJSON,
            handler
        )
        
