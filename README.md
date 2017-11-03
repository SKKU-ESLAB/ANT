[![License](https://img.shields.io/badge/licence-Apache%202.0-brightgreen.svg?style=flat)](LICENSE)

# ANT: AI-based Networked Things Framework
## Quick Start
### How to Get the Source Code

```
$ git clone https://github.com/SKKU-ESLAB/ANT
$ cd ANT
```

### How to Build the Sensor Driver code.
You can build the sensor driver code for the specific target board.
The target-dependent code is in the target/ directory.
```
 $ cd ${ANT_ROOT_DIR}
 $ cd target/<TARGET_BOARD>/sensor-drivers/
 $ cmake .
 $ make
 $ cd out/sensor-drivers/
```
The driver code and specification is in the out/sensor-drivers/ directory.
You need to copy it to the ANT sensor driver code directory
```
$ cp libsensors.so ${ANT_ROOT_DIR}/out/sensor-drivers/
$ cp sensor_config.json ${ANT_ROOT_DIR}/out/sensor-drivers/

```

### How to Install Prerequisites
It is dependent on target device.

In example of Raspberry Pi 2 or 3:

```
$ cd ${ANT_ROOT_DIR}
$ ./target/raspberry-pi2_3/install-deps-raspberry-pi2_3.sh
```

### How to Build
```
$ cd ${ANT_ROOT_DIR}
$ mkdir build
$ cd build
$ cmake ..
$ make -j4
```

### How to Install
In example of Raspberry Pi 2 or 3:

```
$ cd ${ANT_ROOT_DIR}
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


You need target profile on ```target/TARGET_NAME/profile.env``` before running install script.

## [License](https://github.com/SKKU-ESLAB/ANT/wiki/License)
ANT is open source software under the [Apache 2.0 license](http://www.apache.org/licenses/LICENSE-2.0). Complete license and copyright information can be found within the code.

Copyright 2017 SKKU ESLAB, and contributors.

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0 Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
