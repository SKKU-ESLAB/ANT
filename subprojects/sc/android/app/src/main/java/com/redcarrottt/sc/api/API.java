package com.redcarrottt.sc.api;

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

import com.redcarrottt.sc.internal.ClientAdapter;
import com.redcarrottt.sc.internal.Core;

public class API {
    public static void startSC(OnStartSCResult resultListener) {
        Core.singleton().start(resultListener);
    }

    public static void stopSC(OnStopSCResult resultListener) {
        Core.singleton().stop(resultListener);
    }

    public static void registerAdapter(ClientAdapter adapter) {
        Core.singleton().registerAdapter(adapter);
    }

    public static int send(byte[] dataBuffer, int dataLength) {
        return Core.singleton().send(dataBuffer, dataLength, false);
    }

    public static int receive(byte[] dataBuffer) {
        return Core.singleton().receive(dataBuffer, false);
    }
}