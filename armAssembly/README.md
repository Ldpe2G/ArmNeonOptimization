# Arm V7 & v8 inline assembly examples

Showing how to write arm v7 & v8 inline assemble codes with two examples, array weighted sum and rgb2gray.
Tested on Huawei P30 (Kirin 980).

## Build && Run
0. Install `adb` tool with following command

```
sudo apt update
sudo apt install android-tools-adb android-tools-fastboot
```

1. Download [android ndk 16b](https://developer.android.com/ndk/downloads/older_releases.html)

2. Install cmake 3.11

3. Build the project by running the `build_android.sh` script and remember to modify the following parameter.

```
BUILD_ANDROID_NDK_HOME=/home/ldpe2g/Android/android-ndk-r1bn
DEPLOY_DIR=/data/local/tmp/ldp
CMAKE=/home/ldpe2g/Android/cmake-3.11.0-Linux-x86_64/bin/cmake
```

4. The build script will also generate the disassemble codes which will be saved under `datas/disassemble_files` folder.

