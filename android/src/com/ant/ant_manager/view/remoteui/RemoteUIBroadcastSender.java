package com.ant.ant_manager.view.remoteui;

import android.app.Service;
import android.content.Intent;

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
public class RemoteUIBroadcastSender {
    static public void onRemoteNotiUI(Service service, int appId, String description, String
            attachedFilePath) {
        Intent broadcastIntent = new Intent();
        broadcastIntent.setAction(RemoteUIBroadcastReceiver.ACTION);
        broadcastIntent.putExtra(RemoteUIBroadcastReceiver.KEY_EVENT_TYPE,
                RemoteUIBroadcastReceiver.EVENT_TYPE_ON_REMOTE_NOTI_UI);
        broadcastIntent.putExtra(RemoteUIBroadcastReceiver.KEY_ON_REMOTE_NOTI_UI_APP_ID, appId);
        broadcastIntent.putExtra(RemoteUIBroadcastReceiver.KEY_ON_REMOTE_NOTI_UI_DESCRIPTION,
                description);
        broadcastIntent.putExtra(RemoteUIBroadcastReceiver
                .KEY_ON_REMOTE_NOTI_UI_ATTACHED_FILE_PATH, attachedFilePath);
        service.sendBroadcast(broadcastIntent);
    }
}
