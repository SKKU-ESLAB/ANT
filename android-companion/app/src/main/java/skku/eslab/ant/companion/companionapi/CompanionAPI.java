package skku.eslab.ant.companion.companionapi;

import android.util.Log;

import java.util.ArrayList;

import skku.eslab.ant.companion.httpconnection.HTTPConnectionManager;
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
    }

    public void sendMessage(String message) {
        HTTPConnectionManager httpConnectionManager = HTTPConnectionManager.get();
        String url = httpConnectionManager.getTargetAddress() + "/runtime/currentApp/companion";
        httpConnectionManager.sendHTTPRequest(url, "POST", message, null);
    }

    public void setOnReceiveMessage(OnReceiveMessageListener listener) {
        this.mListeners.add(listener);
    }

    @Override
    public void onReceiveHTTPMessage(String uri, String message) {
        Log.d("test", "receive message: " + uri + " / " + message);
        if (uri.equals("/companion")) {
            for (OnReceiveMessageListener listener : mListeners) {
                listener.onReceiveMessageListener(message);
            }
        }
    }
}