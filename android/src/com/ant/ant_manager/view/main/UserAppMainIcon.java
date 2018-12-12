package com.ant.ant_manager.view.main;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.util.Log;
import android.widget.Toast;

import com.ant.ant_manager.model.ANTApp;
import com.ant.ant_manager.view.MainActivity;

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

public class UserAppMainIcon extends MainIcon {
    private static String TAG = "UserAppMainIcon";
    private String mAppName;
    private int mAppState;

    public UserAppMainIcon(MainActivity ownerActivity, int appId, String appName, int iconId, int
            appState) {
        super(ownerActivity, appName, BitmapFactory.decodeResource(ownerActivity.getResources(),
                iconId), appId);
        this.mAppName = appName;
        this.mAppState = appState;
    }

    public UserAppMainIcon(MainActivity ownerActivity, int appId, String appName, String
            iconFilePath, int appState) {
        super(ownerActivity, appName, BitmapFactory.decodeFile(iconFilePath), appId);
        this.mAppName = appName;
        this.mAppState = appState;
    }

    public String getAppName() {
        return this.mAppName;
    }

    public int getAppState() {
        return this.mAppState;
    }

    public String getAppStateString() {
        switch (this.mAppState) {
            case ANTApp.State_Initialized:
            case ANTApp.State_Initializing:
            case ANTApp.State_Installing:
                return "Installing";
            case ANTApp.State_Ready:
                return "Ready";
            case ANTApp.State_Launching:
                return "Launching";
            case ANTApp.State_Running:
                return "Running";
            case ANTApp.State_Removing:
                return "Removing";
            case ANTApp.State_Removed:
                return "Removed";
            default:
                return "Unknown";
        }
    }

    public void updateAppState(int appState) {
        this.mAppState = appState;

        // UI update corresponding to the app's state
        switch (this.mAppState) {
            case ANTApp.State_Initialized:
            case ANTApp.State_Initializing:
            case ANTApp.State_Installing:
                // TODO: make the icon grey
                break;
            case ANTApp.State_Ready:
                // TODO: make the icon normal
                break;
            case ANTApp.State_Launching:
                // TODO: make the icon active a little
                break;
            case ANTApp.State_Running:
                // TODO: make the icon active so much
                break;
            case ANTApp.State_Removing:
                // TODO: make the icon grey
                break;
            case ANTApp.State_Removed:
                // TODO: make the icon transparent
                break;
            default:
                // ignore
                break;
        }
    }

    @Override
    public void onClick() {
        if (!this.mOwnerActivity.isTargetDeviceConnected()) {
            Toast.makeText(this.mOwnerActivity.getApplicationContext(), "Target device is not " +
                    "connected", Toast.LENGTH_SHORT).show();
            return;
        }

        switch (this.mAppState) {
            case ANTApp.State_Initialized:
            case ANTApp.State_Initializing:
            case ANTApp.State_Installing:
                Toast.makeText(this.mOwnerActivity, "This app (" + mAppName + ") is not ready.",
                        Toast.LENGTH_LONG).show();
                break;
            case ANTApp.State_Ready:
                // Launch the application
                this.mOwnerActivity.launchApp(this.getAppId());
                break;
            case ANTApp.State_Launching:
                Toast.makeText(this.mOwnerActivity, "This app (" + mAppName + ") is being " +
                        "launched.", Toast.LENGTH_LONG).show();
                break;
            case ANTApp.State_Running:
                // Show remote config UI
                this.mOwnerActivity.showRemoteConfigUI(this.getAppId());
                break;
            case ANTApp.State_Removing:
                Toast.makeText(this.mOwnerActivity, "Application is being removed...", Toast
                        .LENGTH_LONG).show();
                break;
            case ANTApp.State_Removed:
                Toast.makeText(this.mOwnerActivity, "Application has already been removed.",
                        Toast.LENGTH_LONG).show();
                break;
            default:
                Toast.makeText(this.mOwnerActivity, "Application's state is invalid!", Toast
                        .LENGTH_LONG).show();
                break;
        }

    }

    @Override
    public void onLongClick() {
        switch (this.mAppState) {
            case ANTApp.State_Initialized:
            case ANTApp.State_Initializing:
            case ANTApp.State_Installing:
                // TODO: AppCore Manager state management has a problem. This is temporary solution.
//                Toast.makeText(this.mOwnerActivity, "This app (" + mAppName + ") is not ready.",
//                        Toast.LENGTH_LONG).show();
//                break;
            case ANTApp.State_Ready:
//                Toast.makeText(this.mOwnerActivity, "This app (" + mAppName + ") is not running"
//                        + ".", Toast.LENGTH_LONG).show();
//                break;
            case ANTApp.State_Launching:
            case ANTApp.State_Running:
                // Terminate application
                AlertDialog.Builder alt_bld = new AlertDialog.Builder(this.mOwnerActivity);
                alt_bld.setMessage("Terminate this App ?").setCancelable(false).setPositiveButton
                        ("Yes", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        mOwnerActivity.terminateApp(getAppId());
                        Log.d(TAG, "Request to kill ");
                    }
                }).setNegativeButton("No", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        dialog.cancel();
                    }
                });

                AlertDialog alert = alt_bld.create();
                alert.setTitle(this.getTitle());
                Drawable d = new BitmapDrawable(this.getIconBitmap());
                alert.setIcon(d);
                alert.show();
                break;
            case ANTApp.State_Removing:
                Toast.makeText(this.mOwnerActivity, "Application is being removed...", Toast
                        .LENGTH_LONG).show();
                break;
            case ANTApp.State_Removed:
                Toast.makeText(this.mOwnerActivity, "Application has already been removed.",
                        Toast.LENGTH_LONG).show();
                break;
            default:
                Toast.makeText(this.mOwnerActivity, "Application's state is invalid!", Toast
                        .LENGTH_LONG).show();
                break;
        }
    }
}
