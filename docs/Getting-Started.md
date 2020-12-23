# Getting Started
ANT Lite is a lightweight-version of ANT framework.

ANT Lite is now available on any ARM Cortex-A devices. In future, we will add support for Cortex-M or Cortex-R devices.

## How to Get the Source Code

```
$ git clone https://github.com/SKKU-ESLAB/ANT ant
```

## Prerequisites
```
$ cd ant
$ ./scripts/prepare-ubuntu.sh
```

To run machine learning interence example, please download also the sample models.
```
$ cd ml
$ ./download_sample_model.sh
```

## How to Build
```
$ ./scripts/build-ant.sh
```

## How to Run
```
$ ./out/ant.sh
```
