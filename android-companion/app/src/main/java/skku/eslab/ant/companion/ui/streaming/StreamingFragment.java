package skku.eslab.ant.companion.ui.streaming;

import android.os.Bundle;
import android.os.StrictMode;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.SurfaceHolder;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;
import android.widget.Toast;

import com.ant.ant_manager.view.cameraviewer.GStreamerSurfaceView;

import org.freedesktop.gstreamer.GStreamer;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentActivity;
import androidx.lifecycle.Observer;
import androidx.lifecycle.ViewModelProviders;
import skku.eslab.ant.companion.R;
import skku.eslab.ant.companion.remoteuiapi.RemoteUIAPI;

public class StreamingFragment extends Fragment
        implements SurfaceHolder.Callback {

    private StreamingViewModel mModel;

    private GStreamerSurfaceView mVideoSurfaceView;
    private TextView mMessageTextView;

    @Override
    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        this.mModel = ViewModelProviders.of(this).get(StreamingViewModel.class);
        View root =
                inflater.inflate(R.layout.fragment_streaming, container, false);
        this.mVideoSurfaceView = root.findViewById(R.id.videoSurfaceView);
        this.mMessageTextView = root.findViewById(R.id.messageTextView);

        FragmentActivity activity = getActivity();
        assert activity != null;
        // Initialize GStreamer and warn if it fails
        try {
            GStreamer.init(activity);
        } catch (Exception e) {
            Toast.makeText(activity, e.getMessage(), Toast.LENGTH_LONG).show();
            activity.finish();
        }

        StrictMode.ThreadPolicy policy =
                new StrictMode.ThreadPolicy.Builder().permitAll().build();
        StrictMode.setThreadPolicy(policy);
        SurfaceHolder surfaceHolder = this.mVideoSurfaceView.getHolder();
        surfaceHolder.addCallback(this);

        // Retrieve our previous state, or connectChannel it to default values
        this.mIsPlayingDesired = false;

        RemoteUIAPI.get().getPipeline().observe(this, new Observer<String>() {
            @Override
            public void onChanged(@Nullable String value) {
                updateVideoSurfaceView();
            }
        });
        return root;
    }

    @Override
    public void onDestroyView() {
        super.onDestroyView();

        SurfaceHolder surfaceHolder = this.mVideoSurfaceView.getHolder();
        surfaceHolder.removeCallback(this);

        // Finalize gstreamer
        this.finalizeGstreamer();
    }

    // Initialize native code, build pipeline, etc
    private native void nativeInit(String pipeline);

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

    // Native code will use this to keep private data
    // DO NOT REMOVE IT! (Called by native function)
    private long native_custom_data;

    // Whether the user asked to go to PLAYING
    private boolean mIsPlayingDesired;

    private Object mDelayedSurface;

    private void updateVideoSurfaceView() {
        String pipeline = RemoteUIAPI.get().getPipeline().getValue();
        if (pipeline == null || pipeline.isEmpty()) {
            return;
        }
        if (this.mIsPlayingDesired) {
            this.finalizeGstreamer();
        }
        this.initializeGstreamer(pipeline);
    }

    // Initialize Gstreamer connection
    private void initializeGstreamer(String pipeline) {
        Log.d("CameraViewerActivity", "initializeGstreamer()");
        if (this.mDelayedSurface == null) {
            return;
        }
        this.nativeInit(pipeline);

        try {
            // Wait until gstreamer app function is launched
            Thread.sleep(2000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        this.nativeSurfaceInit(this.mDelayedSurface);

        this.mIsPlayingDesired = true;
        this.nativePlay();
    }

    private void finalizeGstreamer() {
        if (!this.mIsPlayingDesired) {
            return;
        }

        this.nativePause();
        this.nativeFinalize();
        this.mIsPlayingDesired = false;
    }

    static {
        System.loadLibrary("gstreamer_android");
        System.loadLibrary("streaming-fragment");
        nativeClassInit();
    }

    public void surfaceChanged(SurfaceHolder holder, int format, int width,
                               int height) {
        Log.d("GStreamer",
                "Surface changed to format " + format + " width " + width +
                        " height " + height);
        Log.d("GStreamer", "GetSurface(): " + holder.getSurface());
        nativeSurfaceInit(holder.getSurface());
        this.mDelayedSurface = holder.getSurface();

        updateVideoSurfaceView();

        Log.d("GStreamer", "end surfaceChanged");
    }

    public void surfaceCreated(SurfaceHolder holder) {
        Log.d("GStreamer", "Surface created: " + holder.getSurface());
    }

    public void surfaceDestroyed(SurfaceHolder holder) {
        Log.d("GStreamer", "Surface destroyed");
        nativeSurfaceFinalize();
    }

    // Called from native code. This sets the content of the TextView from
    // the UI thread.
    // DO NOT REMOVE IT! (Called by native function)
    private void setMessage(final String message) {
        FragmentActivity activity = getActivity();
        activity.runOnUiThread(new Runnable() {
            public void run() {
                mMessageTextView.setText(message);
            }
        });
    }

    // Called from native code. Native code calls this once it has created
    // its pipeline and the camera_viewer loop is running, so it is ready to
    // accept commands.
    // DO NOT REMOVE IT! (Called by native function)
    private void onGStreamerInitialized() {
        nativePlay();
    }

    // DO NOT REMOVE IT! (Called by native function)
    // Called from native code when the size of the media changes or is first
    // detected.
    // Inform the video surface about the new size and recalculate the layout.
    private void onMediaSizeChanged(int width, int height) {
        Log.i("GStreamer", "Media size changed to " + width + "x" + height);
        FragmentActivity activity = this.getActivity();
        this.mVideoSurfaceView.media_width = width;
        this.mVideoSurfaceView.media_height = height;
        activity.runOnUiThread(new Runnable() {
            public void run() {
                mVideoSurfaceView.requestLayout();
            }
        });
    }
}