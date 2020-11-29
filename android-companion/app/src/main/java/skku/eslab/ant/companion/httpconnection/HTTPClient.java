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

import android.app.Activity;
import android.os.AsyncTask;
import android.util.Log;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;

public class HTTPClient {
    private static HTTPClient singleton;

    public static HTTPClient get() {
        if (singleton == null) {
            singleton = new HTTPClient();
        }
        return singleton;
    }

    private HTTPClient() {
    }

    private Activity mMotherActivity;

    public void setMotherActivity(Activity motherActivity) {
        this.mMotherActivity = motherActivity;
    }

    private String mTargetAddress = "";

    public void setTargetAddress(String targetAddress) {
        this.mTargetAddress = targetAddress;
    }

    public String getTargetAddress() {
        return "http://" + this.mTargetAddress.toString();
    }

    public void sendHTTPRequest(String url, String method, String data,
                                HTTPResponseHandler responseHandler) {
        if (mMotherActivity == null) {
            return;
        }

        final String _url = url;
        final String _method = method;
        final String _data = data;
        final HTTPResponseHandler _responseHandler = responseHandler;
        AsyncTask.execute(new Runnable() {
            @Override
            public void run() {
                try {
                    Log.d("test", "http request: " + _url + " / " + _method
                            + " / " + _data);
                    // Create URL
                    URL targetUrl = new URL(_url);
                    // Create connection
                    HttpURLConnection conn =
                            (HttpURLConnection) targetUrl.openConnection();
                    conn.setRequestMethod(_method);
                    conn.setUseCaches(false);
                    conn.setReadTimeout(1500);
                    conn.setConnectTimeout(1500);
                    conn.setDoInput(true);
                    if (_data != null) {
                        conn.setDoOutput(true);
                        OutputStream os = conn.getOutputStream();
                        os.write(_data.getBytes());
                        os.flush();
                        os.close();
                    }

                    final int responseCode = conn.getResponseCode();

                    InputStream is = conn.getInputStream();
                    BufferedReader br =
                            new BufferedReader(new InputStreamReader(is));
                    String line;
                    final StringBuffer response = new StringBuffer();
                    while ((line = br.readLine()) != null) {
                        response.append(line);
                    }
                    br.close();
                    final String responseText = response.toString();

                    Log.d("test", "response: (" + responseCode + ") " + responseText);
                    if (_responseHandler != null) {
                        mMotherActivity.runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                Log.d("test", "code: " + responseCode + " / text: " + responseText);
                                _responseHandler.onHTTPResponse(responseCode,
                                        responseText);
                            }
                        });
                    }
                } catch (IOException e) {
                    mMotherActivity.runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            Log.d("test", "code: " + 408 + " / text: " + "Request Timeout");
                            if (_responseHandler != null) {
                                _responseHandler.onHTTPResponse(408, "Request timeout");
                            }
                        }
                    });
                }
            }
        });
    }
}
