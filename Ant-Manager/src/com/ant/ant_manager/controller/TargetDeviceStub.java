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

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.os.IBinder;
import android.util.Log;

import com.ant.cmfw.glue.CommBroadcastReceiver;
import com.ant.cmfw.service.CommChannelService;
import com.ant.ant_manager.model.message.AppAckMessage;
import com.ant.ant_manager.model.message.AppCoreAckMessage;
import com.ant.ant_manager.model.message.AppCoreMessage;
import com.ant.ant_manager.model.message.AppMessage;
import com.ant.ant_manager.model.message.BaseMessage;
import com.ant.ant_manager.model.message.CompanionMessage;
import com.ant.ant_manager.model.message.MessageFactory;

import java.io.File;

public class TargetDeviceStub {
    // RPC on CommChannelService
    private static final String TAG = "TargetDeviceStub";
    private CommChannelService mCommChannelServiceStub = null;
    private PrivateCommBroadcastReceiver mCommBroadcastReceiver;

    private final String kCompanionDeviceURI = "/comp0";
    private final String kAppBaseURI = "/thing/apps";
    private final String kAppCoreURI = "/thing/appcore";

    // ANT Manager (Android) -> AppCore Daemon or app (ANT Device)
    private int sendMessage(BaseMessage message) {
        if (this.mCommChannelServiceStub == null) {
            Log.e(TAG, "CommChannel is not initialized");
            return -1;
        }
        Log.d(TAG, "sendMessage(BaseMessage): id=" + message.getMessageId() + " / type=" +
                message.getType() + " / body=" + message.toJSONString());
        if (message.isFileAttached()) {
            this.mCommChannelServiceStub.sendRawMessage(message.toJSONString(), new File(message
                    .getStoredFilePath()));
        } else {
            this.mCommChannelServiceStub.sendRawMessage(message.toJSONString());
        }
        return message.getMessageId();
    }

    public int getAppList() {
        BaseMessage newMessage = MessageFactory.makeAppCoreMessage(kCompanionDeviceURI,
                kAppCoreURI, AppCoreMessage.Type_GetAppList);
        return this.sendMessage(newMessage);
    }

    public int listenAppState(int appId) {
        BaseMessage newMessage = MessageFactory.makeAppCoreMessage(kCompanionDeviceURI,
                kAppCoreURI, AppCoreMessage.Type_ListenAppState);
        AppCoreMessage appCorePayload = (AppCoreMessage) newMessage.getPayload();
        appCorePayload.setParamsListenAppState(appId);
        return this.sendMessage(newMessage);
    }

    public int initializeApp() {
        BaseMessage newMessage = MessageFactory.makeAppCoreMessage(kCompanionDeviceURI,
                kAppCoreURI, AppCoreMessage.Type_InitializeApp);
        return this.sendMessage(newMessage);
    }

    public int installApp(int appId, File packageFile) {
        BaseMessage newMessage = MessageFactory.makeAppCoreMessage(kCompanionDeviceURI,
                kAppCoreURI, AppCoreMessage.Type_InstallApp);
        AppCoreMessage appCorePayload = (AppCoreMessage) newMessage.getPayload();
        appCorePayload.setParamsInstallApp(appId, packageFile.getName());
        newMessage.attachFile(packageFile.getAbsolutePath());
        return this.sendMessage(newMessage);
    }

    public int launchApp(int appId) {
        BaseMessage newMessage = MessageFactory.makeAppCoreMessage(kCompanionDeviceURI,
                kAppCoreURI, AppCoreMessage.Type_LaunchApp);
        AppCoreMessage appCorePayload = (AppCoreMessage) newMessage.getPayload();
        appCorePayload.setParamsLaunchApp(appId);
        return this.sendMessage(newMessage);
    }

    public int terminateApp(int appId) {
        BaseMessage newMessage = MessageFactory.makeAppCoreMessage(kCompanionDeviceURI,
                kAppCoreURI, AppCoreMessage.Type_TerminateApp);
        AppCoreMessage appCorePayload = (AppCoreMessage) newMessage.getPayload();
        appCorePayload.setParamsTerminateApp(appId);
        return this.sendMessage(newMessage);
    }

    public int removeApp(int appId) {
        BaseMessage newMessage = MessageFactory.makeAppCoreMessage(kCompanionDeviceURI,
                kAppCoreURI, AppCoreMessage.Type_RemoveApp);
        AppCoreMessage appCorePayload = (AppCoreMessage) newMessage.getPayload();
        appCorePayload.setParamsRemoveApp(appId);
        return this.sendMessage(newMessage);
    }

    public int getFileList(String path) {
        BaseMessage newMessage = MessageFactory.makeAppCoreMessage(kCompanionDeviceURI,
                kAppCoreURI, AppCoreMessage.Type_GetFileList);
        AppCoreMessage appCorePayload = (AppCoreMessage) newMessage.getPayload();
        appCorePayload.setParamsGetFileList(path);
        return this.sendMessage(newMessage);
    }

    public int getFile(String path) {
        BaseMessage newMessage = MessageFactory.makeAppCoreMessage(kCompanionDeviceURI,
                kAppCoreURI, AppCoreMessage.Type_GetFile);
        AppCoreMessage appCorePayload = (AppCoreMessage) newMessage.getPayload();
        appCorePayload.setParamsGetFile(path);
        return this.sendMessage(newMessage);
    }

    public int getRootPath() {
        BaseMessage newMessage = MessageFactory.makeAppCoreMessage(kCompanionDeviceURI,
                kAppCoreURI, AppCoreMessage.Type_GetRootPath);
        return this.sendMessage(newMessage);
    }

    public int updateAppConfig(int appId, String legacyData) {
        String appURI = kAppBaseURI + "/" + appId;
        BaseMessage newMessage = MessageFactory.makeAppMessage(kCompanionDeviceURI, appURI,
                AppMessage.Type_UpdateAppConfig);
        AppMessage appPayload = (AppMessage) newMessage.getPayload();
        appPayload.setParamsUpdateAppConfig(legacyData);
        return this.sendMessage(newMessage);
    }

    public int getAppIcon(int appId) {
        BaseMessage newMessage = MessageFactory.makeAppCoreMessage(kCompanionDeviceURI,
                kAppCoreURI, AppCoreMessage.Type_GetAppIcon);
        AppCoreMessage appCorePayload = (AppCoreMessage) newMessage.getPayload();
        appCorePayload.setParamsGetAppIcon(appId);
        return this.sendMessage(newMessage);
    }

    // AppCore Daemon (ANT Device) -> ANT Manager (Android)
    private void onReceivedAppCoreAckMessage(BaseMessage message) {
        AppCoreAckMessage payload = (AppCoreAckMessage) message.getPayload();
        switch (payload.getCommandType()) {
            case AppCoreMessage.Type_GetAppList: {
                this.mListener.onAckGetAppList(message);
                break;
            }
            case AppCoreMessage.Type_ListenAppState: {
                this.mListener.onAckListenAppState(message);
                break;
            }
            case AppCoreMessage.Type_InitializeApp: {
                this.mListener.onAckInitializeApp(message);
                break;
            }
            case AppCoreMessage.Type_GetFileList: {
                this.mListener.onAckGetFileList(message);
                break;
            }
            case AppCoreMessage.Type_GetFile: {
                this.mListener.onAckGetFile(message);
                break;
            }
            case AppCoreMessage.Type_GetRootPath: {
                this.mListener.onAckGetRootPath(message);
                break;
            }
            case AppCoreMessage.Type_GetAppIcon: {
                this.mListener.getAppIcon(message);
                break;
            }
            case AppCoreMessage.Type_NotDetermined:
            default: {
                Log.e(TAG, "Cannot receive that command!: " + message.toJSONString());
                break;
            }
        }
    }

    // App (ANT Device) -> ANT Manager (Android)
    private void onReceivedAppAckMessage(BaseMessage message) {
        AppAckMessage payload = (AppAckMessage) message.getPayload();
        switch (payload.getCommandType()) {
            case AppMessage.Type_UpdateAppConfig: {
                this.mListener.onUpdateAppConfig(message);
                break;
            }
        }
    }

    private void onReceivedCompanionMessage(BaseMessage message) {
        CompanionMessage payload = (CompanionMessage) message.getPayload();
        switch (payload.getCommandType()) {
            case CompanionMessage.Type_SendConfigPage: {
                this.mListener.onSendConfigPage(message);
                break;
            }
            case CompanionMessage.Type_SendEventPage: {
                this.mListener.onSendEventPage(message);
                break;
            }
            case CompanionMessage.Type_UpdateSensorData: {
                this.mListener.onUpdateSensorData(message);
                break;
            }
            case CompanionMessage.Type_NotDetermined:
            default: {
                Log.e(TAG, "Cannot receive that command!: " + message.toJSONString());
                break;
            }
        }
    }

    // Interface to CommChannelService
    public void initializeConnection() {
        if (this.mCommChannelServiceStub == null) {
            // Launch CommChannelService for setting connection with target ANT device.
            Intent serviceIntent = new Intent(this.mOwnerService, CommChannelService.class);
            this.mOwnerService.bindService(serviceIntent, this.mCommServiceConnection, Context
                    .BIND_AUTO_CREATE);
        } else {
            // Request to connect channel
            mCommChannelServiceStub.connectChannel(); // RPC to CommChannelService
        }
    }

    public void destroyConnection() {
        this.mOwnerService.unbindService(mCommServiceConnection);
    }

    public int getCommChannelState() {
        if (this.mCommChannelServiceStub == null) {
            Log.e(TAG, "CommChannelService is not connected");
            return CommChannelService.STATE_DISCONNECTED;
        }
        return this.mCommChannelServiceStub.getCommChannelState();
    }

    public void enableLargeDataMode() {
        this.mCommChannelServiceStub.enableLargeDataMode();
    }

    public void lockLargeDataMode() {
        this.mCommChannelServiceStub.lockLargeDataMode();
    }

    public void unlockLargeDataMode() {
        this.mCommChannelServiceStub.unlockLargeDataMode();
    }

    public String getLargeDataIPAddress() {
        return this.mCommChannelServiceStub.getLargeDataIPAddress();
    }

    private ServiceConnection mCommServiceConnection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName componentName, IBinder inputBinder) {
            CommChannelService.CommBinder serviceBinder = (CommChannelService.CommBinder)
                    inputBinder;
            mCommChannelServiceStub = serviceBinder.getService();

            // Set BroadcastReceiver
            IntentFilter broadcastIntentFilter = new IntentFilter();
            broadcastIntentFilter.addAction(CommBroadcastReceiver.ACTION);
            mCommBroadcastReceiver = new PrivateCommBroadcastReceiver();
            mOwnerService.registerReceiver(mCommBroadcastReceiver, broadcastIntentFilter);

            // Initialize path
            mCommChannelServiceStub.setDownloadFilePath(mDownloadPath);

            // Request to connect channel
            mCommChannelServiceStub.connectChannel(); // RPC to CommChannelService
        }

        @Override
        public void onServiceDisconnected(ComponentName componentName) {
            Log.d(TAG, "onServiceDisconnected()");
            mOwnerService.unregisterReceiver(mCommBroadcastReceiver);
            mCommChannelServiceStub = null;
        }
    };

    class PrivateCommBroadcastReceiver extends CommBroadcastReceiver {
        @Override
        public void onCommChannelStateChanged(int prevState, int newState) {
            mListener.onCommChannelStateChanged(prevState, newState);
        }

        // AppCore Daemon (ANT Device) -> ANT Manager (Android)
        @Override
        public void onReceivedRawMessage(String messageStr, String filePath) {
            BaseMessage message = MessageFactory.makeMessageFromJSONString(messageStr);
            message.setStoredFilePath(filePath);
            switch (message.getType()) {
                case BaseMessage.Type_AppCoreAck:
                    onReceivedAppCoreAckMessage(message);
                    break;
                case BaseMessage.Type_AppAck:
                    onReceivedAppAckMessage(message);
                    break;
                case BaseMessage.Type_Companion:
                    onReceivedCompanionMessage(message);
                    break;
                case BaseMessage.Type_App:
                case BaseMessage.Type_AppCore:
                case BaseMessage.Type_NotDetermined:
                default:
                    Log.e(TAG, "Cannot receive that kind of message!: " + messageStr + " / " +
                            filePath);
                    break;
            }
        }
    }

    public TargetDeviceStub(ANTControllerService ownerService, TargetDeviceStubListener listener,
                            String downloadPath) {
        this.mOwnerService = ownerService;
        this.mListener = listener;
        this.mDownloadPath = downloadPath;
    }

    private ANTControllerService mOwnerService;
    private TargetDeviceStubListener mListener;
    private String mDownloadPath;
}
