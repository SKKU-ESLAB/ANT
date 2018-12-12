package com.ant.ant_manager.view.remoteui;

import android.app.Service;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.os.IBinder;
import android.util.Log;

import com.ant.ant_manager.controller.ANTControllerBroadcastReceiver;
import com.ant.ant_manager.controller.ANTControllerService;
import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.ObjectMapper;

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
public class RemoteUIService extends Service {
    private final String TAG = "RemoteUIService";

    private RemoteNotiUIController mNotiController;
    private RESTBasedRemoteUI mRESTBased;

    // Connection to ANTControllerService
    private ANTControllerService mANTControllerServiceStub;
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
            mANTControllerServiceStub = serviceBinder.getService();

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
            mANTControllerServiceStub = null;
        }
    };

    public ANTControllerService getANTControllerStub() {
        return this.mANTControllerServiceStub;
    }

    class PrivateControllerBroadcastReceiver extends ANTControllerBroadcastReceiver {
        PrivateControllerBroadcastReceiver() {
            this.setOnReceivedSensorDataListener(new OnReceivedDataFromTarget() {
                @Override
                public void onReceivedDataFromTarget(String senderUri, String listenerName,
                                                     String data, String attachedFilePath) {
                    Log.d(TAG, "Message coming for " + listenerName + ": " + data);
                    if (listenerName.compareToIgnoreCase("RemoteNotiUI") == 0) {
                        mNotiController.showRemoteUINoti(senderUri, data, attachedFilePath);
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
        this.mNotiController = new RemoteNotiUIController();
        this.mRESTBased = new RESTBasedRemoteUI();
    }

    public void initialize() {
        this.connectControllerService();

        this.mNotiController.initialize(this);
        this.mRESTBased.openServer(this.mRESTListener);
    }

    // RESTBasedRemoteUIListener
    private RESTBasedRemoteUIListener mRESTListener = new RESTBasedRemoteUIListener() {
        @Override
        public void onRESTRemoteUIMessage(String message) {
            try {
                ObjectMapper jsonMapper = new ObjectMapper();
                Map<String, String> messageMap;
                messageMap = jsonMapper.readValue(message, new TypeReference<Map<String, String>>
                            (){});
                String appIdStr = messageMap.get("appId");

                int appId = (!appIdStr.isEmpty()) ? Integer.valueOf(appIdStr) : 0;
                mNotiController.showRemoteUINoti("/thing0/apps/" + appId, message, "");
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    };

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
