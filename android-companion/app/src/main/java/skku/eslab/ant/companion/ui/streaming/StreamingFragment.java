package skku.eslab.ant.companion.ui.streaming;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;
import androidx.lifecycle.Observer;
import androidx.lifecycle.ViewModelProviders;
import skku.eslab.ant.companion.R;

public class StreamingFragment extends Fragment {

    private StreamingViewModel streamingViewModel;

    public View onCreateView(@NonNull LayoutInflater inflater,
                             ViewGroup container, Bundle savedInstanceState) {
        streamingViewModel =
                ViewModelProviders.of(this).get(StreamingViewModel.class);
        View root =
                inflater.inflate(R.layout.fragment_streaming, container, false);
        streamingViewModel.getText().observe(this, new Observer<String>() {
            @Override
            public void onChanged(@Nullable String s) {
                //
            }
        });
        return root;
    }
}