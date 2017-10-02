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

import android.graphics.BitmapFactory;

import com.ant.ant_manager.R;
import com.ant.ant_manager.view.MainActivity;

public class LargeDataTestMainIcon extends MainIcon {

    public LargeDataTestMainIcon(MainActivity ownerActivity) {
        super(ownerActivity, "Test LDPort", BitmapFactory.decodeResource(ownerActivity
                .getResources(), R.drawable.connect));
    }

    @Override
    public void onClick() {
        this.mOwnerActivity.testEnableLargeData();
    }

    @Override
    public void onLongClick() {
        this.mOwnerActivity.testEnableLargeData();
    }
}
