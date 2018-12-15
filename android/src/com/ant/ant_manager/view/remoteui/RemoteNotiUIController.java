package com.ant.ant_manager.view.remoteui;

import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Build;
import android.os.Bundle;
import android.support.v4.app.NotificationCompat;

import com.ant.ant_manager.R;
import com.ant.ant_manager.model.ANTApp;
import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.ObjectMapper;

import java.io.File;
import java.io.IOException;
import java.util.Map;

/* Copyright (c) 2017-2018 SKKU ESLAB, and contributors. All rights reserved.
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

public class RemoteNotiUIController {
    RemoteUIService mService;

    public void initialize(RemoteUIService service) {
        this.mService = service;

        // Create notification channel (from Android Oreo+)
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            NotificationManager nm = (NotificationManager) this.mService.getSystemService(Context
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

    // Remote UI Notification Handler
    public void showRemoteUINoti(String senderUri, String data, String attachedFilePath) {
        // Get ANTApp object from senderUri
        String[] senderUriTokens = senderUri.split("/");
        String appIdStr = senderUriTokens[3];
        int appId = Integer.valueOf(appIdStr);
        ANTApp app = this.mService.getANTControllerStub().getApp(appId);

        // Make notification
        this.makeNotification(app, data, attachedFilePath);

        // Update popup view
        ObjectMapper jsonMapper = new ObjectMapper();
        Map<String, String> messageMap;
        try {
            messageMap = jsonMapper.readValue(data, new TypeReference<Map<String, String>>() {
            });
        } catch (IOException e) {
            e.printStackTrace();
            return;
        }
        String description = messageMap.get("description");
        RemoteUIBroadcastSender.onRemoteNotiUI(this.mService, appId, description, attachedFilePath);
    }

    private static final int kNotificationId = 1234;
    private final String mNotiChannelId = "remoteUINoti";
    private final String mNotiChannelName = "ANT Remote UI Notification";

    private void makeNotification(ANTApp app, String legacyData, String attachedFilePath) {
        Context context = this.mService;

        Intent notificationIntent = new Intent(context, RemoteNotiUIActivity.class);
        Bundle extras = new Bundle();
        extras.putString(RemoteNotiUIActivity.INTENT_KEY_LEGACY_DATA, legacyData);
        extras.putInt(RemoteNotiUIActivity.INTENT_KEY_APP_ID, app.getAppId());
        extras.putString(RemoteNotiUIActivity.INTENT_KEY_ATTACHED_FILE_PATH, attachedFilePath);
        notificationIntent.putExtras(extras);

        PendingIntent contentIntent = PendingIntent.getActivity(context, 1, notificationIntent,
                PendingIntent.FLAG_UPDATE_CURRENT);

        ObjectMapper jsonMapper = new ObjectMapper();
        Map<String, String> messageMap;
        try {
            messageMap = jsonMapper.readValue(legacyData, new TypeReference<Map<String, String>>() {
            });
        } catch (IOException e) {
            e.printStackTrace();
            return;
        }

        NotificationCompat.Builder builder;
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            // Set notification channel (from Android Oreo+)
            builder = new NotificationCompat.Builder(context, this.mNotiChannelId);
        } else {
            builder = new NotificationCompat.Builder(context);
        }

        builder.setCategory("" + app.getAppId()).setContentTitle(app.getName());
        builder.setContentText(messageMap.get("description"));
        builder.setTicker(" " + messageMap.get("appTitle"));

        Bitmap largeIconBitmap;
        if (new File(app.getIconImagePath()).exists()) {
            largeIconBitmap = BitmapFactory.decodeFile(app.getIconImagePath());
        } else {
            largeIconBitmap = BitmapFactory.decodeResource(this.mService.getResources(), R.mipmap
                    .ic_launcher);
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
}
