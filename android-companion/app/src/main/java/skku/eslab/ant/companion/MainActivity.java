package skku.eslab.ant.companion;

import android.content.SharedPreferences;
import android.graphics.Color;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.text.format.Formatter;
import android.view.KeyEvent;
import android.view.View;
import android.view.inputmethod.EditorInfo;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.material.bottomnavigation.BottomNavigationView;

import java.util.Timer;
import java.util.TimerTask;

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

        ConnectionManager connectionManager = ConnectionManager.get();
        connectionManager.setTargetAddress(targetAddress);
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
        editor.apply();
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

        // Setting connection manager
        ConnectionManager connectionManager = ConnectionManager.get();
        connectionManager.setMotherActivity(this);

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

        // Register edittext listeners
        final EditText targetAddressEditText = findViewById(R.id.targetAddressEditText);
        targetAddressEditText.setOnEditorActionListener(new TextView.OnEditorActionListener() {
            @Override
            public boolean onEditorAction(TextView textView, int actionId, KeyEvent keyEvent) {
                if (actionId == EditorInfo.IME_ACTION_DONE) {
                    onUpdateTargetAddress(targetAddressEditText.getText().toString());
                }
                return false;
            }
        });

        // Monitoring task
        TimerTask task = new TimerTask() {
            @Override
            public void run() {
                checkConnectionStatus();
                checkAppStatus();
                requestSettingCompanionAddress();
            }
        };
        new Timer().scheduleAtFixedRate(task, 1000, 2000);
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
        Button appStartStopButton = findViewById(R.id.appStartStopButton);

        switch (connectionStatus) {
            case CS_CONNECTED:
                statusLabel.setTextColor(Color.parseColor("#0000ff"));
                switch (this.mAppStatus.getValue()) {
                    case AS_IDLE:
                    case AS_RUNNING:
                        appStartStopButton.setEnabled(true);
                        break;
                }
                break;
            case CS_DISCONNECTED:
                statusLabel.setTextColor(Color.parseColor("#ff0000"));
                appStartStopButton.setEnabled(false);
                break;
        }
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

    private String mRecentConnectionStatus = CS_DISCONNECTED;

    private void requestSettingCompanionAddress() {
        if (!this.mRecentConnectionStatus.equals(this.mConnectionStatus.getValue())) {
            this.mRecentConnectionStatus = this.mConnectionStatus.getValue();

            if (this.mRecentConnectionStatus == CS_CONNECTED) {
                WifiManager wm = (WifiManager) getSystemService(WIFI_SERVICE);
                String selfIpAddress =
                        Formatter.formatIpAddress(wm.getConnectionInfo().getIpAddress());

                ConnectionManager connectionManager = ConnectionManager.get();
                String url = connectionManager.getTargetAddress() + "/runtime/currentApp" +
                        "/companionAddress";
                connectionManager.sendHTTPRequest(url, "POST", selfIpAddress, new HTTPResponseHandler() {
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
        }
    }

    private void onUpdateTargetAddress(String targetAddress) {
        ConnectionManager connectionManager = ConnectionManager.get();
        connectionManager.setTargetAddress(targetAddress);
    }

    private void checkConnectionStatus() {
        ConnectionManager connectionManager = ConnectionManager.get();
        String url = connectionManager.getTargetAddress() + "/";
        connectionManager.sendHTTPRequest(url, "GET", null, new HTTPResponseHandler() {
            @Override
            public void onHTTPResponse(int code, String message) {
                if (code == 200 && message.equals("Alive")) {
                    mConnectionStatus.setValue(CS_CONNECTED);
                } else {
                    mConnectionStatus.setValue(CS_DISCONNECTED);
                }
            }
        });
    }

    private void checkAppStatus() {
        String connectionStatus = this.mConnectionStatus.getValue();
        assert connectionStatus != null;
        if (!connectionStatus.equals(CS_CONNECTED)) return;

        ConnectionManager connectionManager = ConnectionManager.get();
        String url = connectionManager.getTargetAddress() + "/runtime/currentApp/state";
        connectionManager.sendHTTPRequest(url, "GET", null, new HTTPResponseHandler() {
            @Override
            public void onHTTPResponse(int code, String message) {
                if (code == 200) {
                    switch (message) {
                        case "Idle":
                            mAppStatus.setValue(AS_IDLE);
                            break;
                        case "Running":
                            mAppStatus.setValue(AS_RUNNING);
                            break;
                    }
                }
            }
        });
    }

    public void startApp() {
        mAppStatus.setValue(AS_LAUNCHING);

        ConnectionManager connectionManager = ConnectionManager.get();
        String url = connectionManager.getTargetAddress() + "/runtime/currentApp/command";
        connectionManager.sendHTTPRequest(url, "POST", "start", new HTTPResponseHandler() {
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

        ConnectionManager connectionManager = ConnectionManager.get();
        String url = connectionManager.getTargetAddress() + "/runtime/currentApp/command";
        connectionManager.sendHTTPRequest(url, "POST", "stop", new HTTPResponseHandler() {
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
