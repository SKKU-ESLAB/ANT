package skku.eslab.ant.companion.resourcebench;

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

import skku.eslab.ant.companion.companionapi.CompanionAPI;
import skku.eslab.ant.companion.companionapi.OnReceiveMessageListener;
import skku.eslab.ant.companion.resourceapi.OnResourceRequestListener;
import skku.eslab.ant.companion.resourceapi.Resource;
import skku.eslab.ant.companion.resourceapi.ResourceAPI;
import skku.eslab.ant.companion.resourceapi.ResourceRequest;

public class ResourceBenchHelper implements OnReceiveMessageListener, OnResourceRequestListener {
    private static String TAG = "ResourceBenchHelper";

    private static String kTestRequest = "**ResourceBench**";
    private static String kResourceURI = "/resourcebench";

    private static ResourceBenchHelper singleton;

    public static ResourceBenchHelper get() {
        if (singleton == null) {
            singleton = new ResourceBenchHelper();
        }
        return singleton;
    }

    private ResourceBenchHelper() {
        this.initializeLegacyMode();
        this.initializeResourceMode();
    }

    // Legacy mode
    private void initializeLegacyMode() {
        CompanionAPI companionAPI = CompanionAPI.get();
        companionAPI.registerOnReceiveMessage(this);
    }

    @Override
    public void onReceiveMessageListener(String rawMessage) {
        if (rawMessage.startsWith(kTestRequest)) {
            CompanionAPI companionAPI = CompanionAPI.get();
            String response = rawMessage.substring(kTestRequest.length() + 1);
            companionAPI.sendMessage(response);
        }
    }

    // Resource API mode
    private void initializeResourceMode() {
        ResourceAPI resourceAPI = ResourceAPI.get();
        Resource resource = new Resource(kResourceURI);
        resource.setOnGet(this);
        resourceAPI.registerResource(resource);
    }

    @Override
    public void onResourceRequest(ResourceRequest request) {
        if (request.getTargetUri().compareTo(kResourceURI) == 0) {
            if (request.getMessage().startsWith(kTestRequest)) {
                ResourceAPI resourceAPI = ResourceAPI.get();
                String response = request.getMessage().substring(kTestRequest.length() + 1);
                resourceAPI.sendResponse(request, response);
            }
        }
    }
}
