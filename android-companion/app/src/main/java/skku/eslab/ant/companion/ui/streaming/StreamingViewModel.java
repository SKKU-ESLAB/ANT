package skku.eslab.ant.companion.ui.streaming;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

public class StreamingViewModel extends ViewModel {

    private MutableLiveData<String> mPipeline;
    // TODO: Add StreamingView Handling with Resource API
    public StreamingViewModel() {
        mPipeline = new MutableLiveData<>();
        mPipeline.setValue("");
    }

    public LiveData<String> getPipeline() {
        return mPipeline;
    }
}