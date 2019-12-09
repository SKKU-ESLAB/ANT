package skku.eslab.ant.companion.resourceapi;

/* Copyright (c) 2019, contributors. All rights reserved.
 *
 * Contributor: Gyeonghwan Hong <redcarrottt@gmail.com>
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
public class ResourceResponse {
    private int mRequestId;
    private String mMethod;
    private String mTargetUri;
    private String mMessage;

    ResourceResponse(int requestId, String method, String targetUri,
                            String message) {
        this.mRequestId = requestId;
        this.mMethod = method;
        this.mTargetUri = targetUri;
        this.mMessage = message;
    }

    public ResourceResponse(ResourceRequest request, String message) {
        this.mRequestId = request.getRequestId();
        this.mMethod = request.getMethod();
        this.mTargetUri = request.getTargetUri();
        this.mMessage = message;
    }

    public int getRequestId() {
        return mRequestId;
    }

    public void setRequestId(int requestId) {
        mRequestId = requestId;
    }

    public String getMethod() {
        return mMethod;
    }

    public void setMethod(String method) {
        mMethod = method;
    }

    public String getTargetUri() {
        return mTargetUri;
    }

    public void setTargetUri(String targetUri) {
        mTargetUri = targetUri;
    }

    public String getMessage() {
        return mMessage;
    }

    public void setMessage(String message) {
        mMessage = message;
    }

    public String toRawMessage() {
        return "ResourceResponse\n" + this.mRequestId + "\n" + this.mMethod +
                "\n" + this.mTargetUri + "\n" + mMessage;
    }
}
