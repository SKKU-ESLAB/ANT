/* Copyright (c) 2017-2020 SKKU ESLAB, and contributors. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
import skku.eslab.ant.companion.httpconnection.HTTPClient;
import skku.eslab.ant.companion.httpconnection.HTTPResponseHandler;
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
        HTTPClient httpClient = HTTPClient.get();
        String url = httpClient.getTargetAddress() +
                "/runtime/currentApp/codeInHtml";
        httpClient.sendHTTPRequest(url, "GET", null, new HTTPResponseHandler() {
            @Override
            public void onHTTPResponse(int code, String message) {
                if (code == 200) {
                    message = message.replace("<br />", "\n");
                    onUpdateAppCode(message);
                }
            }
        });
    }

    private void onUpdateAppCode(String appCode) {
        FragmentActivity activity = getActivity();
        do {
            if (activity != null) {
                TextView appCodeTextView =
                        activity.findViewById(R.id.appCodeTextView);
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
