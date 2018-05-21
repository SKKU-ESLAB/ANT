package kr.ac.skku.nyx.selectiveconnection;

import android.Manifest;
import android.content.IntentFilter;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.widget.TextView;

import selective.connection.BTClientAdapter;
import selective.connection.Communicator;
import selective.connection.TCPClientAdapter;
import selective.connection.WFDClientAdapter;


public class MainActivity extends AppCompatActivity implements LogBroadcastReceiver.Callback {
    private MainActivity self = this;
    private Communicator mCommunicator;

    private ReceivingThread mReceivingThread;
    private LogBroadcastReceiver mBroadcastReceiver;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        LogBroadcastSender.setDefaultContext(this);

        this.mBroadcastReceiver = new LogBroadcastReceiver(this);
        IntentFilter broadcastIntentFilter = new IntentFilter();
        broadcastIntentFilter.addAction(LogBroadcastReceiver.kAction);
        this.registerReceiver(this.mBroadcastReceiver, broadcastIntentFilter);

        this.requestPermissions();
        this.initializeCommunication();
    }

    private void initializeCommunication() {
        mCommunicator = Communicator.get_instance();
        TCPClientAdapter tcpClientAdapter = new TCPClientAdapter((short) 2345, "192.168.0.35",
                2345);
        BTClientAdapter btClientAdapter = new BTClientAdapter((short) 3333, "B8:27:EB:77:C3:4A",
                "150e8400-1234-41d4-a716-446655440000");
        WFDClientAdapter wfdClientAdapter = new WFDClientAdapter((short) 3456, 3456, this);

        tcpClientAdapter.set_control_adapter();
        btClientAdapter.set_data_adapter();
        wfdClientAdapter.set_data_adapter();

        this.mReceivingThread = new ReceivingThread();
        this.mReceivingThread.start();
    }

    @Override
    protected void onResume() {
        super.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
    }

    private void requestPermissions() {
        if (ContextCompat.checkSelfPermission(MainActivity.this, Manifest.permission.INTERNET) !=
                PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(MainActivity.this, new String[]{Manifest.permission
                    .INTERNET, Manifest.permission.ACCESS_NETWORK_STATE, Manifest.permission
                    .ACCESS_WIFI_STATE, Manifest.permission.CHANGE_WIFI_STATE, Manifest
                    .permission.CHANGE_NETWORK_STATE, Manifest.permission
                    .ACCESS_COARSE_LOCATION}, 0);
        }
    }

    private class ReceivingThread extends Thread {
        public void run() {
            byte[] buf = new byte[100 * 1024 * 1024];
            String sending_buf = "ACK"; /* Ack Message */

            while (true) {
                int receivedLength = mCommunicator.recv_data(buf, 100 * 1024 * 1024);
                LogBroadcastSender.sendLogMessage(self, "Received: Size=" + receivedLength);
                int sentLength = mCommunicator.send_data(sending_buf.getBytes(), sending_buf
                        .length());
                LogBroadcastSender.sendLogMessage(self, "Sent: Size=" + sentLength);
            }

        }
    }

    @Override
    public void onLogMessage(final String logMessage) {
        final String kTag = "LOG";
        final String kLogMessage = logMessage;
        this.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Log.d(kTag, kLogMessage);
                TextView logTextView = (TextView) findViewById(R.id.logTextView);
                String text = String.valueOf(logTextView.getText());
                logTextView.setText(logMessage + "\n" + text);
                logTextView.invalidate();
            }
        });
    }
}