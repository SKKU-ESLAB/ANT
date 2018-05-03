package selective.connection;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.util.Log;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.IOException;
import java.util.Set;
import java.util.UUID;

/**
 * Created by eslab on 2017-02-20.
 */

public class BTClientAdapter extends NetworkAdapter {
    private UUID uuid;
    private String dev_addr;
    private BluetoothSocket btSocket;
    private String tag = "BTClientAdapter";
    private BufferedOutputStream writer;
    private BufferedInputStream reader;

    public BTClientAdapter(short id, String dev_addr, String svc_uuid) {
        this.dev_id = id;
        this.set_controllable();
        this.dev_addr = dev_addr;
        uuid = UUID.fromString(svc_uuid);
        btSocket = null;
        writer = null;
        reader = null;
        this.dev_type = kBluetooth;
    }
    @Override
    public boolean device_on() {
        return false;
    }

    @Override
    public boolean device_off() {
        return false;
    }

    @Override
    public boolean make_connection() {
        Set<BluetoothDevice> paired_device = BluetoothAdapter.getDefaultAdapter().getBondedDevices();

        for (BluetoothDevice btDevice : paired_device) {
            if (btDevice.getAddress().equals(dev_addr)) {
                try {
                    btSocket = btDevice.createRfcommSocketToServiceRecord(uuid);
                    writer = new BufferedOutputStream(btSocket.getOutputStream());
                    reader = new BufferedInputStream(btSocket.getInputStream());
                } catch (IOException e) {
                    Log.d(tag, "Failed to create RFCOMM Socket corresponding to " + uuid.toString());
                    btSocket = null;
                }
                break;
            }
        }

        for (int i=0; i<2; i++) {
            try {
                btSocket.connect();
                btSocket.getInputStream();
                break;
            } catch (IOException e) {
                Log.d(tag, "Failed to connect");
            }
            try {
                Log.d(tag, "Once more try to connect BT Client");
                Thread.sleep(500);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        return btSocket.isConnected();
    }

    @Override
    public boolean close_connection() {
        if (btSocket != null && btSocket.isConnected()) {
            try {
                btSocket.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        btSocket = null;

        return true;
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

    }
}
