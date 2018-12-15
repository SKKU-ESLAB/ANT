package com.ant.ant_manager.view.remoteui;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

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
public class RemoteUIBroadcastReceiver extends BroadcastReceiver {
    private static final String TAG = "RemoteUIBroadcastReceiver";
    public static final String ACTION = "com.ant.ant_manager.view.remoteui.broadcastreceiver";
    public static final String KEY_EVENT_TYPE = "eventType";

    public static final String EVENT_TYPE_ON_REMOTE_NOTI_UI = "onRemoteNotiUI";
    public static final String KEY_ON_REMOTE_NOTI_UI_APP_ID = "appId";
    public static final String KEY_ON_REMOTE_NOTI_UI_DESCRIPTION = "description";
    public static final String KEY_ON_REMOTE_NOTI_UI_ATTACHED_FILE_PATH = "attachedFilePath";

    @Override
    public void onReceive(Context context, Intent intent) {
        String action = intent.getAction();
        if (action.compareTo(ACTION) == 0) {
            String eventType = intent.getStringExtra(KEY_EVENT_TYPE);
            if (eventType.compareTo(EVENT_TYPE_ON_REMOTE_NOTI_UI) == 0) {
                int appId = intent.getIntExtra(KEY_ON_REMOTE_NOTI_UI_APP_ID, -1);
                String description = intent.getStringExtra(KEY_ON_REMOTE_NOTI_UI_DESCRIPTION);
                String attachedFilePath = intent.getStringExtra
                        (KEY_ON_REMOTE_NOTI_UI_ATTACHED_FILE_PATH);
                if (this.mOnRemoteNotiUIListener != null) {
                    this.mOnRemoteNotiUIListener.onRemoteNotiUI(appId, description,
                            attachedFilePath);
                }
            }
        }
    }

    private OnRemoteNotiUIListener mOnRemoteNotiUIListener;

    public void setOnRemoteNotiUIListener(OnRemoteNotiUIListener listener) {
        this.mOnRemoteNotiUIListener = listener;
    }

    public interface OnRemoteNotiUIListener {
        public void onRemoteNotiUI(int appId, String description, String attachedFilePath);
    }
}
