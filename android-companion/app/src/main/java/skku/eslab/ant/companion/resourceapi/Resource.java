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
public class Resource {
    public Resource(String uri) {
        this.mUri = uri;
        this.mOnGet = null;
        this.mOnPost = null;
        this.mOnPut = null;
        this.mOnDelete = null;
    }

    public String getUri() {
        return mUri;
    }

    public void setUri(String uri) {
        mUri = uri;
    }

    public OnResourceRequestListener getOnGet() {
        return mOnGet;
    }

    public void setOnGet(OnResourceRequestListener onGet) {
        mOnGet = onGet;
    }

    public OnResourceRequestListener getOnPost() {
        return mOnPost;
    }

    public void setOnPost(OnResourceRequestListener onPost) {
        mOnPost = onPost;
    }

    public OnResourceRequestListener getOnPut() {
        return mOnPut;
    }

    public void setOnPut(OnResourceRequestListener onPut) {
        mOnPut = onPut;
    }

    public OnResourceRequestListener getOnDelete() {
        return mOnDelete;
    }

    public void setOnDelete(OnResourceRequestListener onDelete) {
        mOnDelete = onDelete;
    }

    private String mUri;
    private OnResourceRequestListener mOnGet;
    private OnResourceRequestListener mOnPost;
    private OnResourceRequestListener mOnPut;
    private OnResourceRequestListener mOnDelete;

}
