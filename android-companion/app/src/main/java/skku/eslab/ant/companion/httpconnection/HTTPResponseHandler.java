package skku.eslab.ant.companion.httpconnection;

public interface HTTPResponseHandler {
    void onHTTPResponse(int code, String message);
}