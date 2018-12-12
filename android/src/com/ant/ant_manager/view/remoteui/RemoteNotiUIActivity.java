package com.ant.ant_manager.view.remoteui;

/* Copyright (c) 2017-2018 SKKU ESLAB, and contributors. All rights reserved.
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
import android.app.NotificationManager;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.graphics.Color;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.os.IBinder;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Gravity;
import android.view.MenuItem;
import android.view.WindowManager;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.ant.ant_manager.R;
import com.ant.ant_manager.controller.ANTControllerService;
import com.ant.ant_manager.model.ANTApp;
import com.ant.ant_manager.view.EventLogViewerActivity;
import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.ObjectMapper;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.Map;

import static android.content.ContentValues.TAG;

public class RemoteNotiUIActivity extends Activity {
    // ANTControllerService
    private ANTControllerService mControllerServiceStub = null;

    private LinearLayout mLayout;
    private int mAppId;

    public static final String INTENT_KEY_NOTIFICATION_ID = "notificationId";
    public static final String INTENT_KEY_LEGACY_DATA = "jsonData";
    public static final String INTENT_KEY_APP_ID = "appId";
    public static final String INTENT_KEY_ATTACHED_FILE_PATH = "attachedFilePath";

    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_remote_noti_ui);

        // Parameters
        Bundle extras = getIntent().getExtras();
        int id = extras.getInt(INTENT_KEY_NOTIFICATION_ID);
        String jsonString = extras.getString(INTENT_KEY_LEGACY_DATA);
        this.mAppId = extras.getInt(INTENT_KEY_APP_ID);
        String attachedFilePath = extras.getString(INTENT_KEY_ATTACHED_FILE_PATH);

        ObjectMapper jsonMapper = new ObjectMapper();
        Map<String, String> messageMap;
        try {
            messageMap = jsonMapper.readValue(jsonString, new TypeReference<Map<String, String>>() {
            });
        } catch (IOException e) {
            e.printStackTrace();
            return;
        }

        this.mLayout = findViewById(R.id.dynamicLayout);

        // Initialize UI
        for (Map.Entry<String, String> entry : messageMap.entrySet()) {
            if (entry.getKey().equals("text")) {
                TextView tview = new TextView(this);
                tview.setText(entry.getValue());
                tview.setTextSize(20);
                tview.setTextColor(Color.WHITE);
                tview.setGravity(Gravity.CENTER);

                LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(LinearLayout
                        .LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT);
                lp.gravity = Gravity.CENTER;
                lp.setMargins(60, 20, 60, 20);

                tview.setLayoutParams(lp);
                mLayout.addView(tview);

            } else if (entry.getKey().equals("img") && attachedFilePath != null && !attachedFilePath
                    .isEmpty()) {
                try {
                    ImageView iv = new ImageView(this);
                    FileInputStream is;

                    File imgFile = new File(attachedFilePath);
                    is = new FileInputStream(imgFile);
                    iv.setImageDrawable(Drawable.createFromStream(is, entry.getValue()));

                    LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(LinearLayout
                            .LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT);
                    lp.gravity = Gravity.CENTER;
                    lp.setMargins(0, 0, 0, 0);

                    DisplayMetrics metrics = new DisplayMetrics();
                    WindowManager windowManager = (WindowManager) getApplicationContext()
                            .getSystemService(Context.WINDOW_SERVICE);
                    windowManager.getDefaultDisplay().getMetrics(metrics);
                    lp.width = metrics.widthPixels;
                    lp.height = metrics.heightPixels;

                    iv.setLayoutParams(lp);
                    mLayout.addView(iv);

                    is.close();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            } else if (entry.getKey().equals("dateTime")) {
                TextView tview = new TextView(this);
                tview.setText(entry.getValue());
                tview.setTextSize(20);
                tview.setTextColor(Color.RED);
                tview.setGravity(Gravity.CENTER);

                LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(LinearLayout
                        .LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT);
                lp.gravity = Gravity.CENTER;
                lp.setMargins(60, 20, 60, 20);
                tview.setLayoutParams(lp);
                mLayout.addView(tview);
            } else if (entry.getKey().equals("description")) {
                TextView tview = new TextView(this);
                tview.setText(entry.getValue());
                tview.setTextSize(20);
                tview.setTextColor(Color.RED);
                tview.setGravity(Gravity.CENTER);
                LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(LinearLayout
                        .LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT);
                lp.gravity = Gravity.CENTER;
                lp.setMargins(60, 20, 60, 20);
                tview.setLayoutParams(lp);
                mLayout.addView(tview);
            } else {
                //Do Nothing [mAppID, appName, etc]
            }
        }

        NotificationManager nm = (NotificationManager) getSystemService(Context
                .NOTIFICATION_SERVICE);
        nm.cancel(id);

        // Connect controller service
        connectControllerService();
    }

    private void initializeActionBar() {
        if (mControllerServiceStub == null) {
            Log.e(TAG, "Controller is not yet connected");
            return;
        }
        ANTApp targetApp = mControllerServiceStub.getApp(this.mAppId);

        try {
            ActionBar actionBar = this.getActionBar();
            actionBar.setTitle(targetApp.getName());
            actionBar.setDisplayHomeAsUpEnabled(true);

            Drawable dr = Drawable.createFromPath(targetApp.getIconImagePath());
            actionBar.setIcon(dr);
            actionBar.setDisplayUseLogoEnabled(true);

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case android.R.id.home:
                this.finish();

                Intent intent = new Intent(RemoteNotiUIActivity.this, EventLogViewerActivity.class);
                startActivity(intent);
                return true;
            default:
                return super.onOptionsItemSelected(item);
        }
    }

    private void connectControllerService() {
        Intent serviceIntent = new Intent(this, ANTControllerService.class);
        this.bindService(serviceIntent, this.mControllerServiceConnection, Context
                .BIND_AUTO_CREATE);
    }

    private ServiceConnection mControllerServiceConnection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName componentName, IBinder inputBinder) {
            ANTControllerService.ControllerBinder serviceBinder = (ANTControllerService
                    .ControllerBinder) inputBinder;
            mControllerServiceStub = serviceBinder.getService();

            // Update UI
            initializeActionBar();
        }

        @Override
        public void onServiceDisconnected(ComponentName componentName) {
            Log.d(TAG, "onServiceDisconnected()");
            mControllerServiceStub = null;
        }
    };
}