package skku.eslab.ant.companion.gatewayconnector;

import android.app.Activity;

import org.tensorflow.lite.examples.classification.tflite.Classifier;
import org.tensorflow.lite.support.common.TensorOperator;
import org.tensorflow.lite.support.common.ops.NormalizeOp;

import java.io.IOException;
import java.util.ArrayList;

/**
 * This TensorFlowLite classifier works with the float MobileNet model.
 */
public class ClassifierOffloadingFloatMobileNet extends Classifier {

    /**
     * Float MobileNet requires additional normalization of the used input.
     */
    private static final float IMAGE_MEAN = 127.5f;

    private static final float IMAGE_STD = 127.5f;

    /**
     * Float model does not need dequantization in the post-processing. Setting mean and std as 0.0f
     * and 1.0f, repectively, to bypass the normalization.
     */
    private static final float PROBABILITY_MEAN = 0.0f;

    private static final float PROBABILITY_STD = 1.0f;

    /**
     * Initializes a {@code ClassifierFloatMobileNet}.
     *
     * @param activity
     */
    public ClassifierOffloadingFloatMobileNet(Activity activity, Device device, int numThreads)
            throws IOException {
        super(activity, device, numThreads);
    }

    @Override
    protected ArrayList<String> getModelPaths() {
        ArrayList<String> modelPaths = new ArrayList<String>();
        int fragmentsNum = 13;
        for (int i = 0; i < fragmentsNum; i++) {
            modelPaths.add("mod-" + i + ".tflite");
        }
        return modelPaths;
    }

    @Override
    protected String getLabelPath() {
        return "labels-1000.txt";
    }

    @Override
    protected TensorOperator getPreprocessNormalizeOp() {
        return new NormalizeOp(IMAGE_MEAN, IMAGE_STD);
    }

    @Override
    protected TensorOperator getPostprocessNormalizeOp() {
        return new NormalizeOp(PROBABILITY_MEAN, PROBABILITY_STD);
    }
}
