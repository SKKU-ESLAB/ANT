package com.ant.cmfw.ports.bluetooth.socket;

/* Copyright (c) 2017 SKKU ESLAB, and contributors. All rights reserved.
 *
 * Contributor: Gyeonghwan Hong<redcarrottt@gmail.com>
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

import android.bluetooth.BluetoothSocket;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.reflect.Method;

public class FallbackBluetoothSocket extends NativeBluetoothSocket {
    private BluetoothSocket fallbackSocket;

    public FallbackBluetoothSocket(BluetoothSocket tmp) throws
            FallbackException {
        super(tmp);
        try {
            Class<?> clazz = tmp.getRemoteDevice().getClass();
            Class<?>[] paramTypes = new Class<?>[]{Integer.TYPE};
            Method m = clazz.getMethod("createRfcommSocket", paramTypes);
            Object[] params = new Object[]{Integer.valueOf(1)};
            fallbackSocket = (BluetoothSocket) m.invoke(tmp
                    .getRemoteDevice(), params);
        } catch (Exception e) {
            throw new FallbackException(e);
        }
    }

    @Override
    public InputStream getInputStream() throws IOException {
        return fallbackSocket.getInputStream();
    }

    @Override
    public OutputStream getOutputStream() throws IOException {
        return fallbackSocket.getOutputStream();
    }

    @Override
    public void connect() throws IOException {
        fallbackSocket.connect();
    }

    @Override
    public void close() throws IOException {
        fallbackSocket.close();
    }
}
