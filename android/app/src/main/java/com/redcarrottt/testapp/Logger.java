package com.redcarrottt.testapp;

/* Copyright (c) 2018, contributors. All rights reserved.
 *
 * Contributor: 
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

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

class LogReceiver extends BroadcastReceiver {
    public static final String kAction = "LogBroadcast";
    public static final String kKeyLogMessage = "LogMessage";

    private Callback mCallback;

    public LogReceiver(Callback callback) {
        this.mCallback = callback;
    }

    @Override
    public void onReceive(Context context, Intent intent) {
        String action = intent.getAction();
        if (action.compareTo(kAction) == 0) {
            String logMessage = intent.getStringExtra(kKeyLogMessage);
            this.mCallback.onLogMessage(logMessage);
        }
    }

    interface Callback {
        public void onLogMessage(String logMessage);
    }
}

public class Logger {
    public static void print(Context context, String logMessage) {
        Intent broadcastIntent = new Intent();
        broadcastIntent.setAction(LogReceiver.kAction);
        broadcastIntent.putExtra(LogReceiver.kKeyLogMessage, logMessage);
        context.sendBroadcast(broadcastIntent);
    }
    public static void print(String logMessage) {
        print(defaultContext, logMessage);
    }
    public static void print(String kTag, String logMessage) {
        print("[" + kTag + "] " + logMessage);
    }
    public static void setDefaultContext(Context context) {
        Logger.defaultContext = context;
    }
    private static Context defaultContext;
}