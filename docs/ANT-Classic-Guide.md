# ANT Classic Guide
```
$ git clone https://github.com/SKKU-ESLAB/ANT
```

## Prerequisites
It is dependent on target device.

### Common Prerequisites
ANT requires ```cmake```, ```python3```, and ```kconfiglib``` to configure build.
```
$ sudo apt-get install cmake python3 pip3
$ sudo pip3 install kconfiglib
```

### Target-dependent Prerequisites
In example of Raspberry Pi 2 or 3:

```
$ ./target/raspberry-pi2_3/install-deps-raspberry-pi2_3.sh
```

In example of Tegra TX1 or TX2:

```
$ ./target/tegraTX1/install-deps-tegraTX1.sh
```

## Build Configuration

You can configure how to build ANT framework with following commands.
```
$ ./menuconfig.py
```

Then you can see following build configuration menu.

![ANT Build Config](https://user-images.githubusercontent.com/1433126/49520297-48106280-f8e6-11e8-96cb-ee00451e7671.png)

## How to Build

```
$ mkdir build && cd build
$ cmake ..
$ make -j4
```

If you changed your build configuration, you should make your ```build``` directory once again.

## How to Install
In example of Raspberry Pi 2 or 3:

```
$ sudo ./scripts/install.sh --target=raspberry-pi2_3
```
## How to Run

```
$ sudo run_ant
```
