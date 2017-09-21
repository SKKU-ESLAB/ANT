package com.ant.cmfw.devicecontrollers.bluetooth;

import android.Manifest;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.ResultReceiver;
import android.support.annotation.NonNull;
import android.support.v4.app.ActivityCompat;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;

import com.ant.ant_manager.R;

import java.util.Set;

public class BluetoothConnectorActivity extends Activity {
    private static final String TAG = "BTInitializerActivity";

    // Intent to BluetoothConnectorActivity
    public static final String INTENT_KEY_RECEIVER = "BTInitializerResult";

    // SharedPreference
    private static final String PREFERENCE_KEY = "BluetoothConnectorActivity";
    private static final String PREFERENCE_ATTR_KEY_BT_ADDRESS = "BluetoothDeviceAddress";

    // Result
    private boolean mResultDetermined = false;
    private ResultReceiver mReceiver;

    // Bluetooth adapter
    private BluetoothAdapter mBluetoothAdapter = null;

    // BroadcastReceivers
    private BindingDefaultBluetoothDeviceReceiver mBindingDefaultBluetoothDeviceReceiver = null;
    private BluetoothDeviceDiscoveryReceiver mBluetoothDeviceDiscoveryReceiver = null;
    private BindingBluetoothDeviceReceiver mBindingBluetoothDeviceReceiver = null;

    // Discovery UI
    private ArrayAdapter<String> mBoundDevicesArrayAdapter;
    private ArrayAdapter<String> mNewDevicesArrayAdapter;

    @Override
    public void onBackPressed() {
        // cancel Bluetooth initialization
        this.resultInFail("User canceled Bluetooth connection!");
    }

    // Step 0. Draw UI layout and get local bluetooth adapter
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_INDETERMINATE_PROGRESS);
        setContentView(R.layout.activity_bluetooth_connector);

        Intent callerIntent = this.getIntent();
        this.mReceiver = callerIntent.getParcelableExtra(INTENT_KEY_RECEIVER);

        // Get the local Bluetooth adapter
        this.mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

        // Proceed
        this.grantCommunicationDevicePermission();
    }

    @Override
    public void onDestroy() {
        super.onDestroy();

        if (this.mBindingDefaultBluetoothDeviceReceiver != null) {
            this.unregisterReceiver(this.mBindingDefaultBluetoothDeviceReceiver);
            this.mBindingDefaultBluetoothDeviceReceiver = null;
        }
        if (this.mBluetoothDeviceDiscoveryReceiver != null) {
            this.unregisterReceiver(this.mBluetoothDeviceDiscoveryReceiver);
            this.mBluetoothDeviceDiscoveryReceiver = null;
        }
        if (this.mBindingBluetoothDeviceReceiver != null) {
            this.unregisterReceiver(this.mBindingBluetoothDeviceReceiver);
            this.mBindingBluetoothDeviceReceiver = null;
        }

        // If the result is not determined, regard the result as fail
        if (!this.mResultDetermined) this.resultInFail("User canceled Bluetooth connection!");
    }

    // Step 1. Preparation
    // Step 1-1. Grant communication device permission
    private void grantCommunicationDevicePermission() {
        if (checkSelfPermission(Manifest.permission.BLUETOOTH_ADMIN) != PackageManager
                .PERMISSION_GRANTED || checkSelfPermission(Manifest.permission
                .ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
            // If communication device permission is not granted, request the
            // permission once more.
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission
                    .ACCESS_COARSE_LOCATION, Manifest.permission.BLUETOOTH_ADMIN}, 1);
        } else {
            // Proceed
            turnOnBluetoothDevice();
        }
    }

    public void onRequestPermissionsResult(int requestCode, @NonNull String permissions[],
                                           @NonNull int[] grantResults) {
        if (grantResults.length <= 0 || grantResults[0] != PackageManager.PERMISSION_GRANTED ||
                grantResults[1] != PackageManager.PERMISSION_GRANTED) {
            // If communication device permission is eventually not granted,
            // cancel communication setting.
            resultInFail("Cannot be granted communication device permission!");
        } else {
            // Proceed
            turnOnBluetoothDevice();
        }
    }

    // Step 1-2. Turn on bluetooth device
    private void turnOnBluetoothDevice() {
        // Checking Bluetooth device via BluetoothAdapter
        if (BluetoothAdapter.getDefaultAdapter() == null) {
            // If bluetooth adapter is not found, cancel initialization
            resultInFail("Bluetooth device is required!");
        } else if (!BluetoothAdapter.getDefaultAdapter().isEnabled()) {
            // If bluetooth device is turned off, try to turn bluetooth on.
            Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBtIntent, 1);
        } else {
            // Proceed
            this.tryBindingAndConnectingDefaultBluetoothDevice();
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        if (resultCode == Activity.RESULT_CANCELED) {
            // If trying to turn bluetooth on is failed, cancel initialization
            this.resultInFail("Failed to be granted bluetooth permission");
        } else {
            // Proceed
            this.tryBindingAndConnectingDefaultBluetoothDevice();
        }
    }

    // Step 2. connectChannel to Bluetooth device without discovery
    // Step 2-1. Try binding and connecting default Bluetooth device
    private void tryBindingAndConnectingDefaultBluetoothDevice() {
        // Restore bluetooth name from local storage
        SharedPreferences sharedPreferences = this.getSharedPreferences(PREFERENCE_KEY, Context
                .MODE_PRIVATE);
        String bluetoothAddress = sharedPreferences.getString(PREFERENCE_ATTR_KEY_BT_ADDRESS, "");

        BluetoothDevice bluetoothDevice = findBondedBluetoothDevice(bluetoothAddress);
        if (bluetoothDevice != null) {
            // Receive binding result
            IntentFilter filter = new IntentFilter();
            filter.addAction(BluetoothDevice.ACTION_BOND_STATE_CHANGED);
            this.mBindingDefaultBluetoothDeviceReceiver = new
                    BindingDefaultBluetoothDeviceReceiver(bluetoothDevice);
            this.registerReceiver(this.mBindingDefaultBluetoothDeviceReceiver, filter);

            // Request binding default bluetooth device
            boolean immResult = bluetoothDevice.createBond();
            if (!immResult) {
                // Skip and proceed to Step 3
                this.prepareDiscoveryUI();
            }
        } else {
            // Skip and proceed to Step 3
            this.prepareDiscoveryUI();
        }
    }

    class BindingDefaultBluetoothDeviceReceiver extends BroadcastReceiver {
        private BluetoothDevice mBindingBluetoothDevice;

        public BindingDefaultBluetoothDeviceReceiver(BluetoothDevice bindingBluetoothDevice) {
            this.mBindingBluetoothDevice = bindingBluetoothDevice;
        }

        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            if (BluetoothDevice.ACTION_BOND_STATE_CHANGED.compareTo(action) == 0) {
                BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);

                if (this.mBindingBluetoothDevice != null) {
                    if (device.getBondState() == BluetoothDevice.BOND_BONDED) {
                        String bindingBluetoothName = this.mBindingBluetoothDevice.getName();
                        String bindingBluetoothAddress = this.mBindingBluetoothDevice.getAddress();
                        String thisBluetoothName = device.getName();
                        String thisBluetoothAddress = device.getAddress();
                        if (bindingBluetoothName.compareTo(thisBluetoothName) == 0 &&
                                bindingBluetoothAddress.compareTo(thisBluetoothAddress) == 0) {
                            // Return select
                            resultInSelect(bindingBluetoothAddress);
                        }
                    }
                }
                // TODO: what if bonding is not done forever?
            }
        }
    }

    // Step 3. Connect to Bluetooth device with discovery
    // Step 3-1. Prepare discovery UI
    private void prepareDiscoveryUI() {
        // Initialize array adapters. One for already paired devices and
        // one for newly discovered devices
        this.mBoundDevicesArrayAdapter = new ArrayAdapter<>(this, R.layout
                .template_listview_item_device);
        this.mNewDevicesArrayAdapter = new ArrayAdapter<>(this, R.layout
                .template_listview_item_device);

        // Find and set up the ListView for paired devices
        ListView pairedListView = (ListView) findViewById(R.id.paired_devices);
        pairedListView.setAdapter(this.mBoundDevicesArrayAdapter);
        pairedListView.setOnItemClickListener(mPairedDeviceListOnClickListener);

        // Find and set up the ListView for newly discovered devices
        ListView newDevicesListView = (ListView) findViewById(R.id.new_devices);
        newDevicesListView.setAdapter(mNewDevicesArrayAdapter);
        newDevicesListView.setOnItemClickListener(mNewDeviceListOnClickListener);

        // Register for broadcasts when a device is discovered
        IntentFilter filter = new IntentFilter();
        filter.addAction(BluetoothDevice.ACTION_FOUND);
        filter.addAction(BluetoothAdapter.ACTION_DISCOVERY_FINISHED);
        this.mBluetoothDeviceDiscoveryReceiver = new BluetoothDeviceDiscoveryReceiver();
        this.registerReceiver(this.mBluetoothDeviceDiscoveryReceiver, filter);

        // Get a set of currently paired devices
        Set<BluetoothDevice> pairedDevices = this.mBluetoothAdapter.getBondedDevices();

        // If there are paired devices, add each one to the ArrayAdapter
        if (pairedDevices.size() > 0) {
            findViewById(R.id.title_paired_devices).setVisibility(View.VISIBLE);
            for (BluetoothDevice device : pairedDevices) {
                this.mBoundDevicesArrayAdapter.add(device.getAddress() + "\n" + device.getName());
            }
        } else {
            String noDevices = getResources().getText(R.string.none_paired).toString();
            this.mBoundDevicesArrayAdapter.add(noDevices);
        }

        // Turn on sub-titles
        findViewById(R.id.title_paired_devices).setVisibility(View.VISIBLE);
        findViewById(R.id.title_new_devices).setVisibility(View.VISIBLE);

        Button scanButton = (Button) findViewById(R.id.button_scan);
        scanButton.setOnClickListener(this.mScanButtonOnClickListener);

        // If we're already discovering, stop it
        if (this.mBluetoothAdapter.isDiscovering()) {
            this.cancelDiscovery();
        }

        // Request discover from BluetoothAdapter
        this.startDiscovery();
    }

    private View.OnClickListener mScanButtonOnClickListener = new View.OnClickListener() {
        @Override
        public void onClick(View view) {
            startDiscovery();
        }
    };

    class BluetoothDeviceDiscoveryReceiver extends BroadcastReceiver {
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            // TODO: refine discovery UI
            // When discovery finds a device
            if (BluetoothDevice.ACTION_FOUND.compareTo(action) == 0) {
                // Get the BluetoothDevice object from the Intent
                BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                if (device.getBondState() != BluetoothDevice.BOND_BONDED) {
                    if (device.getName() != null) {
                        mNewDevicesArrayAdapter.add(device.getAddress() + "\n" + device.getName());
                    } else {
                        mNewDevicesArrayAdapter.add(device.getAddress());
                    }
                }
            } else if (BluetoothAdapter.ACTION_DISCOVERY_FINISHED.compareTo(action) == 0) {
                // When discovery is finished, change the Activity title
                setProgressBarIndeterminateVisibility(false);
                setTitle(R.string.select_device);
                Button scanButton = (Button) findViewById(R.id.button_scan);
                scanButton.setVisibility(View.VISIBLE);

                Log.d(TAG, "Discovery done");
                if (mNewDevicesArrayAdapter.getCount() == 0) {
                    String noDevices = getResources().getText(R.string.none_found).toString();
                    mNewDevicesArrayAdapter.add(noDevices);
                }
            }
        }
    }

    // Step 3-2. Start discovery
    private void startDiscovery() {
        // Initialize new device list
        this.mNewDevicesArrayAdapter.clear();
        this.mNewDevicesArrayAdapter.notifyDataSetChanged();

        // Indicate scanning in the title
        this.setProgressBarIndeterminateVisibility(true);
        this.setTitle(R.string.scanning);

        // Hide scan button until discovery is done
        Button scanButton = (Button) findViewById(R.id.button_scan);
        scanButton.setVisibility(View.GONE);

        // Request discover from BluetoothAdapter
        this.mBluetoothAdapter.startDiscovery();
    }

    private void cancelDiscovery() {
        this.setTitle(R.string.select_device);
        Button scanButton = (Button) findViewById(R.id.button_scan);
        scanButton.setVisibility(View.VISIBLE);

        this.mBluetoothAdapter.cancelDiscovery();
    }

    private AdapterView.OnItemClickListener mPairedDeviceListOnClickListener = new AdapterView
            .OnItemClickListener() {
        public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
            String entryString = mBoundDevicesArrayAdapter.getItem(position);
            if (entryString == null || entryString.compareTo(getResources().getText(R.string
                    .none_found).toString()) == 0) {
                return;
            }

            String[] bluetoothNameAddress = entryString.split("\\r?\\n");
            String bluetoothAddress = bluetoothNameAddress[0];

            cancelDiscovery();
            resultInSelect(bluetoothAddress);
        }
    };

    private AdapterView.OnItemClickListener mNewDeviceListOnClickListener = new AdapterView
            .OnItemClickListener() {
        public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
            String entryString = mNewDevicesArrayAdapter.getItem(position);
            if (entryString == null || entryString.compareTo(getResources().getText(R.string
                    .none_found).toString()) == 0) {
                return;
            }

            String[] bluetoothNameAddress = entryString.split("\\r?\\n");
            String bluetoothAddress = bluetoothNameAddress[0];

            cancelDiscovery();
            tryBindingBluetoothDevice(bluetoothAddress);
        }
    };

    // Step 3-3. (When selecting a bluetooth device) try binding the device
    private void tryBindingBluetoothDevice(String bluetoothAddress) {
        BluetoothDevice bluetoothDevice = getBluetoothDevice(bluetoothAddress);
        if (bluetoothDevice != null) {
            // Receive binding result
            IntentFilter filter = new IntentFilter();
            filter.addAction(BluetoothDevice.ACTION_BOND_STATE_CHANGED);
            this.mBindingBluetoothDeviceReceiver = new BindingBluetoothDeviceReceiver
                    (bluetoothDevice);
            this.registerReceiver(this.mBindingBluetoothDeviceReceiver, filter);

            // Request binding default bluetooth device
            boolean immResult = bluetoothDevice.createBond();
            if (!immResult) {
                // Result fail
                this.resultInFail("Cannot bind the Bluetooth device");
            }
        } else {
            // Result fail
            this.resultInFail("Cannot find the Bluetooth device, thus cannot bind that");
        }
    }

    class BindingBluetoothDeviceReceiver extends BroadcastReceiver {
        private BluetoothDevice mBindingBluetoothDevice;

        public BindingBluetoothDeviceReceiver(BluetoothDevice bindingBluetoothDevice) {
            this.mBindingBluetoothDevice = bindingBluetoothDevice;
        }

        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            if (BluetoothDevice.ACTION_BOND_STATE_CHANGED.compareTo(action) == 0) {
                BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                if (this.mBindingBluetoothDevice != null) {
                    if (device.getBondState() == BluetoothDevice.BOND_BONDED) {
                        String bindingBluetoothAddress = this.mBindingBluetoothDevice.getAddress();
                        String thisBluetoothAddress = device.getAddress();
                        if (bindingBluetoothAddress.compareTo(thisBluetoothAddress) == 0) {
                            // Return select
                            resultInSelect(bindingBluetoothAddress);
                        }
                    }
                }
                // TODO: what if bonding is not done forever?
            }
        }
    }

    // Result Part
    private void resultInSelect(String bluetoothAddress) {
        Bundle bundle = new Bundle();
        bundle.putString(BluetoothDeviceController.ConnectProcedure
                .BluetoothConnectorResultReceiver.RECEIVER_KEY_BT_ADDRESS, bluetoothAddress);
        this.mReceiver.send(Activity.RESULT_OK, bundle);
        Log.d(TAG, "Select success: " + bluetoothAddress);
        this.mResultDetermined = true;
        finish();
    }

    private void resultInFail(String failMessage) {
        Bundle bundle = new Bundle();
        bundle.putString(BluetoothDeviceController.ConnectProcedure
                .BluetoothConnectorResultReceiver.RECEIVER_KEY_FAIL_MESSAGE, failMessage);
        this.mReceiver.send(Activity.RESULT_CANCELED, bundle);
        Log.d(TAG, "Select fail: " + failMessage);
        this.mResultDetermined = true;
        finish();
    }

    // Utility functions
    private BluetoothDevice findBondedBluetoothDevice(String bluetoothAddress) {
        if (bluetoothAddress == null || bluetoothAddress.isEmpty()) {
            return null;
        }
        Set<BluetoothDevice> bluetoothDevices = BluetoothAdapter.getDefaultAdapter()
                .getBondedDevices();
        if (bluetoothDevices.size() > 0) {
            for (BluetoothDevice bluetoothDevice : bluetoothDevices) {
                String deviceAddress = bluetoothDevice.getAddress();
                if (deviceAddress.compareTo(bluetoothAddress) == 0) {
                    return bluetoothDevice;
                }
            }
        }
        return null;
    }

    private BluetoothDevice getBluetoothDevice(String bluetoothAddress) {
        if (bluetoothAddress == null || bluetoothAddress.isEmpty()) {
            return null;
        }
        BluetoothDevice bluetoothDevice = BluetoothAdapter.getDefaultAdapter().getRemoteDevice
                (bluetoothAddress);
        return bluetoothDevice;
    }
}