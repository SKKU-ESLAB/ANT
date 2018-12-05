[![License](https://img.shields.io/badge/licence-Apache%202.0-brightgreen.svg?style=flat)](LICENSE)

# ANT: AI-based Networked Things Framework
<img src="https://raw.githubusercontent.com/SKKU-ESLAB/ANT/master/docs/logo.png" width="30%" align="right" />

## Quick Start
### How to Get the Source Code

```
$ git clone https://github.com/SKKU-ESLAB/ANT
```

### Prerequisites
It is dependent on target device.

#### Common Prerequisites
ANT requires ```cmake```, ```python3```, and ```kconfiglib``` to configure build.
```
$ sudo apt-get install cmake python3 pip3
$ sudo pip3 install kconfiglib
```

#### Target-dependent Prerequisites
In example of Raspberry Pi 2 or 3:

```
$ ./target/raspberry-pi2_3/install-deps-raspberry-pi2_3.sh
```

In example of Tegra TX1 or TX2:

```
$ ./target/tegraTX1/install-deps-tegraTX1.sh
```

### Build Configuration

You can configure how to build ANT framework with following commands.
```
$ ./menuconfig.py
```

Then you can see following build configuration menu.

![ANT Build Config](https://user-images.githubusercontent.com/1433126/49520297-48106280-f8e6-11e8-96cb-ee00451e7671.png)

### How to Build

```
$ mkdir build && cd build
$ cmake ..
$ make -j4
```

If you changed your build configuration, you should make your ```build``` directory once again.

### How to Install
In example of Raspberry Pi 2 or 3:

```
$ sudo ./scripts/install.sh --target=raspberry-pi2_3
```
### How to Run

```
$ sudo run_ant
```

## For Your Information (FYI)
### SDP session registration problem
When you meet some error in registering SDP session after install ANT framework,
please reboot.
(For more details, please refer to #44)
```
$ sudo reboot
```

### Bluetooth Issues
When you are using ANT with raspberry pi or tegra, you need to make it to bluetooth-pair with the mobile phone.
Our ANT mobile Android App provides bluetooth discovery feature when it starts the app.
You need to make the device discoverable with the bluetooth.

In raspberry pi, 
#### Change hci0 interface to Scan Mode
```
$ sudo hciconfig hci0 piscan
```

For more information, see docs/blueooth.md


### Wifi-Direct Issues
#### Remove the P2P group (Wifi-direct) on the wifi interface
remove group p2p-wlan0-1 on the wifi interface wlan0
```
sudo wpa_cli -i wlan0 p2p_group_remove p2p-wlan0-1
```

You need target profile on ```target/TARGET_NAME/profile.env``` before running install script.

## Demo Video

* [ANT Framework 1.0 (Youtube)](https://www.youtube.com/watch?v=QHFb4IKi8wM&t=34s)

## [License](https://github.com/SKKU-ESLAB/ANT/wiki/License)
ANT is open source software under the [Apache 2.0 license](http://www.apache.org/licenses/LICENSE-2.0). Complete license and copyright information can be found within the code.

Copyright 2017 SKKU ESLAB, and contributors.

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0 Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
