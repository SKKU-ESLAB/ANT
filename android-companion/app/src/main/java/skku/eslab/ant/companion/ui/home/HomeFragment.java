package skku.eslab.ant.companion.ui.home;

import android.os.Bundle;
import android.os.Handler;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentActivity;
import androidx.lifecycle.ViewModelProviders;
import skku.eslab.ant.companion.ConnectionManager;
import skku.eslab.ant.companion.HTTPResponseHandler;
import skku.eslab.ant.companion.R;

public class HomeFragment extends Fragment {

    private HomeViewModel homeViewModel;

    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        homeViewModel = ViewModelProviders.of(this).get(HomeViewModel.class);
        View root = inflater.inflate(R.layout.fragment_home, container, false);

        // TODO: event-based update
        new Handler().postDelayed(new Runnable() {
            @Override
            public void run() {
                updateAppCode();
            }
        }, 500);
        return root;
    }

    private void updateAppCode() {
        ConnectionManager connectionManager = ConnectionManager.get();
        String url = connectionManager.getTargetAddress() + "/runtime/currentApp/code";
        connectionManager.sendHTTPRequest(url, "GET", null, new HTTPResponseHandler() {
            @Override
            public void onHTTPResponse(int code, String message) {
                if (code == 200) {
                    onUpdateAppCode(message);
                }
            }
        });
    }

    private void onUpdateAppCode(String appCode) {
        FragmentActivity activity = getActivity();
        do {
            if(activity != null) {
                TextView appCodeTextView = activity.findViewById(R.id.appCodeTextView);
                appCodeTextView.setText(appCode);
            } else {
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        } while (activity == null);
    }
}