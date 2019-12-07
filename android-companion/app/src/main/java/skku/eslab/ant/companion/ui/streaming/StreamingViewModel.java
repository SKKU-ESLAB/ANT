package skku.eslab.ant.companion.ui.streaming;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;
import skku.eslab.ant.companion.resourceapi.OnResourceRequestListener;
import skku.eslab.ant.companion.resourceapi.Resource;
import skku.eslab.ant.companion.resourceapi.ResourceAPI;
import skku.eslab.ant.companion.resourceapi.ResourceRequest;

public class StreamingViewModel extends ViewModel {
    private final String kUri = "/remoteui/streamingview/pipeline";
    private Resource mPipelineResource;
    private MutableLiveData<String> mPipeline;

    public StreamingViewModel() {
        mPipeline = new MutableLiveData<>();
        mPipeline.setValue("");

        this.initializeResource();
    }

    private void initializeResource() {
        this.mPipelineResource = new Resource(kUri);
        this.mPipelineResource.setOnPost(new OnResourceRequestListener() {
            @Override
            public void onResourceRequest(ResourceRequest request) {
                String pipeline = request.getMessage();
                mPipeline.postValue(pipeline);
                ResourceAPI.get().sendResponse(request, "Success");
            }
        });
        this.mPipelineResource.setOnGet(new OnResourceRequestListener() {
            @Override
            public void onResourceRequest(ResourceRequest request) {
                String pipeline = mPipeline.getValue();
                ResourceAPI.get().sendResponse(request, pipeline);
            }
        });
        ResourceAPI.get().registerResource(this.mPipelineResource);
    }

    LiveData<String> getPipeline() {
        return mPipeline;
    }
}