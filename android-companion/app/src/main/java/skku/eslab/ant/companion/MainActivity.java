package skku.eslab.ant.companion;

import android.content.SharedPreferences;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.material.bottomnavigation.BottomNavigationView;

import java.io.IOException;
import java.net.HttpURLConnection;
import java.net.URL;

import androidx.appcompat.app.AppCompatActivity;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.Observer;
import androidx.navigation.NavController;
import androidx.navigation.Navigation;
import androidx.navigation.ui.AppBarConfiguration;
import androidx.navigation.ui.NavigationUI;

public class MainActivity extends AppCompatActivity {
    private final String SP_FILENAME = "ANT";
    private final String SP_TARGET_ADDRESS = "TargetAddress";
    private final String SP_DEFAULT_TARGET_ADDRESS = "192.168.0.33:8001";

    private MutableLiveData<String> mConnectionStatus = new MutableLiveData<>();
    private final String CS_DISCONNECTED = "Disconnected";
    private final String CS_CONNECTED = "Connected";
    // TODO: hardcoded: no CS_CONNECTING, CS_DISCONNECTING

    private MutableLiveData<String> mAppStatus = new MutableLiveData<>();
    private final String AS_IDLE = "Idle";
    private final String AS_LAUNCHING = "Launching";
    private final String AS_RUNNING = "Running";
    private final String AS_TERMINATING = "Terminating";

    @Override
    protected void onStart() {
        super.onStart();

        // Setting to target address edit text
        SharedPreferences sharedPref = getSharedPreferences(SP_FILENAME, MODE_PRIVATE);
        String targetAddress = sharedPref.getString(SP_TARGET_ADDRESS, SP_DEFAULT_TARGET_ADDRESS);
        EditText targetAddressEditText = findViewById(R.id.targetAddressEditText);
        targetAddressEditText.setText(targetAddress);
    }

    @Override
    protected void onStop() {
        super.onStop();

        // Setting from target address edit text
        EditText targetAddressEditText = findViewById(R.id.targetAddressEditText);
        String targetAddress = targetAddressEditText.getText().toString();

        SharedPreferences sharedPref = getSharedPreferences(SP_FILENAME, MODE_PRIVATE);
        SharedPreferences.Editor editor = sharedPref.edit();
        editor.putString(SP_TARGET_ADDRESS, targetAddress);
        editor.commit();
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        BottomNavigationView navView = findViewById(R.id.nav_view);
        // Passing each menu ID as a set of Ids because each
        // menu should be considered as top level destinations.
        AppBarConfiguration appBarConfiguration =
                new AppBarConfiguration.Builder(R.id.navigation_home, R.id.navigation_dashboard,
                        R.id.navigation_notifications).build();
        NavController navController = Navigation.findNavController(this, R.id.nav_host_fragment);
        NavigationUI.setupActionBarWithNavController(this, navController,
                appBarConfiguration);
        NavigationUI.setupWithNavController(navView, navController);

        // Register data observers
        this.mConnectionStatus.observe(this, new Observer<String>() {
            @Override
            public void onChanged(String appStatus) {
                onUpdateConnectionStatus(appStatus);
            }
        });
        this.mAppStatus.observe(this, new Observer<String>() {
            @Override
            public void onChanged(String appStatus) {
                onUpdateAppStatus(appStatus);
            }
        });

        // Set data
        this.mConnectionStatus.setValue(CS_DISCONNECTED);
        this.mAppStatus.setValue(AS_IDLE);

        // Register button listeners
        Button appStartStopButton = findViewById(R.id.appStartStopButton);
        appStartStopButton.setOnClickListener(onClickAppStartStopButton);

        // Monitor thread
        Thread monitorThread = new Thread() {
            @Override
            public void run() {
                while (true) {
                    checkConnectionStatus();
                    checkAppStatus();
                    try {
                        Thread.sleep(1000);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
        };
        monitorThread.start();
    }

    private View.OnClickListener onClickAppStartStopButton = new View.OnClickListener() {
        @Override
        public void onClick(View view) {
            if (!mConnectionStatus.getValue().equals(CS_CONNECTED)) {
                return;
            }

            switch (mAppStatus.getValue()) {
                case AS_IDLE:
                    startApp();
                    break;
                case AS_RUNNING:
                    stopApp();
                    break;
            }
        }
    };

    private void onUpdateConnectionStatus(String connectionStatus) {
        TextView statusLabel = findViewById(R.id.statusLabel);
        statusLabel.setText(connectionStatus);
    }

    private void onUpdateAppStatus(String appStatus) {
        Button appStartStopButton = findViewById(R.id.appStartStopButton);
        switch (appStatus) {
            case AS_IDLE:
                appStartStopButton.setText("Start");
                appStartStopButton.setEnabled(true);
                break;
            case AS_RUNNING:
                appStartStopButton.setText("Stop");
                appStartStopButton.setEnabled(true);
                break;
            case AS_LAUNCHING:
                appStartStopButton.setText("Now launching");
                appStartStopButton.setEnabled(false);
                break;
            case AS_TERMINATING:
                appStartStopButton.setText("Now terminating");
                appStartStopButton.setEnabled(false);
                break;
        }
    }

    private String getTargetAddress() {
        EditText targetAddress = findViewById(R.id.targetAddressEditText);
        return "http://" + targetAddress.getText().toString();
    }

    interface HTTPResponseHandler {
        void onHTTPResponse(int code, String message);
    }

    private void sendHTTPRequest(String url, String method,
                                 String data, HTTPResponseHandler responseHandler) {
        final String _url = url;
        final String _method = method;
        final String _data = data;
        final HTTPResponseHandler _responseHandler = responseHandler;
        AsyncTask.execute(new Runnable() {
            @Override
            public void run() {
                try {
                    // Create URL
                    URL targetUrl = new URL(_url);
                    // Create connection
                    HttpURLConnection conn =
                            (HttpURLConnection) targetUrl.openConnection();
                    conn.setRequestMethod(_method);
                    conn.setUseCaches(false);
                    if (_data != null) {
                        conn.setDoOutput(true);
                        conn.getOutputStream().write(_data.getBytes());
                    }

                    _responseHandler.onHTTPResponse(conn.getResponseCode(),
                            conn.getResponseMessage());
                    Log.e("test",
                            "response code: " + conn.getResponseCode()
                                    + " / message: " + conn.getResponseMessage());
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });
    }

    private void checkConnectionStatus() {
        String url = getTargetAddress() + "/alive";
        sendHTTPRequest(url, "GET", null, new HTTPResponseHandler() {
            @Override
            public void onHTTPResponse(int code, String message) {
                if (code == 200 && message.equals("Success")) {
                    mConnectionStatus.setValue(CS_CONNECTED);
                } else {
                    mConnectionStatus.setValue(CS_DISCONNECTED);
                }
            }
        });
    }

    private void checkAppStatus() {
        String connectionStatus = this.mConnectionStatus.getValue();
        if (!connectionStatus.equals(CS_CONNECTED)) return;

        String url = getTargetAddress() + "/alive";
        sendHTTPRequest(url, "GET", null, new HTTPResponseHandler() {
            @Override
            public void onHTTPResponse(int code, String message) {
                if (code == 200 && message.equals("Success")) {
                    mConnectionStatus.setValue(CS_CONNECTED);
                } else {
                    mConnectionStatus.setValue(CS_DISCONNECTED);
                }
            }
        });
    }

    public void startApp() {
        mAppStatus.setValue(AS_LAUNCHING);
        String url = getTargetAddress() + "/runtime/currentApp/command";
        sendHTTPRequest(url, "POST", "start", new HTTPResponseHandler() {
            @Override
            public void onHTTPResponse(int code, String message) {
                if (code == 200 && message.equals("Success")) {
                    mAppStatus.setValue(AS_RUNNING);
                } else {
                    Toast.makeText(MainActivity.this, "Failed to start app: " + message,
                            Toast.LENGTH_SHORT).show();
                    mAppStatus.setValue(AS_IDLE);
                }
            }
        });
    }

    public void stopApp() {
        mAppStatus.setValue(AS_LAUNCHING);
        String url = getTargetAddress() + "/runtime/currentApp/command";
        sendHTTPRequest(url, "POST", "stop", new HTTPResponseHandler() {
            @Override
            public void onHTTPResponse(int code, String message) {
                if (code == 200 && message.equals("Success")) {
                    mAppStatus.setValue(AS_RUNNING);
                } else {
                    Toast.makeText(MainActivity.this, "Failed to stop app: " + message,
                            Toast.LENGTH_SHORT).show();
                    mAppStatus.setValue(AS_IDLE);
                }
            }
        });
    }
}
