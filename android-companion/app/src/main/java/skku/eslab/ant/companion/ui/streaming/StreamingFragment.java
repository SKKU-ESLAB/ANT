package skku.eslab.ant.companion.ui.streaming;

import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.PixelFormat;
import android.graphics.Rect;
import android.os.Bundle;
import android.os.StrictMode;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;
import android.widget.Toast;

import com.ant.ant_manager.view.cameraviewer.GStreamerSurfaceView;

import org.freedesktop.gstreamer.GStreamer;

import java.util.ArrayList;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentActivity;
import androidx.lifecycle.Observer;
import androidx.lifecycle.ViewModelProviders;
import skku.eslab.ant.companion.R;
import skku.eslab.ant.companion.remoteuiapi.BoundingBox;
import skku.eslab.ant.companion.remoteuiapi.RemoteUIAPI;

class BoundingBoxesManager implements SurfaceHolder.Callback {
    private final String TAG = "LabelSurfaceViewHolder";
    private ArrayList<BoundingBox> mBoundingBoxes = new ArrayList<>();

    // LabelSurfaceView SurfaceHolder
    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        drawCanvas(holder);
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int frmt, int w, int h) {
        Log.i(TAG, "surface changed: (" + w + ", " + h + ")");
        drawCanvas(holder);
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
    }

    public void setBoundingBoxes(ArrayList<BoundingBox> boundingBoxes) {
        this.mBoundingBoxes = boundingBoxes;
    }

    public void drawCanvas(SurfaceHolder holder) {
        Log.i(TAG, "Trying to draw...");

        Canvas canvas = holder.lockCanvas();
        if (canvas == null) {
            Log.e(TAG, "Cannot draw onto the canvas as it's null");
        } else {
            drawBoundingBoxes(canvas, this.mBoundingBoxes);
            holder.unlockCanvasAndPost(canvas);
        }
    }

    private void drawBoundingBoxes(final Canvas canvas, ArrayList<BoundingBox> boundingBoxes) {
        canvas.drawARGB(0, 128, 128, 128);

        for (BoundingBox boundingBox : boundingBoxes) {
            Rect rect = new Rect((int) boundingBox.xMin, (int) boundingBox.yMin, (int) boundingBox.xMax, (int) boundingBox.yMax);
            Paint paint = new Paint();
            paint.setColor(Color.RED);
            paint.setStyle(Paint.Style.STROKE);
            paint.setStrokeWidth(10f);
            canvas.drawRect(rect, paint);
            canvas.drawText(boundingBox.labelText, (int) boundingBox.xMin, (int) boundingBox.yMin, paint);
        }
    }
}

public class StreamingFragment extends Fragment implements SurfaceHolder.Callback {
//    private final String TAG = "StreamingFragment";

    private StreamingViewModel mModel;

    private GStreamerSurfaceView mVideoSurfaceView;
    private TextView mStatusTextView;
    private TextView mLabelTextView;

    private SurfaceView mBoundingBoxesSurfaceView;
    private BoundingBoxesManager mBoundingBoxesManager;

    @Override
    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        this.mModel = ViewModelProviders.of(this).get(StreamingViewModel.class);
        View root =
                inflater.inflate(R.layout.fragment_streaming, container, false);
        this.mVideoSurfaceView = root.findViewById(R.id.videoSurfaceView);
        this.mBoundingBoxesSurfaceView = root.findViewById(R.id.boundingBoxesSurfaceView);
        this.mStatusTextView = root.findViewById(R.id.statusTextView);
        this.mLabelTextView = root.findViewById(R.id.labelTextView);

        this.mBoundingBoxesManager = new BoundingBoxesManager();
        SurfaceHolder boundingBoxesSurfaceHolder = this.mBoundingBoxesSurfaceView.getHolder();
        boundingBoxesSurfaceHolder.addCallback(this.mBoundingBoxesManager);
        boundingBoxesSurfaceHolder.setFormat(PixelFormat.TRANSPARENT);

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
        SurfaceHolder videoSurfaceHolder = this.mVideoSurfaceView.getHolder();
        videoSurfaceHolder.addCallback(this);

        // Retrieve our previous state, or connectChannel it to default values
        this.mIsPlayingDesired = false;

        RemoteUIAPI.get().getPipeline().observe(this, new Observer<String>() {
            @Override
            public void onChanged(@Nullable String value) {
                updateVideoSurfaceView();
            }
        });
        RemoteUIAPI.get().getLabelText().observe(this, new Observer<String>() {
            @Override
            public void onChanged(String s) {
                updateLabelTextView();
            }
        });
        RemoteUIAPI.get().getBoundingBoxesJSON().observe(this, new Observer<String>() {
            @Override
            public void onChanged(String s) {
                updateBoundingBoxesSurfaceView();
            }
        });

        this.mBoundingBoxesSurfaceView.setZOrderOnTop(true);
        this.mBoundingBoxesSurfaceView.setZOrderMediaOverlay(true);
        this.mVideoSurfaceView.setZOrderMediaOverlay(true);
        return root;
    }

    @Override
    public void onDestroyView() {
        super.onDestroyView();

        SurfaceHolder videoSurfaceHolder = this.mVideoSurfaceView.getHolder();
        videoSurfaceHolder.removeCallback(this);

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

    private void updateLabelTextView() {
        String labelText = RemoteUIAPI.get().getLabelText().getValue();
        this.mLabelTextView.setText(labelText);
    }

    private void updateBoundingBoxesSurfaceView() {
        ArrayList<BoundingBox> boundingBoxes = RemoteUIAPI.get().getBoundingBoxes();
        this.mBoundingBoxesManager.setBoundingBoxes(boundingBoxes);
        this.mBoundingBoxesManager.drawCanvas(this.mBoundingBoxesSurfaceView.getHolder());
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

        // Update video surface view
        updateVideoSurfaceView();

        // Update label surface view's size
        this.mBoundingBoxesSurfaceView.getHolder().setFixedSize(width, height);

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
                mStatusTextView.setText(message);
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