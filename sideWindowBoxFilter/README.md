# Side-window Box Filter

C++ implementation of the [Side Window Filtering(CVPR 2019)](https://github.com/YuanhaoGong/SideWindowFilter).

## Some results
The original box filter does not preserve edges. But side window box filter can preserve edges and corners, as shown in following figure. 

### filter normal image
<div align='center'>
  <img src='results/org_compare.png'>
</div>

### filter image with salt pepper noise
<div align='center'>
  <img src='results/pepper_compare.png'>
</div>

## Build && Run
0. Install `adb` tool with following command

```
sudo apt update
sudo apt install android-tools-adb android-tools-fastboot
```

1. Download [android ndk 15c](https://developer.android.com/ndk/downloads/older_releases.html)

2. Install cmake 3.11

3. Build the project by running the `build_android.sh` script and remember to modify the following parameter.

```
BUILD_ANDROID_NDK_HOME=/home/ldpe2g/Android/android-ndk-r15c
DEPLOY_DIR=/data/local/tmp/ldp
CMAKE=/home/ldpe2g/Android/cmake-3.11.0-Linux-x86_64/bin/cmake
```

4. After successfully building the project, the executable `runUnitTests` will be pushed to the folder `${DEPLOY_DIR}` on your android mobile and run. If every thing goes right, you should see the following outputs on your terminal.


```


```
