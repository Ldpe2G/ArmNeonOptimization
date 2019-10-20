# Median Filter In Constant Time

C++ implementation of the paper [Median Filter In Constant Time](https://nomis80.org/ctmf.pdf). Successfully build on linux and android(tested on HUAWEI P30, HiSilicon KIRIN 980). 

## Build

### Build on linux
0. Install cmake 3.11

1. Build the project by running the `scripts/build_linux.sh` script and remember to modify the following parameter.

```
CMAKE_PATH=cmake
```

2. After successfully building the project, the executable `runUnitTests` will be executed. If every thing goes right, you should see the following outputs on your terminal.

```
[==========] Running 15 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 15 tests from netTest
[ RUN      ] netTest.medianFilterFloatRaduis1SpeedTest

#################Normal Float MedianFilfer Radius 1 Average Cost time: 11.6482ms

[       OK ] netTest.medianFilterFloatRaduis1SpeedTest (119 ms)
[ RUN      ] netTest.medianFilterUint16Raduis1SpeedTest

#################Normal Uint16 MedianFilfer Radius 1 Average Cost time: 10.5113ms

[       OK ] netTest.medianFilterUint16Raduis1SpeedTest (107 ms)
[ RUN      ] netTest.constantTimeMedianFilterUint16Raduis1SpeedTest

#################Constant time Uint16 MedianFilfer Radius 1 Average Cost time: 33.889ms

[       OK ] netTest.constantTimeMedianFilterUint16Raduis1SpeedTest (341 ms)
[ RUN      ] netTest.medianFilterFloatRaduis2SpeedTest

#################Normal Float MedianFilfer Radius 2 Average Cost time: 78.2531ms

[       OK ] netTest.medianFilterFloatRaduis2SpeedTest (784 ms)
[ RUN      ] netTest.medianFilterUint16Raduis2SpeedTest

#################Normal Uint16 MedianFilfer Radius 2 Average Cost time: 63.8422ms

[       OK ] netTest.medianFilterUint16Raduis2SpeedTest (641 ms)
[ RUN      ] netTest.constantTimeMedianFilterUint16Raduis2SpeedTest

#################Constant Time Uint16 MedianFilfer Radius 2 Average Cost time: 36.4003ms

[       OK ] netTest.constantTimeMedianFilterUint16Raduis2SpeedTest (365 ms)
[ RUN      ] netTest.medianFilterFloatRaduis3SpeedTest

#################Normal Float MedianFilfer Radius 3 Average Cost time: 265.695ms

[       OK ] netTest.medianFilterFloatRaduis3SpeedTest (2660 ms)
[ RUN      ] netTest.medianFilterUint16Raduis3SpeedTest

#################Normal Uint16 MedianFilfer Radius 3 Average Cost time: 213.539ms

[       OK ] netTest.medianFilterUint16Raduis3SpeedTest (2137 ms)
[ RUN      ] netTest.constantTimeMedianFilterUint16Raduis3SpeedTest

#################Constant time Uint16 MedianFilfer Radius 3 Average Cost time: 36.9943ms

[       OK ] netTest.constantTimeMedianFilterUint16Raduis3SpeedTest (371 ms)
[ RUN      ] netTest.medianFilterFloatRaduis4SpeedTest

#################Normal Float MedianFilfer Radius 4 Average Cost time: 683.465ms

[       OK ] netTest.medianFilterFloatRaduis4SpeedTest (6837 ms)
[ RUN      ] netTest.medianFilterUint16Raduis4SpeedTest

#################Normal Uint16 MedianFilfer Radius 4 Average Cost time: 545.849ms

[       OK ] netTest.medianFilterUint16Raduis4SpeedTest (5461 ms)
[ RUN      ] netTest.constantTimeMedianFilterUint16Raduis4SpeedTest

#################Constant time Uint16 MedianFilfer Radius 4 Average Cost time: 37.2009ms

[       OK ] netTest.constantTimeMedianFilterUint16Raduis4SpeedTest (373 ms)
[ RUN      ] netTest.medianFilterFloatRaduis5SpeedTest

#################Normal Float MedianFilfer Radius 5 Average Cost time: 1470.26ms

[       OK ] netTest.medianFilterFloatRaduis5SpeedTest (14705 ms)
[ RUN      ] netTest.medianFilterUint16Raduis5SpeedTest

#################Normal Uint16 MedianFilfer Radius 5 Average Cost time: 1179.28ms

[       OK ] netTest.medianFilterUint16Raduis5SpeedTest (11795 ms)
[ RUN      ] netTest.constantTimeMedianFilterUint16Raduis5SpeedTest

#################Constant time Uint16 MedianFilfer Radius 5 Average Cost time: 37.2977ms

[       OK ] netTest.constantTimeMedianFilterUint16Raduis5SpeedTest (375 ms)
[----------] 15 tests from netTest (47071 ms total)

[----------] Global test environment tear-down
[==========] 15 tests from 1 test suite ran. (47071 ms total)
[  PASSED  ] 15 tests.

```

### Build on android

0. Install `adb` tool with following command

```
sudo apt update
sudo apt install android-tools-adb android-tools-fastboot
```

1. Download [android ndk 15c](https://developer.android.com/ndk/downloads/older_releases.html)

2. Install cmake 3.11

3. Build the project by running the `scripts/build_android.sh` script and remember to modify the following parameter.

```
BUILD_ANDROID_NDK_HOME=/home/ldpe2g/Android/android-ndk-r15c
DEPLOY_DIR=/data/local/tmp/ldp
CMAKE_PATH=cmake
```

4. You can modify the `USE_NEON=ON` flag to enable or disable neon support. But you should see a huge performance degration on normal median filter when disable neon.

5. After successfully building the project, the executable `runUnitTests` will be pushed to the folder `${DEPLOY_DIR}` on your android mobile and run. If every thing goes right, you should see the similar outputs on your terminal as you build on linux.

```
[==========] Running 15 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 15 tests from netTest
[ RUN      ] netTest.medianFilterFloatRaduis1SpeedTest

#################Normal Float MedianFilfer Radius 1 Average Cost time: 17.029ms

[       OK ] netTest.medianFilterFloatRaduis1SpeedTest (182 ms)
[ RUN      ] netTest.medianFilterUint16Raduis1SpeedTest

#################Normal Uint16 MedianFilfer Radius 1 Average Cost time: 7.6964ms

[       OK ] netTest.medianFilterUint16Raduis1SpeedTest (81 ms)
[ RUN      ] netTest.constantTimeMedianFilterUint16Raduis1SpeedTest

#################Constant time Uint16 MedianFilfer Radius 1 Average Cost time: 69.9052ms

[       OK ] netTest.constantTimeMedianFilterUint16Raduis1SpeedTest (703 ms)
[ RUN      ] netTest.medianFilterFloatRaduis2SpeedTest

#################Normal Float MedianFilfer Radius 2 Average Cost time: 57.2293ms

[       OK ] netTest.medianFilterFloatRaduis2SpeedTest (576 ms)
[ RUN      ] netTest.medianFilterUint16Raduis2SpeedTest

#################Normal Uint16 MedianFilfer Radius 2 Average Cost time: 25.823ms

[       OK ] netTest.medianFilterUint16Raduis2SpeedTest (261 ms)
[ RUN      ] netTest.constantTimeMedianFilterUint16Raduis2SpeedTest

#################Constant Time Uint16 MedianFilfer Radius 2 Average Cost time: 74.136ms

[       OK ] netTest.constantTimeMedianFilterUint16Raduis2SpeedTest (745 ms)
[ RUN      ] netTest.medianFilterFloatRaduis3SpeedTest

#################Normal Float MedianFilfer Radius 3 Average Cost time: 173.699ms

[       OK ] netTest.medianFilterFloatRaduis3SpeedTest (1742 ms)
[ RUN      ] netTest.medianFilterUint16Raduis3SpeedTest

#################Normal Uint16 MedianFilfer Radius 3 Average Cost time: 72.2515ms

[       OK ] netTest.medianFilterUint16Raduis3SpeedTest (726 ms)
[ RUN      ] netTest.constantTimeMedianFilterUint16Raduis3SpeedTest

#################Constant time Uint16 MedianFilfer Radius 3 Average Cost time: 75.1661ms

[       OK ] netTest.constantTimeMedianFilterUint16Raduis3SpeedTest (755 ms)
[ RUN      ] netTest.medianFilterFloatRaduis4SpeedTest

#################Normal Float MedianFilfer Radius 4 Average Cost time: 420.3ms

[       OK ] netTest.medianFilterFloatRaduis4SpeedTest (4208 ms)
[ RUN      ] netTest.medianFilterUint16Raduis4SpeedTest

#################Normal Uint16 MedianFilfer Radius 4 Average Cost time: 170.781ms

[       OK ] netTest.medianFilterUint16Raduis4SpeedTest (1711 ms)
[ RUN      ] netTest.constantTimeMedianFilterUint16Raduis4SpeedTest

#################Constant time Uint16 MedianFilfer Radius 4 Average Cost time: 76.0269ms

[       OK ] netTest.constantTimeMedianFilterUint16Raduis4SpeedTest (764 ms)
[ RUN      ] netTest.medianFilterFloatRaduis5SpeedTest

#################Normal Float MedianFilfer Radius 5 Average Cost time: 915.703ms

[       OK ] netTest.medianFilterFloatRaduis5SpeedTest (9162 ms)
[ RUN      ] netTest.medianFilterUint16Raduis5SpeedTest

#################Normal Uint16 MedianFilfer Radius 5 Average Cost time: 368.779ms

[       OK ] netTest.medianFilterUint16Raduis5SpeedTest (3691 ms)
[ RUN      ] netTest.constantTimeMedianFilterUint16Raduis5SpeedTest

#################Constant time Uint16 MedianFilfer Radius 5 Average Cost time: 75.8295ms

[       OK ] netTest.constantTimeMedianFilterUint16Raduis5SpeedTest (762 ms)
[----------] 15 tests from netTest (26070 ms total)

[----------] Global test environment tear-down
[==========] 15 tests from 1 test suite ran. (26070 ms total)
[  PASSED  ] 15 tests.

```

## Run median filter on Linux

After successfully building the project on linux, you can then use the `scripts/run_filters.sh` to run the median filter with you own images. And remember to modify the following one or more parameters, then have fun :)!

```
INPUT_PATH=${basepath}/images/jian20_salt_pepper_noise.jpg
FILTER_RADIUS=3
ITERATION_NUM=1
MEDIAN_FILTER_OUT_PATH_PREFIX=${basepath}/results/median_filter_radius${FILTER_RADIUS}_iter${ITERATION_NUM}
```


## Run median filter on Android

After successfully building the project on android, you can then use the `scripts/run_filters_android.sh` to run the median filter with you own images on your android device. And remember to modify the following one or more parameters, then have fun :)!

```
FILTER_RADIUS=3
ITERATION_NUM=1

INPUT_NAME=jian20_salt_pepper_noise.jpg

MEDIAN_FILTER_OUT_NAME=android_median_filter_radius${FILTER_RADIUS}_iter${ITERATION_NUM}

INPUT_PATH=${basepath}/images/${INPUT_NAME}
OUTPUT_PATH=${basepath}/results/
```

