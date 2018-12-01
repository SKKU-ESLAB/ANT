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

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.Parcelable;

import com.ant.cmfw.service.CommChannelService;
import com.ant.ant_manager.model.ANTApp;
import com.ant.ant_manager.model.message.params.ParamFileListEntry;

abstract public class ANTControllerBroadcastReceiver extends BroadcastReceiver {
    private static final String TAG = "ANTControllerBroadcastReceiver";
    public static final String ACTION = "com.ant.controller.broadcastreceiver";
    public static final String KEY_EVENT_TYPE = "eventType";

    public static final String EVENT_TYPE_ON_COMM_CHANNEL_STATE_CHANGED =
            "onCommChannelStateChanged";
    public static final String KEY_ON_COMM_CHANNEL_STATE_CHANGED_PREV_STATE = "prevState";
    public static final String KEY_ON_COMM_CHANNEL_STATE_CHANGED_NEW_STATE = "newState";


    public static final String EVENT_TYPE_ON_APP_STATE_CHANGED = "onAppStateChanged";
    public static final String KEY_ON_APP_STATE_CHANGED_APP_ID = "appId";
    public static final String KEY_ON_APP_STATE_CHANGED_APP_STATE = "appState";

    public static final String EVENT_TYPE_ON_RECEIVED_EVENT = "onReceivedEvent";
    public static final String KEY_ON_RECEIVED_EVENT_APP_ID = "appId";
    public static final String KEY_ON_RECEIVED_EVENT_LEGACY_DATA = "legacyData";
    public static final String KEY_ON_RECEIVED_EVENT_IS_NOTI = "isNoti";

    public static final String EVENT_TYPE_ON_RECEIVED_DATA_FROM_TARGET = "onReceivedDataFromTarget";
    public static final String KEY_ON_RECEIVED_DATA_FROM_TARGET_SENDER_URI = "senderUri";
    public static final String KEY_ON_RECEIVED_DATA_FROM_TARGET_LISTENER_NAME = "listenerName";
    public static final String KEY_ON_RECEIVED_DATA_FROM_TARGET_DATA = "data";
    public static final String KEY_ON_RECEIVED_DATA_FROM_TARGET_ATTACHED_FILE_PATH =
            "attachedFilePath";

    public static final String EVENT_TYPE_ON_RECEIVED_APP_CONFIG = "onReceivedAppConfig";
    public static final String KEY_ON_RECEIVED_APP_CONFIG_APP_ID = "appId";
    public static final String KEY_ON_RECEIVED_APP_CONFIG_LEGACY_DATA = "legacyData";

    public static final String EVENT_TYPE_ON_RESULT_UPDATE_APP_LIST = "onResultUpdateAppList";
    public static final String KEY_ON_RESULT_UPDATE_APP_LIST_APP_LIST = "appList";

    public static final String EVENT_TYPE_ON_RESULT_GET_FILE_LIST = "onGetFileList";
    public static final String KEY_ON_RESULT_GET_FILE_LIST_COMMAND_MESSAGE_ID = "commandMessageId";
    public static final String KEY_ON_RESULT_GET_FILE_LIST_PATH = "path";
    public static final String KEY_ON_RESULT_GET_FILE_LIST_FILE_LIST = "fileList";

    public static final String EVENT_TYPE_ON_RESULT_GET_FILE = "onGetFile";
    public static final String KEY_ON_RESULT_GET_FILE_COMMAND_MESSAGE_ID = "commandMessageId";
    public static final String KEY_ON_RESULT_GET_FILE_STORED_FILE_PATH = "storedFilePath";

    public static final String EVENT_TYPE_ON_RESULT_GET_TARGET_ROOT_PATH = "onResultGetTargetRoot";
    public static final String KEY_ON_RESULT_GET_TARGET_ROOT_PATH_COMMAND_MESSAGE_ID =
            "commandMessageId";
    public static final String KEY_ON_RESULT_GET_TARGET_ROOT_PATH_PATH = "path";

    public static final String EVENT_TYPE_ON_RESULT_UPDATE_APP_CONFIG = "onResultUpdateAppConfig";
    public static final String KEY_ON_RESULT_UPDATE_APP_CONFIG_COMMAND_MESSAGE_ID =
            "commandMessageId";
    public static final String KEY_ON_RESULT_UPDATE_APP_CONFIG_IS_SUCCEED = "isSucceed";

    @Override
    public void onReceive(Context context, Intent intent) {
        String action = intent.getAction();
        if (action.compareTo(ACTION) == 0) {
            String eventType = intent.getStringExtra(KEY_EVENT_TYPE);
            if (eventType.compareTo(EVENT_TYPE_ON_COMM_CHANNEL_STATE_CHANGED) == 0) {
                int prevState = intent.getIntExtra(KEY_ON_COMM_CHANNEL_STATE_CHANGED_PREV_STATE,
                        CommChannelService.STATE_DISCONNECTED);
                int newState = intent.getIntExtra(KEY_ON_COMM_CHANNEL_STATE_CHANGED_NEW_STATE,
                        CommChannelService.STATE_DISCONNECTED);
                if (this.mOnCommChannelStateChangedListener != null)
                    this.mOnCommChannelStateChangedListener.onCommChannelStateChanged(prevState,
                            newState);
            } else if (eventType.compareTo(EVENT_TYPE_ON_APP_STATE_CHANGED) == 0) {
                int appId = intent.getIntExtra(KEY_ON_APP_STATE_CHANGED_APP_ID, -1);
                int appState = intent.getIntExtra(KEY_ON_APP_STATE_CHANGED_APP_STATE, ANTApp
                        .State_Initializing);
                if (this.mOnAppStateChangedListener != null)
                    this.mOnAppStateChangedListener.onAppStateChanged(appId, appState);
            } else if (eventType.compareTo(EVENT_TYPE_ON_RECEIVED_EVENT) == 0) {
                int appId = intent.getIntExtra(KEY_ON_RECEIVED_EVENT_APP_ID, -1);
                String legacyData = intent.getStringExtra(KEY_ON_RECEIVED_EVENT_LEGACY_DATA);
                boolean isNoti = intent.getBooleanExtra(KEY_ON_RECEIVED_EVENT_IS_NOTI, false);
                if (this.mOnReceivedEventListener != null)
                    this.mOnReceivedEventListener.onReceivedEvent(appId, legacyData, isNoti);
            } else if (eventType.compareTo(EVENT_TYPE_ON_RECEIVED_APP_CONFIG) == 0) {
                int appId = intent.getIntExtra(KEY_ON_RECEIVED_EVENT_APP_ID, -1);
                String legacyData = intent.getStringExtra(KEY_ON_RECEIVED_APP_CONFIG_LEGACY_DATA);
                if (this.mOnReceivedAppConfigListener != null)
                    this.mOnReceivedAppConfigListener.onReceivedAppConfig(appId, legacyData);
            } else if (eventType.compareTo(EVENT_TYPE_ON_RECEIVED_DATA_FROM_TARGET) == 0) {
                String senderUri = intent.getStringExtra
                        (KEY_ON_RECEIVED_DATA_FROM_TARGET_SENDER_URI);
                String listenerName = intent.getStringExtra
                        (KEY_ON_RECEIVED_DATA_FROM_TARGET_LISTENER_NAME);
                String data = intent.getStringExtra(KEY_ON_RECEIVED_DATA_FROM_TARGET_DATA);
                String attachedFilePath = intent.getStringExtra
                        (KEY_ON_RECEIVED_DATA_FROM_TARGET_ATTACHED_FILE_PATH);
                if (this.mOnReceivedDataFromTarget != null)
                    this.mOnReceivedDataFromTarget.onReceivedDataFromTarget(senderUri,
                            listenerName, data, attachedFilePath);
            } else if (eventType.compareTo(EVENT_TYPE_ON_RESULT_UPDATE_APP_LIST) == 0) {
                Parcelable[] appListParcelable = intent.getParcelableArrayExtra
                        (KEY_ON_RESULT_UPDATE_APP_LIST_APP_LIST);
                ANTApp[] appList = new ANTApp[appListParcelable.length];
                for (int i = 0; i < appListParcelable.length; i++) {
                    appList[i] = (ANTApp) appListParcelable[i];
                }
                if (this.mOnResultUpdateAppListListener != null)
                    this.mOnResultUpdateAppListListener.onResultUpdateAppList(appList);
            } else if (eventType.compareTo(EVENT_TYPE_ON_RESULT_GET_FILE_LIST) == 0) {
                int commandMessageId = intent.getIntExtra
                        (KEY_ON_RESULT_GET_FILE_LIST_COMMAND_MESSAGE_ID, -1);
                String path = intent.getStringExtra(KEY_ON_RESULT_GET_FILE_LIST_PATH);
                Parcelable[] fileListParcelable = intent.getParcelableArrayExtra
                        (KEY_ON_RESULT_GET_FILE_LIST_FILE_LIST);
                ParamFileListEntry[] fileList = new ParamFileListEntry[fileListParcelable.length];
                for (int i = 0; i < fileListParcelable.length; i++) {
                    fileList[i] = (ParamFileListEntry) fileListParcelable[i];
                }
                if (this.mOnResultGetFileListListener != null) {
                    this.mOnResultGetFileListListener.onResultGetFileList(commandMessageId, path,
                            fileList);
                }
            } else if (eventType.compareTo(EVENT_TYPE_ON_RESULT_GET_FILE) == 0) {
                int commandMessageId = intent.getIntExtra
                        (KEY_ON_RESULT_GET_FILE_COMMAND_MESSAGE_ID, -1);
                String storedFilePath = intent.getStringExtra
                        (KEY_ON_RESULT_GET_FILE_STORED_FILE_PATH);
                if (this.mOnResultGetFileListener != null) {
                    this.mOnResultGetFileListener.onResultGetFile(commandMessageId, storedFilePath);
                }
            } else if (eventType.compareTo(EVENT_TYPE_ON_RESULT_GET_TARGET_ROOT_PATH) == 0) {
                int commandMessageId = intent.getIntExtra
                        (KEY_ON_RESULT_GET_TARGET_ROOT_PATH_COMMAND_MESSAGE_ID, -1);
                String path = intent.getStringExtra(KEY_ON_RESULT_GET_TARGET_ROOT_PATH_PATH);
                if (this.mOnResultGetTargetRootPathListener != null) {
                    this.mOnResultGetTargetRootPathListener.onResultGetTargetRootPath
                            (commandMessageId, path);
                }
            } else if (eventType.compareTo(EVENT_TYPE_ON_RESULT_UPDATE_APP_CONFIG) == 0) {
                int commandMessageId = intent.getIntExtra
                        (KEY_ON_RESULT_UPDATE_APP_CONFIG_COMMAND_MESSAGE_ID, -1);
                boolean isSucceed = intent.getBooleanExtra
                        (KEY_ON_RESULT_UPDATE_APP_CONFIG_IS_SUCCEED, false);
                if (this.mOnResultUpdateAppConfigListener != null) {
                    this.mOnResultUpdateAppConfigListener.onResultUpdateAppConfig
                            (commandMessageId, isSucceed);
                }
            }
        }
    }

    private OnCommChannelStateChangedListener mOnCommChannelStateChangedListener;
    private OnAppStateChangedListener mOnAppStateChangedListener;
    private OnReceivedEventListener mOnReceivedEventListener;
    private OnReceivedDataFromTarget mOnReceivedDataFromTarget;
    private OnReceivedAppConfigListener mOnReceivedAppConfigListener;
    private OnResultUpdateAppListListener mOnResultUpdateAppListListener;
    private OnResultGetFileListListener mOnResultGetFileListListener;
    private OnResultGetFileListener mOnResultGetFileListener;
    private OnResultGetTargetRootPathListener mOnResultGetTargetRootPathListener;
    private OnResultUpdateAppConfigListener mOnResultUpdateAppConfigListener;

    public void setOnCommChannelStateChangedListener(OnCommChannelStateChangedListener listener) {
        this.mOnCommChannelStateChangedListener = listener;
    }

    public void setOnAppStateChangedListener(OnAppStateChangedListener listener) {
        this.mOnAppStateChangedListener = listener;
    }

    public void setOnReceivedEventListener(OnReceivedEventListener listener) {
        this.mOnReceivedEventListener = listener;
    }

    public void setOnReceivedSensorDataListener(OnReceivedDataFromTarget listener) {
        this.mOnReceivedDataFromTarget = listener;
    }

    public void setOnReceivedAppConfigListener(OnReceivedAppConfigListener listener) {
        this.mOnReceivedAppConfigListener = listener;
    }

    public void setOnResultUpdateAppListListener(OnResultUpdateAppListListener listener) {
        this.mOnResultUpdateAppListListener = listener;
    }

    public void setOnResultGetFileListListener(OnResultGetFileListListener listener) {
        this.mOnResultGetFileListListener = listener;
    }

    public void setOnResultGetFileListener(OnResultGetFileListener listener) {
        this.mOnResultGetFileListener = listener;
    }

    public void setOnResultGetTargetRootPathListener(OnResultGetTargetRootPathListener listener) {
        this.mOnResultGetTargetRootPathListener = listener;
    }

    public void setOnResultUpdateAppConfigListener(OnResultUpdateAppConfigListener listener) {
        this.mOnResultUpdateAppConfigListener = listener;
    }

    public interface OnCommChannelStateChangedListener {
        public void onCommChannelStateChanged(int prevState, int newState);
    }

    public interface OnAppStateChangedListener {
        public void onAppStateChanged(int appId, int appState);
    }

    public interface OnReceivedEventListener {
        public void onReceivedEvent(int appId, String legacyData, boolean isNoti);
    }

    public interface OnReceivedAppConfigListener {
        public void onReceivedAppConfig(int appId, String legacyData);
    }

    public interface OnReceivedDataFromTarget {
        public void onReceivedDataFromTarget(String senderUri, String listenerName, String data,
                                             String attachedFilePath);
    }

    public interface OnResultUpdateAppListListener {
        public void onResultUpdateAppList(ANTApp[] appList);
    }

    public interface OnResultGetFileListListener {
        public void onResultGetFileList(int commandMessageId, String path, ParamFileListEntry[]
                fileList);
    }

    public interface OnResultGetFileListener {
        public void onResultGetFile(int commandMessageId, String storedFilePath);
    }

    public interface OnResultGetTargetRootPathListener {
        public void onResultGetTargetRootPath(int commandMessageId, String path);
    }

    public interface OnResultUpdateAppConfigListener {
        public void onResultUpdateAppConfig(int commandMessageId, boolean isSucceed);
    }
}