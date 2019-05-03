# Arm Neon Optimization Of Box Filter

Showing how to optimize the box filter operation on arm divices step by step.

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

4. After successfully building the project, the executable `runUnitTests` will be pushed to your android mobile and run. If every thing goes right, you should see the following outputs on your terminal.


```
[==========] Running 6 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 6 tests from netTest
[ RUN      ] netTest.org_boxfilter
          [0] BoxFilfer Cost time: 1679.91ms
          [1] BoxFilfer Cost time: 1669.2ms
          [2] BoxFilfer Cost time: 1668.06ms
          [3] BoxFilfer Cost time: 1669.61ms
          [4] BoxFilfer Cost time: 1667.7ms
          [5] BoxFilfer Cost time: 1667.84ms
          [6] BoxFilfer Cost time: 1677.63ms
          [7] BoxFilfer Cost time: 1670.87ms
          [8] BoxFilfer Cost time: 1667.56ms
          [9] BoxFilfer Cost time: 1671.46ms

          BoxFilfer Average Cost time: 1670.99ms
[       OK ] netTest.org_boxfilter (17257 ms)
[ RUN      ] netTest.fast_boxfilter
          [0] Fast BoxFilfer Cost time: 341.669ms
          [1] Fast BoxFilfer Cost time: 341.137ms
          [2] Fast BoxFilfer Cost time: 341.387ms
          [3] Fast BoxFilfer Cost time: 340.85ms
          [4] Fast BoxFilfer Cost time: 341.06ms
          [5] Fast BoxFilfer Cost time: 341.151ms
          [6] Fast BoxFilfer Cost time: 341.605ms
          [7] Fast BoxFilfer Cost time: 340.94ms
          [8] Fast BoxFilfer Cost time: 341.17ms
          [9] Fast BoxFilfer Cost time: 340.895ms

          Fast BoxFilfer Average Cost time: 341.186ms
[       OK ] netTest.fast_boxfilter (3958 ms)
[ RUN      ] netTest.fast_boxfilter_v2
          [0] Fast BoxFilfer V2 Cost time: 75.034ms
          [1] Fast BoxFilfer V2 Cost time: 75.52ms
          [2] Fast BoxFilfer V2 Cost time: 75.206ms
          [3] Fast BoxFilfer V2 Cost time: 75.028ms
          [4] Fast BoxFilfer V2 Cost time: 75.324ms
          [5] Fast BoxFilfer V2 Cost time: 74.995ms
          [6] Fast BoxFilfer V2 Cost time: 75.311ms
          [7] Fast BoxFilfer V2 Cost time: 75.237ms
          [8] Fast BoxFilfer V2 Cost time: 75.218ms
          [9] Fast BoxFilfer V2 Cost time: 75.44ms

          Fast BoxFilfer V2 Average Cost time: 75.2313ms
[       OK ] netTest.fast_boxfilter_v2 (1308 ms)
[ RUN      ] netTest.fast_boxfilter_v2_neon_intrinsics
          [0] Fast BoxFilfer V2 Neon Intrinsics Cost time: 72.076ms
          [1] Fast BoxFilfer V2 Neon Intrinsics Cost time: 72.52ms
          [2] Fast BoxFilfer V2 Neon Intrinsics Cost time: 72.959ms
          [3] Fast BoxFilfer V2 Neon Intrinsics Cost time: 72.4ms
          [4] Fast BoxFilfer V2 Neon Intrinsics Cost time: 72.465ms
          [5] Fast BoxFilfer V2 Neon Intrinsics Cost time: 72.367ms
          [6] Fast BoxFilfer V2 Neon Intrinsics Cost time: 72.753ms
          [7] Fast BoxFilfer V2 Neon Intrinsics Cost time: 72.778ms
          [8] Fast BoxFilfer V2 Neon Intrinsics Cost time: 72.297ms
          [9] Fast BoxFilfer V2 Neon Intrinsics Cost time: 72.758ms

          Fast BoxFilfer V2 Neon Intrinsics Average Cost time: 72.5373ms
[       OK ] netTest.fast_boxfilter_v2_neon_intrinsics (1272 ms)
[ RUN      ] netTest.fast_boxfilter_v2_neon_asm
          [0] Fast BoxFilfer V2 Neon Asm Cost time: 73.163ms
          [1] Fast BoxFilfer V2 Neon Asm Cost time: 72.803ms
          [2] Fast BoxFilfer V2 Neon Asm Cost time: 72.829ms
          [3] Fast BoxFilfer V2 Neon Asm Cost time: 73.206ms
          [4] Fast BoxFilfer V2 Neon Asm Cost time: 72.73ms
          [5] Fast BoxFilfer V2 Neon Asm Cost time: 73.08ms
          [6] Fast BoxFilfer V2 Neon Asm Cost time: 72.65ms
          [7] Fast BoxFilfer V2 Neon Asm Cost time: 72.883ms
          [8] Fast BoxFilfer V2 Neon Asm Cost time: 72.85ms
          [9] Fast BoxFilfer V2 Neon Asm Cost time: 72.598ms

          Fast BoxFilfer V2 Neon Asm Average Cost time: 72.8792ms
[       OK ] netTest.fast_boxfilter_v2_neon_asm (1275 ms)
[ RUN      ] netTest.fast_boxfilter_v2_neon_asm_v2
          [0] Fast BoxFilfer V2 Neon AsmV2 Cost time: 62.454ms
          [1] Fast BoxFilfer V2 Neon AsmV2 Cost time: 63.037ms
          [2] Fast BoxFilfer V2 Neon AsmV2 Cost time: 62.932ms
          [3] Fast BoxFilfer V2 Neon AsmV2 Cost time: 63.445ms
          [4] Fast BoxFilfer V2 Neon AsmV2 Cost time: 62.876ms
          [5] Fast BoxFilfer V2 Neon AsmV2 Cost time: 63.137ms
          [6] Fast BoxFilfer V2 Neon AsmV2 Cost time: 63.33ms
          [7] Fast BoxFilfer V2 Neon AsmV2 Cost time: 63.589ms
          [8] Fast BoxFilfer V2 Neon AsmV2 Cost time: 63.07ms
          [9] Fast BoxFilfer V2 Neon AsmV2 Cost time: 63.237ms

          Fast BoxFilfer V2 Neon AsmV2 Average Cost time: 63.1107ms
[       OK ] netTest.fast_boxfilter_v2_neon_asm_v2 (1177 ms)
[----------] 6 tests from netTest (26247 ms total)

[----------] Global test environment tear-down
[==========] 6 tests from 1 test suite ran. (26248 ms total)
[  PASSED  ] 6 tests.

```
