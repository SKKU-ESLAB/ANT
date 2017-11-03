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

import com.jjoe64.graphview.GraphView;
import com.jjoe64.graphview.series.DataPoint;
import com.jjoe64.graphview.series.LineGraphSeries;
import com.ant.ant_manager.R;
import com.ant.ant_manager.controller.LegacyJSONParser;
import com.ant.ant_manager.controller.ANTControllerBroadcastReceiver;
import com.ant.ant_manager.controller.ANTControllerService;

import static android.R.id.message;

abstract public class SensorViewerActivity extends Activity {
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

    SensorData sensor1;
    SensorData sensor2;
    SensorData sensor3;
    SensorData sensor4;
    SensorData sensor5;
    SensorData sensor6;
    SensorData sensor7;

    String sensorName1 = "Touch";
    String sensorName2 = "Accelerometer";
    String sensorName3 = "Motion";
    String sensorName4 = "Sound";
    String sensorName5 = "Light";
    String sensorName6 = "Vibration";
    String sensorName7 = "Temperature";

    private final String TAG = "SensorViewer";
    private boolean mIsUIReady = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

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


        sensor1 = new SensorData(0, 100, sensorName1, (GraphView) findViewById(R.id.graph1),
                (TextView) findViewById(R.id.textView11));
        sensor2 = new SensorData(0, 100, sensorName2, (GraphView) findViewById(R.id.graph2),
                (TextView) findViewById(R.id.textView22));
        sensor3 = new SensorData(0, 100, sensorName3, (GraphView) findViewById(R.id.graph3),
                (TextView) findViewById(R.id.textView33));
        sensor4 = new SensorData(0, 100, sensorName4, (GraphView) findViewById(R.id.graph4),
                (TextView) findViewById(R.id.textView4));
        sensor5 = new SensorData(0, 100, sensorName5, (GraphView) findViewById(R.id.graph5),
                (TextView) findViewById(R.id.textView5));
        sensor6 = new SensorData(0, 100, sensorName6, (GraphView) findViewById(R.id.graph6),
                (TextView) findViewById(R.id.textView6));
        sensor7 = new SensorData(0, 100, sensorName7, (GraphView) findViewById(R.id.graph7),
                (TextView) findViewById(R.id.textView7));

        sensor1.setGraphLineColor(Color.RED);
        sensor2.setGraphLineColor(Color.BLUE);
        sensor3.setGraphLineColor(Color.WHITE);
        sensor4.setGraphLineColor(Color.YELLOW);
        sensor5.setGraphLineColor(Color.WHITE);
        sensor6.setGraphLineColor(Color.BLUE);
        sensor7.setGraphLineColor(Color.YELLOW);
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

    abstract public void onMessageFromTarget(String listenerName, String message);

    public void onSensorViewerMessage(String message) {
        LegacyJSONParser jp = new LegacyJSONParser(message);

        Double val1 = Double.parseDouble(jp.getValueByKey(sensor1.getSensorName()));
        Double val2 = Double.parseDouble(jp.getValueByKey(sensor2.getSensorName()));
        Double val3 = Double.parseDouble(jp.getValueByKey(sensor3.getSensorName()));
        Double val4 = Double.parseDouble(jp.getValueByKey(sensor4.getSensorName()));
        Double val5 = Double.parseDouble(jp.getValueByKey(sensor5.getSensorName()));
        Double val6 = Double.parseDouble(jp.getValueByKey(sensor6.getSensorName()));
        Double val7 = Double.parseDouble(jp.getValueByKey(sensor7.getSensorName()));

        if (this.mIsUIReady == true) {
            if (val1 >= 0) {
                sensor1.setEnabled(true);
                sensor1.setCurValue(val1);
            } else {
                sensor1.setEnabled(false);
            }
            if (val2 >= 0) {
                sensor2.setEnabled(true);
                sensor2.setCurValue(val2);
            } else {
                sensor2.setEnabled(false);
            }
            if (val3 >= 0) {
                sensor3.setEnabled(true);
                sensor3.setCurValue(val3);
            } else {
                sensor3.setEnabled(false);
            }
            if (val4 >= 0) {
                sensor4.setEnabled(true);
                sensor4.setCurValue(val4);
            } else {
                sensor4.setEnabled(false);
            }
            if (val5 >= 0) {
                sensor5.setEnabled(true);
                sensor5.setCurValue(val5);
            } else {
                sensor5.setEnabled(false);
            }
            if (val6 >= 0) {
                sensor6.setEnabled(true);
                sensor6.setCurValue(val6);
            } else {
                sensor6.setEnabled(false);
            }
            if (val7 >= 0) {
                sensor7.setEnabled(true);
                sensor7.setCurValue(val7);
            } else {
                sensor7.setEnabled(false);
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

                Double sensor1Data = sensor1.getCurValue();
                Double sensor2Data = sensor2.getCurValue();
                Double sensor3Data = sensor3.getCurValue();
                Double sensor4Data = sensor4.getCurValue();
                Double sensor5Data = sensor5.getCurValue();
                Double sensor6Data = sensor6.getCurValue();
                Double sensor7Data = sensor7.getCurValue();

                sensor1.appendData(new DataPoint(graph2LastXValue, sensor1Data), true,
                        sensor1.getMax());
                sensor2.appendData(new DataPoint(graph2LastXValue, sensor2Data), true,
                        sensor2.getMax());
                sensor3.appendData(new DataPoint(graph2LastXValue, sensor3Data), true,
                        sensor3.getMax());
                sensor4.appendData(new DataPoint(graph2LastXValue, sensor4Data), true,
                        sensor4.getMax());
                sensor5.appendData(new DataPoint(graph2LastXValue, sensor5Data), true,
                        sensor5.getMax());
                sensor6.appendData(new DataPoint(graph2LastXValue, sensor6Data), true,
                        sensor6.getMax());
                sensor7.appendData(new DataPoint(graph2LastXValue, sensor7Data), true,
                        sensor7.getMax());

                //mSeries2.appendData(new DataPoint(graph2LastXValue,
                // randNum), true, 40);
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
                    Log.d(TAG, "Message coming for " + listenerName + ": " + message);
                    if (listenerName.compareTo("sensorviewer") == 0) {
                        onSensorViewerMessage(data);
                    } else {
                        onMessageFromTarget(listenerName, data);
                    }
                }
            });
        }
    }

    class SensorData {
        int min;
        int max;
        double curValue;
        String sensorName;

        GraphView graph;
        TextView textV;

        LineGraphSeries<DataPoint> mSeries;
        int mUniqueColor;
        boolean mIsEnabled = false;

        public SensorData(int min, int max, String sensorName, GraphView graph, TextView textV) {
            this.min = min;
            this.max = max;
            this.graph = graph;
            this.curValue = 0.0;
            this.sensorName = sensorName;
            this.textV = textV;
            this.textV.setText(sensorName + "(Disconnected)");
            mSeries = new LineGraphSeries<DataPoint>();

            graph.addSeries(mSeries);
            graph.getViewport().setXAxisBoundsManual(true);
            graph.getViewport().setMinX(min);
            graph.getViewport().setMaxX(max);

            graph.getViewport().setYAxisBoundsManual(false);

            mSeries.setThickness(5);

        }

        public void setCurValue(double var) {
            this.curValue = var;
        }

        public double getCurValue() {
            return this.curValue;
        }


        public void appendData(DataPoint a, boolean b, int c) {
            mSeries.appendData(a, b, c);
        }

        public int getMax() {
            return this.max;
        }

        public String getSensorName() {
            return this.sensorName;
        }

        public void setTextViewColor(int color) {
            textV.setTextColor(color);
        }

        public void setGraphLineColor(int color) {
            this.mUniqueColor = color;
            this.updateView();
        }

        public void setEnabled(boolean isEnabled) {
            this.mIsEnabled = isEnabled;
            this.updateView();
        }

        private void updateView() {
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    if (mIsEnabled == true) {
                        mSeries.setColor(mUniqueColor);
                        textV.setText(sensorName);
                    } else {
                        mSeries.setColor(Color.GRAY);
                        textV.setText(sensorName + " (Disconnected)");
                    }
                }
            });
        }
    }
}