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

package skku.eslab.ant.companion.httpconnection;

import java.io.IOException;
import java.io.InputStream;
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
public class HTTPServer extends NanoHTTPD {
    private ArrayList<HTTPServerListener> mListeners = new ArrayList<>();

    public HTTPServer(int port) {
        super(port);
    }

    public void addListener(HTTPServerListener listener) {
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
                    InputStream is = session.getInputStream();
                    is.read(buffer, 0, contentLength);
                    is.close();
                    String message = new String(buffer);
                    for (HTTPServerListener listener : this.mListeners) {
                        listener.onReceiveHTTPMessage(session.getUri(), message);
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
