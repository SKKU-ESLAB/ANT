# How to Build

1. Locate to the JNI directory
  1. ```$ cd ant/android-companion/app/jni```
2. Prepare Gstreamer library for Android (Version: 1.14.0)
  * Version dependency is very very important!
  * Filename: gstreamer-1.0-android-universal-1.14.0.tar.bz2
  1. Download from here: https://gstreamer.freedesktop.org/data/pkg/android/1.14.0/gstreamer-1.0-android-universal-1.14.0.tar.bz2
    * If you cannot find the file, refer to: https://gstreamer.freedesktop.org/documentation/installing/for-android-development.html?gi-language=c#building-the-tutorials
  2. After download, set environment variables
    * Example: ```$ export GSTREAMER_ROOT=/home/me/gstreamer/arm64```
3. Prepare Android NDK (Version: android-ndk-r18b)
  * Version dependency is very very important!
  * Filename: android-ndk-r18b-linux-x86_64.zip
  1. Download from here: https://dl.google.com/android/repository/android-ndk-r16b-linux-x86_64.zip
    * If you cannot find the file, refer to: https://developer.android.com/ndk/downloads
  2. After download, set environemnt variables
    * Example: ```$ export PATH=/home/me/android-sdk-tools/android-ndk-r16b:$PATH```
4. Build it!
  1. ```$ ndk-build```
  * Then you can get shared libraries:
    * ```ant/android-companion/app/libs/arm64-v8a/libgstreamer_android.so```
    * ```ant/android-companion/app/libs/arm64-v8a/libstreaming-fragment.so```

# Additional Issues
## Build for armv7 target
In default, its target device is ```arm64```.

If you want to build it for armv7, edit ```APP_ABI``` in ```Application.mk``` and environment variable ```GSTREAMER_ROOT```.

## multiple definition error
You can configure gstreamer's makefile
* ```gstreamer/arm64/share/gst-android/ndk-build/gstreamer-1.0.mk```

Edit following line:
*```GSTREAMER_LD := -fuse-ld=gold$(EXE_SUFFIX) -Wl,-soname,lib$(GSTREAMER_ANDROID_MODULE_NAME).so```
into
* ```GSTREAMER_LD := -fuse-ld=gold$(EXE_SUFFIX) -Wl,--allow-multiple-definition,-soname,lib$(GSTREAMER_ANDROID_MODULE_NAME).so```