package skku.eslab.ant.companion.gatewayconnector;

import android.app.Activity;
import android.util.Log;

import org.tensorflow.lite.examples.classification.tflite.Classifier;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.util.List;

import skku.eslab.ant.companion.MainActivity;
import skku.eslab.ant.companion.resourceapi.ResourceAPI;

public class GatewayConnector {
    private static final String TAG = "GatewayConnector";

    private MainActivity mMainActivity;
    private Classifier mClassifier;
    private ServerThread mServerThread;

    private final int kPort = 3000;
    private final int kNumFragments = 13;

    public GatewayConnector(MainActivity activity) {
        this.mMainActivity = activity;
        this.initializeClassifier();
        this.initializeSocketServer();
    }

    private void initializeClassifier() {
        try {
            this.mClassifier = Classifier.create(this.mMainActivity,
                    Classifier.Model.OFFLOADING_FLOAT_MOBILENET
                    , Classifier.Device.CPU, 4);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void initializeSocketServer() {
        this.mServerThread = new ServerThread();
        this.mServerThread.start();
    }

    ServerSocket mServerSocket;
    Socket mSocket;
    BufferedInputStream mInputStream;
    BufferedOutputStream mOutputStream;

    long mPrevTimestampMS = 0;
    long mAvgInferenceTimeMS = 0;

    protected byte[] receive(int dataLength) {
        byte[] dataBuffer = new byte[dataLength];
        if (this.mSocket == null || !this.mSocket.isConnected() || this.mInputStream == null) {
            Log.w(TAG, "Receive - Socket closed! 1");
            return null;
        }
        try {
            int receivedBytes = 0;
            while (receivedBytes < dataLength) {
                int onceReceivedBytes = this.mInputStream.read(dataBuffer, receivedBytes,
                        dataLength - receivedBytes);
                if (onceReceivedBytes < 0) {
                    Log.e(TAG, "Receive failed! 1");
                    return null;
                }
                receivedBytes += onceReceivedBytes;
            }
            return dataBuffer;
        } catch (IOException e) {
            Log.w(TAG, "Receive - Socket closed! 2 / " + e.getMessage());
            return null;
        }
    }

    protected int send(byte[] dataBuffer, int dataLength) {
        if (this.mSocket == null || !this.mSocket.isConnected() || this.mOutputStream == null) {
            Log.w(TAG, "Send - Socket closed! 1");
            return -1;
        }
        try {
            this.mOutputStream.write(dataBuffer, 0, dataLength);
            this.mOutputStream.flush();
            return dataLength;
        } catch (IOException e) {
            Log.w(TAG, "Send - Socket closed! 2 / " + e.getMessage());
            return -2;
        }
    }

    private int mNextStartIdx = -1;
    public void setNextStartIdx(int nextStartIdx) {
        this.mNextStartIdx = nextStartIdx;
    }

    class ServerThread extends Thread {
        private void setEnablePartitioningUI(final boolean isEnabled) {
            mMainActivity.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    mMainActivity.enablePartitioningPointUI(isEnabled);
                }
            });
        }
        @Override
        public void run() {
            while (true) {
                try {
                    Log.i(TAG, "Listening connection: port=" + kPort);
                    mServerSocket = new ServerSocket();
                    mServerSocket.setReuseAddress(true);
                    mServerSocket.bind(new InetSocketAddress(kPort));
                    mSocket = mServerSocket.accept();
                    Log.i(TAG, "Connected!: " + mSocket.getInetAddress().getHostAddress());
                    setEnablePartitioningUI(true);
                    mInputStream = new BufferedInputStream(mSocket.getInputStream());
                    mOutputStream = new BufferedOutputStream(mSocket.getOutputStream());
                    while (true) {
                        // Get length
                        byte[] lengthBuffer = receive(4);
                        ByteBuffer lengthByteBuffer = ByteBuffer.wrap(lengthBuffer);
                        int length = lengthByteBuffer.getInt();

                        // Get start idx
                        byte[] startIdxBuffer = receive(4);
                        ByteBuffer startIdxByteBuffer = ByteBuffer.wrap(startIdxBuffer);
                        int startIdx = startIdxByteBuffer.getInt();
                        if(mNextStartIdx < 0) {
                            mNextStartIdx = startIdx;
                            mMainActivity.setPartitioningPoint(mNextStartIdx);
                        }

                        // Get payload
                        byte[] payloadBuffer = receive(length);
                        ByteBuffer payloadByteBuffer = ByteBuffer.wrap(payloadBuffer);

                        List<Classifier.Recognition> recognitionList =
                                mClassifier.recognizeImage(payloadByteBuffer, startIdx,
                                        kNumFragments - 1);
                        final Classifier.Recognition top1 = recognitionList.get(0);

                        // Record speed
                        final float kMovingAvgMomentum = 0.9f;
                        long nowTimestampMS = System.currentTimeMillis();
                        boolean isDisplaySpeed = false;
                        if (mPrevTimestampMS != 0) {
                            long inferenceTime = nowTimestampMS - mPrevTimestampMS;
                            mAvgInferenceTimeMS =
                                    (long) (mAvgInferenceTimeMS * kMovingAvgMomentum + inferenceTime * (1 - kMovingAvgMomentum));
                            isDisplaySpeed = true;
                        }
                        mPrevTimestampMS = nowTimestampMS;

                        String speedText = "";
                        if (isDisplaySpeed) {
                            float fps = (1000.0f) / (float) mAvgInferenceTimeMS;
                            speedText = "" + mAvgInferenceTimeMS + " ms "
                                    + "(" + String.format("%.2f", fps) + "fps)";
                        }

                        // Label
                        final String labelText = "" + top1.getTitle() +
                                " (" + String.format("%.1f", top1.getConfidence() * 100) + "%) " + speedText;

                        setRemoteUILabel(labelText);

                        ByteBuffer resultByteBuffer = ByteBuffer.allocate(4);
                        resultByteBuffer.putInt(mNextStartIdx);
                        send(resultByteBuffer.array(), 4);
                    }
                } catch (IOException e) {
                    Log.e(TAG, "IOException");
//                    mSocket = null;
//                    mInputStream = null;
//                    mOutputStream = null;
                    e.printStackTrace();
                    setRemoteUILabel(" ");
                } catch (NullPointerException e) {
                    Log.e(TAG, "NullPointerException");
                    try {
                        mInputStream.close();
                        mOutputStream.close();
                        mSocket.close();
                    } catch (IOException ioException) {
                        ioException.printStackTrace();
                    }
//                    mSocket = null;
//                    mInputStream = null;
//                    mOutputStream = null;
                    e.printStackTrace();
                    setRemoteUILabel(" ");
                }
                setEnablePartitioningUI(false);
            }
        }

        private void setRemoteUILabel(final String labelText) {
            mMainActivity.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    ResourceAPI resourceAPI = ResourceAPI.get();

                    resourceAPI.sendRequest("POST",
                            "/remoteui/streamingview/labelText",
                            labelText, null);
                }
            });
        }
    }
}