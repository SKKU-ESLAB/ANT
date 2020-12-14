[![License](https://img.shields.io/badge/licence-Apache%202.0-brightgreen.svg?style=flat)](LICENSE)

# ANT: AI-based Networked Things Framework
<img src="https://raw.githubusercontent.com/SKKU-ESLAB/ANT/master/docs/logo.png" width="30%" align="right" />

## Quick Start
ANT is being migrated to ANT lite, so its current build process is temporary.

### How to Get the Source Code

```
$ git clone https://github.com/SKKU-ESLAB/ANT ant
```

### Prerequisites
```
$ cd ant
$ ./scripts/prepare-ubuntu.sh
```

To run machine learning interence example, please download also the sample models.
```
$ cd ml
$ ./download_sample_model.sh
```

### How to Build
```
$ ./scripts/build-ant.sh
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
$ ./out/ant.sh
```
<!--
```
$ sudo run_ant
```
-->

## Demo Video

* [ANT Framework 1.0 (Youtube)](https://youtu.be/AhO3crw8jro)

## [License](https://github.com/SKKU-ESLAB/ANT/wiki/License)
ANT is open source software under the [Apache 2.0 license](http://www.apache.org/licenses/LICENSE-2.0). Complete license and copyright information can be found within the code.

Copyright 2017-2020 SKKU ESLAB, and contributors.

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0 Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
