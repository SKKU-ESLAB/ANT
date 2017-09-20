[![License](https://img.shields.io/badge/licence-Apache%202.0-brightgreen.svg?style=flat)](LICENSE)

# ANT: AI-based Networked Things Framework
## Quick Start
### How to Get the Source Code

```
$ git clone https://github.com/sinban04/ant-alpha
$ cd ant-alpha
```

### How to Make new branch with Target Device's Branch
* Alpha 1(Raspberry Pi 2 or Raspberry Pi 3)
```
$ git checkout origin/raspberry-pi2_3 -b raspberry-pi2_3
```
* Alpha 2(Nvidia TX1)
```
$ git checkout origin/tegraTX1 -b tegraTX1
```
* Alpha 3
```
$ git checkout origin/alpha3 -b alpha3
```
* Alpha 4
```
$ git checkout origin/alpha4-dahlia -b alpha4-dahlia
```
* Beta 1
```
$ git checkout origin/beta1-estrella -b beta1-estrella
```

### How to Build the Sensor Driver code.
You can build the sensor driver code for the specific target board.
The target-dependent code is in the target/ directory.
```
 $ cd target/<TARGET_BOARD>/sensor-drivers/
 $ cmake .
 $ make
 $ cd out/sensor-drivers/
```
The driver code and specification is in the out/sensor-drivers/ directory.
You need to copy it to the ANT sensor driver code directory
```
$ cp libsensors.so ${ANT_DIR}/out/sensor-drivers/
$ cp sensor_config.json ${ANT_DIR}/out/sensor-drivers/

```

### How to Install Prerequisites
It is dependent on target device.

In example of tegraTX1:

```
$ ./target/tegraTX1/install-deps-tegraTX1.sh
```

### How to Build
```
$ cmake -Bbuild -H.
$ make
```

For more details, see [Getting Started](https://github.com/sinban04/ant-alpha/wiki/Getting-Started).

### How to Install (Alpha 3)
In example of tegraTX1:

```
$ sudo ./scripts/install.sh --target=tegraTX1
```

You need target profile on ```target/TARGET_NAME/profile.env``` before running install script.

## Documentation
* [Getting Started](https://github.com/sinban04/ant-alpha/wiki/Getting-Started)

## Videos
* [ANT Demo Video: Raspberry Pi 2](https://www.youtube.com/watch?v=6iI4zDDX-YE) (2016.01.14)
* [ANT Demo Scenario Video: Smart Guardner](https://www.youtube.com/watch?v=oYkVgrFMAuc) (2016.01.12)
* [ANT Demo Video: Nvidia TX1](https://www.youtube.com/watch?v=MFXGAeuxfxE) (2016.10.06)

## [License](https://github.com/sinban04/ant-alpha/wiki/License)
ANT is open source software under the [Apache 2.0 license](http://www.apache.org/licenses/LICENSE-2.0). Complete license and copyright information can be found within the code.

Copyright 2017 SKKU ESLAB, and contributors.

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0 Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
