package com.ant.ant_manager.view;

/* Copyright (c) 2017 SKKU ESLAB, and contributors. All rights reserved.
 *
 * Contributor: Gyeonghwan Hong<redcarrottt@gmail.com>
 *              Dongig Sin<dongig@skku.edu>
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

import android.annotation.SuppressLint;
import android.app.ActionBar;
import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.util.Log;
import android.view.MenuItem;
import android.widget.TextView;

import com.ant.ant_manager.R;
import com.ant.ant_manager.controller.ANTControllerBroadcastReceiver;
import com.ant.ant_manager.controller.ANTControllerService;
import com.jjoe64.graphview.GraphView;
import com.jjoe64.graphview.series.DataPoint;
import com.jjoe64.graphview.series.LineGraphSeries;

import java.util.ArrayList;

public class SensorViewerActivity extends Activity {
    // ANTControllerService
    private ANTControllerService mControllerServiceStub = null;
    private PrivateControllerBroadcastReceiver mControllerBroadcastReceiver;

    // Intent
    public static final String INTENT_KEY_APP_ID = "appId";

    private int mAppId;

    private final Handler mHandler = new Handler();

    private Runnable mTimer2;

    private LineGraphSeries<DataPoint> mSeries2;
    private double graph2LastXValue = 5d;

    ArrayList<SensorData> mSensorDataList = new ArrayList<>();
    int numSensors = 7;
    String[] mSensorNameList = {"Touch", "Accelerometer", "Motion", "Sound", "Light",
            "Vibration", "Temperature"};
    int[] mSensorGraphViewList = {R.id.touchSensorGraphView, R.id.accSensorGraphView, R.id
            .motionSensorGraphView, R.id.soundSensorGraphView, R.id.lightSensorGraphView, R.id
            .vibSensorGraphView, R.id.tempSensorGraphView};
    int[] mSensorTextViewList = {R.id.touchSensorTextView, R.id.accSensorTextView, R.id
            .motionSensorTextView, R.id.soundSensorTextView, R.id.lightSensorTextView, R.id
            .vibSensorTextView, R.id.tempSensorTextView};
    int[] mSensorGraphLineColor = {Color.RED, Color.BLUE, Color.WHITE, Color.YELLOW, Color.WHITE,
            Color.BLUE, Color.YELLOW};

    private final String TAG = "SensorViewer";
    private boolean mIsUIReady = false;

    protected void initializeSensorDataList() {
        for (int i = 0; i < numSensors; i++) {
            String sensorName = this.mSensorNameList[i];
            GraphView sensorGraphView = (GraphView) findViewById(this.mSensorGraphViewList[i]);
            TextView sensorTextView = (TextView) findViewById(this.mSensorTextViewList[i]);
            SensorData newSensorData = new SensorData(0, 100, sensorName, sensorGraphView,
                    sensorTextView);
            this.mSensorDataList.add(newSensorData);
            newSensorData.setGraphLineColor(this.mSensorGraphLineColor[i]);
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_sensor_viewer);

        // Parameters
        Intent intent = this.getIntent();
        this.mAppId = intent.getIntExtra(INTENT_KEY_APP_ID, -1);
        if (this.mAppId < 0) {
            Log.e(TAG, "Invalid application id!");
            this.finish();
        }

        ActionBar actionBar = getActionBar();
        actionBar.setTitle("Sensor Viewer");
        actionBar.setDisplayHomeAsUpEnabled(true);
        actionBar.setLogo(R.drawable.sensor);
        actionBar.setDisplayUseLogoEnabled(true);

        this.initializeSensorDataList();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

        // Terminate JavaScript SensorViewer App
        this.mControllerServiceStub.terminateAppOneWay(this.mAppId);

        // Disconnect Controller Service
        this.disconnectControllerService();
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case android.R.id.home:
                this.finish();
                return true;
            default:
                return super.onOptionsItemSelected(item);
        }
    }

    public void onSensorViewerMessage(String message) {
        String[] tokens = message.split("\\s+");
        if (tokens.length < 7) {
            Log.e(TAG, "# of tokens is too small! : " + message);
            return;
        }

        if (this.mIsUIReady) {
            for (int i = 0; i < this.numSensors; i++) {
                Double sensorValue = Double.parseDouble(tokens[i]);
                SensorData sensorData = this.mSensorDataList.get(i);
                if (sensorValue >= 0) {
                    sensorData.setEnabled(true);
                    sensorData.setCurrentValue(sensorValue);
                } else {
                    sensorData.setEnabled(false);
                }
            }
        }
    }

    @Override
    public void onResume() {
        super.onResume();

        connectControllerService();

        mTimer2 = new Runnable() {
            @Override
            public void run() {
                graph2LastXValue += 1d;
                for (int i = 0; i < numSensors; i++) {
                    SensorData sensorData = mSensorDataList.get(i);
                    Double sensorValue = sensorData.getCurrentValue();
                    sensorData.appendData(new DataPoint(graph2LastXValue, sensorValue), true,
                            sensorData.getMax());
                }
                mHandler.postDelayed(this, 200);
            }

        };
        mHandler.postDelayed(mTimer2, 1000);

        this.mIsUIReady = true;
    }

    @Override
    public void onBackPressed() {
        super.onBackPressed();
    }

    @Override
    public void onPause() {
        super.onPause();
        mControllerServiceStub.terminateAppOneWay(this.mAppId);
        mHandler.removeCallbacks(mTimer2);
        this.mIsUIReady = false;
    }

    private void connectControllerService() {
        Intent serviceIntent = new Intent(this, ANTControllerService.class);
        this.bindService(serviceIntent, this.mControllerServiceConnection, Context
                .BIND_AUTO_CREATE);
    }

    private void disconnectControllerService() {
        if (this.mControllerServiceConnection != null)
            this.unbindService(this.mControllerServiceConnection);
        if (this.mControllerBroadcastReceiver != null)
            this.unregisterReceiver(this.mControllerBroadcastReceiver);
    }

    private ServiceConnection mControllerServiceConnection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName componentName, IBinder inputBinder) {
            ANTControllerService.ControllerBinder serviceBinder = (ANTControllerService
                    .ControllerBinder) inputBinder;
            mControllerServiceStub = serviceBinder.getService();

            // Set BroadcastReceiver
            IntentFilter broadcastIntentFilter = new IntentFilter();
            broadcastIntentFilter.addAction(ANTControllerBroadcastReceiver.ACTION);
            mControllerBroadcastReceiver = new PrivateControllerBroadcastReceiver();
            registerReceiver(mControllerBroadcastReceiver, broadcastIntentFilter);

            // Initialize connection with camera viewer
            mControllerServiceStub.launchAppOneWay(mAppId);
        }

        @Override
        public void onServiceDisconnected(ComponentName componentName) {
            Log.d(TAG, "onServiceDisconnected()");
            unregisterReceiver(mControllerBroadcastReceiver);
            mControllerServiceStub = null;
        }
    };

    class PrivateControllerBroadcastReceiver extends ANTControllerBroadcastReceiver {
        PrivateControllerBroadcastReceiver() {
            this.setOnReceivedSensorDataListener(new OnReceivedDataFromTarget() {
                @Override
                public void onReceivedDataFromTarget(String listenerName, String data) {
                    Log.d(TAG, "Message coming for " + listenerName + ": " + data);
                    if (listenerName.compareToIgnoreCase("sensorviewer") == 0) {
                        onSensorViewerMessage(data);
                    }
                }
            });
        }
    }

    class SensorData {
        private int mMin;
        private int mMax;
        private double mCurrentValue;
        private String mSensorName;

        private GraphView mGraphView;
        private TextView mTextView;

        private LineGraphSeries<DataPoint> mLineGraphSeries;
        private int mUniqueColor;
        private boolean mIsEnabled = false;

        SensorData(int min, int max, String sensorName, GraphView graphView, TextView textView) {
            this.mMin = min;
            this.mMax = max;
            this.mGraphView = graphView;
            this.mCurrentValue = 0.0;
            this.mSensorName = sensorName;
            this.mTextView = textView;
            this.mTextView.setText(sensorName + "(Loading...)");
            mLineGraphSeries = new LineGraphSeries<DataPoint>();

            graphView.addSeries(mLineGraphSeries);
            graphView.getViewport().setXAxisBoundsManual(true);
            graphView.getViewport().setMinX(min);
            graphView.getViewport().setMaxX(max);
            graphView.getViewport().setYAxisBoundsManual(false);

            mLineGraphSeries.setThickness(5);
        }

        void setCurrentValue(double newValue) {
            this.mCurrentValue = newValue;
        }

        double getCurrentValue() {
            return this.mCurrentValue;
        }

        void appendData(DataPoint dataPoint, boolean scrollToEnd, int maxDataPoints) {
            mLineGraphSeries.appendData(dataPoint, scrollToEnd, maxDataPoints);
        }

        int getMax() {
            return this.mMax;
        }

        String getSensorName() {
            return this.mSensorName;
        }

        void setTextViewColor(int color) {
            mTextView.setTextColor(color);
        }

        void setGraphLineColor(int color) {
            this.mUniqueColor = color;
            this.updateView();
        }

        void setEnabled(boolean isEnabled) {
            this.mIsEnabled = isEnabled;
            this.updateView();
        }

        private void updateView() {
            runOnUiThread(new Runnable() {
                @SuppressLint("SetTextI18n")
                @Override
                public void run() {
                    if (mIsEnabled) {
                        mLineGraphSeries.setColor(mUniqueColor);
                        mTextView.setText(getSensorName());
                    } else {
                        mLineGraphSeries.setColor(Color.GRAY);
                        mTextView.setText(getSensorName() + " (Loading...)");
                    }
                }
            });
        }
    }
}