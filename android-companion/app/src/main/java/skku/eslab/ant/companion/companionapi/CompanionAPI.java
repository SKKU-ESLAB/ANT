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

 package skku.eslab.ant.companion.companionapi;

import android.util.Log;

import java.io.IOException;
import java.util.ArrayList;

import skku.eslab.ant.companion.httpconnection.HTTPClient;
import skku.eslab.ant.companion.httpconnection.HTTPServer;
import skku.eslab.ant.companion.httpconnection.HTTPServerListener;

public class CompanionAPI implements HTTPServerListener {
    private HTTPServer mHTTPServer;
    private ArrayList<OnReceiveMessageListener> mListeners = new ArrayList<>();

    private static CompanionAPI singleton;

    public static CompanionAPI get() {
        if (singleton == null) {
            singleton = new CompanionAPI();
        }
        return singleton;
    }

    private CompanionAPI() {
        final int port = 8002;
        this.mHTTPServer = new HTTPServer(port);
        this.mHTTPServer.addListener(this);

        try {
            this.mHTTPServer.start();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void sendMessage(String message) {
        HTTPClient httpClient = HTTPClient.get();
        String url =
                httpClient.getTargetAddress() + "/runtime/currentApp/companion";
        httpClient.sendHTTPRequest(url, "POST", message, null);
    }

    public void registerOnReceiveMessage(OnReceiveMessageListener listener) {
        this.mListeners.add(listener);
    }

    public boolean unregisterOnReceiveMessage(
            OnReceiveMessageListener listener) {
        for (OnReceiveMessageListener item : this.mListeners) {
            if (item == listener) {
                this.mListeners.remove(item);
                return true;
            }
        }
        return false;
    }

    @Override
    public void onReceiveHTTPMessage(String uri, String message) {
        Log.d("test", "receive message: " + uri + " / " + message);
        if (uri.contains("/companion")) {
            for (OnReceiveMessageListener listener : mListeners) {
                listener.onReceiveMessageListener(message);
            }
        }
    }
}