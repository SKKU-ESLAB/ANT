package kr.ac.skku.nyx.selectiveconnection;

import android.Manifest;
import android.content.pm.PackageManager;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;


import selective.connection.*;



public class MainActivity extends AppCompatActivity {
    private Communicator cm;
    private BTClientAdapter ca;
    private BTClientAdapter na3;
    private BTClientAdapter na31;
    private BTClientAdapter na32;

    private TCPClientAdapter na;
    private TCPClientAdapter na4;
    private TCPClientAdapter na5;
    private WFDClientAdapter na2;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        requestPermissions();

        cm = Communicator.get_instance();

        ca = new BTClientAdapter((short)1234, "00:15:83:CE:4D:1F","150e8400-e29b-41d4-a716-446655440000");
        na = new TCPClientAdapter((short)2345, "192.168.0.40", 2345);
        na4 = new TCPClientAdapter((short)5555,"192.168.0.40", 5555);
        na5 = new TCPClientAdapter((short)6666,"192.168.0.40", 6666);
        na2 = new WFDClientAdapter((short)3456, 3456, this);
        na3 = new BTClientAdapter((short)3333, "00:15:83:CE:4D:1F", "150e8400-1234-41d4-a716-446655440000");
        na31 = new BTClientAdapter((short)3334, "00:15:83:CE:4D:1F", "6be60100-3629-11e7-a919-92ebcb67fe33");
        na32 = new BTClientAdapter((short)3335, "00:15:83:CE:4D:1F", "6be60470-3629-11e7-a919-92ebcb67fe33");

        //ca.set_control_adapter();
        na.set_control_adapter();

        na3.set_data_adapter();
        //na31.set_data_adapter();
        //na32.set_data_adapter();

        //na.set_data_adapter();
        //na4.set_data_adapter();
        //na5.set_data_adapter();

        na2.set_data_adapter();

        new Thread() {
            private String tag = "Recved";
            public void run() {
                // byte[] buf = new byte[8192];
                byte[] buf = new byte[100*1024*1024];
                String sending_buf = "hihihii123213114adskfjklfsaj;fklsjflkejwakfjwkljaewlajwlkj12";

                while (true) {
                    int res = cm.recv_data(buf, 100*1024*1024);
                    Log.d(tag, Integer.toString(res) + "data received");
                    int res2 = cm.send_data(sending_buf.getBytes(), 60);
                    //Log.d(tag, "data sent");
                }

            }
        }.start();
    }

    @Override
    protected void onResume() {
        super.onResume();

        //na2.onResume(this);
    }

    @Override
    protected void onPause() {
        super.onPause();

        //na2.onPause(this);
    }

    private void requestPermissions() {
        if (ContextCompat.checkSelfPermission(MainActivity.this, Manifest.permission.INTERNET) != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(MainActivity.this, new String[] {Manifest.permission.INTERNET, Manifest.permission.ACCESS_NETWORK_STATE, Manifest.permission.ACCESS_WIFI_STATE, Manifest.permission.CHANGE_WIFI_STATE, Manifest.permission.CHANGE_NETWORK_STATE}, 0);
        }
    }

}
