#!/usr/bin/env bash

set -e
set -x

basepath=$(cd `dirname $0`/; pwd)

BUILD_DIR=${basepath}/build

BUILD_ANDROID_NDK_HOME=/home/ldpe2g/Android/android-ndk-r15c
DEPLOY_DIR=/data/local/tmp/ldp
CMAKE=/home/ldpe2g/Android/cmake-3.11.0-Linux-x86_64/bin/cmake

rm -rf ${BUILD_DIR}
if [[ ! -d ${BUILD_DIR} ]]; then
    mkdir -p ${BUILD_DIR}
fi

cd ${BUILD_DIR}
$CMAKE \
-DCMAKE_TOOLCHAIN_FILE=${BUILD_ANDROID_NDK_HOME}/build/cmake/android.toolchain.cmake \
-DANDROID_NDK=${BUILD_ANDROID_NDK_HOME} \
-DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
-DANDROID_ABI=armeabi-v7a with NEON \
-DANDROID_NATIVE_API_LEVEL=9 \
-DANDROID_TOOLCHAIN=gcc \
-DANDROID_ARM_NEON=TRUE \
../

make all -j4

adb shell "mkdir -p ${DEPLOY_DIR}/lib"
adb push ${basepath}/build/unit_test/runUnitTests ${DEPLOY_DIR}
adb push ${basepath}/build/src/libboxfilter.so ${DEPLOY_DIR}/lib

adb shell "cd ${DEPLOY_DIR}; export LD_LIBRARY_PATH=./lib ; ./runUnitTests"
