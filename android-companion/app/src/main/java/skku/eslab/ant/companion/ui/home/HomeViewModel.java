package skku.eslab.ant.companion.ui.home;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

public class HomeViewModel extends ViewModel {

    private MutableLiveData<Boolean> mIsAppRunning;

    public HomeViewModel() {
        this.connectTarget();
    }

    public void connectTarget() {
        // TODO: not yet implemented
    }

    private void checkAndUpdateAppStatus() {
        // TODO: not yet implemented
    }

    public void startApp() {
        // TODO: not yet implemented
    }

    public void stopApp() {
        // TODO: not yet implemented
    }

    public void loadAppCode() {
        // TODO: not yet implemented
    }

    public LiveData<Boolean> isAppRunning() {
        return this.mIsAppRunning;
    }
}