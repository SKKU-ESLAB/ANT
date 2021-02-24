[![License](https://img.shields.io/badge/licence-Apache%202.0-brightgreen.svg?style=flat)](LICENSE)

# ANT: AI-based Networked Things Framework
<img src="https://raw.githubusercontent.com/SKKU-ESLAB/ANT/master/docs/logo.png" width="30%" align="right" />

* [Documents](https://SKKU-ESLAB.github.io/ANT)

## Quick Start

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

### How to Run
1. Run ANT daemon
```
$ ./out/antd
```
2. Then, open the ANT dashboard website or ANT Android companion app

## Demo Video

* [ANT Framework 1.0 (Youtube)](https://youtu.be/AhO3crw8jro)

## [License](https://github.com/SKKU-ESLAB/ANT/wiki/License)
ANT is open source software under the [Apache 2.0 license](http://www.apache.org/licenses/LICENSE-2.0). Complete license and copyright information can be found within the code.

Copyright 2017-2020 SKKU ESLAB, and contributors.

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0 Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
