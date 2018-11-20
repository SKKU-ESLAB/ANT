package com.redcarrottt.testapp;

/* Copyright (c) 2017-2018. All rights reserved.
 *  Gyeonghwan Hong (redcarrottt@gmail.com)
 *  Eunsoo Park (esevan.park@gmail.com)
 *  Injung Hwang (sinban04@gmail.com)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import android.Manifest;
import android.annotation.SuppressLint;
import android.content.Context;
import android.content.IntentFilter;
import android.content.pm.PackageManager;
import android.graphics.Color;
import android.os.Bundle;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;

import com.redcarrottt.sc.api.API;
import com.redcarrottt.sc.api.OnStartSCResult;
import com.redcarrottt.sc.api.OnStopSCResult;
import com.redcarrottt.sc.internal.ClientAdapter;
import com.redcarrottt.sc.internal.ClientAdapterStateListener;
import com.redcarrottt.sc.internal.bt.BtClientAdapter;
import com.redcarrottt.sc.internal.wfd.WfdClientAdapter;

import java.text.NumberFormat;
import java.util.ArrayList;
import java.util.Date;

import static com.redcarrottt.sc.internal.ExpConfig.VERBOSE_BANDWIDTH_UPDATE;

public class MainActivity extends AppCompatActivity implements LogReceiver.Callback,
        CommInitializerResult {
    private static final String kTag = "MainActivity";

    // Components
    private ArrayList<LogListViewItem> mLogListViewData = new ArrayList<>();
    private LogListViewAdapter mLogListViewAdapter;
    private SpeedWatcherThread mSpeedWatcherThread;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        // Initialize SwitchProfileButton
        Button switchProfileButton = (Button) findViewById(R.id.switchProfileButton);
        switchProfileButton.setOnClickListener(onClickSwitchProfileButton);
        this.updateSwitchProfileButton();

        // Initialize StartButton
        Button startButton = (Button) findViewById(R.id.startButton);
        startButton.setOnClickListener(onClickStartButton);

        // Initialize SpeedWatcherThread for BandwidthTextView
        this.mSpeedWatcherThread = new SpeedWatcherThread();
        this.mSpeedWatcherThread.start();

        // Initialize LogListView
        this.mLogListViewAdapter = new LogListViewAdapter(this, this.mLogListViewData);
        ListView logListView = (ListView) findViewById(R.id.logListView);
        logListView.setAdapter(this.mLogListViewAdapter);

        // Initialize Log Receiver
        Logger.setDefaultContext(this);
        LogReceiver logReceiver = new LogReceiver(this);
        IntentFilter broadcastIntentFilter = new IntentFilter();
        broadcastIntentFilter.addAction(LogReceiver.kAction);
        this.registerReceiver(logReceiver, broadcastIntentFilter);

        // Require permissions for SC
        this.requestPermissions();

        // Initialize Communication Interfaces
        Logger.VERB(kTag, "Initializing network interfaces...");
        mCommInitializer = new CommInitializer(this, this);
        mCommInitializer.start();

        sSingleton = this;
    }


    private CommInitializer mCommInitializer;

    @Override
    public void onCommInitializerResult(boolean isSuccess) {
        if (isSuccess) {
            Logger.VERB(kTag, "All network Interfaces ready");
            Button startButton = (Button) findViewById(R.id.startButton);
            startButton.setEnabled(true);
        } else {
            Logger.ERR(kTag, "Initialization failed");
        }
    }

    private View.OnClickListener onClickStartButton = new View.OnClickListener() {
        @Override
        public void onClick(View view) {
            // Initialize SC
            initializeCommunication();
            Button startButton = (Button) findViewById(R.id.startButton);
            startButton.setEnabled(false);
        }
    };

    private String[] mProfiles = {"Lab", "Home"};
    private int mPresentProfile = 0;
    private View.OnClickListener onClickSwitchProfileButton = new View.OnClickListener() {
        @Override
        public void onClick(View view) {
            final int kMaxProfiles = mProfiles.length;
            mPresentProfile = (mPresentProfile + 1) % kMaxProfiles;
            updateSwitchProfileButton();
        }
    };

    private void updateSwitchProfileButton() {
        Button switchProfileButton = (Button) findViewById(R.id.switchProfileButton);
        switchProfileButton.setText(this.mProfiles[this.mPresentProfile] + " Profile");
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

    private void initializeCommunication() {
        String btAddress = "";
        switch (this.mPresentProfile) {
            case 0:
                btAddress = "B8:27:EB:D9:FA:85";
                break;
            case 1:
                btAddress = "B8:27:EB:77:C3:4A";
                break;
        }

        if (btAddress == "") {
            Logger.ERR(kTag, "No bluetooth address is defined!");
            return;
        } else {
            Logger.VERB(kTag, "BT Address: " + btAddress);
        }


        // Setting adapters
        this.mBtClientAdapter = BtClientAdapter.singleton(1, "BT", btAddress,
                "150e8400-1234-41d4-a716-446655440000", this);
        this.mBtClientAdapter.listenState(this.mClientAdapterStateListener);
        API.registerAdapter(this.mBtClientAdapter);

        this.mWfdClientAdapter = WfdClientAdapter.singleton(2, "WFD", 3455, this);
        this.mWfdClientAdapter.listenState(this.mClientAdapterStateListener);
        API.registerAdapter(this.mWfdClientAdapter);

        // Start the selective connection
        API.startSC(onStartSCResult);
    }

    @Override
    protected void onResume() {
        super.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        API.stopSC(onStopSCResult);
    }

    // ClientAdapters
    private BtClientAdapter mBtClientAdapter;
    private WfdClientAdapter mWfdClientAdapter;

    private int mBtClientAdapterState;
    private int mWfdClientAdapterState;

    private ClientAdapterStateListener mClientAdapterStateListener = new
            ClientAdapterStateListener() {

        @Override
        public void onUpdateClientAdapterState(ClientAdapter adapter, int oldState, int newState) {
            // Update local state variable
            if (adapter == mBtClientAdapter) {
                mBtClientAdapterState = newState;
            } else if (adapter == mWfdClientAdapter) {
                mWfdClientAdapterState = newState;
            }

            // Update state text
            final String btClientAdapterState = ClientAdapter.stateToString(mBtClientAdapterState);
            final String wfdClientAdapterState = ClientAdapter.stateToString
                    (mWfdClientAdapterState);

            final String btClientAdapterStateColor = (mBtClientAdapterState == 2) ? "#0000ff" :
                    "#000000";
            final String wfdClientAdapterStateColor = (mWfdClientAdapterState == 2) ? "#0000ff" :
                    "#000000";

            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    // Update UI
                    TextView btStateTextView = (TextView) findViewById(R.id.btStateTextView);
                    btStateTextView.setText("BT: " + btClientAdapterState);
                    btStateTextView.setTextColor(Color.parseColor(btClientAdapterStateColor));

                    TextView wfdStateTextView = (TextView) findViewById(R.id.wfdStateTextView);
                    wfdStateTextView.setText("WFD: " + wfdClientAdapterState);
                    wfdStateTextView.setTextColor(Color.parseColor(wfdClientAdapterStateColor));

                    // Add Log
                    Logger.VERB("Adapters", "BT: " + btClientAdapterState + " / WFD: " +
                            wfdClientAdapterState);
                }
            });
        }
    };

    private OnStartSCResult onStartSCResult = new OnStartSCResult() {
        private int mRetries = 0;
        private static final int kMaxRetries = 5;

        @Override
        public void onDoneStartSC(boolean isSuccess) {
            if (isSuccess) {
                // Starting Selective Connection is done
                ReceivingThread receivingThread = new ReceivingThread();
                receivingThread.start();
            } else {
                // Retry to start SC
                if (mRetries++ < kMaxRetries) {
                    Logger.VERB(kTag, "Failed to start SC... Retry to start.");
                    try {
                        Thread.sleep(5000);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    API.startSC(onStartSCResult);
                }
            }
        }
    };

    private OnStopSCResult onStopSCResult = new OnStopSCResult() {
        @Override
        public void onDoneStopSC(boolean isSuccess) {
            if (!isSuccess) {
                Log.e(kTag, "Stopping Selective Connection failed");
            }
        }
    };

    private class SpeedWatcherThread extends Thread {
        private Date mLastAccessedTS;
        private int mPrevValue;
        private int mTotalDataSize; // Bytes
        private final int kSleepMS = 1000;

        public void arrive(int dataSize) {
            this.mTotalDataSize += dataSize;
        }

        private int getSpeed() {
            int speed;

            Date startTS = this.mLastAccessedTS;
            Date endTS = new Date();
            int prevValue = this.mPrevValue;
            int presentValue = this.mTotalDataSize;

            speed = (int) ((float) (presentValue - prevValue) / ((float) (endTS.getTime() -
                    startTS.getTime()) / 1000));

            this.mLastAccessedTS = endTS;
            this.mPrevValue = presentValue;

            return speed;
        }

        @SuppressLint("SetTextI18n")
        @SuppressWarnings("InfiniteLoopStatement")
        @Override
        public void run() {
            this.setName("SpeedWatcher");
            this.mPrevValue = 0;
            this.mTotalDataSize = 0;
            this.mLastAccessedTS = new Date();
            while (true) {
                final int speed = this.getSpeed();
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        TextView bandwidthTextView = (TextView) findViewById(R.id
                                .bandwidthTextView);
                        NumberFormat format = NumberFormat.getNumberInstance();
                        if (VERBOSE_BANDWIDTH_UPDATE) {
                            Log.d(kTag, "Bandwidth update: " + format.format(speed) + "B/s");
                        }
                        bandwidthTextView.setText("Bandwidth: " + format.format(speed) + "B/s");
                    }
                });
                try {
                    Thread.sleep(kSleepMS);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    private class ReceivingThread extends Thread {
        private static final String kTag = "Recv";
        private boolean mIsAlive;
        private final boolean kVerboseReceivingThread = false;

        ReceivingThread() {
            this.mIsAlive = false;
        }

        @Override
        public void run() {
            this.setName("AppReceiving");
            this.mIsAlive = true;
            byte[] buf = new byte[100 * 1024 * 1024];
            String sending_buf = "ACK"; /* Ack Message */

            while (this.mIsAlive) {
                int receivedLength = API.receive(buf);
                mSpeedWatcherThread.arrive(receivedLength);
                if (kVerboseReceivingThread) {
                    Logger.DEBUG(kTag, "Received: Size=" + receivedLength);
                }
            }
        }

        public void terminate() {
            this.mIsAlive = false;
        }
    }

    @Override
    public void onLogMessage(final int logLevel, final String logTag, final String logMessage) {
        final int kPrintThreshold = LogLevel.VERB;
        final String kTag = "LOG";
        this.runOnUiThread(new Runnable() {
            @SuppressLint("SetTextI18n")
            @Override
            public void run() {
                String printLevel = "";
                int color = Color.BLACK;
                switch (logLevel) {
                    case LogLevel.ERR:
                        Log.e(logTag, logMessage);
                        printLevel = "E";
                        color = Color.RED;
                        break;
                    case LogLevel.WARN:
                        Log.w(logTag, logMessage);
                        printLevel = "W";
                        color = Color.parseColor("#FF8000");
                        break;
                    case LogLevel.VERB:
                        Log.i(logTag, logMessage);
                        printLevel = "V";
                        color = Color.parseColor("#0072C1");
                        break;
                    case LogLevel.DEBUG:
                        Log.d(logTag, logMessage);
                        printLevel = "D";
                        color = Color.parseColor("#5C676F");
                        break;
                }
                String printMessage = "[" + printLevel + "/" + logTag + "] " + logMessage;

                if (logLevel <= kPrintThreshold) {
                    mLogListViewData.add(0, new LogListViewItem(printMessage, color));
                    mLogListViewAdapter.notifyDataSetChanged();
                }
            }
        });
    }

    static private MainActivity sSingleton;
    private boolean mIsPending = false;

    static public void setPendingState(boolean isPending) {
        final MainActivity self = sSingleton;
        if (self.mIsPending == isPending) {
            return;
        }
        self.mIsPending = isPending;

        self.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                TextView pendingStatusTextView = (TextView) self.findViewById(R.id
                        .pendingStatusTextView);
                if (self.mIsPending) {
                    pendingStatusTextView.setText("Pending...");
                } else {
                    pendingStatusTextView.setText("");
                }
            }
        });
    }

    static public void setNextSeqNo(int seq_no_control, int seq_no_data) {
        final MainActivity self = sSingleton;
        final String next_seq_no_str = "SeqNo (Ctrl=" + seq_no_control + " / Data=" + seq_no_data
                + ")";
        self.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                TextView nextSeqNoTextView = (TextView) self.findViewById(R.id.nextSeqNoTextView);
                nextSeqNoTextView.setText(next_seq_no_str);
            }
        });
    }
}

class LogListViewItem {
    private String mText;
    private int mColor;

    public String getText() {
        return this.mText;
    }

    public int getColor() {
        return this.mColor;
    }

    LogListViewItem(String text, int color) {
        this.mText = text;
        this.mColor = color;
    }
}

class LogListViewAdapter extends BaseAdapter {
    private LayoutInflater mInflater;
    private int mLayout;
    private ArrayList<LogListViewItem> mData;

    LogListViewAdapter(Context context, ArrayList<LogListViewItem> data) {
        this.mInflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        this.mLayout = R.layout.item;
        this.mData = data;
    }

    @Override
    public int getCount() {
        return this.mData.size();
    }

    @Override
    public String getItem(int position) {
        return this.mData.get(position).getText();
    }

    @Override
    public long getItemId(int position) {
        return position;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        if (convertView == null) {
            convertView = mInflater.inflate(this.mLayout, parent, false);
        }

        LogListViewItem item = this.mData.get(position);
        TextView contentsTextView = (TextView) convertView.findViewById(R.id.contentsTextView);
        contentsTextView.setText(item.getText());
        contentsTextView.setTextColor(item.getColor());
        return contentsTextView;
    }
}