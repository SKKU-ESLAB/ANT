package com.ant.cmfw.glue;

import android.app.Service;
import android.content.Intent;

public class CommBroadcastSender {
    public static void onCommChannelStateChanged(Service service, int prevState, int newState) {
        Intent broadcastIntent = new Intent();
        broadcastIntent.setAction(CommBroadcastReceiver.ACTION);
        broadcastIntent.putExtra(CommBroadcastReceiver.KEY_EVENT_TYPE,
                CommBroadcastReceiver.EVENT_TYPE_ON_COMM_CHANNEL_STATE_CHANGED);
        broadcastIntent.putExtra(CommBroadcastReceiver.KEY_COMM_CHANNEL_PREV_STATE,
                prevState);
        broadcastIntent.putExtra(CommBroadcastReceiver.KEY_COMM_CHANNEL_NEW_STATE,
                newState);
        service.sendBroadcast(broadcastIntent);
    }

    public static void onReceivedRawMessage(Service service,
                                            String message, String filePath) {
        Intent broadcastIntent = new Intent();
        broadcastIntent.setAction(CommBroadcastReceiver.ACTION);
        broadcastIntent.putExtra(CommBroadcastReceiver.KEY_EVENT_TYPE,
                CommBroadcastReceiver.EVENT_TYPE_ON_RECEIVED_RAW_MESSAGE);
        broadcastIntent.putExtra(CommBroadcastReceiver.KEY_MESSAGE,
                message);
        broadcastIntent.putExtra(CommBroadcastReceiver.KEY_FILE_PATH,
                filePath);
        service.sendBroadcast(broadcastIntent);
    }
}