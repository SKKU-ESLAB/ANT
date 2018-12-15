package com.ant.ant_manager.view.remoteui;

import android.util.Log;

import com.koushikdutta.async.http.body.AsyncHttpRequestBody;
import com.koushikdutta.async.http.server.AsyncHttpServer;
import com.koushikdutta.async.http.server.AsyncHttpServerRequest;
import com.koushikdutta.async.http.server.AsyncHttpServerResponse;
import com.koushikdutta.async.http.server.HttpServerRequestCallback;

/* Copyright (c) 2017-2018 SKKU ESLAB, and contributors. All rights reserved.
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
interface RESTBasedRemoteUIListener {
    void onRESTRemoteUIMessage(String message);
}

public class RESTBasedRemoteUI {
    private static final String TAG = "RESTBasedRemoteUI";

    private RESTBasedRemoteUIListener mListener = null;

    public void openServer(RESTBasedRemoteUIListener listener) {
        AsyncHttpServer server = new AsyncHttpServer();
        this.mListener = listener;

        server.post("/", new HttpServerRequestCallback() {
            @Override
            public void onRequest(AsyncHttpServerRequest request, AsyncHttpServerResponse
                    response) {
                AsyncHttpRequestBody<String> body = request.getBody();
                Log.d(TAG, "REST Remote UI Message: " + body.get());

                mListener.onRESTRemoteUIMessage(body.get());

                response.send("Remote UI Updated");
            }
        });

        server.listen(5000);
    }
}