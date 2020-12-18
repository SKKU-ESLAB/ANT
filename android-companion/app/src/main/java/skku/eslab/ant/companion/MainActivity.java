package skku.eslab.ant.companion;

import android.content.Intent;
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
import android.widget.SeekBar;
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
import skku.eslab.ant.companion.companionapi.CompanionAPI;
import skku.eslab.ant.companion.gatewayconnector.GatewayConnector;
import skku.eslab.ant.companion.httpconnection.HTTPClient;
import skku.eslab.ant.companion.httpconnection.HTTPResponseHandler;
import skku.eslab.ant.companion.remoteuiapi.RemoteUIAPI;
import skku.eslab.ant.companion.resourceapi.OnResourceRequestListener;
import skku.eslab.ant.companion.resourceapi.Resource;
import skku.eslab.ant.companion.resourceapi.ResourceAPI;
import skku.eslab.ant.companion.resourceapi.ResourceRequest;
import skku.eslab.ant.companion.resourcebench.ResourceBenchHelper;

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

    final String RPI3_ADDRESS = "192.168.0.33:8001";
    final String OXU4_ADDRESS = "192.168.0.18:8001";
    final String JTX2_ADDRESS = "115.145.209.162:8001";

    private GatewayConnector mGatewayConnector;

    @Override
    protected void onStart() {
        super.onStart();

        // Setting to target address edit text
        SharedPreferences sharedPref =
                getSharedPreferences(SP_FILENAME, MODE_PRIVATE);
        String targetAddress = sharedPref
                .getString(SP_TARGET_ADDRESS, SP_DEFAULT_TARGET_ADDRESS);
        EditText targetAddressEditText =
                findViewById(R.id.targetAddressEditText);
        targetAddressEditText.setText(targetAddress);

        HTTPClient httpClient = HTTPClient.get();
        httpClient.setTargetAddress(targetAddress);

        // Initialize APIs
        CompanionAPI.get();
        ResourceAPI.get();
        RemoteUIAPI.get();

        // Resource Benchmark Test
        ResourceBenchHelper.get();

        // Gateway Connector
        this.mGatewayConnector = new GatewayConnector(this);
    }

    @Override
    protected void onStop() {
        super.onStop();

        // Setting from target address edit text
        EditText targetAddressEditText =
                findViewById(R.id.targetAddressEditText);
        String targetAddress = targetAddressEditText.getText().toString();

        SharedPreferences sharedPref =
                getSharedPreferences(SP_FILENAME, MODE_PRIVATE);
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
                new AppBarConfiguration.Builder(R.id.navigation_home,
                        R.id.navigation_dashboard,
                        R.id.navigation_notifications).build();
        NavController navController =
                Navigation.findNavController(this, R.id.nav_host_fragment);
        NavigationUI.setupActionBarWithNavController(this, navController,
                appBarConfiguration);
        NavigationUI.setupWithNavController(navView, navController);

        // Setting connection manager
        HTTPClient httpClient = HTTPClient.get();
        httpClient.setMotherActivity(this);

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

        // Register edittext listeners
        final EditText targetAddressEditText =
                findViewById(R.id.targetAddressEditText);
        targetAddressEditText.setOnEditorActionListener(
                new TextView.OnEditorActionListener() {
                    @Override
                    public boolean onEditorAction(TextView textView,
                                                  int actionId,
                                                  KeyEvent keyEvent) {
                        if (actionId == EditorInfo.IME_ACTION_DONE) {
                            onUpdateTargetAddress(
                                    targetAddressEditText.getText().toString());
                        }
                        return false;
                    }
                });

        // Register button listeners
        Button appStartStopButton = findViewById(R.id.appStartStopButton);
        appStartStopButton.setOnClickListener(onClickAppStartStopButton);

        final Button settingButton = findViewById(R.id.settingButton);
        settingButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                int isVisible = targetAddressEditText.getVisibility();
                isVisible = (isVisible == View.VISIBLE) ? View.INVISIBLE : View.VISIBLE;
                targetAddressEditText.setVisibility(isVisible);
            }
        });

        // Partitioning UI
        SeekBar partitioningSeekBar = findViewById(R.id.partitioningSeekbar);
        partitioningSeekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                int partitioningPoint = seekBar.getProgress();
                mGatewayConnector.setNextStartIdx(partitioningPoint);
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

        // Companion test
        if (isCompanionTestEnabled) {
            this.initializeCompanionTest();
        }
    }

    public void enablePartitioningPointUI(boolean isEnabled) {
        // Partitioning point UI
        TextView leftPartitioningLabel = findViewById(R.id.leftPartitioningLabel);
        SeekBar partitioningSeekbar = findViewById(R.id.partitioningSeekbar);
        TextView rightPartitioningLabel = findViewById(R.id.rightPartitioningLabel);
        int visibility = (isEnabled) ? View.VISIBLE : View.INVISIBLE;
        leftPartitioningLabel.setVisibility(visibility);
        partitioningSeekbar.setVisibility(visibility);
        rightPartitioningLabel.setVisibility(visibility);
    }

    public void setPartitioningPoint(int partitioningPoint) {
        SeekBar partitioningSeekbar = findViewById(R.id.partitioningSeekbar);
        partitioningSeekbar.setProgress(partitioningPoint);
    }

    private final boolean isCompanionTestEnabled = true;

    private Resource mTesterResource;

    private void initializeCompanionTest() {
        this.mTesterResource = new Resource("/tester");
        this.mTesterResource.setOnPost(new OnResourceRequestListener() {
            @Override
            public void onResourceRequest(ResourceRequest request) {
                ResourceAPI.get().sendResponse(request, request.getMessage());
            }
        });
        ResourceAPI.get().registerResource(this.mTesterResource);
    }

    private View.OnClickListener onClickAppStartStopButton =
            new View.OnClickListener() {
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

    final String CONNECTION_STATUS_BROADCASE = "ant.companion.connectionstatus";

    private void sendConnectionStatusBroadcast(String connectionStatus) {
        Intent intent = new Intent(CONNECTION_STATUS_BROADCASE);
        intent.putExtra("value", connectionStatus);
        sendBroadcast(intent);
    }

    private void onUpdateConnectionStatus(String connectionStatus) {

        sendConnectionStatusBroadcast(connectionStatus);

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

    private void requestSettingCompanionAddress() {
        WifiManager wm = (WifiManager) getSystemService(WIFI_SERVICE);
        String selfIpAddress = Formatter
                .formatIpAddress(wm.getConnectionInfo().getIpAddress());

        HTTPClient httpClient = HTTPClient.get();
        String url = httpClient.getTargetAddress() + "/runtime/currentApp" +
                "/companionAddress";
        httpClient.sendHTTPRequest(url, "POST", selfIpAddress,
                new HTTPResponseHandler() {
                    @Override
                    public void onHTTPResponse(int code, String message) {
                        // Not implemented
                    }
                });
    }

    private void onUpdateTargetAddress(String targetAddress) {
        HTTPClient httpClient = HTTPClient.get();
        httpClient.setTargetAddress(targetAddress);
    }

    private String mRecentConnectionStatus = CS_DISCONNECTED;

    private void checkConnectionStatus() {
        HTTPClient httpClient = HTTPClient.get();
        String url = httpClient.getTargetAddress() + "/";
        httpClient.sendHTTPRequest(url, "GET", null, new HTTPResponseHandler() {
            @Override
            public void onHTTPResponse(int code, String message) {
                String connectionStatus;
                if (code == 200 && message.equals("Alive")) {
                    connectionStatus = CS_CONNECTED;
                } else {
                    connectionStatus = CS_DISCONNECTED;
                }
                mConnectionStatus.setValue(connectionStatus);
                mRecentConnectionStatus = mConnectionStatus.getValue();
            }
        });
    }

    private void checkAppStatus() {
        String connectionStatus = this.mConnectionStatus.getValue();
        assert connectionStatus != null;
        if (!connectionStatus.equals(CS_CONNECTED))
            return;

        HTTPClient httpClient = HTTPClient.get();
        String url =
                httpClient.getTargetAddress() + "/runtime/currentApp/state";
        httpClient.sendHTTPRequest(url, "GET", null, new HTTPResponseHandler() {
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

        HTTPClient httpClient = HTTPClient.get();
        String url =
                httpClient.getTargetAddress() + "/runtime/currentApp/command";
        httpClient.sendHTTPRequest(url, "POST", "start",
                new HTTPResponseHandler() {
                    @Override
                    public void onHTTPResponse(int code, String message) {
                        if (code == 200 && message.equals("Success")) {
                            mAppStatus.setValue(AS_RUNNING);
                        } else {
                            Toast.makeText(MainActivity.this,
                                    "Failed to start app: " + message,
                                    Toast.LENGTH_SHORT).show();
                            mAppStatus.setValue(AS_IDLE);
                        }
                    }
                });
    }

    public void stopApp() {
        mAppStatus.setValue(AS_LAUNCHING);

        HTTPClient httpClient = HTTPClient.get();
        String url =
                httpClient.getTargetAddress() + "/runtime/currentApp/command";
        httpClient.sendHTTPRequest(url, "POST", "stop",
                new HTTPResponseHandler() {
                    @Override
                    public void onHTTPResponse(int code, String message) {
                        if (code == 200 && message.equals("Success")) {
                            mAppStatus.setValue(AS_RUNNING);
                        } else {
                            Toast.makeText(MainActivity.this,
                                    "Failed to stop app: " + message,
                                    Toast.LENGTH_SHORT).show();
                            mAppStatus.setValue(AS_IDLE);
                        }
                    }
                });
    }
}
