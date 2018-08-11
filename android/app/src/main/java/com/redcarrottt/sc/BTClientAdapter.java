package com.redcarrottt.sc;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;

import com.redcarrottt.testapp.Logger;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.IOException;
import java.util.Set;
import java.util.UUID;

/* Copyright (c) 2017-2018. All rights reserved.
 *  Gyeonghwan Hong (redcarrottt@gmail.com)
 *  Eunsoo Park (esevan.park@gmail.com)
 *  Injung Hwang (sinban04@gmail.com)
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

public class BTClientAdapter extends NetworkAdapter {
    private UUID uuid;
    private String dev_addr;
    private BluetoothSocket btSocket;
    private String kTag = "BTClientAdapter";
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
        Set<BluetoothDevice> paired_device = BluetoothAdapter.getDefaultAdapter()
                .getBondedDevices();

        for (BluetoothDevice btDevice : paired_device) {
            if (btDevice.getAddress().equals(dev_addr)) {
                try {
                    btSocket = btDevice.createRfcommSocketToServiceRecord(uuid);
                    writer = new BufferedOutputStream(btSocket.getOutputStream());
                    reader = new BufferedInputStream(btSocket.getInputStream());
                } catch (IOException e) {
                    Logger.print(kTag, "Failed to create RFCOMM Socket corresponding to " + uuid
                            .toString());
                    btSocket = null;
                }
                break;
            }
        }

        for (int i = 0; i < 2; i++) {
            try {
                btSocket.connect();
                btSocket.getInputStream();
                break;
            } catch (IOException e) {
                Logger.print(kTag, "Failed to connect");
            }
            try {
                Logger.print(kTag, "Once more try to connect BT Client");
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
            Logger.print(kTag, "reader is null");
            return -1;
        }

        int res = 0;

        try {
            int read_bytes = 0;
            while (read_bytes < len) {
                int bytes = reader.read(buf, read_bytes, len - read_bytes);
                if (bytes < 0) {
                    len = -1;
                    break;
                }
                read_bytes += bytes;
            }
            res = len;
        } catch (Exception e) {
            e.printStackTrace();
            res = -1;
        }
        return res;
    }

    @Override
    public void on_control_recv(byte[] buf, int len) {

    }
}
