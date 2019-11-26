package skku.eslab.ant.companion.httpconnection;

import android.app.Activity;
import android.os.AsyncTask;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;

public class HTTPConnectionManager {
    private static HTTPConnectionManager singleton;

    public static HTTPConnectionManager get() {
        if (singleton == null) {
            singleton = new HTTPConnectionManager();
        }
        return singleton;
    }

    private HTTPConnectionManager() {
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
//                    Log.d("test", "http request: " + _url + " / " + _method
//                    + " / " + _data);
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

//                    Log.d("test", "response: (" + responseCode + ") " + responseText);
                    if (_responseHandler != null) {
                        mMotherActivity.runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                _responseHandler.onHTTPResponse(responseCode,
                                        responseText);
                            }
                        });
                    }
                } catch (IOException e) {
                    mMotherActivity.runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            _responseHandler.onHTTPResponse(404, "Not Found");
                        }
                    });
                }
            }
        });
    }
}
