# Side-window Median Filter

C++ implementation of the paper [Side Window Filtering(CVPR 2019)](https://github.com/YuanhaoGong/SideWindowFilter) of the median filter. Successfully build on linux and android. No need to install `opencv`, we use the [stb lib](https://github.com/nothings/stb) to read and save images.

## Some results
Side window median filter can preserve edges and corners better than the original median filter, as shown in the following figures. 

### filter normal image
<div align='center'>
  <img src='results/show_1.png'>
</div>

### filter image with salt pepper noise
<div align='center'>
  <img src='results/show_2.png'>
</div>

## Build

### Build linux
0. Install cmake 3.11

1. Build the project by running the `scripts/build_linux.sh` script and remember to modify the following parameter.

```
CMAKE_PATH=cmake
```

2. After successfully building the project, the executable `runUnitTests` will be executed. If every thing goes right, you should see the following outputs on your terminal.

```
[==========] Running 2 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 2 tests from netTest
[ RUN      ] netTest.medianFilterSpeedTest
          [0] MedianFilfer Cost time: 274.514ms
          [1] MedianFilfer Cost time: 268.929ms
          [2] MedianFilfer Cost time: 268.607ms
          [3] MedianFilfer Cost time: 268.613ms
          [4] MedianFilfer Cost time: 268.494ms
          [5] MedianFilfer Cost time: 268.851ms
          [6] MedianFilfer Cost time: 268.598ms
          [7] MedianFilfer Cost time: 268.636ms
          [8] MedianFilfer Cost time: 268.891ms
          [9] MedianFilfer Cost time: 268.796ms

          MedianFilfer Average Cost time: 269.293ms
[       OK ] netTest.medianFilterSpeedTest (2696 ms)
[ RUN      ] netTest.sideWindowMedianFilterSpeedTest
          [0] Side Window MedianFilfer Cost time: 546.448ms
          [1] Side Window MedianFilfer Cost time: 543.731ms
          [2] Side Window MedianFilfer Cost time: 543.195ms
          [3] Side Window MedianFilfer Cost time: 544.231ms
          [4] Side Window MedianFilfer Cost time: 543.111ms
          [5] Side Window MedianFilfer Cost time: 544.559ms
          [6] Side Window MedianFilfer Cost time: 544.929ms
          [7] Side Window MedianFilfer Cost time: 543.785ms
          [8] Side Window MedianFilfer Cost time: 542.317ms
          [9] Side Window MedianFilfer Cost time: 542.43ms

          Side Window MedianFilfer Average Cost time: 543.874ms
[       OK ] netTest.sideWindowMedianFilterSpeedTest (5443 ms)
[----------] 2 tests from netTest (8139 ms total)

[----------] Global test environment tear-down
[==========] 2 tests from 1 test suite ran. (8139 ms total)
[  PASSED  ] 2 tests.
```

### Build android

0. Install `adb` tool with following command

```
sudo apt update
sudo apt install android-tools-adb android-tools-fastboot
```

1. Download [android ndk 15c](https://developer.android.com/ndk/downloads/older_releases.html)

2. Install cmake 3.11

3. Build the project by running the `scripts/build_android.sh` script and remember to modify the following parameter.

```
BUILD_ANDROID_NDK_HOME=/mnt/d/downloads/android-ndk-r15c
DEPLOY_DIR=/data/local/tmp/ldp
CMAKE_PATH=/mnt/d/downloads/cmake-3.11.4/bin/cmake
```

4. After successfully building the project, the executable `runUnitTests` will be pushed to the folder `${DEPLOY_DIR}` on your android mobile and run. If every thing goes right, you should see the similar outputs on your terminal as you build on linux.

```
[==========] Running 2 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 2 tests from netTest
[ RUN      ] netTest.medianFilterSpeedTest
          [0] MedianFilfer Cost time: 232.383ms
          [1] MedianFilfer Cost time: 175.886ms
          [2] MedianFilfer Cost time: 173.125ms
          [3] MedianFilfer Cost time: 172.96ms
          [4] MedianFilfer Cost time: 173.178ms
          [5] MedianFilfer Cost time: 173.366ms
          [6] MedianFilfer Cost time: 173.105ms
          [7] MedianFilfer Cost time: 173.318ms
          [8] MedianFilfer Cost time: 173.126ms
          [9] MedianFilfer Cost time: 172.927ms

          MedianFilfer Average Cost time: 179.337ms
[       OK ] netTest.medianFilterSpeedTest (1805 ms)
[ RUN      ] netTest.sideWindowMedianFilterSpeedTest
          [0] Side Window MedianFilfer Cost time: 404.822ms
          [1] Side Window MedianFilfer Cost time: 404.55ms
          [2] Side Window MedianFilfer Cost time: 404.259ms
          [3] Side Window MedianFilfer Cost time: 404.199ms
          [4] Side Window MedianFilfer Cost time: 403.188ms
          [5] Side Window MedianFilfer Cost time: 404.102ms
          [6] Side Window MedianFilfer Cost time: 404.603ms
          [7] Side Window MedianFilfer Cost time: 403.546ms
          [8] Side Window MedianFilfer Cost time: 402.466ms
          [9] Side Window MedianFilfer Cost time: 403.791ms

          Side Window MedianFilfer Average Cost time: 403.953ms
[       OK ] netTest.sideWindowMedianFilterSpeedTest (4050 ms)
[----------] 2 tests from netTest (5855 ms total)

[----------] Global test environment tear-down
[==========] 2 tests from 1 test suite ran. (5855 ms total)
[  PASSED  ] 2 tests.
```

## Run comparation on Linux

After successfully building the project on linux, you can then use the `scripts/run_compare.sh` to run the comparation with you own images. And remember to modify the following one or more parameters, then have fun :)!

```
INPUT_PATH=${basepath}/images/jian20_small_salt_pepper_noise.jpg
FILTER_RADIUS=3
ITERATION_NUM=1
MEDIAN_FILTER_OUT_PATH=${basepath}/results/median_filter_radius${FILTER_RADIUS}_iter${ITERATION_NUM}.jpg
SIDE_WINDOW_MEDIAN_FILTER_OUT_PATH=${basepath}/results/side_window_median_filter_radius${FILTER_RADIUS}_iter${ITERATION_NUM}.jpg
```


## Run comparation on Android

After successfully building the project on android, you can then use the `scripts/run_compare_android.sh` to run the comparation with you own images on your android device. And remember to modify the following one or more parameters, then have fun :)!

```
FILTER_RADIUS=3
ITERATION_NUM=10

INPUT_NAME=jian20_salt_pepper_noise.jpg

MEDIAN_FILTER_OUT_NAME=median_filter_radius${FILTER_RADIUS}_iter${ITERATION_NUM}.jpg
SIDE_WINDOW_MEDIAN_FILTER_OUT_NAME=side_window_median_filter_radius${FILTER_RADIUS}_iter${ITERATION_NUM}.jpg

INPUT_PATH=${basepath}/images/${INPUT_NAME}
OUTPUT_PATH=${basepath}/results/
```

