package com.redcarrottt.sc;

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

public class API {
    public static void start_sc() {
        Core.getInstance().start();
    }

    public static void stop_sc() {
        Core.getInstance().stop();
    }

    public static void register_control_adapter(ClientAdapter adapter) {
        Core.getInstance().registerControlAdapter(adapter);
    }

    public static void register_data_adapter(ClientAdapter adapter) {
        Core.getInstance().registerDataAdapter(adapter);
    }

    public static int send(byte[] dataBuffer, int dataLength) {
        return Core.getInstance().send(dataBuffer, dataLength);
    }

    public static int receive(byte[] dataBuffer) {
        return Core.getInstance().receive(dataBuffer);
    }
}