package com.redcarrottt.sc.wfd;

import com.redcarrottt.sc.Device;

public class WfdDevice extends Device {
    @Override
    protected boolean turnOnImpl() {
        return false;
    }

    @Override
    protected boolean turnOffImpl() {
        return false;
    }

    // Constructor
    public WfdDevice(String name) {
        super(name);
    }
}