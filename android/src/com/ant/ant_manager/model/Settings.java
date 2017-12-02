package com.ant.ant_manager.model;

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

import android.content.Context;
import android.util.Log;

import java.io.File;

public class Settings {
    private static final String TAG = "Settings";

    private File mANTDir;
    private File mANTIconDir;
    private File mANTRemoteUIDir;
    private File mANTRemoteStorageDir;
    private File mANTCloudDir;
    private File mANTTempDir;

    public Settings(Context context) {
        File antDir = new File(context.getFilesDir().getAbsolutePath(), "ant");
        File antRUIDir = new File(antDir, "remoteUI");
        File antRemoteStorageDir = new File(antDir, "remoteStorage");
        File antIconDir = new File(antDir, "icons");
        File antCloudDir = new File(antDir, "cloud");
        File tempDir = new File(antDir, "temp");

        if (!antDir.exists()) {
            if (!antDir.mkdir()) {
                Log.e(TAG, "Failed to make ANT root directory");
            }
        }
        if (!antRUIDir.exists()) {
            if (!antRUIDir.mkdir()) {
                Log.e(TAG, "Failed to make ANT remote UI directory");
            }
        }
        if (!antRemoteStorageDir.exists()) {
            if (!antRemoteStorageDir.mkdir()) {
                Log.e(TAG, "Failed to make ANT remote storage directory");
            }
        }
        if (!antIconDir.exists()) {
            if (!antIconDir.mkdir()) {
                Log.e(TAG, "Failed to make ANT icon directory");
            }
        }
        if (!antCloudDir.exists()) {
            if (!antCloudDir.mkdir()) {
                Log.e(TAG, "Failed to make ANT cloud service directory");
            }
        }
        if (!tempDir.exists()) {
            if (!tempDir.mkdir()) {
                Log.e(TAG, "Failed to make ANT temp directory");
            }
        }

        this.mANTDir = antDir;
        this.mANTRemoteUIDir = antRUIDir;
        this.mANTRemoteStorageDir = antRemoteStorageDir;
        this.mANTIconDir = antIconDir;
        this.mANTCloudDir = antCloudDir;
        this.mANTTempDir = tempDir;
    }

    // Getters/Setters
    public File getAntDir() {
        return mANTDir;
    }

    public File getIconDir() {
        return mANTIconDir;
    }

    public File getRemoteUIDir() {
        return mANTRemoteUIDir;
    }

    public File getRemoteStorageDir() {
        return mANTRemoteStorageDir;
    }

    public File getCloudDir() {
        return mANTCloudDir;
    }

    public File getTempDir() {
        return mANTTempDir;
    }
}
