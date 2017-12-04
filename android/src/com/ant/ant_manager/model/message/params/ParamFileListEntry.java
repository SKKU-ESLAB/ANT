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

public class ParamFileListEntry implements Parcelable {
    public ParamFileListEntry(String fileName, int fileType, int fileSizeBytes, String fileTime) {
        this.fileName = fileName;
        this.fileType = fileType;
        this.fileSizeBytes = fileSizeBytes;
        this.fileTime = fileTime;
    }

    public String fileName;
    public int fileType;
    public int fileSizeBytes;
    public String fileTime;

    // Android Parcelable
    @Override
    public int describeContents() {
        return 0;
    }

    @Override
    public void writeToParcel(Parcel out, int flags) {
        // ParamFileListEntry -> Parcel
        // fileName
        out.writeString(this.fileName);

        // fileType
        out.writeInt(this.fileType);

        // fileSizeBytes
        out.writeInt(this.fileSizeBytes);

        // fileTime
        out.writeString(this.fileTime);
    }

    public static final Parcelable.Creator<ParamFileListEntry> CREATOR = new Parcelable
            .Creator<ParamFileListEntry>() {
        public ParamFileListEntry createFromParcel(Parcel in) {
            // Parcel -> ParamFileListEntry
            String fileName = in.readString();
            int fileType = in.readInt();
            int fileSizeBytes = in.readInt();
            String fileTime = in.readString();

            ParamFileListEntry entry = new ParamFileListEntry(fileName, fileType, fileSizeBytes,
                    fileTime);
            return entry;
        }

        public ParamFileListEntry[] newArray(int size) {
            return new ParamFileListEntry[size];
        }
    };
}
