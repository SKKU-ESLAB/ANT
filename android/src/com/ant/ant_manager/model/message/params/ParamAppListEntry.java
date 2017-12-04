package com.ant.ant_manager.model.message.params;

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

import android.os.Parcel;
import android.os.Parcelable;

public class ParamAppListEntry implements Parcelable {
    public ParamAppListEntry(int appId, String appName, boolean isDefaultApp) {
        this.appId = appId;
        this.appName = appName;
        this.isDefaultApp = isDefaultApp;
    }

    public int appId;
    public String appName;
    public boolean isDefaultApp;

    // Android Parcelable
    @Override
    public int describeContents() {
        return 0;
    }

    @Override
    public void writeToParcel(Parcel out, int flags) {
        // ParamAppListEntry -> Parcel
        // appId
        out.writeInt(this.appId);

        // appName
        out.writeString(this.appName);

        // isDefaultApp
        out.writeInt((this.isDefaultApp) ? 1 : 0);
    }

    public static final Parcelable.Creator<ParamAppListEntry> CREATOR = new Parcelable
            .Creator<ParamAppListEntry>() {
        public ParamAppListEntry createFromParcel(Parcel in) {
            // Parcel -> ParamAppListEntry
            int appId = in.readInt();
            String appName = in.readString();
            boolean isDefaultApp = (in.readInt() != 0);

            ParamAppListEntry entry = new ParamAppListEntry(appId, appName, isDefaultApp);
            return entry;
        }

        public ParamAppListEntry[] newArray(int size) {
            return new ParamAppListEntry[size];
        }
    };
}
