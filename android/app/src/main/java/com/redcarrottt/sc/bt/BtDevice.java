package com.redcarrottt.sc.bt;

import com.redcarrottt.sc.Device;

/* Copyright (c) 2017-2018. All rights reserved.
 *  Gyeonghwan Hong (redcarrottt@gmail.com)
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

public class BtDevice extends Device {
    @Override
    protected boolean turnOnImpl() {
        // TODO:
        return false;
    }

    @Override
    protected boolean turnOffImpl() {
        // TODO:
        return false;
    }

    // Constructor
    public BtDevice(String name) {
        // TODO:
        super(name);
    }
}
