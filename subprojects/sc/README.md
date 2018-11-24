# P2PForYourThings
C++ APIs for you to easily setup and communicate via smart P2P (using available p2p modules among Wi-Fi Direct / Bluetooth / Bluetooth LE)

## Supported Devices
* Raspberry-pi 3
* NVIDIA Jetson-TX1

# Android
[P2PForYourThings-android](https://github.com/sinban04/P2PForYourThings-android)


# Quick Start
## How to Get the Source Code
```
$ git clone https://github.com/sinban04/P2PForYourThings.git
$ cd P2PForYourThings
```

## Pre-requisite
- Install Bluetooth library
```
$ sudo apt-get install libbluetooth-dev
```

Check your bluetooth version and if it is equal to or higher than 5.0, then
```
$ sudo vi /etc/systemd/system/dbus-org.bluez.service

ExecStart=/usr/lib/bluetooth/bluetoothd --compat

$ sudo systemctl daemon-reload
$ sudo systemctl restart bluetooth
$ sudo chmod 777 /var/run/sdp
```

- Check the wifi module support p2p
```
$ iw list
```

- Modify the wpa_supplicant.conf (```/etc/wpa_supplicant/wpa_supplicant.conf```)
Add two parameters

> update_config=1
>
> driver_param=p2p_device=1

- Install UDHCPD for wifi-direct
```
$ sudo apt-get install udhcpd
$ sudo touch /var/lib/misc/udhcpd.leases
```

- Set ```wpa_supplicant``` configuration
```
sudo wpa_supplicant -iwlan0 -c /etc/wpa_supplicant/wpa_supplicant.conf
```


## How to Build the Source Code
```
// No need to make the new directories now (Already exists)
$ mkdir bin obj ./device/obj 
$ make
```
Then you can find the binary file of the communicator test example,
which is in the ```tests/``` directory.

## How to Run
```
$ sudo ./bin/test
```

# Notes
## Bluetooth Issues
### Bluetooth Version Compatibility
Check your bluetooth version using
```
$ sudo vi /etc/systemd/system/dbus-org.bluez.service

ExecStart=/usr/lib/bluetooth/bluetoothd --compat

$ sudo systemctl daemon-reload
$ sudo systemctl restart bluetooth
$ sudo chmod 777 /var/run/sdp
```

### Check the bluetooth HCI interface
```
$ hciconfig -a
```

### Change hci0 interface to Scan Mode 
Set page scan and inquiry can (Discoverable)
```
$ sudo hciconfig hci0 piscan
```

### Change the hci0 interface name
e.g., change the name of the bluetooth interface to ```pi```
```
$ sudo hciconfig hci0 name pi
```

### Turn on the interface
```
$ sudo hciconfig hci0 up
```

### Turn on the bluetoothd (For the case, it's off)
```
sudo service bluetooth start
```
When the bluetooth.target is masked, unmask it
```
$ sudo systemctl unmask bluetooth
```
Check the bluetoothd exists
```
$ ps -ef | grep bluetooth
```

## Wifi-Direct Issues
### Remove the P2P group (Wifi-direct) on the wifi interface
remove group ```p2p-wlan0-0``` on the wifi interface ```wlan0```
```
sudo wpa_cli -i wlan0 p2p_group_remove p2p-wlan0-0
```
