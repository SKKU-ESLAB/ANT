package com.ant.ant_manager.view.remoteui;

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
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.content.res.Resources;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.support.v4.app.NotificationCompat;

import com.ant.ant_manager.R;
import com.ant.ant_manager.controller.LegacyJSONParser;
import com.ant.ant_manager.model.ANTApp;

public class RemoteNotiUI {
    private static final int kNotificationId = 1234;

    // Input Json Format
    /*{
     * 		"appTitle":"collision detector",
     * 		mAppID: "2",
    		time: "2015-08-02. 15:02",
    		description: "collision detection!!!",
    		text: "collision is detected!!",
    		img: 234234
      }
    */
    public static void makeNotification(Context context, ANTApp app, String legacyData) {
        NotificationManager nm = (NotificationManager) context.getSystemService(Context
                .NOTIFICATION_SERVICE);
        Resources res = context.getResources();

        Intent notificationIntent = new Intent(context, RemoteNotiUIActivity.class);
        Bundle extras = new Bundle();
        extras.putString("jsonData", legacyData);
        extras.putString("checkNoti", "1");
        notificationIntent.putExtras(extras);

        PendingIntent contentIntent = PendingIntent.getActivity(context, 1, notificationIntent,
                PendingIntent.FLAG_UPDATE_CURRENT);

        LegacyJSONParser jp = new LegacyJSONParser(legacyData);
        int appId = Integer.parseInt(jp.getValueByKey("mAppID"));

        NotificationCompat.Builder builder = new NotificationCompat.Builder(context);
        builder.setCategory("" + appId).setContentTitle(app.getName());
        builder.setContentText(jp.getValueByKey("description"));
        builder.setTicker(" " + jp.getValueByKey("appTitle"));
        builder.setLargeIcon(BitmapFactory.decodeFile(app.getIconImagePath()));
        builder.setSmallIcon(R.mipmap.ic_launcher);
        builder.setContentIntent(contentIntent);
        builder.setAutoCancel(true);
        builder.setWhen(System.currentTimeMillis());
        builder.setDefaults(Notification.DEFAULT_SOUND | Notification.DEFAULT_VIBRATE |
                Notification.DEFAULT_LIGHTS);
        builder.setNumber(1);
        Notification n = builder.build();
        nm.notify(kNotificationId, n);
    }
}
