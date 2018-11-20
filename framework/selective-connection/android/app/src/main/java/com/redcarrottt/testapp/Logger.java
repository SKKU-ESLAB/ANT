package com.redcarrottt.testapp;

/* Copyright (c) 2018, contributors. All rights reserved.
 * Gyeonghwan Hong (redcarrottt@gmail.com)
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
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

class LogLevel {
    public static final int ERR = 1;
    public static final int WARN = 2;
    public static final int VERB = 3;
    public static final int DEBUG = 4;
}

class LogReceiver extends BroadcastReceiver {
    public static final String kAction = "LogBroadcast";
    public static final String kKeyLogLevel = "LogLevel";
    public static final String kKeyLogTag = "LogTag";
    public static final String kKeyLogMessage = "LogMessage";

    private Callback mCallback;

    public LogReceiver(Callback callback) {
        this.mCallback = callback;
    }

    @Override
    public void onReceive(Context context, Intent intent) {
        String action = intent.getAction();
        assert action != null;
        if (action.equals(kAction)) {
            String logTag = intent.getStringExtra(kKeyLogTag);
            String logMessage = intent.getStringExtra(kKeyLogMessage);
            int logLevel = intent.getIntExtra(kKeyLogLevel, LogLevel.DEBUG);
            this.mCallback.onLogMessage(logLevel, logTag, logMessage);
        }
    }

    interface Callback {
        void onLogMessage(final int logLevel, String logTag, String logMessage);
    }
}

public class Logger {
    private static void print(Context context, int logLevel, String logTag, String logMessage) {
        Intent broadcastIntent = new Intent();
        broadcastIntent.setAction(LogReceiver.kAction);
        broadcastIntent.putExtra(LogReceiver.kKeyLogLevel, logLevel);
        broadcastIntent.putExtra(LogReceiver.kKeyLogTag, logTag);
        broadcastIntent.putExtra(LogReceiver.kKeyLogMessage, logMessage);
        context.sendBroadcast(broadcastIntent);
    }

    private static void print(String tag, int logLevel, String logMessage) {
        print(defaultContext, logLevel, tag, logMessage);
    }

    public static void ERR(String tag, String logMessage) {
        print(tag, LogLevel.ERR, logMessage);
    }

    public static void WARN(String tag, String logMessage) {
        print(tag, LogLevel.WARN, logMessage);
    }

    public static void VERB(String tag, String logMessage) {
        print(tag, LogLevel.VERB, logMessage);
    }

    public static void DEBUG(String tag, String logMessage) {
        print(tag, LogLevel.DEBUG, logMessage);
    }

    public static void THREAD_LAUNCH(String threadName) {
        VERB(threadName, "Thread(" + threadName + "/" + Thread.currentThread().getId() + ") " +
                "Launch");
    }

    public static void THREAD_ADAPTER_LAUNCH(String adapterName, String threadName) {
        VERB(threadName, "Thread(" + adapterName + "-" + threadName + "/" + Thread.currentThread
                ().getId() + ") Launch");
    }

    public static void THREAD_FINISH(String threadName) {
        VERB(threadName, "Thread(" + threadName + "/" + Thread.currentThread().getId() + ") " +
                "Finish");
    }

    public static void THREAD_ADAPTER_FINISH(String adapterName, String threadName) {
        VERB(threadName, "Thread(" + adapterName + "-" + threadName + "/" + Thread.currentThread
                ().getId() + ") Finish");
    }

    public static void THREAD_FAIL(String threadName) {
        ERR(threadName, "Thread(" + threadName + "/" + Thread.currentThread().getId() + ") " +
                "Fail");
    }

    public static void THREAD_ADAPTER_FAIL(String adapterName, String threadName) {
        ERR(threadName, "Thread(" + adapterName + "-" + threadName + "/" + Thread.currentThread
                ().getId() + ") Fail");
    }

    public static void setDefaultContext(Context context) {
        Logger.defaultContext = context;
    }

    @SuppressLint("StaticFieldLeak")
    private static Context defaultContext;
}