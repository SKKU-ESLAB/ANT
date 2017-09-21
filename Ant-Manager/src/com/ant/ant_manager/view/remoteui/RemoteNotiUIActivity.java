package com.ant.ant_manager.view.remoteui;

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
import com.ant.ant_manager.controller.LegacyJSONParser;
import com.ant.ant_manager.controller.ANTControllerService;
import com.ant.ant_manager.model.ANTApp;
import com.ant.ant_manager.view.EventLogViewerActivity;

import java.io.File;
import java.io.FileInputStream;

import static android.content.ContentValues.TAG;

public class RemoteNotiUIActivity extends Activity {
    // ANTControllerService
    private ANTControllerService mControllerServiceStub = null;

    private LinearLayout mLayout;
    private String mIsCheckNoti;
    private int mAppId;

    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_remote_noti_ui);

        // Parameters
        Bundle extras = getIntent().getExtras();
        int id = extras.getInt("notificationId");
        String jsonString = extras.getString("jsonData");
        this.mIsCheckNoti = extras.getString("mIsCheckNoti");

        LegacyJSONParser jp = new LegacyJSONParser(jsonString);
        String appId = jp.getValueByKey("mAppID");
        this.mAppId = Integer.parseInt(appId);
        String isNoti = jp.getValueByKey("isNoti");

        this.mLayout = (LinearLayout) findViewById(R.id.dynamicLayout);

        // Initialize UI
        while (jp.hasMoreValue()) {
            String ret[] = new String[2];
            ret = jp.getNextKeyValue();

            if (ret[0].equals("text")) {
                TextView tview = new TextView(this);
                tview.setText(ret[1]);
                tview.setTextSize(20);
                tview.setTextColor(Color.WHITE);
                tview.setGravity(Gravity.CENTER);

                LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(LinearLayout
                        .LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT);
                lp.gravity = Gravity.CENTER;
                lp.setMargins(60, 20, 60, 20);

                tview.setLayoutParams(lp);
                mLayout.addView(tview);

            } else if (ret[0].equals("img")) {

                try {
                    ImageView iv = new ImageView(this);
                    FileInputStream is;


                    if (isNoti.equals("2")) {
                        File cloudDir = mControllerServiceStub.getSettings().getCloudDir();
                        is = new FileInputStream(new File(cloudDir, ret[1]));
                    } else {
                        File remoteUIDir = mControllerServiceStub.getSettings().getRemoteUIDir();
                        is = new FileInputStream(new File(remoteUIDir, ret[1]));

                    }
                    iv.setImageDrawable(Drawable.createFromStream(is, ret[1]));

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
            } else if (ret[0].equals("dateTime")) {
                TextView tview = new TextView(this);
                tview.setText(ret[1]);
                tview.setTextSize(20);
                tview.setTextColor(Color.RED);
                tview.setGravity(Gravity.CENTER);

                LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(LinearLayout
                        .LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT);
                lp.gravity = Gravity.CENTER;
                lp.setMargins(60, 20, 60, 20);
                tview.setLayoutParams(lp);
                mLayout.addView(tview);
            } else if (ret[0].equals("description")) {
                TextView tview = new TextView(this);
                tview.setText(ret[1]);
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
                if (mIsCheckNoti.equals("1")) {
                    Intent intent = new Intent(RemoteNotiUIActivity.this, EventLogViewerActivity
                            .class);
                    startActivity(intent);
                }
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