package com.ant.ant_manager.view.remoteui;

import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Build;
import android.os.Bundle;
import android.os.IBinder;
import android.support.v4.app.NotificationCompat;
import android.util.Log;

import com.ant.ant_manager.R;
import com.ant.ant_manager.controller.ANTControllerBroadcastReceiver;
import com.ant.ant_manager.controller.ANTControllerService;
import com.ant.ant_manager.controller.LegacyJSONParser;
import com.ant.ant_manager.model.ANTApp;

import java.io.File;

/* Copyright (c) 2018, contributors. All rights reserved.
 *
 * Contributor: Gyeonghwan Hong <redcarrottt@gmail.com>
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
public class RemoteUIService extends Service {
    private final String TAG = "RemoteUIService";

    // Remote UI Notification Handler
    private void onRemoteUINoti(String senderUri, String data, String attachedFilePath) {
        // Get ANTApp object from senderUri
        String[] senderUriTokens = senderUri.split("/");
        String appIdStr = senderUriTokens[3];
        int appId = Integer.valueOf(appIdStr);
        ANTApp app = this.mControllerServiceStub.getApp(appId);

        this.makeNotification(app, data, attachedFilePath);
    }

    private static final int kNotificationId = 1234;
    private final String mNotiChannelId = "remoteUINoti";
    private final String mNotiChannelName = "ANT Remote UI Notification";

    private void makeNotification(ANTApp app, String legacyData, String attachedFilePath) {
        Context context = this;

        Intent notificationIntent = new Intent(context, RemoteNotiUIActivity.class);
        Bundle extras = new Bundle();
        extras.putString(RemoteNotiUIActivity.INTENT_KEY_LEGACY_DATA, legacyData);
        extras.putInt(RemoteNotiUIActivity.INTENT_KEY_APP_ID, app.getAppId());
        extras.putString(RemoteNotiUIActivity.INTENT_KEY_ATTACHED_FILE_PATH, attachedFilePath);
        notificationIntent.putExtras(extras);

        PendingIntent contentIntent = PendingIntent.getActivity(context, 1, notificationIntent,
                PendingIntent.FLAG_UPDATE_CURRENT);

        LegacyJSONParser jp = new LegacyJSONParser(legacyData);

        NotificationCompat.Builder builder;
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            // Set notification channel (from Android Oreo+)
            builder = new NotificationCompat.Builder(context, this.mNotiChannelId);
        } else {
            builder = new NotificationCompat.Builder(context);
        }

        builder.setCategory("" + app.getAppId()).setContentTitle(app.getName());
        builder.setContentText(jp.getValueByKey("description"));
        builder.setTicker(" " + jp.getValueByKey("appTitle"));

        Bitmap largeIconBitmap;
        if (new File(app.getIconImagePath()).exists()) {
            largeIconBitmap = BitmapFactory.decodeFile(app.getIconImagePath());
        } else {
            largeIconBitmap = BitmapFactory.decodeResource(getResources(), R.mipmap.ic_launcher);
        }
        builder.setLargeIcon(largeIconBitmap);
        builder.setSmallIcon(R.drawable.notiicon);
        if (attachedFilePath != null && !attachedFilePath.isEmpty() && new File(attachedFilePath)
                .exists()) {
            Bitmap pictureBitmap = BitmapFactory.decodeFile(attachedFilePath);
            NotificationCompat.BigPictureStyle bigPictureStyle = new NotificationCompat
                    .BigPictureStyle(builder);
            bigPictureStyle.bigPicture(pictureBitmap);

        }

        builder.setContentIntent(contentIntent);
        builder.setAutoCancel(true);
        builder.setWhen(System.currentTimeMillis());
        builder.setDefaults(Notification.DEFAULT_SOUND | Notification.DEFAULT_VIBRATE |
                Notification.DEFAULT_LIGHTS);
        builder.setNumber(1);
        Notification noti = builder.build();

        NotificationManager nm = (NotificationManager) context.getSystemService(Context
                .NOTIFICATION_SERVICE);
        assert nm != null;
        nm.notify(kNotificationId, noti);
    }

    // Connection to ANTControllerService
    private ANTControllerService mControllerServiceStub;
    private PrivateControllerBroadcastReceiver mControllerBroadcastReceiver;

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
                public void onReceivedDataFromTarget(String senderUri, String listenerName,
                                                     String data, String attachedFilePath) {
                    Log.d(TAG, "Message coming for " + listenerName + ": " + data);
                    if (listenerName.compareToIgnoreCase("RemoteNotiUI") == 0) {
                        onRemoteUINoti(senderUri, data, attachedFilePath);
                    }
                }
            });
        }
    }

    // Android Service
    private int mBindersCount = 0;
    private final IBinder mBinder;

    // Android Service
    public RemoteUIService() {
        this.mBinder = new RemoteUIBinder();
    }

    public void initialize() {
        this.connectControllerService();

        // Create notification channel (from Android Oreo+)
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            NotificationManager nm = (NotificationManager) this.getSystemService(Context
                    .NOTIFICATION_SERVICE);
            assert nm != null;

            NotificationChannel notiChannel = nm.getNotificationChannel(this.mNotiChannelId);
            if (notiChannel == null) {
                notiChannel = new NotificationChannel(this.mNotiChannelId, this.mNotiChannelName,
                        NotificationManager.IMPORTANCE_HIGH);
                nm.createNotificationChannel(notiChannel);
            }
        }
    }

    @Override
    public IBinder onBind(Intent intent) {
        this.mBindersCount++;
        return this.mBinder;
    }

    @Override
    public boolean onUnbind(Intent intent) {
        this.mBindersCount--;
        // If no one use this service, destroy connection with ANT device.
        if (this.mBindersCount == 0) {
            this.disconnectControllerService();
        }
        return false;
    }

    // Binder (for the caller of RemoteUIService)
    public class RemoteUIBinder extends android.os.Binder {
        public RemoteUIService getService() {
            return RemoteUIService.this;
        }
    }
}
