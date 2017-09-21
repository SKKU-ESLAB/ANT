package com.ant.ant_manager.view.remoteui;

import android.app.ActionBar;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.DatePickerDialog;
import android.app.TimePickerDialog;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.ServiceConnection;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.os.IBinder;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.BaseAdapter;
import android.widget.DatePicker;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.TimePicker;
import android.widget.Toast;

import com.ant.ant_manager.R;
import com.ant.ant_manager.controller.LegacyJSONParser;
import com.ant.ant_manager.controller.ANTControllerBroadcastReceiver;
import com.ant.ant_manager.controller.ANTControllerService;
import com.ant.ant_manager.model.ANTApp;

import java.util.ArrayList;
import java.util.Calendar;

import static android.content.ContentValues.TAG;

public class RemoteConfigUIActivity extends Activity {
    // ANTControllerService
    private ANTControllerService mControllerServiceStub = null;
    private PrivateControllerBroadcastReceiver mControllerBroadcastReceiver;
    private final RemoteConfigUIActivity self = this;

    ListView listView;
    static ConfigListAdapter mConfigListAdapter;

    ArrayList<ConfigListItem> mConfigList;
    String inputJson;
    LegacyJSONParser jp;

    String mAppID;
    String mRqID;
    String mPid;

    final String kDefaultStatus = ">";

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(com.ant.ant_manager.R.layout.template_listview);

        String title = getIntent().getStringExtra("title");
        Toast.makeText(getApplicationContext(), title, Toast.LENGTH_LONG).show();
        inputJson = getIntent().getStringExtra("jsonData");

        jp = new LegacyJSONParser(inputJson);
        mAppID = jp.getValueByKey("mAppID");
        mRqID = jp.getValueByKey("mRqID");
        mPid = jp.getValueByKey("mPid");

        int iconId = getIntent().getIntExtra("iconID", com.ant.ant_manager.R.drawable.app);

        listView = (ListView) findViewById(com.ant.ant_manager.R.id.mainListView);

        mConfigList = getSettingMenuList(inputJson);

        mConfigListAdapter = new ConfigListAdapter(getApplicationContext(),
                RemoteConfigUIActivity.this, mConfigList);
        listView.setAdapter(mConfigListAdapter);
        listView.setOnItemClickListener(mItemClickListener);

        // Connect controller service
        this.connectControllerService();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(com.ant.ant_manager.R.menu.menu_remote_config_ui, menu);
        return true;
    }

    //back button
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case android.R.id.home:
                this.finish();
                break;

            case com.ant.ant_manager.R.id.action_config_ok:
                if (completeConfigSetting()) {
                    LegacyJSONParser jp = new LegacyJSONParser();
                    jp.makeNewJson();
                    jp.addJsonKeyValue("appID", mAppID);
                    jp.addJsonKeyValue("reID", mRqID);
                    jp.addJsonKeyValue("pid", mPid);
                    for (int i = 0; i < mConfigList.size(); i++) {
                        jp.addJsonKeyValue(mConfigList.get(i).getTitle(), mConfigList.get(i)
                                .getStatus());
                    }
                    if (mControllerServiceStub != null) {
                        mControllerServiceStub.updateAppConfigAsync(Integer.parseInt(mAppID), jp
                                .getJsonData());
                    } else {
                        Toast.makeText(this, "Controller service is not connected!", Toast
                                .LENGTH_LONG);
                    }
                }
                break;
            default:
                return super.onOptionsItemSelected(item);
        }
        return true;
    }

    public boolean completeConfigSetting() {
        for (int i = 0; i < mConfigList.size(); i++) {
            if (mConfigList.get(i).getStatus().equals(kDefaultStatus)) {
                Toast.makeText(getApplicationContext(), "Select all of the option!", Toast
                        .LENGTH_LONG).show();
                return false;
            }
        }
        Toast.makeText(getApplicationContext(), "Set this configuration", Toast.LENGTH_LONG).show();
        return true;
    }

    private String mSelected;
    private AdapterView.OnItemClickListener mItemClickListener = new AdapterView
            .OnItemClickListener() {
        @Override
        public void onItemClick(AdapterView<?> parent, View view, int position, long l_position) {
            final ConfigListItem item = (ConfigListItem) parent.getAdapter().getItem(position);

            mSelected = item.getStatus();

            int selectedFlag = item.flag;

            if (selectedFlag == 1) {
                // Str textBox - constrain >> length
                if (item.getAlterList().size() == 1) {
                    final String length = item.getAlterList().get(0);
                    AlertDialog.Builder alert = new AlertDialog.Builder(RemoteConfigUIActivity
                            .this);
                    alert.setTitle(item.getTitle()); // Set Alert dialog title
                    // here
                    alert.setMessage("Length : " + length); // Message here

                    final EditText input = new EditText(RemoteConfigUIActivity.this);
                    alert.setView(input);

                    alert.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface dialog, int whichButton) {

                            String inputStr = input.getEditableText().toString();
                            if (inputStr.length() < Integer.parseInt(length)) {
                                item.setStatus(inputStr);
                            } else {
                                Toast.makeText(getApplicationContext(), "Input str length is too"
                                        + " long", Toast.LENGTH_LONG).show();
                            }

                        }
                    });
                    alert.setNegativeButton("CANCEL", new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface dialog, int whichButton) {
                            dialog.cancel();
                        }
                    });
                    AlertDialog alertDialog = alert.create();
                    alertDialog.show();
                }

                // Number textBox - constrain >> range
                else if (item.getAlterList().size() == 2) {

                    AlertDialog.Builder alert = new AlertDialog.Builder(RemoteConfigUIActivity
                            .this);
                    alert.setTitle(item.getTitle()); // Set Alert dialog title
                    // here
                    alert.setMessage("Range : " + item.getAlterList().get(0) + " ~ " + item
                            .getAlterList().get(1)); // Message
                    // here

                    final EditText input = new EditText(RemoteConfigUIActivity.this);
                    alert.setView(input);

                    alert.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface dialog, int whichButton) {
                            String inputStr = input.getEditableText().toString();
                            Double inputNumber = 0.0;
                            try {
                                inputNumber = Double.parseDouble(inputStr);

                                if (Double.parseDouble(item.getAlterList().get(0)) < inputNumber
                                        && inputNumber < Double.parseDouble(item.getAlterList()
                                        .get(1))) {
                                    item.setStatus(inputStr);
                                } else {
                                    Toast.makeText(getApplicationContext(), "Input Number is out"
                                            + " of range", Toast.LENGTH_LONG).show();
                                }
                            } catch (NumberFormatException e) {
                                Toast.makeText(getApplicationContext(), "Input mMainIconList is "
                                        + "not number format", Toast.LENGTH_LONG).show();
                            }
                        }
                    });
                    alert.setNegativeButton("CANCEL", new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface dialog, int whichButton) {
                            dialog.cancel();
                        }
                    });
                    AlertDialog alertDialog = alert.create();
                    alertDialog.show();
                }
            } else if (selectedFlag == 2) {
                // single choice dialog
                AlertDialog.Builder ab = new AlertDialog.Builder(RemoteConfigUIActivity.this);
                ab.setTitle(item.getTitle());

                ab.setSingleChoiceItems(item.getListArray(), item.getListPosition(mSelected), new
                        DialogInterface.OnClickListener() {

                            public void onClick(DialogInterface dialog, int whichButton) {
                                mSelected = item.getListArray()[whichButton];
                            }
                        }).setPositiveButton(com.ant.ant_manager.R.string.alert_dialog_ok, new
                        DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int whichButton) {
                                if (item.getStatus().equals(kDefaultStatus)) {
                                    item.setStatus(item.getListArray()[0]);
                                } else {
                                    item.setStatus(mSelected);
                                }
                                mConfigListAdapter.updateDisplay();

                            }
                        }).setNegativeButton(com.ant.ant_manager.R.string.alert_dialog_cancel, new
                        DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int whichButton) {
                                dialog.cancel();
                            }
                        });
                AlertDialog alertDialog = ab.create();
                alertDialog.show();
            } else if (selectedFlag == 3) {
                //*9****************Not implemented***********************
                AlertDialog.Builder builder = new AlertDialog.Builder(RemoteConfigUIActivity.this);
                builder.setTitle(item.getTitle()).setMultiChoiceItems(item.getListArray(), null,
                        new DialogInterface.OnMultiChoiceClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog, int which, boolean isChecked) {
                                if (isChecked) {
                                }
                            }
                        }).setPositiveButton(com.ant.ant_manager.R.string.alert_dialog_ok, new
                        DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog, int id) {
                                //Create onlcick method
                            }
                        }).setNegativeButton(com.ant.ant_manager.R.string.alert_dialog_cancel, new
                        DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog, int id) {
                                //Create onlcick method
                            }
                        });
                builder.show();

            } else if (selectedFlag == 4) {

                final Calendar c = Calendar.getInstance();
                int mYear = c.get(Calendar.YEAR);
                int mMonth = c.get(Calendar.MONTH);
                int mDay = c.get(Calendar.DAY_OF_MONTH);

                DatePickerDialog dpd = new DatePickerDialog(RemoteConfigUIActivity.this, new
                        DatePickerDialog.OnDateSetListener() {

                            @Override
                            public void onDateSet(DatePicker view, int year, int monthOfYear, int
                                    dayOfMonth) {

                                String _year, _month, _day;
                                _year = String.valueOf(year);

                                _month = String.valueOf(monthOfYear + 1);
                                if (_month.length() == 1) _month = "0" + _month;

                                _day = String.valueOf(dayOfMonth);
                                if (_day.length() == 1) _day = "0" + _day;

                                item.setStatus(_year + "-" + _month + "-" + _day);
                                mConfigListAdapter.updateDisplay();
                            }
                        }, mYear, mMonth, mDay);
                dpd.show();

            } else if (selectedFlag == 5) {

                int mHour = 10;
                int mMinute = 10;

                TimePickerDialog tpd = new TimePickerDialog(RemoteConfigUIActivity.this, new
                        TimePickerDialog.OnTimeSetListener() {

                            @Override
                            public void onTimeSet(TimePicker view, int hourOfDay, int minute) {

                                String hour, min;

                                hour = String.valueOf(hourOfDay);
                                if (hour.length() == 1) hour = "0" + hour;

                                min = String.valueOf(minute);
                                if (min.length() == 1) min = "0" + min;

                                item.setStatus(hour + ":" + min);
                                mConfigListAdapter.updateDisplay();
                            }
                        }, mHour, mMinute, false);
                tpd.show();

            }
        }
    };

    //Make the list to show on display by parsing input Json string
    public ArrayList<ConfigListItem> getSettingMenuList(String jsonData) {
        ArrayList<ConfigListItem> arr = new ArrayList<ConfigListItem>();

        while (jp.hasMoreValue()) {
            String ret[] = new String[2];
            ret = jp.getNextKeyValue();

            ArrayList<String> strArr = parseConfigItem(ret[1]);
            ArrayList<String> strTemp = new ArrayList<String>();
            String key = "", description = "";
            for (int i = 0; i < strArr.size(); i++) {
                if (i == 0) {
                    key = strArr.get(i);
                } else if (i == 1) {
                    description = strArr.get(i);
                } else {
                    strTemp.add(strArr.get(i));
                    Log.d("ANT", strArr.get(i));
                }

            }

            if (ret[0].equals("strTB")) {

                arr.add(new ConfigListItem(key, description, kDefaultStatus, 1, strTemp));
            } else if (ret[0].equals("numTB")) {

                arr.add(new ConfigListItem(key, description, kDefaultStatus, 1, strTemp));
            } else if (ret[0].equals("sDialog")) {
                arr.add(new ConfigListItem(key, description, kDefaultStatus, 2, strTemp));

            } else if (ret[0].equals("mDialog")) {
                arr.add(new ConfigListItem(key, description, kDefaultStatus, 3, strTemp));

            } else if (ret[0].equals("dateDialog")) {
                arr.add(new ConfigListItem(key, description, kDefaultStatus, 4, strTemp));
            } else if (ret[0].equals("timeDialog")) {
                arr.add(new ConfigListItem(key, description, kDefaultStatus, 5, strTemp));
            } else {
                Log.d("ANT", "Config other item : " + ret[0]);
            }
        }
        return arr;
    }

    private ArrayList<String> parseConfigItem(String str) {
        ArrayList<String> arr = new ArrayList<String>();
        String tmp = "";
        for (int i = 0; i < str.length(); i++) {

            if (str.charAt(i) == '[') {
                arr.add(tmp);
                tmp = "";
            } else if (str.charAt(i) == '/') {
                arr.add(tmp);
                tmp = "";
            } else if (str.charAt(i) == ']') {
                arr.add(tmp);
                tmp = "";
            } else if (str.charAt(i) == '"') {
                continue;
            } else {
                tmp += str.charAt(i);
            }
        }
        return arr;
    }

    class ConfigListAdapter extends BaseAdapter {
        private Context mContext;
        private Activity mActivity;
        private ArrayList<ConfigListItem> arr;

        private int pos;

        private LinearLayout layout;

        //	private Typeface myFont;
        public ConfigListAdapter(Context mContext, Activity mActivity, ArrayList<ConfigListItem>
                arr_item) {
            this.mContext = mContext;
            this.mActivity = mActivity;
            this.arr = arr_item;
        }

        @Override
        public int getCount() {
            return arr.size();
        }

        @Override
        public Object getItem(int position) {
            return arr.get(position);
        }

        public long getItemId(int position) {
            return position;
        }

        @Override
        public View getView(int position, View convertView, ViewGroup parent) {

            if (convertView == null) {
                int res = 0;
                res = com.ant.ant_manager.R.layout.template_listview_item_no_icon;
                LayoutInflater mInflater = (LayoutInflater) mContext.getSystemService(Context
                        .LAYOUT_INFLATER_SERVICE);
                convertView = mInflater.inflate(res, parent, false);
            }

            pos = position;
            if (arr.size() != 0) {
                TextView title = (TextView) convertView.findViewById(R.id.tv_title);
                title.setText(arr.get(pos).getTitle());
                TextView subtitle = (TextView) convertView.findViewById(R.id.tv_subTitle);
                subtitle.setText(arr.get(pos).getSubtitle());
                if (arr.get(pos).getSubtitle().equals(null)) {
                }
                TextView status = (TextView) convertView.findViewById(R.id.tv_status);
                status.setText(arr.get(pos).getStatus());
            }
            return convertView;
        }

        public void updateDisplay() {
            this.notifyDataSetChanged();
        }
    }

    class ConfigListItem {

        public String mainTitle;
        public String subTitle;
        public String status;
        public int flag;
        // 1:textbox, 2:single 3:multiple 4:Date 5:Time

        public ArrayList<String> alternativeTitle;

        public ConfigListItem() {
            this.mainTitle = null;
            this.subTitle = null;
            this.status = null;
            alternativeTitle = new ArrayList<String>();
        }

        public ConfigListItem(String main, String sub, String status) {
            this.mainTitle = main;
            this.subTitle = sub;
            this.status = status;
            //alternativeTitle = null;

            alternativeTitle = new ArrayList<String>();
        }

        public ConfigListItem(String main, String sub, String status, int flag) {
            this.mainTitle = main;
            this.subTitle = sub;
            this.status = status;
            this.flag = flag;
            //alternativeTitle = null;

            alternativeTitle = new ArrayList<String>();
        }


        public ConfigListItem(String main, String sub, String status, int flag, ArrayList<String>
                arr) {
            this.mainTitle = main;
            this.subTitle = sub;
            this.status = status;
            this.flag = flag;

            alternativeTitle = new ArrayList<String>();
            listDeepCopy(arr);
        }


        public ConfigListItem(String main, String sub, String status, ArrayList<String> arr) {
            this.mainTitle = main;
            this.subTitle = sub;
            this.status = status;
            alternativeTitle = new ArrayList<String>();
            listDeepCopy(arr);
        }

        public void listDeepCopy(ArrayList<String> arr) {
            for (int i = 0; i < arr.size(); i++)
                alternativeTitle.add(arr.get(i));
        }

        public void setAppName(String c) {
            this.mainTitle = c;
        }

        public void setSubtitle(String c) {
            this.subTitle = c;
        }

        public void setStatus(String c) {
            this.status = c;
        }


        public String getTitle() {
            return this.mainTitle;
        }

        public String getSubtitle() {
            return this.subTitle;
        }

        public String getStatus() {
            return this.status;
        }

        public ArrayList<String> getAlterList() {
            return this.alternativeTitle;
        }

        //convert ArrayList to string array
        public String[] getListArray() {
            String[] str = new String[alternativeTitle.size()];

            str = alternativeTitle.toArray(str);
            return str;
        }

        //return position number of target string from arraylist
        public int getListPosition(String targetString) {
            for (int i = 0; i < alternativeTitle.size(); i++) {
                if (this.status.equals(alternativeTitle.get(i))) return i;
            }
            return 0;
        }
    }

    private void initializeActionBar() {
        ANTApp targetApp = this.mControllerServiceStub.getApp(Integer.parseInt(this.mAppID));

        try {
            ActionBar actionBar = getActionBar();
            actionBar.setTitle(targetApp.getName());
            actionBar.setDisplayHomeAsUpEnabled(true);
            actionBar.setDisplayHomeAsUpEnabled(true);
            actionBar.setDisplayShowCustomEnabled(true);
            Drawable dr = Drawable.createFromPath(targetApp.getIconImagePath());
            actionBar.setIcon(dr);
            actionBar.setDisplayUseLogoEnabled(true);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void connectControllerService() {
        Intent serviceIntent = new Intent(this, ANTControllerService.class);
        this.bindService(serviceIntent, this.mControllerServiceConnection, Context
                .BIND_AUTO_CREATE);
    }

    private ServiceConnection mControllerServiceConnection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName componentName, IBinder inputBinder) {
            ANTControllerService.ControllerBinder serviceBinder = (ANTControllerService
                    .ControllerBinder) inputBinder;
            mControllerServiceStub = serviceBinder.getService();

            // Update UI
            initializeActionBar();
        }

        @Override
        public void onServiceDisconnected(ComponentName componentName) {
            Log.d(TAG, "onServiceDisconnected()");
            mControllerServiceStub = null;
        }
    };

    class PrivateControllerBroadcastReceiver extends ANTControllerBroadcastReceiver {
        PrivateControllerBroadcastReceiver() {
            this.setOnResultUpdateAppConfigListener(new OnResultUpdateAppConfigListener() {
                @Override
                public void onResultUpdateAppConfig(int commandMessageId, boolean isSucceed) {
                    if (isSucceed) {
                        Toast.makeText(self, "Config is applied successfully!", Toast
                                .LENGTH_LONG);
                        self.finish();
                    } else {
                        Toast.makeText(self, "Config cannot be applied!", Toast
                                .LENGTH_LONG);
                    }
                }
            });
        }
    }
}