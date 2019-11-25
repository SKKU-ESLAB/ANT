package skku.eslab.ant.companion.ui.streaming;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

public class StreamingViewModel extends ViewModel {

    private MutableLiveData<String> mText;

    public StreamingViewModel() {
        mText = new MutableLiveData<>();
        mText.setValue("This is dashboard fragment");
    }

    public LiveData<String> getText() {
        return mText;
    }
}