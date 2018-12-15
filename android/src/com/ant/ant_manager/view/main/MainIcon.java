package com.ant.ant_manager.view.main;

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

import android.graphics.Bitmap;
import android.util.Log;
import android.widget.ImageView;
import android.widget.PopupWindow;

import com.ant.ant_manager.view.MainActivity;
import com.ant.ant_manager.view.remoteui.RemoteNotiPopupView;

public abstract class MainIcon {
    private static final String TAG = "MainIcon";
    private String mTitle;
    private Bitmap mIconBitmap;
    private int mAppId;

    protected MainActivity mOwnerActivity;
    RemoteNotiPopupView mPopupView;
    ImageView mIconImageView;

    public MainIcon(MainActivity ownerActivity, String title, Bitmap iconBitmap, int appId) {
        this.mOwnerActivity = ownerActivity;
        this.mTitle = title;
        this.mIconBitmap = iconBitmap;
        this.mAppId = appId;
        this.mPopupView = null;
        this.mIconImageView = null;
    }

    public void setIconImageView(ImageView iconImageView) {
        this.mIconImageView = iconImageView;
    }

    public void updatePopupView(String description, String attachedFilePath) {
        if (this.mIconImageView == null) {
            Log.e(TAG, "No icon image view mapped");
            return;
        }

        if (this.mPopupView == null) {
            this.mPopupView = new RemoteNotiPopupView(this.mOwnerActivity, description,
                    attachedFilePath);
            this.mPopupView.setOnDismissListener(this.mPopupViewDismissListener);
            this.mPopupView.show(this.mIconImageView);
        } else {
            this.mPopupView.setText(description);
            this.mPopupView.setImage(attachedFilePath);
        }
    }

    private PopupWindow.OnDismissListener mPopupViewDismissListener = new PopupWindow
            .OnDismissListener() {
        @Override
        public void onDismiss() {
            mPopupView = null;
        }
    };

    public String getTitle() {
        return this.mTitle;
    }

    public Bitmap getIconBitmap() {
        return this.mIconBitmap;
    }

    public int getAppId() {
        return this.mAppId;
    }

    abstract public void onClick();

    abstract public void onLongClick();
}