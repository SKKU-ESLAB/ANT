[![License](https://img.shields.io/badge/licence-Apache%202.0-brightgreen.svg?style=flat)](LICENSE)

# ANT: AI-based Networked Things Framework
<img src="https://raw.githubusercontent.com/SKKU-ESLAB/ANT/master/docs/logo.png" width="30%" align="right" />

## Quick Start
ANT is being migrated to ANT lite, so its current build process is temporary.

### How to Get the Source Code

```
$ git clone https://github.com/SKKU-ESLAB/ANT ant --recursive
```

### Prerequisites
```
$ cd ant/lite/test
$ ./install-prerequisites-on-rpi3.sh
```
<!--
```
# TARGET_BOARD=[rpi2,rpi3,oxu3,oxu4,tx1,tx2]
$ ./tools/install-deps.sh --target-board={TARGET_BOARD}
$ sudo apt-get install cmake python3 pip3
$ sudo pip3 install kconfiglib
```

### Build Configuration

You can configure how to build ANT framework with following commands.
```
$ ./menuconfig.py
```
-->

### How to Build
```
$ ./build-ant.sh
```
<!--
```
$ mkdir build && cd build
$ cmake ..
$ make -j4
```

If you changed your build configuration, you should make your ```build``` directory once again.

### How to Install
```
$ sudo make install
```
-->
### How to Run
```
$ ./ant.sh
```
<!--
```
$ sudo run_ant
```
-->

## Demo Video

* [ANT Framework 1.0 (Youtube)](https://www.youtube.com/watch?v=QHFb4IKi8wM&t=34s)

## [License](https://github.com/SKKU-ESLAB/ANT/wiki/License)
ANT is open source software under the [Apache 2.0 license](http://www.apache.org/licenses/LICENSE-2.0). Complete license and copyright information can be found within the code.

Copyright 2017-2019 SKKU ESLAB, and contributors.

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0 Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.