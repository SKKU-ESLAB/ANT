package com.ant.ant_manager.model;

/* Copyright (c) 2017 SKKU ESLAB, and contributors. All rights reserved.
 *
 * Contributor: Dongig Sin<dongig@skku.edu>
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

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.SQLException;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.provider.BaseColumns;
import android.util.Log;

import java.util.ArrayList;

public class ANTEventList {
    private static final String DATABASE_NAME = "ANTEventList.db";
    private static final int DATABASE_VERSION = 1;
    private SQLiteDatabase mDB;

    private ArrayList<ANTEvent> mEventList = new ArrayList<ANTEvent>();

    private class DatabaseHelper extends SQLiteOpenHelper {
        DatabaseHelper(Context context, String name, SQLiteDatabase.CursorFactory factory, int
                version) {
            super(context, name, factory, version);
        }

        public void onCreate(SQLiteDatabase db) {
            db.execSQL(EventListDBColumn._CREATE);
        }

        @Override
        public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
            db.execSQL("DROP TABLE IF EXISTS " + EventListDBColumn.TABLENAME);
            onCreate(db);
        }
    }

    public void open(Context context) throws SQLException {
        DatabaseHelper mEventDBHelper = new DatabaseHelper(context, DATABASE_NAME, null,
                DATABASE_VERSION);

        this.mDB = mEventDBHelper.getWritableDatabase();
        this.mEventList = getAllEventArrayList();
    }

    public void close() {
        this.mDB.close();
        this.mEventList.clear();
    }

    public ArrayList<ANTEvent> addEvent(String eventAppID, String eventAppName, String
            eventDescription, String eventTime, String eventJsonData) {
        ContentValues values = new ContentValues();

        values.put(EventListDBColumn.eventAppID, eventAppID);
        values.put(EventListDBColumn.eventAppName, eventAppName);
        values.put(EventListDBColumn.eventDescription, eventDescription);
        values.put(EventListDBColumn.eventTime, eventTime);
        values.put(EventListDBColumn.eventJsonData, eventJsonData);

        long id = mDB.insert(EventListDBColumn.TABLENAME, null, values);

        // success
        if (id != -1) {
            mEventList.add(0, new ANTEvent(id, eventAppID, eventAppName, eventDescription,
                    eventTime, eventJsonData));

            Log.d("ANT", "SUCCESS ADD EVENT");
        }

        return this.mEventList;
    }

    public ArrayList<ANTEvent> removeEvent(long id) {
        // TODO : erase img file that the noti page involve
        int _id = mDB.delete(EventListDBColumn.TABLENAME, "_id=" + id, null);
        if (_id > 0) {
            for (int i = 0; i < mEventList.size(); i++) {
                if (mEventList.get(i).getEventId() == id) {
                    mEventList.remove(i);
                    break;
                }
            }
        }
        return this.mEventList;
    }

    // Select All
    private Cursor getAllEvents() {
        return mDB.query(EventListDBColumn.TABLENAME, null, null, null, null, null, "eventTime "
                + "desc");
    }

    public ArrayList<ANTEvent> eventAllClear() {
        if (mDB.delete(EventListDBColumn.TABLENAME, null, null) > 0) {
            mEventList.clear();

        }
        return this.mEventList;
    }

    // Get event
    public ArrayList<ANTEvent> getCurEventArrayList() {
        return this.mEventList;
    }

    public ArrayList<ANTEvent> getAllEventArrayList() {
        Cursor mCursor;
        ArrayList<ANTEvent> list = new ArrayList<ANTEvent>();
        mCursor = null;
        mCursor = getAllEvents();

        while (mCursor.moveToNext()) {

            ANTEvent e = new ANTEvent(mCursor.getLong(mCursor.getColumnIndex("_id")), mCursor
                    .getString(mCursor.getColumnIndex("eventAppID")), mCursor.getString(mCursor
                    .getColumnIndex("eventAppName")), mCursor.getString(mCursor.getColumnIndex
                    ("eventDescription")), mCursor.getString(mCursor.getColumnIndex("eventTime"))
                    , mCursor.getString(mCursor.getColumnIndex("eventJsonData")));

            list.add(e);
        }

        mCursor.close();
        this.mEventList = list;
        return this.mEventList;
    }
}

class EventListDBColumn implements BaseColumns {
    static final String eventAppID = "eventAppID";
    static final String eventAppName = "eventAppName";
    static final String eventDescription = "eventDescription";
    static final String eventTime = "eventTime";
    static final String eventJsonData = "eventJsonData";

    static final String TABLENAME = "EVENTTABLE";
    static final String _CREATE = "create table " + TABLENAME + "" + "(" + _ID + " " + "integer " +
            "primary key autoincrement, " + eventAppID + " text not null , " + eventAppName + " " +
            "text not " + "null , " + eventDescription + " text not null , " + eventTime + " text" +
            " not null , " + eventJsonData + " text not " + "null )";
}