# Testing ANT Framework
## App Benchmark (JavaScript Benchmark)
ANT app benchmark is composed of several JavaScript benchmark workloads.
It can show how ANT's JavaScript engine saves memory consumption 

* ```appbench/v8-deltablue.js```: V8 benchmark's deltablue
* ```appbench/v8-raytrace.js```: V8 benchmark's raytrace
* ```appbench/sp-access-binary-trees.js```: SunSpider benchmark's access-binary-trees
* ```appbench/sp-crypto-sha1.js```: SunSpider benchmark's crypto-sha1

## Resource API Benchmark
ANT resource API benchmark calls legacy IPC/RPC or resource-API-based IPC/RPC.
It measures the overhead of ANT's resource API.

* ```resourcebench/resource-api.js```
* ```resourcebench/legacy-api.js```

## Compatibility Test
ANT compatibility test is composed of test case code for ANT APIs.
If a device passes the compatibility test, the device is compatible with ANT framework.

* ```compatibility/streaming.js```

* Passed Devices
  * Raspberry Pi 2
  * Raspberry Pi 3
  * Odroid-XU3
  * Odroid-XU4
  * Jetson TX1
  * Jetson TX2
  * AGX Xavier
  
![Testing on Odroid-XU4](https://github.com/RedCarrottt/ANT/blob/master/test/Test-Odroid-XU4.jpg)
