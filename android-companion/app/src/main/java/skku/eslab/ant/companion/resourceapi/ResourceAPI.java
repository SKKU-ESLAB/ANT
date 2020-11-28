package skku.eslab.ant.companion.resourceapi;

import android.util.Log;

import java.util.HashMap;

import skku.eslab.ant.companion.companionapi.CompanionAPI;
import skku.eslab.ant.companion.companionapi.OnReceiveMessageListener;

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
public class ResourceAPI implements OnReceiveMessageListener {
    private static String TAG = "ResourceAPI";
    private HashMap<String, Resource> mResourceDirectory = new HashMap<>();
    private HashMap<Integer, OnResourceResponseListener> mResponseListeners =
            new HashMap<>();
    private int mRequestId = 0;

    private static ResourceAPI singleton;

    public static ResourceAPI get() {
        if (singleton == null) {
            singleton = new ResourceAPI();
        }
        return singleton;
    }

    private ResourceAPI() {
        CompanionAPI companionAPI = CompanionAPI.get();
        companionAPI.registerOnReceiveMessage(this);
    }

    public boolean registerResource(Resource resource) {
        if (this.mResourceDirectory.get(resource.getUri()) != null) {
            // Already exists
            return false;
        }
        this.mResourceDirectory.put(resource.getUri(), resource);
        return true;
    }

    public void unregisterResource(Resource resource) {
        this.mResourceDirectory.remove(resource.getUri());
    }

    public void sendRequest(String method, String targetUri, String message,
                            OnResourceResponseListener responseListener) {
        int requestId = this.mRequestId;
        ResourceRequest request =
                new ResourceRequest(requestId, method, targetUri, message);
        this.mRequestId++;

        this.mResponseListeners.put(requestId, responseListener);

        CompanionAPI companionAPI = CompanionAPI.get();
        companionAPI.sendMessage(request.toRawMessage());
    }

    public void sendResponse(ResourceRequest request, String message) {
        ResourceResponse response = new ResourceResponse(request, message);

        CompanionAPI companionAPI = CompanionAPI.get();
        companionAPI.sendMessage(response.toRawMessage());
    }

    @Override
    public void onReceiveMessageListener(String rawMessage) {
        int firstLineEnd = rawMessage.indexOf("\n");
        String firstLine = rawMessage.substring(0, firstLineEnd);
        if (firstLine.compareTo("ResourceRequest") != 0 &&
                firstLine.compareTo("ResourceResponse") == 0) {
            return;
        }
        int secondLineEnd = rawMessage.indexOf("\n", firstLineEnd + 1);
        int thirdLineEnd = rawMessage.indexOf("\n", secondLineEnd + 1);
        int fourthLineEnd = rawMessage.indexOf("\n", thirdLineEnd + 1);

        String secondLine =
                rawMessage.substring(firstLineEnd + 1, secondLineEnd);
        String method = rawMessage.substring(secondLineEnd + 1, thirdLineEnd);
        String targetUri =
                rawMessage.substring(thirdLineEnd + 1, fourthLineEnd);
        String message = rawMessage.substring(fourthLineEnd + 1);

        int requestId = Integer.valueOf(secondLine);

        if (firstLine.compareTo("ResourceRequest") == 0) {
            Resource resource = this.mResourceDirectory.get(targetUri);
            if (resource == null) {
                Log.d(TAG, "Ignore incoming request for : " + targetUri);
                return;
            }
            ResourceRequest incomingRequest =
                    new ResourceRequest(requestId, method, targetUri, message);
            switch (method) {
                case "GET":
                    resource.getOnGet().onResourceRequest(incomingRequest);
                    break;
                case "POST":
                    resource.getOnPost().onResourceRequest(incomingRequest);
                    break;
                case "PUT":
                    resource.getOnPut().onResourceRequest(incomingRequest);
                    break;
                case "DELETE":
                    resource.getOnDelete().onResourceRequest(incomingRequest);
                    break;
            }
        } else if (firstLine.compareTo("ResourceResponse") == 0) {
            OnResourceResponseListener responseListener =
                    this.mResponseListeners.get(requestId);
            if (responseListener == null) {
                Log.w(TAG, "Ignore incoming response for: " + targetUri +
                        " / requestId=" + requestId);
                return;
            }

            ResourceResponse incomingResponse =
                    new ResourceResponse(requestId, method, targetUri, message);
            responseListener.onResourceResponse(incomingResponse);
        }
    }
}
