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
import android.app.AlertDialog;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.ServiceConnection;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
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
import android.widget.Toast;

import com.ant.ant_manager.R;
import com.ant.ant_manager.controller.ANTControllerBroadcastReceiver;
import com.ant.ant_manager.controller.ANTControllerService;
import com.ant.ant_manager.model.ANTApp;

import java.util.ArrayList;

public class AppManagerActivity extends Activity {
    // ANTControllerService
    private static final String TAG = "AppManagerActivity";
    private ANTControllerService mControllerServiceStub = null;
    private AppManagerActivity self = this;

    private AppListAdapter mAppListAdapter;
    private ArrayList<AppListItem> mAppList = new ArrayList<>();

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
        actionBar.setTitle("Apps");
        actionBar.setDisplayHomeAsUpEnabled(true);
        actionBar.setLogo(com.ant.ant_manager.R.drawable.icon_app_manager);
        actionBar.setDisplayUseLogoEnabled(true);

        ListView appListView = (ListView) findViewById(R.id.mainListView);
        this.mAppListAdapter = new AppListAdapter();
        appListView.setAdapter(mAppListAdapter);
        appListView.setOnItemClickListener(mItemClickListener);
    }

    protected void onResume() {
        super.onResume();
        mAppListAdapter.updateUI();
    }

    private AdapterView.OnItemClickListener mItemClickListener = new AdapterView
            .OnItemClickListener() {
        @Override
        public void onItemClick(AdapterView<?> parent, View view, int position, long l_position) {
            final AppListItem item = (AppListItem) parent.getAdapter().getItem(position);

            if (item.isDefaultApp()) {
                Toast.makeText(self, "You cannot delete default app.", Toast.LENGTH_LONG).show();
            } else {
                AlertDialog.Builder alt_bld = new AlertDialog.Builder(AppManagerActivity.this);
                alt_bld.setMessage("Delete this app ?").setCancelable(false).setPositiveButton
                        ("Yes", new DialogInterface.OnClickListener() {

                    public void onClick(DialogInterface dialog, int id) {
                        // Action for 'Yes' Button
                        int appId = item.getAppID();
                        if (mControllerServiceStub != null)
                            mControllerServiceStub.removeAppOneWay(appId);
                    }
                }).setNegativeButton("No", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        // Action for 'NO' Button
                        dialog.cancel();
                    }
                });

                AlertDialog alert = alt_bld.create();
                alert.setTitle(item.getAppName());
                Drawable d = new BitmapDrawable(getResources(), item.getAppIcon());
                alert.setIcon(d);
                alert.show();
            }
        }

    };

    public void updateUI() {
        if (this.mAppListAdapter != null) {
            this.mAppListAdapter.updateUI();
        }
    }

    private class AppListAdapter extends BaseAdapter {
        @Override
        public int getCount() {
            return mAppList.size();
        }

        @Override
        public Object getItem(int position) {
            return mAppList.get(position);
        }

        public long getItemId(int position) {
            return position;
        }

        @Override
        public View getView(int position, View convertView, ViewGroup parent) {
            if (convertView == null) {
                LayoutInflater mInflater = (LayoutInflater) self.getSystemService(Context
                        .LAYOUT_INFLATER_SERVICE);
                convertView = mInflater.inflate(com.ant.ant_manager.R.layout
                        .template_listview_item_icon, parent, false);
            }
            if (mAppList.size() != 0) {
                TextView ci_nickname_text = (TextView) convertView.findViewById(R.id.tv_title);
                ci_nickname_text.setText(mAppList.get(position).getAppName());
                TextView ci_content_text = (TextView) convertView.findViewById(R.id.tv_subTitle);

                String state = "";
                AppListItem appListItem = mAppList.get(position);
                int appState = appListItem.getAppState();
                switch (appState) {
                    case ANTApp.State_Initialized:
                    case ANTApp.State_Initializing:
                    case ANTApp.State_Installing:
                        ci_content_text.setTextColor(Color.GRAY);
                        break;
                    case ANTApp.State_Ready:
                        ci_content_text.setTextColor(Color.WHITE);
                        break;
                    case ANTApp.State_Launching:
                        ci_content_text.setTextColor(Color.RED);
                        break;
                    case ANTApp.State_Running:
                        ci_content_text.setTextColor(Color.RED);
                        break;
                    case ANTApp.State_Removing:
                    case ANTApp.State_Removed:
                        ci_content_text.setTextColor(Color.GRAY);
                        break;
                    default:
                        // ignore
                        break;
                }
                ci_content_text.setText(state);

                ImageView iv = (ImageView) convertView.findViewById(R.id.imageView11);
                iv.setImageBitmap(mAppList.get(position).getAppIcon());
            }
            return convertView;
        }

        void updateUI() {
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    notifyDataSetChanged();
                }
            });
        }
    }

    public void updateAppList() {
        if (this.mControllerServiceStub == null) return;
        ArrayList<ANTApp> appList = this.mControllerServiceStub.getAppList();

        this.mAppList.clear();
        for (ANTApp app : appList) {
            int appId = app.getAppId();
            String appName = app.getName();
            String iconImagePath = app.getIconImagePath();
            Bitmap iconBitmap = BitmapFactory.decodeFile(iconImagePath);
            int appState = app.getState();
            boolean isDefaultApp = app.isDefaultApp();
            this.mAppList.add(new AppListItem(appId, appName, iconBitmap, appState, isDefaultApp));
        }
        this.updateUI();
    }

    private class AppListItem {
        private int mAppID;
        private String mAppName;
        private Bitmap mAppIcon;
        private int mAppState;
        private boolean mIsDefaultApp;

        AppListItem(int appID, String appName, Bitmap appIcon, int appState, boolean isDefaultApp) {
            this.mAppID = appID;
            this.mAppName = appName;
            this.mAppIcon = appIcon;
            this.mAppState = appState;
            this.mIsDefaultApp = isDefaultApp;
        }

        int getAppID() {
            return this.mAppID;
        }

        String getAppName() {
            return this.mAppName;
        }

        Bitmap getAppIcon() {
            return mAppIcon;
        }

        int getAppState() {
            return this.mAppState;
        }

        boolean isDefaultApp() {
            return this.mIsDefaultApp;
        }

        void updateAppState(int appState) {
            this.mAppState = appState;
            self.updateUI();
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
    }

    private ServiceConnection mControllerServiceConnection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName componentName, IBinder inputBinder) {
            ANTControllerService.ControllerBinder serviceBinder = (ANTControllerService
                    .ControllerBinder) inputBinder;
            mControllerServiceStub = serviceBinder.getService();

            // Update app list
            updateAppList();
        }

        @Override
        public void onServiceDisconnected(ComponentName componentName) {
            Log.d(TAG, "onServiceDisconnected()");
            mControllerServiceStub = null;
        }
    };

    class PrivateControllerBroadcastReceiver extends ANTControllerBroadcastReceiver {
        PrivateControllerBroadcastReceiver() {
            this.setOnAppStateChangedListener(new OnAppStateChangedListener() {
                @Override
                public void onAppStateChanged(int appId, int appState) {
                    for (AppListItem appListItem : mAppList) {
                        if (appListItem.getAppID() == appId) {
                            if (appState == ANTApp.State_Removed) {
                                // App is removed
                                mAppList.remove(appListItem);
                            } else {
                                // App state is updated
                                appListItem.updateAppState(appState);
                            }
                        }
                    }
                }
            });
        }
    }
}