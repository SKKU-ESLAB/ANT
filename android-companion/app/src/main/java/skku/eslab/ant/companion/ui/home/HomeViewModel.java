package skku.eslab.ant.companion.ui.home;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

public class HomeViewModel extends ViewModel {
    private MutableLiveData<Boolean> mIsAppRunning = new MutableLiveData<>();

    public HomeViewModel() {
    }

    public void loadAppCode() {
        // TODO: not yet implemented
    }

    public LiveData<Boolean> isAppRunning() {
        return this.mIsAppRunning;
    }
}