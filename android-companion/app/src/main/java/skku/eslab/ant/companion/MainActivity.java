package skku.eslab.ant.companion;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.google.android.material.bottomnavigation.BottomNavigationView;

import androidx.appcompat.app.AppCompatActivity;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.Observer;
import androidx.navigation.NavController;
import androidx.navigation.Navigation;
import androidx.navigation.ui.AppBarConfiguration;
import androidx.navigation.ui.NavigationUI;

public class MainActivity extends AppCompatActivity {
    private MutableLiveData<String> mConnectionStatus = new MutableLiveData<>();
    private final String CS_DISCONNECTED = "Disconnected";
    private final String CS_CONNECTING = "Connecting";
    private final String CS_CONNECTED = "Connected";
    // TODO: hardcoded: no CS_DISCONNECTING

    private MutableLiveData<String> mAppStatus = new MutableLiveData<>();
    private final String AS_IDLE = "Idle";
    private final String AS_LAUNCHING = "Launching";
    private final String AS_RUNNING = "Running";
    private final String AS_TERMINATING = "Terminating";

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
        Button connectButton = findViewById(R.id.connectButton);
        connectButton.setOnClickListener(onClickConnectButton);

        Button appStartStopButton = findViewById(R.id.appStartStopButton);
        connectButton.setOnClickListener(onClickAppStartStopButton);

        // Try to connect target
        this.connectTarget();
    }

    private View.OnClickListener onClickConnectButton = new View.OnClickListener() {
        @Override
        public void onClick(View view) {
            connectTarget();
        }
    };

    private View.OnClickListener onClickAppStartStopButton = new View.OnClickListener() {
        @Override
        public void onClick(View view) {
            if (!mConnectionStatus.getValue().equals(CS_CONNECTED)) {
                return;
            }

            if (mAppStatus.getValue().equals(AS_IDLE)) {
                startApp();
            } else if (mAppStatus.getValue().equals(AS_RUNNING)) {
                stopApp();
            }
        }
    };

    private void onUpdateConnectionStatus(String connectionStatus) {
        TextView statusLabel = findViewById(R.id.statusLabel);
        statusLabel.setText(connectionStatus);

        Button connectButton = findViewById(R.id.connectButton);
        if (connectionStatus.equals(CS_DISCONNECTED)) {
            connectButton.setEnabled(true);
        } else if (connectionStatus.equals(CS_CONNECTED)) {
            connectButton.setEnabled(false);
        }
    }

    private void onUpdateAppStatus(String appStatus) {
        Button appStartStopButton = findViewById(R.id.appStartStopButton);
        if (appStatus.equals(AS_IDLE)) {
            appStartStopButton.setText("Start");
            appStartStopButton.setEnabled(true);
        } else if (appStatus.equals(AS_RUNNING)) {
            appStartStopButton.setText("Stop");
            appStartStopButton.setEnabled(true);
        } else if (appStatus.equals(AS_LAUNCHING)) {
            appStartStopButton.setText("Now launching");
            appStartStopButton.setEnabled(false);
        } else if (appStatus.equals(AS_TERMINATING)) {
            appStartStopButton.setText("Now terminating");
            appStartStopButton.setEnabled(false);
        }
    }

    private void connectTarget() {
        // TODO: not yet implemented

    }

    private void checkAndUpdateAppStatus() {
        // TODO: not yet implemented
    }

    public void startApp() {
        // TODO: not yet implemented
    }

    public void stopApp() {
        // TODO: not yet implemented
    }
}
