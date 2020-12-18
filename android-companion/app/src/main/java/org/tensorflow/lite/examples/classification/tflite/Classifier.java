package org.tensorflow.lite.examples.classification.tflite;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.RectF;
import android.os.SystemClock;
import android.os.Trace;
import android.util.Log;

import org.tensorflow.lite.DataType;
import org.tensorflow.lite.Interpreter;
import org.tensorflow.lite.Tensor;
import org.tensorflow.lite.gpu.GpuDelegate;
import org.tensorflow.lite.nnapi.NnApiDelegate;
import org.tensorflow.lite.support.common.FileUtil;
import org.tensorflow.lite.support.common.TensorOperator;
import org.tensorflow.lite.support.common.TensorProcessor;
import org.tensorflow.lite.support.image.ImageProcessor;
import org.tensorflow.lite.support.image.TensorImage;
import org.tensorflow.lite.support.image.ops.ResizeOp;
import org.tensorflow.lite.support.image.ops.ResizeOp.ResizeMethod;
import org.tensorflow.lite.support.image.ops.ResizeWithCropOrPadOp;
import org.tensorflow.lite.support.image.ops.Rot90Op;
import org.tensorflow.lite.support.label.TensorLabel;
import org.tensorflow.lite.support.tensorbuffer.TensorBuffer;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.MappedByteBuffer;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import java.util.Map;
import java.util.PriorityQueue;

import skku.eslab.ant.companion.gatewayconnector.ClassifierOffloadingFloatMobileNet;

import static java.lang.Math.min;

/**
 * A classifier specialized to label images using TensorFlow Lite.
 */
public abstract class Classifier {
    public static final String TAG = "ClassifierWithSupport";

    /**
     * The model type used for classification.
     */
    public enum Model {
        OFFLOADING_FLOAT_MOBILENET
    }

    /**
     * The runtime device type used for executing classification.
     */
    public enum Device {
        CPU,
        NNAPI,
        GPU
    }

    /**
     * Number of results to show in the UI.
     */
    private static final int MAX_RESULTS = 3;

    /** The loaded TensorFlow Lite model. */

    /**
     * Image size along the x axis.
     */
    private int mImageSizeX;

    /**
     * Image size along the y axis.
     */
    private int mImageSizeY;

    /**
     * Optional GPU delegate for accleration.
     */
    private GpuDelegate mGPUDelegate = null;

    /**
     * Optional NNAPI delegate for accleration.
     */
    private NnApiDelegate mNNAPIDelegate = null;

    /**
     * An instance of the driver class to run model inference with Tensorflow Lite.
     */
    protected ArrayList<Interpreter> mInterpreters = new ArrayList<>();

    /**
     * Options for configuring the Interpreter.
     */
    private final Interpreter.Options mInterpreterOptions = new Interpreter.Options();

    /**
     * Labels corresponding to the output of the vision model.
     */
    private final List<String> mLabels;

    /**
     * Input image TensorBuffer.
     */
    private TensorImage mInputImageBuffer;

    // Intermediate feature map tensor buffers
    private ArrayList<TensorBuffer> mOutFeatureMapBuffers = new ArrayList<>();

    /**
     * Processer to apply post processing of the output probability.
     */
    private final TensorProcessor mPostProcessor;

    /**
     * Creates a classifier with the provided configuration.
     *
     * @param activity   The current Activity.
     * @param model      The model to use for classification.
     * @param device     The device to use for classification.
     * @param numThreads The number of threads to use for classification.
     * @return A classifier with the desired configuration.
     */
    public static Classifier create(Activity activity, Model model, Device device, int numThreads)
            throws IOException {
        if (model == Model.OFFLOADING_FLOAT_MOBILENET) {
            return new ClassifierOffloadingFloatMobileNet(activity, device, numThreads);
        } else {
            throw new UnsupportedOperationException();
        }
    }


    /**
     * An immutable result returned by a org.tensorflow.lite.examples.classification.tflite.Classifier describing what was recognized.
     */
    public static class Recognition {
        /**
         * A unique identifier for what has been recognized. Specific to the class, not the instance of
         * the object.
         */
        private final String id;

        /**
         * Display name for the recognition.
         */
        private final String title;

        /**
         * A sortable score for how good the recognition is relative to others. Higher should be better.
         */
        private final Float confidence;

        /**
         * Optional location within the source image for the location of the recognized object.
         */
        private RectF location;

        public Recognition(
                final String id, final String title, final Float confidence, final RectF location) {
            this.id = id;
            this.title = title;
            this.confidence = confidence;
            this.location = location;
        }

        public String getId() {
            return id;
        }

        public String getTitle() {
            return title;
        }

        public Float getConfidence() {
            return confidence;
        }

        public RectF getLocation() {
            return new RectF(location);
        }

        public void setLocation(RectF location) {
            this.location = location;
        }

        @Override
        public String toString() {
            String resultString = "";
            if (id != null) {
                resultString += "[" + id + "] ";
            }

            if (title != null) {
                resultString += title + " ";
            }

            if (confidence != null) {
                resultString += String.format("(%.1f%%) ", confidence * 100.0f);
            }

            if (location != null) {
                resultString += location + " ";
            }

            return resultString.trim();
        }

    }

    private void __setInferenceSettings(Device device, int numThreads) {
        switch (device) {
            case NNAPI:
                mNNAPIDelegate = new NnApiDelegate();
                mInterpreterOptions.addDelegate(mNNAPIDelegate);
                break;
            case GPU:
                mGPUDelegate = new GpuDelegate();
                mInterpreterOptions.addDelegate(mGPUDelegate);
                break;
            case CPU:
                break;
        }
        mInterpreterOptions.setNumThreads(numThreads);
    }

    /**
     * Initializes a {@code org.tensorflow.lite.examples.classification.tflite.Classifier}.
     */
    protected Classifier(Activity activity, Device device, int numThreads) throws IOException {
        // Loads labels out from the label file.
        this.mLabels = FileUtil.loadLabels(activity, getLabelPath());

        // Inference settings
        this.__setInferenceSettings(device, numThreads);

        // Load models
        ArrayList<String> modelPaths = getModelPaths();
        for (String modelPath : modelPaths) {
            MappedByteBuffer tfliteModel = FileUtil.loadMappedFile(activity, modelPath);
            Interpreter interpreter = new Interpreter(tfliteModel, mInterpreterOptions);
            this.mInterpreters.add(interpreter);
        }

        // Initialize interpreter
        for (Interpreter interpreter : this.mInterpreters) {
            int index = this.mInterpreters.indexOf(interpreter);

            // Input
            Tensor inTensor = interpreter.getInputTensor(0);
            DataType inDataType = inTensor.dataType();
            int[] inShape = inTensor.shape();
            if (index == 0) {
                // Input image
                this.mImageSizeY = inShape[1];
                this.mImageSizeX = inShape[2];
                this.mInputImageBuffer = new TensorImage(inDataType);
            }

            // Output
            Tensor outTensor = interpreter.getOutputTensor(0);
            int[] outShape = outTensor.shape();
            DataType outDataType = outTensor.dataType();
            TensorBuffer outFeatureMapBuffer = TensorBuffer.createFixedSize(outShape,
                    outDataType);
            this.mOutFeatureMapBuffers.add(outFeatureMapBuffer);
        }

        // Creates the post processor for the output probability.
        mPostProcessor = new TensorProcessor.Builder().add(getPostprocessNormalizeOp()).build();
    }

    /**
     * Runs inference and returns the classification results.
     */
    public List<Recognition> recognizeImage(ByteBuffer inputBuffer, int start_idx, int end_idx) {
        for (int i = start_idx; i <= end_idx; i++) {
            Interpreter interpreter = this.mInterpreters.get(i);
            int index = this.mInterpreters.indexOf(interpreter);
            ByteBuffer inBuffer;
            if (index == start_idx)
                inBuffer = inputBuffer;
            else
                inBuffer = this.mOutFeatureMapBuffers.get(index - 1).getBuffer();
            ByteBuffer outBuffer = this.mOutFeatureMapBuffers.get(index).getBuffer();

            interpreter.run(inBuffer.rewind(), outBuffer.rewind());
        }

        // Gets the map of label and probability.
        int finalOutBufferIndex = this.mOutFeatureMapBuffers.size() - 1;
        TensorBuffer finalOutBuffer = this.mOutFeatureMapBuffers.get(finalOutBufferIndex);
        Log.e(TAG, finalOutBuffer.getShape().toString());
        Map<String, Float> labeledProbability =
                new TensorLabel(mLabels, mPostProcessor.process(finalOutBuffer))
                        .getMapWithFloatValue();

        // Gets top-k results.
        return getTopKProbability(labeledProbability);
    }

    /**
     * Closes the interpreter and model to release resources.
     */
    public void close() {
        for (Interpreter interpreter : this.mInterpreters) {
            if (interpreter != null) {
                interpreter.close();
            }
        }
        this.mInterpreters.clear();

        if (mGPUDelegate != null) {
            mGPUDelegate.close();
            mGPUDelegate = null;
        }
        if (mNNAPIDelegate != null) {
            mNNAPIDelegate.close();
            mNNAPIDelegate = null;
        }
    }

    /**
     * Get the image size along the x axis.
     */
    public int getImageSizeX() {
        return mImageSizeX;
    }

    /**
     * Get the image size along the y axis.
     */
    public int getImageSizeY() {
        return mImageSizeY;
    }

    /**
     * Loads input image, and applies preprocessing.
     */
    private TensorImage loadImage(final Bitmap bitmap, int sensorOrientation) {
        // Profiling
        Trace.beginSection("loadImage");
        long startTimeForLoadImage = SystemClock.uptimeMillis();

        // Loads bitmap into a TensorImage.
        mInputImageBuffer.load(bitmap);

        // Creates processor for the TensorImage.
        int cropSize = min(bitmap.getWidth(), bitmap.getHeight());
        int numRotation = sensorOrientation / 90;
        // TODO(b/143564309): Fuse ops inside ImageProcessor.
        ImageProcessor imageProcessor =
                new ImageProcessor.Builder()
                        .add(new ResizeWithCropOrPadOp(cropSize, cropSize))
                        // TODO(b/169379396): investigate the impact of the resize algorithm on accuracy.
                        // To get the same inference results as lib_task_api, which is built on top of the Task
                        // Library, use ResizeMethod.BILINEAR.
                        .add(new ResizeOp(mImageSizeX, mImageSizeY, ResizeMethod.NEAREST_NEIGHBOR))
                        .add(new Rot90Op(numRotation))
                        .add(getPreprocessNormalizeOp())
                        .build();
        TensorImage outImage = imageProcessor.process(mInputImageBuffer);

        // Profiling
        long endTimeForLoadImage = SystemClock.uptimeMillis();
        Trace.endSection();
        Log.v(TAG, "Timecost to load the image: " + (endTimeForLoadImage - startTimeForLoadImage));

        return outImage;
    }

    /**
     * Gets the top-k results.
     */
    private static List<Recognition> getTopKProbability(Map<String, Float> labelProb) {
        // Find the best classifications.
        PriorityQueue<Recognition> pq =
                new PriorityQueue<>(
                        MAX_RESULTS,
                        new Comparator<Recognition>() {
                            @Override
                            public int compare(Recognition lhs, Recognition rhs) {
                                // Intentionally reversed to put high confidence at the head of the queue.
                                return Float.compare(rhs.getConfidence(), lhs.getConfidence());
                            }
                        });

        for (Map.Entry<String, Float> entry : labelProb.entrySet()) {
            pq.add(new Recognition("" + entry.getKey(), entry.getKey(), entry.getValue(), null));
        }

        final ArrayList<Recognition> recognitions = new ArrayList<>();
        int recognitionsSize = min(pq.size(), MAX_RESULTS);
        for (int i = 0; i < recognitionsSize; ++i) {
            recognitions.add(pq.poll());
        }
        return recognitions;
    }

    /**
     * Gets the name of the model file stored in Assets.
     */
    protected abstract ArrayList<String> getModelPaths();

    /**
     * Gets the name of the label file stored in Assets.
     */
    protected abstract String getLabelPath();

    /**
     * Gets the TensorOperator to nomalize the input image in preprocessing.
     */
    protected abstract TensorOperator getPreprocessNormalizeOp();

    /**
     * Gets the TensorOperator to dequantize the output probability in post processing.
     *
     * <p>For quantized model, we need de-quantize the prediction with NormalizeOp (as they are all
     * essentially linear transformation). For float model, de-quantize is not required. But to
     * uniform the API, de-quantize is added to float model too. Mean and std are set to 0.0f and
     * 1.0f, respectively.
     */
    protected abstract TensorOperator getPostprocessNormalizeOp();
}
