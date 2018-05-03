package selective.connection;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.DhcpInfo;
import android.net.NetworkInfo;
import android.net.wifi.WifiManager;
import android.net.wifi.WpsInfo;
import android.net.wifi.p2p.WifiP2pConfig;
import android.net.wifi.p2p.WifiP2pDevice;
import android.net.wifi.p2p.WifiP2pDeviceList;
import android.net.wifi.p2p.WifiP2pGroup;
import android.net.wifi.p2p.WifiP2pInfo;
import android.net.wifi.p2p.WifiP2pManager;
import android.util.Log;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.util.Arrays;

import kr.ac.skku.nyx.selectiveconnection.MainActivity;

import static android.os.Looper.getMainLooper;

/**
 * Created by eslab on 2017-02-16.
 */

public class WFDClientAdapter extends NetworkAdapter {
    private String dev_addr;
    private String wps_pin;
    private Object server_up_trigger;
    private Object ip_trigger;
    private Object connected_trigger;
    private String tag = "WFDClientAdapter";
    private int port;
    private String ip;
    private Socket sock;
    private BufferedInputStream reader;
    private BufferedOutputStream writer;

    private WifiP2pManager mManager;
    private WifiP2pManager.Channel mChannel;

    private boolean connected_notified;

    private final IntentFilter intentFilter = new IntentFilter();

    private WifiDirectBroadcastReceiver bReceiver;
    private Activity givenCt;

    public WFDClientAdapter(short dev_id, int port, Activity ct) {
        this.dev_id = dev_id;
        dev_addr = null;
        wps_pin = null;
        this.port = port;

        server_up_trigger = new Object();
        ip_trigger = new Object();
        connected_trigger = new Object();
        mManager = (WifiP2pManager) ct.getSystemService(Context.WIFI_P2P_SERVICE);
        mChannel = mManager.initialize(ct, getMainLooper(), null);

        bReceiver = new WifiDirectBroadcastReceiver();

        intentFilter.addAction(WifiP2pManager.WIFI_P2P_STATE_CHANGED_ACTION);
        intentFilter.addAction(WifiP2pManager.WIFI_P2P_PEERS_CHANGED_ACTION);
        intentFilter.addAction(WifiP2pManager.WIFI_P2P_CONNECTION_CHANGED_ACTION);
        intentFilter.addAction(WifiP2pManager.WIFI_P2P_DISCOVERY_CHANGED_ACTION);
        intentFilter.addAction(WifiP2pManager.WIFI_P2P_THIS_DEVICE_CHANGED_ACTION);
        ct.registerReceiver(bReceiver, intentFilter);
        givenCt = ct;
        this.dev_type = kWifiDirect;
    }

    public void onResume(Context ct) {
        ct.registerReceiver(bReceiver, intentFilter);
    }

    public void onPause(Context ct) {
        if (mManager != null) {
            mManager.stopPeerDiscovery(mChannel, null);
        }
        ct.unregisterReceiver(bReceiver);
    }
    @Override
    public boolean device_on() {
        return true;
    }

    @Override
    public boolean device_off() {
        return true;
    }

    @Override
    public boolean make_connection() {
        boolean res = false;
        connected_notified = false;
        ip = null;

        if (dev_addr == null) {
            try {
                synchronized(server_up_trigger) {
                    server_up_trigger.wait(5000);
                }
                if (dev_addr != null){
                    Log.d(tag, "Server up");
                    res = true;
                } else {
                    Log.d(tag, "Waited");
                    res = false;
                }
            } catch (InterruptedException e) {
                res = false;
            }
        }

        if (!res) return res;

        mManager.discoverPeers(mChannel, new WifiP2pManager.ActionListener() {
            @Override
            public void onSuccess() {
                Log.d(tag, "Discover peers succeeded");

            }

            @Override
            public void onFailure(int reason) {
                Log.d(tag, "Discover peers failed");
            }
        });

        try {
            if (!connected_notified) {
                synchronized (connected_trigger) {
                    connected_trigger.wait(15000);
                }
            }

            if (connected_notified) {
                res = true;
            } else {
                dev_addr = null;
                Log.d(tag, "Timeout");
                mManager.stopPeerDiscovery(mChannel, null);
                res = false;
            }
        } catch (InterruptedException e) {
            res = false;
        }

        if (!res) return res;

        send_ctrl_msg(new String("Connected").getBytes(), 9);

        try {
            synchronized(ip_trigger) {
                ip_trigger.wait(15000);
            }
            if (ip == null) {
                dev_addr = null;
                res = false;
            } else {
                res = true;
            }
        } catch (InterruptedException e) {
            res = false;
        }

        if (!res) return res;

        try {
            if (sock != null)
                sock.close();

            Log.d(tag, "Connect to " + ip);
            sock = new Socket();
            sock.bind(null);
            sock.connect(new InetSocketAddress(ip, port), 2000);

            writer = new BufferedOutputStream(sock.getOutputStream());
            reader = new BufferedInputStream(sock.getInputStream());

            res = true;
        } catch (Exception e) {
            e.printStackTrace();
            Log.d(tag, "Failed to connect to server");

            writer = null;
            reader = null;
            res = false;
        }

        return res;
    }

    @Override
    public boolean close_connection() {
        ;
        boolean res = false;
        try {
            sock.close();
            res = true;
            ip = null;
            reader = null;
            writer = null;
            sock = null;
            dev_addr = null;
            wps_pin = null;

        } catch (IOException e) {
            e.printStackTrace();
        }
        return res;
    }

    @Override
    public int send(byte[] buf, int len) {
        if (writer == null) {
            return -1;
        }

        int res = 0;

        try {
            writer.write(buf, 0, len);
            writer.flush();
            res = len;
        } catch (IOException e) {
            e.printStackTrace();
            res = -1;
        }
        return res;
    }

    @Override
    public int recv(byte[] buf, int len) {
        if (reader == null) {
            Log.d(tag, "reader is null");
            return -1;
        }

        int res = 0;

        try {
            int read_bytes = 0;
            while (read_bytes < len) {
                int bytes = reader.read(buf, read_bytes, len-read_bytes);
                if (bytes < 0) {
                    len = -1;
                    break;
                }

                read_bytes += bytes;
            }
            res = len;
        }catch (Exception e) {
            e.printStackTrace();
            res = -1;
        }
        return res;
    }

    @Override
    public void on_control_recv(byte[] buf, int len) {
        if (len == 17) {
            dev_addr = new String(Arrays.copyOfRange(buf, 0, len));
        } else if (len == 8) {
            wps_pin = new String(Arrays.copyOfRange(buf, 0, len));

            Log.d(tag, dev_addr + " & " + wps_pin);
            synchronized(server_up_trigger) {
                server_up_trigger.notifyAll();
            }
        } else { //IP Addr
            ip = new String(Arrays.copyOfRange(buf, 0, len));
            Log.d(tag, ip);

            synchronized(ip_trigger) {
                ip_trigger.notifyAll();
            }
        }
    }

    class WifiDirectBroadcastReceiver extends BroadcastReceiver {
        private WifiP2pManager.PeerListListener mPeerListListner;
        private WifiP2pManager.ConnectionInfoListener mConnectionListener;
        private String tag = "BroadcastReceiver";

        public WifiDirectBroadcastReceiver() {
            mPeerListListner = new WifiP2pManager.PeerListListener() {
                @Override
                public void onPeersAvailable(WifiP2pDeviceList peers) {
                    Log.d(tag, "onPeersAvailable occurred!");
                    WifiP2pConfig config = new WifiP2pConfig();

                    if (dev_addr == null) {
                        Log.d(tag, "dev addr is not set yet");
                        return;
                    }
                    Log.d(tag, "find the peer and request WFD");

                    for (WifiP2pDevice device : peers.getDeviceList()) {
                        if(device.deviceAddress.equals(dev_addr)) {
                            if (device.status == WifiP2pDevice.AVAILABLE) {
                                config.deviceAddress = dev_addr;
                                if (device.wpsPbcSupported()) {
                                    Log.d(tag, "PBC");
                                    config.wps.setup = WpsInfo.PBC;
                                } else if(device.wpsKeypadSupported()) {
                                    Log.d(tag, "KEYPAD");
                                    config.wps.setup = WpsInfo.KEYPAD;
                                } else if(device.wpsDisplaySupported()) {
                                    Log.d(tag, "DISPLAY");
                                    config.wps.setup = WpsInfo.DISPLAY;

                                }
                                config.groupOwnerIntent = 0;
                                config.wps.pin = wps_pin;
                                Log.d(tag, "Request to connect: " + device.deviceName + config.deviceAddress + "(" + config.wps.pin + ")");
                                mManager.connect(mChannel, config, new WifiP2pManager.ActionListener() {
                                    @Override
                                    public void onSuccess() {
                                        Log.d(tag, "Manager tries to connect");
                                    }

                                    @Override
                                    public void onFailure(int reason) {
                                    }
                                });
                                break;
                            }
                        }
                    }
                }
            };

        }
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            Log.d(tag, "onReceive occurred!");

            if (WifiP2pManager.WIFI_P2P_PEERS_CHANGED_ACTION.equals(action)) {
                // Call WifiP2pManager.requestPeers() to get a list of current peers
                Log.d(tag, "WIFI_P2P_PEERS_CHANGED_ACTION occurred");
                if (mManager != null) {
                    mManager.requestPeers(mChannel, mPeerListListner);
                }
            } else if (WifiP2pManager.WIFI_P2P_STATE_CHANGED_ACTION.equals(action)) {
                int state = intent.getIntExtra(WifiP2pManager.EXTRA_WIFI_STATE, -1);
                if(state == WifiP2pManager.WIFI_P2P_STATE_ENABLED)
                {
                    Log.d(tag, "Wifi P2P is enabled");
                }
                else
                {
                    Log.d(tag, "Wifi P2P is not enabled");
                }
                Log.d(tag, "P2P state changed - " + state);

            } else if (WifiP2pManager.WIFI_P2P_CONNECTION_CHANGED_ACTION.equals(action)) {
                // Respond to new connection or disconnections
                NetworkInfo networkInfo = (NetworkInfo) intent.getParcelableExtra(WifiP2pManager.EXTRA_NETWORK_INFO);
                Log.d(tag, "Connection changed - " + networkInfo.toString());
                if (networkInfo.isConnected()) {
                    //Log.d(tag, "Connected:" + dev_addr);
                    WifiP2pGroup p2pGroup = (WifiP2pGroup) intent.getParcelableExtra(WifiP2pManager.EXTRA_WIFI_P2P_GROUP);
                    if (p2pGroup.getOwner().deviceAddress != null && p2pGroup.getOwner().deviceAddress.equals(dev_addr)) {
                        Log.d(tag, "Con" +
                                "nected:" + dev_addr);
                    }
                    synchronized(connected_trigger) {
                        connected_notified = true;
                        connected_trigger.notifyAll();
                    }
                }
            } else if (WifiP2pManager.WIFI_P2P_DISCOVERY_CHANGED_ACTION.equals(action)) {
                int discoveryState = intent.getIntExtra(WifiP2pManager.EXTRA_DISCOVERY_STATE,
                        WifiP2pManager.WIFI_P2P_DISCOVERY_STOPPED);
                //Log.d(tag, "Discovery state changed: " + discoveryState);
            }
        }
    }
}
