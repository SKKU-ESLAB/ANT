package skku.eslab.ant.companion;

public interface HTTPResponseHandler {
    void onHTTPResponse(int code, String message);
}