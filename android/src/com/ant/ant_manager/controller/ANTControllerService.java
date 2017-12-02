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

import android.annotation.SuppressLint;
import android.app.Service;
import android.content.Intent;
import android.net.Uri;
import android.os.IBinder;
import android.util.Log;
import android.util.SparseArray;

import com.ant.cmfw.service.CommChannelService;
import com.ant.ant_manager.model.ANTApp;
import com.ant.ant_manager.model.ANTEvent;
import com.ant.ant_manager.model.ANTEventList;
import com.ant.ant_manager.model.Settings;
import com.ant.ant_manager.model.message.AppAckMessage;
import com.ant.ant_manager.model.message.AppCoreAckMessage;
import com.ant.ant_manager.model.message.BaseMessage;
import com.ant.ant_manager.model.message.CompanionMessage;
import com.ant.ant_manager.model.message.params.ParamAppListEntry;
import com.ant.ant_manager.model.message.params.ParamFileListEntry;
import com.ant.ant_manager.model.message.params.ParamsGetAppList;
import com.ant.ant_manager.model.message.params.ParamsGetFileList;
import com.ant.ant_manager.model.message.params.ParamsGetRootPath;
import com.ant.ant_manager.model.message.params.ParamsInitializeApp;
import com.ant.ant_manager.model.message.params.ParamsListenAppState;
import com.ant.ant_manager.model.message.params.ParamsSendConfigPage;
import com.ant.ant_manager.model.message.params.ParamsSendEventPage;
import com.ant.ant_manager.model.message.params.ParamsUpdateAppConfig;
import com.ant.ant_manager.model.message.params.ParamsSendToCompanion;
import com.ant.ant_manager.view.remoteui.RemoteNotiUI;

import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserException;
import org.xmlpull.v1.XmlPullParserFactory;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

public class ANTControllerService extends Service {
    private static String TAG = "ANTControllerService";
    private int mBindersCount = 0;
    private final IBinder mBinder;

    // TargetDeviceStub
    private final ANTControllerService self = this;
    private TargetDeviceStub mTargetDeviceStub = null;
    private PrivateAppCoreStubListener mTargetDeviceStubListener = null;

    // Models
    @SuppressLint("UseSparseArrays")
    private HashMap<Integer, ANTApp> mAppList = new HashMap<>();
    private ANTEventList mEventList;
    private Settings mSettings;

    public ANTControllerService() {
        this.mEventList = new ANTEventList();
        this.mBinder = new ControllerBinder();
    }

    // Connection with target device
    public void initializeConnectionAsync() {
        if (this.mTargetDeviceStub == null) {
            Log.e(TAG, "AppCoreStub is not initialized");
            return;
        }
        this.mTargetDeviceStub.initializeConnection();
    }

    public void destroyConnectionAsync() {
        if (this.mTargetDeviceStub == null) {
            Log.e(TAG, "AppCoreStub is not initialized");
            return;
        }
        this.mTargetDeviceStub.destroyConnection();
    }

    public int getCommChannelState() {
        if (this.mTargetDeviceStub == null) {
            Log.e(TAG, "AppCoreStub is not initialized");
            return CommChannelService.STATE_DISCONNECTED;
        }
        return this.mTargetDeviceStub.getCommChannelState();
    }

    public void enableLargeDataMode() {
        this.mTargetDeviceStub.enableLargeDataMode();
    }

    public void lockLargeDataMode() {
        this.mTargetDeviceStub.lockLargeDataMode();
    }

    public void unlockLargeDataMode() {
        this.mTargetDeviceStub.unlockLargeDataMode();
    }

    public String getLargeDataIPAddress() {
        return this.mTargetDeviceStub.getLargeDataIPAddress();
    }

    // Control functions (Sync)
    public ANTApp getApp(int appId) {
        return this.mAppList.get(appId);
    }

    public ArrayList<ANTApp> getAppList() {
        ArrayList<ANTApp> appList = new ArrayList<>();
        for (Map.Entry<Integer, ANTApp> appListEntry : this.mAppList.entrySet()) {
            ANTApp app = appListEntry.getValue();
            appList.add(app);
        }
        return appList;
    }

    public ArrayList<ANTEvent> getEventList() {
        return this.mEventList.getAllEventArrayList();
    }

    public Settings getSettings() {
        return this.mSettings;
    }

    // Control functions (Async)
    public int updateAppListAsync() {
        return this.mUpdateAppListProcedure.start();
    }

    public int getFileListAsync(String path) {
        return this.mTargetDeviceStub.getFileList(path);
    }

    public int getFileAsync(String path) {
        return this.mTargetDeviceStub.getFile(path);
    }

    public int getTargetRootPathAsync() {
        return this.mTargetDeviceStub.getRootPath();
    }

    public void updateAppConfigAsync(int appId, String legacyData) {
        this.mTargetDeviceStub.updateAppConfig(appId, legacyData);
    }

    // Control functions (OneWay)
    public void installAppOneWay(String packageFilePath) {
        this.mInstallProcedure.start(packageFilePath);
    }

    public void removeAppOneWay(int appId) {
        this.mTargetDeviceStub.removeApp(appId);
    }

    public void launchAppOneWay(int appId) {
        this.mTargetDeviceStub.launchApp(appId);
    }

    public void terminateAppOneWay(int appId) {
        this.mTargetDeviceStub.terminateApp(appId);
    }

    public void installApkOneWay(File apkFile) {
        // TODO: it is not used now, but to be used in future
        //For Companion type//
        Uri apkUri = Uri.fromFile(apkFile);
        try {
            Intent intent = new Intent(Intent.ACTION_VIEW);
            intent.setDataAndType(apkUri, "application/vnd.android.package-archive");
            startActivity(intent);
        } catch (Exception e) {
            Log.d("ANT", e.getMessage());
        }
    }

    // CompanionMessage callbacks
    private void onReceivedEvent(int appId, String legacyData, boolean isNoti) {
        LegacyJSONParser legacyJSONParser = new LegacyJSONParser(legacyData);
        String appIdStr = String.valueOf(appId);
        String appTitle = legacyJSONParser.getValueByKey("appTitle");
        String description = legacyJSONParser.getValueByKey("description");
        String dateTime = legacyJSONParser.getValueByKey("dateTime");
        String eventJsonData = legacyJSONParser.getJsonData();
        this.mEventList.addEvent(appIdStr, appTitle, description, dateTime, eventJsonData);

        ANTApp app = this.getApp(appId);

        if (isNoti) RemoteNotiUI.makeNotification(this, app, legacyData);
    }

    private void onReceivedConfig(int appId, String legacyData) {
        ANTApp app = this.getApp(appId);
        app.setConfigJSONString(legacyData);
    }

    private UpdateAppListProcedure mUpdateAppListProcedure = new UpdateAppListProcedure();
    private InstallProcedure mInstallProcedure = new InstallProcedure();

    private class UpdateAppListProcedure {
        // TODO: caching app icons
        // WaitingAppTable: <key: Integer commandMessageId, value: ANTApp waitingApp>
        private HashMap<Integer, ANTApp> mWaitingAppList = new HashMap<>();
        private ArrayList<ANTApp> mReadyAppList = new ArrayList<>();

        public int start() {
            if (!this.mWaitingAppList.isEmpty() || !this.mReadyAppList.isEmpty()) {
                Log.e(TAG, "Cannot update app list since previous request did not finish.");
                return -1;
            }
            int messageId = mTargetDeviceStub.getAppList();
            return messageId;
        }

        public void onAckGetAppList(ArrayList<ParamAppListEntry> originalAppList) {
            // Update AppList of ANTControllerService
            this.mWaitingAppList.clear();
            for (ParamAppListEntry entry : originalAppList) {
                int appId = entry.appId;
                String appName = entry.appName;
                boolean isDefaultApp = entry.isDefaultApp;

                // iconFile is not determined!
                ANTApp app = new ANTApp(appId, appName, "", isDefaultApp);

                // Request app icon
                int requestMessageId = mTargetDeviceStub.getAppIcon(app.getAppId());
                this.mWaitingAppList.put(requestMessageId, app);
            }
        }

        public void onAckGetAppIcon(int commandMessageId, String appIconPath) {
            // Check waiting app list
            ANTApp thisApp = this.mWaitingAppList.get(commandMessageId);
            if (thisApp == null) {
                Log.w(TAG, "There is no app " + commandMessageId + " in waiting list.");
                return;
            }

            if (appIconPath.compareTo("") != 0) {
                // Move to icon directory regardless of icon file caching
                File originalIconFile = new File(appIconPath);
                String iconDirPath = mSettings.getIconDir().getAbsolutePath();
                File targetIconFile = new File(iconDirPath, originalIconFile.getName());
                boolean isSucceed = originalIconFile.renameTo(targetIconFile);
                if (!isSucceed) {
                    Log.e(TAG, "Renaming file is refused: " + originalIconFile.getAbsolutePath()
                            + " -> " + targetIconFile.getAbsolutePath());
                }

                // Set app icon path
                thisApp.setIconImagePath(targetIconFile.getAbsolutePath());
            }

            // Move the app to ready app list
            this.mWaitingAppList.remove(commandMessageId);
            this.mReadyAppList.add(thisApp);
            if (mWaitingAppList.isEmpty()) {
                this.finish();
            }
        }

        public void finish() {
            // Update app list
            mAppList.clear();
            for (ANTApp app : this.mReadyAppList) {
                mAppList.put(app.getAppId(), app);
            }

            // Notify Listener
            ANTApp[] appListArray = this.mReadyAppList.toArray(new ANTApp[this.mReadyAppList
                    .size()]);
            ANTControllerBroadcastSender.onResultUpdateAppList(self, appListArray);

            // Finalize
            this.mWaitingAppList.clear();
            this.mReadyAppList.clear();
        }
    }

    private class InstallProcedure {
        // One way procedure: no return
        // InitializeTransaction: <key: Integer initializeMessageId, value: String packageFilePath>
        private SparseArray<String> mInitializeTransactions = new SparseArray<>();

        // InstallTransaction: <key: Integer appId, value: String packageFilePath>
        private SparseArray<String> mInstallTransactions = new SparseArray<>();

        public void start(String packageFilePath) {
            // Command 1: open app
            int messageId = mTargetDeviceStub.initializeApp();
            this.mInitializeTransactions.put(messageId, packageFilePath);
        }

        public void onInitializedApp(int initializeMessageId, int appId) {
            // Check if there is transaction
            String packageFilePath = this.mInitializeTransactions.get(initializeMessageId);
            if (packageFilePath == null) return;
            this.mInitializeTransactions.remove(initializeMessageId);

            // Check and register the package to app list
            boolean checkRes = this.registerPackageToAppList(appId, packageFilePath);
            if (!checkRes) return;

            // Command 2: listen app state
            mTargetDeviceStub.listenAppState(appId);

            // Check package file
            File packageFile = new File(packageFilePath);
            if (!packageFile.exists()) {
                Log.e(TAG, "App package file does not exist!");
                return;
            }
            if (!packageFile.isFile()) {
                Log.e(TAG, "Package file path does not indicate a file!");
                return;
            }

            // Add install transaction
            mInstallTransactions.put(appId, packageFilePath);

            // Command 3: install app
            mTargetDeviceStub.installApp(appId, packageFile);
        }

        private boolean registerPackageToAppList(int appId, String packageFilePath) {
            try {
                // Unarchive app package file
                String unarchiveDirPath = mSettings.getTempDir().getAbsolutePath();
                Unzip.unzip(packageFilePath, unarchiveDirPath);
                final String kManifestFileName = "manifest.xml";
                File manifestFile = new File(unarchiveDirPath, kManifestFileName);

                // Get app information
                String name = this.getFieldFromManifest(manifestFile.getAbsolutePath(), "label");
                String iconFileName = this.getFieldFromManifest(manifestFile.getAbsolutePath(),
                        "icon");
                if (name == null || iconFileName == null) {
                    Log.e(TAG, "Failed to get app information from manifest file.");
                    return false;
                }

                // Move icon file to icon directory
                File iconFile = new File(unarchiveDirPath, iconFileName);
                File newIconFile = new File(mSettings.getIconDir(), iconFile.getName());
                boolean isSucceed = iconFile.renameTo(newIconFile);
                if (!isSucceed) {
                    Log.e(TAG, "Renaming file is refused: " + iconFile.getAbsolutePath() + " -> "
                            + newIconFile.getAbsolutePath());
                }

                // Clear the unarchive directory
                File unarchiveDir = new File(unarchiveDirPath);
                for (File file : unarchiveDir.listFiles()) {
                    file.delete();
                }

                // Insert to app list
                mAppList.put(appId, new ANTApp(appId, name, newIconFile.getAbsolutePath(), false));
                return true;
            } catch (IOException e) {
                Log.e(TAG, "Failed to get app information from manifest file.");
                e.printStackTrace();
                return false;
            }
        }

        private String getFieldFromManifest(String manifestFilePath, String fieldName) {
            File manifestFile = new File(manifestFilePath);
            try {
                XmlPullParserFactory xmlParserFactory = XmlPullParserFactory.newInstance();
                XmlPullParser xmlParser = xmlParserFactory.newPullParser();
                FileInputStream fileInputStream = new FileInputStream(manifestFile);
                xmlParser.setInput(fileInputStream, null);
                int event = xmlParser.getEventType();
                while (event != XmlPullParser.END_DOCUMENT) {
                    String tagName = xmlParser.getName();
                    if (tagName != null && tagName.compareTo(fieldName) == 0) {
                        xmlParser.next();
                        return xmlParser.getText();
                    }
                    event = xmlParser.getEventType();
                    xmlParser.next();
                }
                return null;
            } catch (XmlPullParserException e) {
                e.printStackTrace();
                return null;
            } catch (FileNotFoundException e) {
                e.printStackTrace();
                return null;
            } catch (IOException e) {
                e.printStackTrace();
                return null;
            }
        }

        public void onAppStateChanged(int appId, int appState) {
            String packageFilePath = this.mInstallTransactions.get(appId);
            if (packageFilePath != null) {
                if (appState == ANTApp.State_Ready) {
                    // Remove the package file if the install has done
                    this.mInstallTransactions.remove(appId);
                    File packageFile = new File(packageFilePath);
                    packageFile.delete();
                }
            }
        }
    }

    private class PrivateAppCoreStubListener implements TargetDeviceStubListener {
        // TargetDeviceStubListener
        @Override
        public void onCommChannelStateChanged(int prevState, int newState) {
            ANTControllerBroadcastSender.onCommChannelStateChanged(self, prevState, newState);
        }

        @Override
        public void onAckGetAppList(BaseMessage message) {
            // Get parameters
            String iconArchiveFilePath = message.getStoredFilePath();
            AppCoreAckMessage payload = (AppCoreAckMessage) message.getPayload();
            int commandMessageId = payload.getCommandMessageId();
            ParamsGetAppList params = payload.getParamsGetAppList();
            ArrayList<ParamAppListEntry> originalAppList = params.appList;

            // Listeners
            mUpdateAppListProcedure.onAckGetAppList(originalAppList);
        }

        @Override
        public void onAckInitializeApp(BaseMessage message) {
            // Get parameters
            int messageId = message.getMessageId();
            AppCoreAckMessage payload = (AppCoreAckMessage) message.getPayload();
            ParamsInitializeApp params = payload.getParamsInitializeApp();
            int appId = params.appId;

            // Listeners
            mInstallProcedure.onInitializedApp(messageId, appId);
        }

        @Override
        public void onAckListenAppState(BaseMessage message) {
            // Get parameters
            AppCoreAckMessage payload = (AppCoreAckMessage) message.getPayload();
            ParamsListenAppState params = payload.getParamsListenAppState();
            int appId = params.appId;
            int appState = params.appState;

            // Listeners
            mAppList.get(appId).setState(appState);
            mInstallProcedure.onAppStateChanged(appId, appState);
            ANTControllerBroadcastSender.onAppStateChanged(self, appId, appState);
        }

        @Override
        public void onAckGetFileList(BaseMessage message) {
            // Get parameters
            AppCoreAckMessage payload = (AppCoreAckMessage) message.getPayload();
            int commandMessageId = payload.getCommandMessageId();
            ParamsGetFileList params = payload.getParamsGetFileList();
            String path = params.path;
            ParamFileListEntry[] fileList = params.fileList.toArray(new ParamFileListEntry[params
                    .fileList.size()]);

            // Listeners
            ANTControllerBroadcastSender.onResultGetFileList(self, commandMessageId, path,
                    fileList);
        }

        @Override
        public void onAckGetFile(BaseMessage message) {
            // Get parameters
            AppCoreAckMessage payload = (AppCoreAckMessage) message.getPayload();
            int commandMessageId = payload.getCommandMessageId();
            String storedFileName = message.getStoredFilePath();

            // Listeners
            ANTControllerBroadcastSender.onResultGetFile(self, commandMessageId, storedFileName);
        }

        @Override
        public void onAckGetRootPath(BaseMessage message) {
            // Get parameters
            AppCoreAckMessage payload = (AppCoreAckMessage) message.getPayload();
            int commandMessageId = payload.getCommandMessageId();
            ParamsGetRootPath params = payload.getParamsGetRootPath();
            String rootPath = params.rootPath;

            // Listeners
            ANTControllerBroadcastSender.onResultGetTargetRootPath(self, commandMessageId,
                    rootPath);
        }

        @Override
        public void onSendEventPage(BaseMessage message) {
            // Get parameters
            CompanionMessage payload = (CompanionMessage) message.getPayload();
            ParamsSendEventPage params = payload.getParamsSendEventPage();
            int appId = params.appId;
            String legacyData = params.legacyData;
            boolean isNoti = params.isNoti;

            // Listeners
            self.onReceivedEvent(appId, legacyData, isNoti);
            ANTControllerBroadcastSender.onReceivedEvent(self, appId, legacyData, isNoti);
        }

        @Override
        public void onSendConfigPage(BaseMessage message) {
            // Get parameters
            CompanionMessage payload = (CompanionMessage) message.getPayload();
            ParamsSendConfigPage params = payload.getParamsSendConfigPage();
            int appId = params.appId;
            String legacyData = params.legacyData;

            // Listeners
            self.onReceivedConfig(appId, legacyData);
            ANTControllerBroadcastSender.onReceivedAppConfig(self, appId, legacyData);
        }

        @Override
        public void onSendToCompanion(BaseMessage message) {
            // Get parameters
            CompanionMessage payload = (CompanionMessage) message.getPayload();
            ParamsSendToCompanion params = payload.getParamsSendToCompanion();
            String listenerName = params.listenerName;
            String data = params.data;

            // Listeners
            ANTControllerBroadcastSender.onReceivedDataFromTarget(self, listenerName, data);
        }

        @Override
        public void getAppIcon(BaseMessage message) {
            // Get parameters
            AppCoreAckMessage payload = (AppCoreAckMessage) message.getPayload();
            int commandMessageId = payload.getCommandMessageId();
            boolean isFileAttached = message.isFileAttached();
            String iconFilePath;
            if (isFileAttached) iconFilePath = message.getStoredFilePath();
            else iconFilePath = "";

            // Listeners
            mUpdateAppListProcedure.onAckGetAppIcon(commandMessageId, iconFilePath);
        }

        @Override
        public void onUpdateAppConfig(BaseMessage message) {
            // Get parameters
            AppAckMessage payload = (AppAckMessage) message.getPayload();
            int commandMessageId = payload.getCommandMessageId();
            ParamsUpdateAppConfig params = payload.getParamsUpdateAppConfig();
            boolean isSucceed = params.isSucceed;

            // TODO: success!
            // Listeners
            ANTControllerBroadcastSender.onResultUpdateAppConfig(self, commandMessageId,
                    isSucceed);
        }
    }

    // Android Service
    @Override
    public IBinder onBind(Intent intent) {
        this.mBindersCount++;
        if (this.mSettings == null) {
            this.mSettings = new Settings(this);
        }
        if (this.mTargetDeviceStubListener == null) {
            this.mTargetDeviceStubListener = new PrivateAppCoreStubListener();
        }
        if (this.mTargetDeviceStub == null) {
            this.mTargetDeviceStub = new TargetDeviceStub(this, this.mTargetDeviceStubListener,
                    this.mSettings.getTempDir().getAbsolutePath());
        }
        this.mEventList.open(this);
        return this.mBinder;
    }

    @Override
    public boolean onUnbind(Intent intent) {
        this.mBindersCount--;
        // If no one use this service, destroy connection with ANT device.
        if (this.mBindersCount == 0) {
            this.destroyConnectionAsync();
        }
        return false;
    }

    // Binder (for the caller of CommChannelService)
    public class ControllerBinder extends android.os.Binder {
        public ANTControllerService getService() {
            return ANTControllerService.this;
        }
    }
}