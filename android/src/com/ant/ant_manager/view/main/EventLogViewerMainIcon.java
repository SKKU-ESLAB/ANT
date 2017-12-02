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

import android.content.Intent;
import android.graphics.BitmapFactory;
import android.widget.Toast;

import com.ant.ant_manager.R;
import com.ant.ant_manager.view.EventLogViewerActivity;
import com.ant.ant_manager.view.MainActivity;

public class EventLogViewerMainIcon extends MainIcon {

    public EventLogViewerMainIcon(MainActivity ownerActivity) {
        super(ownerActivity, "Event Log", BitmapFactory.decodeResource(ownerActivity.getResources
                (), R.drawable.eventlogger));
    }

    @Override
    public void onClick() {
        Intent intent = new Intent(this.mOwnerActivity, EventLogViewerActivity.class);
        mOwnerActivity.startActivity(intent);
    }

    @Override
    public void onLongClick() {
        // System app -> cannot terminate
        Toast.makeText(this.mOwnerActivity, "Built-in menu cannot be terminated.", Toast
                .LENGTH_LONG).show();
    }
}
