package com.ant.ant_manager.view;

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

import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.os.PowerManager;
import android.os.StrictMode;
import android.util.Log;
import android.view.MenuItem;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.inputmethod.InputMethodManager;
import android.widget.TextView;
import android.widget.Toast;

import com.ant.cmfw.service.CommChannelService;
import com.ant.ant_manager.R;
import com.ant.ant_manager.controller.ANTControllerBroadcastReceiver;
import com.ant.ant_manager.controller.ANTControllerService;
import com.ant.ant_manager.view.cameraviewer.GStreamerSurfaceView;

import org.freedesktop.gstreamer.GStreamer;

public class CameraViewerActivity extends Activity implements SurfaceHolder.Callback {
    private static final String TAG = "CameraViewerActivity";

    // Intent
    public static final String INTENT_KEY_APP_ID = "appId";

    // ANTControllerService
    private ANTControllerService mControllerServiceStub = null;
    private PrivateControllerBroadcastReceiver mControllerBroadcastReceiver;

    private int mAppId;

    // Initialize native code, build pipeline, etc
    private native void nativeInit(String addr, String vport);

    // Destroy pipeline and shutdown native code
    private native void nativeFinalize();

    // Set the URI of the media to play
    private native void nativeSetUri(String uri);

    // Set pipeline to PLAYING
    private native void nativePlay();

    // Set pipeline to PAUSED
    private native void nativePause();

    // Initialize native class: cache Method IDs for callbacks
    private static native boolean nativeClassInit();

    // A new surface is available
    private native void nativeSurfaceInit(Object surface);

    // Surface about to be destroyed
    private native void nativeSurfaceFinalize();

    // Native code will use this to keep private mMainIconList
    private long native_custom_data;


    // Whether the user asked to go to PLAYING
    private boolean is_playing_desired;

    // Current position, reported by native code
    private int position;

    // Current clip duration, reported by native code
    private int duration;

    // Whether this clip is stored locally or is being streamed
    private boolean is_local_media;

    // Position where the users want to seek to
    private int desired_position;

    // URI of the clip being played
    private String mediaUri;

    private final int kANTCameraPort = 5000;

    private Object mDelayedSurface;

    // Maximum 1 frame JPEG, size of 1080p
    private byte[] frame;

    // WakeLock
    private PowerManager.WakeLock mCpuWakeLock;

    private void acquireCpuWakeLock() {
        Context context = this;
        PowerManager pm = (PowerManager) context.getSystemService(Context.POWER_SERVICE);
        mCpuWakeLock = pm.newWakeLock(PowerManager.SCREEN_BRIGHT_WAKE_LOCK | PowerManager
                .ACQUIRE_CAUSES_WAKEUP | PowerManager.ON_AFTER_RELEASE, "ANT Camera Wakelock");
        mCpuWakeLock.acquire();
    }

    private void releaseCpuWakeLock() {
        if (mCpuWakeLock != null) mCpuWakeLock.release();
    }

    // Called when the activity is first created.
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // Parameters
        Intent intent = this.getIntent();
        this.mAppId = intent.getIntExtra(INTENT_KEY_APP_ID, -1);
        if (this.mAppId < 0) {
            Log.e(TAG, "Invalid application id!");
            this.finish();
        }

        // Initialize GStreamer and warn if it fails
        try {
            GStreamer.init(this);
            //Toast.makeText(getApplicationContext(), "Gstreamer Set
            // success", Toast.LENGTH_LONG).show();

        } catch (Exception e) {
            Toast.makeText(this, e.getMessage(), Toast.LENGTH_LONG).show();
            this.finish();
            return;
        }

        // Initialize UI
        setContentView(R.layout.activity_camera_viewer);

        if (android.os.Build.VERSION.SDK_INT > 9) {
            StrictMode.ThreadPolicy policy = new StrictMode.ThreadPolicy.Builder().permitAll()
                    .build();
            StrictMode.setThreadPolicy(policy);
        }

        SurfaceView sv = (SurfaceView) this.findViewById(R.id.surface_video);
        SurfaceHolder sh = sv.getHolder();
        sh.addCallback(this);

        // Retrieve our previous state, or connectChannel it to default values
        if (savedInstanceState != null) {
            is_playing_desired = savedInstanceState.getBoolean("playing");
            position = savedInstanceState.getInt("position");
            duration = savedInstanceState.getInt("duration");
            mediaUri = savedInstanceState.getString("mediaUri");
            Log.i("GStreamer", "Activity created with saved state:");
        } else {
            is_playing_desired = false;
            position = duration = 0;
            mediaUri = "http://docs.gstreamer.com/media/sintel_trailer-368p.ogv";
            Log.i("GStreamer", "Activity created with no saved state:");
        }
        is_local_media = false;
        Log.i("GStreamer", "  playing:" + is_playing_desired + " position:" + position + " " +
                "duration: " + duration + " uri: " + mediaUri);

    }

    // Initialize Gstreamer connection
    public void initializeGstreamerConnection(String ipAddress, int port) {
        Log.d("CameraViewerActivity", "initializeGstreamerConnection()");
        nativeInit(ipAddress, "" + port);

        try {
            Thread.sleep(2000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        nativeSurfaceInit(mDelayedSurface);

        is_playing_desired = true;
        nativePlay();
    }


    protected void onSaveInstanceState(Bundle outState) {
        Log.d("GStreamer", "Saving state, playing:" + is_playing_desired + " " + "position:" +
                position + " duration: " + duration + " uri: " + mediaUri);
        outState.putBoolean("playing", is_playing_desired);
        outState.putString("mediaUri", mediaUri);
    }

    @Override
    protected void onResume() {
        super.onResume();

        // Initialize camera viewer's connection
        this.initializeCameraViewer();

        // Acquire wakelock
        this.acquireCpuWakeLock();
    }

    @Override
    protected void onPause() {
        super.onPause();
        mControllerServiceStub.terminateAppOneWay(this.mAppId);
        mControllerServiceStub.unlockLargeDataMode();
        this.releaseCpuWakeLock();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

        // Finalize gstreamer
        this.nativeFinalize();

        // Terminate JavaScript CameraViewer App
        this.mControllerServiceStub.terminateAppOneWay(this.mAppId);

        // Disconnect Controller Service
        this.disconnectControllerService();
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case android.R.id.home:
                this.finish();
                return true;
            default:
                return super.onOptionsItemSelected(item);
        }
    }

    // Called from native code. This sets the content of the TextView from
    // the UI thread.
    private void setMessage(final String message) {
        final TextView tv = (TextView) this.findViewById(R.id.textview_message);
        runOnUiThread(new Runnable() {
            public void run() {
                tv.setText(message);
            }
        });
    }

    // Set the URI to play, and record whether it is a local or remote file
    private void setMediaUri() {
        nativeSetUri(mediaUri);
        is_local_media = mediaUri.startsWith("file://");
    }

    // Called from native code. Native code calls this once it has created
    // its pipeline and the camera_viewer loop is running, so it is ready to
    // accept
    // commands.
    private void onGStreamerInitialized() {
        Log.i("GStreamer", "GStreamer initialized:");
        Log.i("GStreamer", "  playing:" + is_playing_desired + " position:" + position + " uri: "
                + mediaUri);

        // Restore previous playing state
        setMediaUri();
        //nativeSetPosition (position);
        if (is_playing_desired) {
            nativePlay();
        } else {
            nativePause();
        }
    }

    static {
        System.loadLibrary("gstreamer_android");
        System.loadLibrary("camera-viewer");
        nativeClassInit();
    }

    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        Log.d("GStreamer", "Surface changed to format " + format + " width " + width + " height "
                + height);
        Log.d("GStreamer", "GetSurface(): " + holder.getSurface());
        //nativeSurfaceInit(holder.getSurface());
        this.mDelayedSurface = holder.getSurface();

        Log.d("GStreamer", "end surfaceChanged");
    }

    public void surfaceCreated(SurfaceHolder holder) {
        Log.d("GStreamer", "Surface created: " + holder.getSurface());
    }

    public void surfaceDestroyed(SurfaceHolder holder) {
        Log.d("GStreamer", "Surface destroyed");
        nativeSurfaceFinalize();
    }

    // Called from native code when the size of the media changes or is first
    // detected.
    // Inform the video surface about the new size and recalculate the layout.
    private void onMediaSizeChanged(int width, int height) {
        Log.i("GStreamer", "Media size changed to " + width + "x" + height);
        final GStreamerSurfaceView gsv = (GStreamerSurfaceView) this.findViewById(R.id
                .surface_video);
        gsv.media_width = width;
        gsv.media_height = height;
        runOnUiThread(new Runnable() {
            public void run() {
                gsv.requestLayout();
            }
        });
    }

    void exitFromRunLoop() {
    }

    private void initializeCameraViewer() {
        if (this.mControllerServiceStub == null) {
            this.connectControllerService();
        } else {
            int commChannelState = mControllerServiceStub.getCommChannelState();
            if (commChannelState != CommChannelService.STATE_CONNECTED_LARGE_DATA) {
                // If large mMainIconList mode is not enabled, request to enable large
                // mMainIconList mode
                mControllerServiceStub.enableLargeDataMode();
            } else {
                // If large mMainIconList mode has already been enabled, launch and open
                // camera viewer
                launchAndConnectToCameraViewer();
            }
        }
    }

    private void connectControllerService() {
        Intent serviceIntent = new Intent(this, ANTControllerService.class);
        this.bindService(serviceIntent, this.mControllerServiceConnection, Context
                .BIND_AUTO_CREATE);
    }

    private void disconnectControllerService() {
        if (this.mControllerServiceConnection != null)
            this.unbindService(this.mControllerServiceConnection);
        if (this.mControllerBroadcastReceiver != null)
            this.unregisterReceiver(this.mControllerBroadcastReceiver);
    }

    private ServiceConnection mControllerServiceConnection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName componentName, IBinder inputBinder) {
            ANTControllerService.ControllerBinder serviceBinder = (ANTControllerService
                    .ControllerBinder) inputBinder;
            mControllerServiceStub = serviceBinder.getService();

            // Set BroadcastReceiver
            IntentFilter broadcastIntentFilter = new IntentFilter();
            broadcastIntentFilter.addAction(ANTControllerBroadcastReceiver.ACTION);
            mControllerBroadcastReceiver = new PrivateControllerBroadcastReceiver();
            registerReceiver(mControllerBroadcastReceiver, broadcastIntentFilter);

            // Initialize connection with camera viewer
            initializeCameraViewer();
        }

        @Override
        public void onServiceDisconnected(ComponentName componentName) {
            Log.d(TAG, "onServiceDisconnected()");
            unregisterReceiver(mControllerBroadcastReceiver);
            mControllerServiceStub = null;
        }
    };

    class PrivateControllerBroadcastReceiver extends ANTControllerBroadcastReceiver {
        PrivateControllerBroadcastReceiver() {
            this.setOnCommChannelStateChangedListener(new OnCommChannelStateChangedListener() {
                @Override
                public void onCommChannelStateChanged(int prevState, int newState) {
                    if (newState == CommChannelService.STATE_CONNECTED_LARGE_DATA) {
                        // Succeed to connect large data port
                        launchAndConnectToCameraViewer();
                    } else if (newState == CommChannelService.STATE_CONNECTING_LARGE_DATA) {
                        Log.d(TAG, "Connecting large data port...");
                    } else {
                        Log.d(TAG, "Large data port disconnected or failed to connect");
                    }
                }
            });
        }
    }

    private void launchAndConnectToCameraViewer() {
        // Request to launch camera viewer app
        this.mControllerServiceStub.launchAppOneWay(mAppId);
        this.mControllerServiceStub.lockLargeDataMode();

        // Request to get large mMainIconList(Wi-fi Direct) IP address
        String ipAddress = this.mControllerServiceStub.getLargeDataIPAddress();

        // Initialize Gstreamer connection
        initializeGstreamerConnection(ipAddress, kANTCameraPort);
    }
}