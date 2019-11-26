package skku.eslab.ant.companion.companionapi;

import java.io.IOException;
import java.util.ArrayList;

import fi.iki.elonen.NanoHTTPD;


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
public class CompanionServer extends NanoHTTPD {
    private ArrayList<CompanionServerListener> mListeners = new ArrayList<>();

    private static CompanionServer singleton = null;

    public static boolean setSingleton(int port) {
        if (singleton == null) {
            singleton = new CompanionServer(port);
            return true;
        } else {
            return false;
        }
    }

    public static CompanionServer get() {
        return singleton;
    }

    private CompanionServer(int port) {
        super(port);
    }

    public void addListener(CompanionServerListener listener) {
        this.mListeners.add(listener);
    }

    @Override
    public Response serve(IHTTPSession session) {
        String responseText = "Failed";
        // TODO: Filtering uri not starting with "/companion"
        if (session.getMethod() == Method.POST) {
            String contentLengthStr = session.getHeaders().get("content-length");
            if (contentLengthStr != null) {
                int contentLength = Integer.parseInt(contentLengthStr);
                byte[] buffer = new byte[contentLength];
                try {
                    session.getInputStream().read(buffer, 0, contentLength);
                    String message = buffer.toString();
                    for (CompanionServerListener listener : this.mListeners) {
                        listener.onReceiveMessage(session.getUri(), message);
                    }
                    responseText = "Success";
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
        return new NanoHTTPD.Response(responseText);
    }
}