package com.redcarrottt.sc.wfd;

import com.redcarrottt.sc.Device;

public class WfdDevice extends Device {
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

    // Singleton
    public static WfdDevice getSingleton() {
        if(sSingleton == null) {
            sSingleton = new WfdDevice();
        }
        return sSingleton;
    }
    private static WfdDevice sSingleton = null;

    // Constructor
    private WfdDevice() {
        super("Wi-fi Direct");
    }
}