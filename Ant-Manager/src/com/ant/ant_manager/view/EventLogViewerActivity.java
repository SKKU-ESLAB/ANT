package com.ant.ant_manager.view;

/* Copyright (c) 2017 SKKU ESLAB, and contributors. All rights reserved.
 *
 * Contributor: Dongig Sin<dongig@skku.edu>
 *              Gyeonghwan Hong<redcarrottt@gmail.com>
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
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.os.Bundle;
import android.os.IBinder;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.TextView;

import com.ant.ant_manager.R;
import com.ant.ant_manager.controller.ANTControllerBroadcastReceiver;
import com.ant.ant_manager.controller.ANTControllerService;
import com.ant.ant_manager.model.ANTApp;
import com.ant.ant_manager.model.ANTEvent;
import com.ant.ant_manager.view.remoteui.RemoteNotiUIActivity;

import java.util.ArrayList;

import static android.content.ContentValues.TAG;

public class EventLogViewerActivity extends Activity {
    // ANTControllerService
    private ANTControllerService mControllerServiceStub = null;
    private PrivateControllerBroadcastReceiver mControllerBroadcastReceiver;

    // Intent
    private static final String INTENT_KEY_APP_ID = "appId";

    private ListView mEventListView;
    private EventListAdapter mEventListAdapter;

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(com.ant.ant_manager.R.layout.template_listview);

        // Initialize UI
        this.initializeUI();

        // Connect controller service
        this.connectControllerService();
    }

    protected void onDestroy() {
        super.onDestroy();
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

    private void initializeUI() {
        ActionBar actionBar = getActionBar();
        assert actionBar != null;
        actionBar.setTitle("Event Log Viewer");
        actionBar.setDisplayHomeAsUpEnabled(true);
        actionBar.setLogo(com.ant.ant_manager.R.drawable.eventlogger);
        actionBar.setDisplayUseLogoEnabled(true);

        mEventListView = (ListView) findViewById(com.ant.ant_manager.R.id.mainListView);
        mEventListAdapter = new EventListAdapter(this);
        mEventListView.setAdapter(mEventListAdapter);
        mEventListView.setOnItemClickListener(mItemClickListener);
    }

    protected void onResume() {
        super.onResume();
        mEventListAdapter.updateUI();
    }

    private AdapterView.OnItemClickListener mItemClickListener = new AdapterView
            .OnItemClickListener() {
        @Override
        public void onItemClick(AdapterView<?> parent, View view, int position, long l_position) {
            ANTEvent item = (ANTEvent) parent.getAdapter().getItem(position);
            Log.d("ANT", item.getEventJsonData());
            Intent notificationIntent = new Intent(EventLogViewerActivity.this,
                    RemoteNotiUIActivity.class);
            Bundle extras = new Bundle();
            extras.putString("jsonData", item.getEventJsonData());
            extras.putString("checkNoti", "0");
            notificationIntent.putExtras(extras);
            startActivity(notificationIntent);
        }
    };

    public void updateUI() {
        if (mEventListAdapter != null) {
            mEventListAdapter.updateUI();
        }
    }

    private class EventListAdapter extends BaseAdapter {
        private Context mContext;
        private ArrayList<ANTEvent> eventList = new ArrayList<ANTEvent>();
        private int pos;

        EventListAdapter(Context mContext) {
            this.mContext = mContext;
            this.updateEventList();
        }

        private void updateEventList() {
            if (mControllerServiceStub != null) {
                this.eventList.clear();
                this.eventList.addAll(mControllerServiceStub.getEventList());
            }
        }

        void updateUI() {
            this.updateEventList();
            this.notifyDataSetChanged();
        }

        @Override
        public int getCount() {
            return eventList.size();
        }

        @Override
        public Object getItem(int position) {
            return eventList.get(position);
        }

        public long getItemId(int position) {
            return position;
        }

        @Override
        public View getView(int position, View convertView, ViewGroup parent) {
            if (convertView == null) {
                LayoutInflater inflater = (LayoutInflater) mContext.getSystemService(Context
                        .LAYOUT_INFLATER_SERVICE);
                convertView = inflater.inflate(com.ant.ant_manager.R.layout
                        .template_listview_item_icon, parent, false);
            }
            pos = position;
            if (eventList.size() != 0) {
                TextView textAppName = (TextView) convertView.findViewById(R.id.tv_title);
                textAppName.setText(eventList.get(pos).getEventAppName());

                TextView textDescription = (TextView) convertView.findViewById(R.id.tv_subTitle);

                String desc = "";
                desc = eventList.get(pos).getEventDescription();
                textDescription.setTextColor(Color.WHITE);
                textDescription.setText(desc);


                TextView textDate = (TextView) convertView.findViewById(R.id.tv_status);
                textDate.setTextColor(Color.WHITE);

                String timeStr = eventList.get(pos).getEventTime();
                String[] dateTime = timeStr.split(" ");

                Log.d("ANT", String.valueOf(dateTime.length) + "  " + dateTime[0] + " " +
                        dateTime[1] + "length:" + dateTime[1].length());

                if (dateTime[1].length() == 6) {

                    String time = dateTime[1].substring(0, 2) + ":" + dateTime[1].substring(2, 4)
                            + ":" + dateTime[1].substring(4, 6);
                    textDate.setText(dateTime[0] + "\n" + time);
                } else {
                    textDate.setText(timeStr);
                }

                ImageView iv = (ImageView) convertView.findViewById(R.id.imageView11);
                int appId = Integer.parseInt(eventList.get(pos).getEventAppId());
                ANTApp app = mControllerServiceStub.getApp(appId);
                iv.setImageBitmap(BitmapFactory.decodeFile(app.getIconImagePath()));
            }
            return convertView;
        }
    }

    private void connectControllerService() {
        Intent serviceIntent = new Intent(this, ANTControllerService.class);
        this.bindService(serviceIntent, this.mControllerServiceConnection, Context
                .BIND_AUTO_CREATE);
    }

    private void disconnectControllerService() {
        if (this.mControllerServiceConnection != null)
            this.unbindService(this.mControllerServiceConnection);
        if(this.mControllerBroadcastReceiver != null)
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

            // Update UI
            updateUI();
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
            this.setOnReceivedEventListener(new OnReceivedEventListener() {
                @Override
                public void onReceivedEvent(int appId, String legacyData, boolean isNoti) {
                    updateUI();
                }
            });
        }
    }
}