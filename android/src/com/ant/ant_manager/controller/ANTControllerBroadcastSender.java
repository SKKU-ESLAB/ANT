package com.ant.ant_manager.controller;

/* Copyright (c) 2017 SKKU ESLAB, and contributors. All rights reserved.
 *
 * Contributor: Gyeonghwan Hong<redcarrottt@gmail.com>
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

import android.app.Service;
import android.content.Intent;

import com.ant.ant_manager.model.ANTApp;
import com.ant.ant_manager.model.message.params.ParamFileListEntry;

public class ANTControllerBroadcastSender {

    static public void onCommChannelStateChanged(Service service, int prevState, int newState) {
        Intent broadcastIntent = new Intent();
        broadcastIntent.setAction(ANTControllerBroadcastReceiver.ACTION);
        broadcastIntent.putExtra(ANTControllerBroadcastReceiver.KEY_EVENT_TYPE,
                ANTControllerBroadcastReceiver.EVENT_TYPE_ON_COMM_CHANNEL_STATE_CHANGED);
        broadcastIntent.putExtra(ANTControllerBroadcastReceiver
                .KEY_ON_COMM_CHANNEL_STATE_CHANGED_PREV_STATE, prevState);
        broadcastIntent.putExtra(ANTControllerBroadcastReceiver
                .KEY_ON_COMM_CHANNEL_STATE_CHANGED_NEW_STATE, newState);
        service.sendBroadcast(broadcastIntent);
    }

    static public void onAppStateChanged(Service service, int appId, int appState) {
        Intent broadcastIntent = new Intent();
        broadcastIntent.setAction(ANTControllerBroadcastReceiver.ACTION);
        broadcastIntent.putExtra(ANTControllerBroadcastReceiver.KEY_EVENT_TYPE,
                ANTControllerBroadcastReceiver.EVENT_TYPE_ON_APP_STATE_CHANGED);
        broadcastIntent.putExtra(ANTControllerBroadcastReceiver.KEY_ON_APP_STATE_CHANGED_APP_ID,
                appId);
        broadcastIntent.putExtra(ANTControllerBroadcastReceiver
                .KEY_ON_APP_STATE_CHANGED_APP_STATE, appState);
        service.sendBroadcast(broadcastIntent);
    }

    static public void onReceivedEvent(Service service, int appId, String legacyData, boolean
            isNoti) {
        Intent broadcastIntent = new Intent();
        broadcastIntent.setAction(ANTControllerBroadcastReceiver.ACTION);
        broadcastIntent.putExtra(ANTControllerBroadcastReceiver.KEY_EVENT_TYPE,
                ANTControllerBroadcastReceiver.EVENT_TYPE_ON_RECEIVED_EVENT);
        broadcastIntent.putExtra(ANTControllerBroadcastReceiver.KEY_ON_RECEIVED_EVENT_APP_ID,
                appId);
        broadcastIntent.putExtra(ANTControllerBroadcastReceiver
                .KEY_ON_RECEIVED_EVENT_LEGACY_DATA, legacyData);
        broadcastIntent.putExtra(ANTControllerBroadcastReceiver.KEY_ON_RECEIVED_EVENT_IS_NOTI,
                isNoti);
        service.sendBroadcast(broadcastIntent);
    }

    static public void onReceivedAppConfig(Service service, int appId, String legacyData) {
        Intent broadcastIntent = new Intent();
        broadcastIntent.setAction(ANTControllerBroadcastReceiver.ACTION);
        broadcastIntent.putExtra(ANTControllerBroadcastReceiver.KEY_EVENT_TYPE,
                ANTControllerBroadcastReceiver.EVENT_TYPE_ON_RECEIVED_APP_CONFIG);
        broadcastIntent.putExtra(ANTControllerBroadcastReceiver
                .KEY_ON_RECEIVED_APP_CONFIG_APP_ID, appId);
        broadcastIntent.putExtra(ANTControllerBroadcastReceiver
                .KEY_ON_RECEIVED_APP_CONFIG_LEGACY_DATA, legacyData);
        service.sendBroadcast(broadcastIntent);
    }

    static public void onReceivedDataFromTarget(Service service, String senderUri, String
            listenerName, String data, String attachedFilePath) {
        Intent broadcastIntent = new Intent();
        broadcastIntent.setAction(ANTControllerBroadcastReceiver.ACTION);
        broadcastIntent.putExtra(ANTControllerBroadcastReceiver.KEY_EVENT_TYPE,
                ANTControllerBroadcastReceiver.EVENT_TYPE_ON_RECEIVED_DATA_FROM_TARGET);
        broadcastIntent.putExtra(ANTControllerBroadcastReceiver
                .KEY_ON_RECEIVED_DATA_FROM_TARGET_SENDER_URI, senderUri);
        broadcastIntent.putExtra(ANTControllerBroadcastReceiver
                .KEY_ON_RECEIVED_DATA_FROM_TARGET_LISTENER_NAME, listenerName);
        broadcastIntent.putExtra(ANTControllerBroadcastReceiver
                .KEY_ON_RECEIVED_DATA_FROM_TARGET_DATA, data);
        broadcastIntent.putExtra(ANTControllerBroadcastReceiver
                .KEY_ON_RECEIVED_DATA_FROM_TARGET_ATTACHED_FILE_PATH, attachedFilePath);
        service.sendBroadcast(broadcastIntent);
    }

    static public void onResultUpdateAppList(Service service, ANTApp[] appList) {
        Intent broadcastIntent = new Intent();
        broadcastIntent.setAction(ANTControllerBroadcastReceiver.ACTION);
        broadcastIntent.putExtra(ANTControllerBroadcastReceiver.KEY_EVENT_TYPE,
                ANTControllerBroadcastReceiver.EVENT_TYPE_ON_RESULT_UPDATE_APP_LIST);
        broadcastIntent.putExtra(ANTControllerBroadcastReceiver
                .KEY_ON_RESULT_UPDATE_APP_LIST_APP_LIST, appList);
        service.sendBroadcast(broadcastIntent);
    }

    static public void onResultGetFileList(Service service, int commandMessageId, String path,
                                           ParamFileListEntry[] fileList) {
        Intent broadcastIntent = new Intent();
        broadcastIntent.setAction(ANTControllerBroadcastReceiver.ACTION);
        broadcastIntent.putExtra(ANTControllerBroadcastReceiver.KEY_EVENT_TYPE,
                ANTControllerBroadcastReceiver.EVENT_TYPE_ON_RESULT_GET_FILE_LIST);
        broadcastIntent.putExtra(ANTControllerBroadcastReceiver
                .KEY_ON_RESULT_GET_FILE_LIST_COMMAND_MESSAGE_ID, commandMessageId);
        broadcastIntent.putExtra(ANTControllerBroadcastReceiver.KEY_ON_RESULT_GET_FILE_LIST_PATH,
                path);
        broadcastIntent.putExtra(ANTControllerBroadcastReceiver
                .KEY_ON_RESULT_GET_FILE_LIST_FILE_LIST, fileList);
        service.sendBroadcast(broadcastIntent);
    }

    static public void onResultGetFile(Service service, int commandMessageId, String
            storedFilePath) {
        Intent broadcastIntent = new Intent();
        broadcastIntent.setAction(ANTControllerBroadcastReceiver.ACTION);
        broadcastIntent.putExtra(ANTControllerBroadcastReceiver.KEY_EVENT_TYPE,
                ANTControllerBroadcastReceiver.EVENT_TYPE_ON_RESULT_GET_FILE);
        broadcastIntent.putExtra(ANTControllerBroadcastReceiver
                .KEY_ON_RESULT_GET_FILE_COMMAND_MESSAGE_ID, commandMessageId);
        broadcastIntent.putExtra(ANTControllerBroadcastReceiver
                .KEY_ON_RESULT_GET_FILE_STORED_FILE_PATH, storedFilePath);
        service.sendBroadcast(broadcastIntent);
    }

    static public void onResultGetTargetRootPath(Service service, int commandMessageId, String
            path) {
        Intent broadcastIntent = new Intent();
        broadcastIntent.setAction(ANTControllerBroadcastReceiver.ACTION);
        broadcastIntent.putExtra(ANTControllerBroadcastReceiver.KEY_EVENT_TYPE,
                ANTControllerBroadcastReceiver.EVENT_TYPE_ON_RESULT_GET_TARGET_ROOT_PATH);
        broadcastIntent.putExtra(ANTControllerBroadcastReceiver
                .KEY_ON_RESULT_GET_TARGET_ROOT_PATH_COMMAND_MESSAGE_ID, commandMessageId);
        broadcastIntent.putExtra(ANTControllerBroadcastReceiver
                .KEY_ON_RESULT_GET_TARGET_ROOT_PATH_PATH, path);
        service.sendBroadcast(broadcastIntent);
    }

    static public void onResultUpdateAppConfig(Service service, int commandMessageId, boolean
            isSucceed) {
        Intent broadcastIntent = new Intent();
        broadcastIntent.setAction(ANTControllerBroadcastReceiver.ACTION);
        broadcastIntent.putExtra(ANTControllerBroadcastReceiver.KEY_EVENT_TYPE,
                ANTControllerBroadcastReceiver.EVENT_TYPE_ON_RESULT_UPDATE_APP_CONFIG);
        broadcastIntent.putExtra(ANTControllerBroadcastReceiver
                .KEY_ON_RESULT_UPDATE_APP_CONFIG_COMMAND_MESSAGE_ID, commandMessageId);
        broadcastIntent.putExtra(ANTControllerBroadcastReceiver
                .KEY_ON_RESULT_UPDATE_APP_CONFIG_IS_SUCCEED, isSucceed);
        service.sendBroadcast(broadcastIntent);
    }
}
