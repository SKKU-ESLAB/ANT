package skku.eslab.ant.companion.remoteuiapi;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import skku.eslab.ant.companion.resourceapi.OnResourceRequestListener;
import skku.eslab.ant.companion.resourceapi.Resource;
import skku.eslab.ant.companion.resourceapi.ResourceAPI;
import skku.eslab.ant.companion.resourceapi.ResourceRequest;

/* Copyright (c) 2017-2020 SKKU ESLAB, and contributors. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
public class RemoteUIAPI {
    private final String kPipelineUri = "/remoteui/streamingview/pipeline";
    private final String kLabelTextUri = "/remoteui/streamingview/labelText";
    private static RemoteUIAPI singleton;
    private MutableLiveData<String> mPipeline;
    private MutableLiveData<String> mLabelText;
    private Resource mPipelineResource;
    private Resource mLabelTextResource;

    public static RemoteUIAPI get() {
        if (singleton == null) {
            singleton = new RemoteUIAPI();
        }
        return singleton;
    }

    private RemoteUIAPI() {
        mPipeline = new MutableLiveData<>();
        mPipeline.setValue("");
        mLabelText = new MutableLiveData<>();
        mLabelText.setValue("");

        this.initializePipelineResource();
        this.initializeLabelTextResource();
    }

    private void initializePipelineResource() {
        this.mPipelineResource = new Resource(kPipelineUri);
        this.mPipelineResource.setOnPost(new OnResourceRequestListener() {
            @Override
            public void onResourceRequest(ResourceRequest request) {
                String pipeline = request.getMessage();
                mPipeline.postValue(pipeline);
                ResourceAPI.get().sendResponse(request, "Success");
            }
        });
        this.mPipelineResource.setOnGet(new OnResourceRequestListener() {
            @Override
            public void onResourceRequest(ResourceRequest request) {
                String pipeline = mPipeline.getValue();
                ResourceAPI.get().sendResponse(request, pipeline);
            }
        });
        ResourceAPI.get().registerResource(this.mPipelineResource);
    }

    private void initializeLabelTextResource() {
        this.mLabelTextResource = new Resource(kLabelTextUri);
        this.mLabelTextResource.setOnPost(new OnResourceRequestListener() {
            @Override
            public void onResourceRequest(ResourceRequest request) {
                String labelText = request.getMessage();
                mLabelText.postValue(labelText);
                ResourceAPI.get().sendResponse(request, "Success");
            }
        });
        this.mLabelTextResource.setOnGet(new OnResourceRequestListener() {
            @Override
            public void onResourceRequest(ResourceRequest request) {
                String labelText = mLabelText.getValue();
                ResourceAPI.get().sendResponse(request, labelText);
            }
        });
        ResourceAPI.get().registerResource(this.mLabelTextResource);
    }

    public LiveData<String> getPipeline() {
        return this.mPipeline;
    }

    public MutableLiveData<String> getLabelText() {
        return this.mLabelText;
    }
}
