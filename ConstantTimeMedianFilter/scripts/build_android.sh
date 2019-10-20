#!/usr/bin/env bash

set -e
set -x

basepath=$(cd `dirname $0`/..; pwd)

BUILD_DIR=${basepath}/build_android

BUILD_ANDROID_NDK_HOME=/home/ldpe2g/Android/android-ndk-r15c
DEPLOY_DIR=/data/local/tmp/ldp
CMAKE_PATH=cmake

rm -rf ${BUILD_DIR}
if [[ ! -d ${BUILD_DIR} ]]; then
    mkdir -p ${BUILD_DIR}
fi

USE_NEON=OFF

cd ${BUILD_DIR}
${CMAKE_PATH} \
-DCMAKE_TOOLCHAIN_FILE=${BUILD_ANDROID_NDK_HOME}/build/cmake/android.toolchain.cmake \
-DANDROID_NDK=${BUILD_ANDROID_NDK_HOME} \
-DANDROID_ABI=armeabi-v7a with NEON \
-DANDROID_NATIVE_API_LEVEL=9 \
-DANDROID_TOOLCHAIN=gcc \
-DANDROID_ARM_NEON=TRUE \
-DUSE_NEON=${USE_NEON} \
../

make all -j4

adb shell "mkdir -p ${DEPLOY_DIR}/lib"
adb push ${BUILD_DIR}/unit_test/runUnitTests ${DEPLOY_DIR}
adb push ${BUILD_DIR}/run_median_filters ${DEPLOY_DIR}
adb push ${BUILD_DIR}/src/libmedianFilter.so ${DEPLOY_DIR}/lib

adb shell "cd ${DEPLOY_DIR}; export LD_LIBRARY_PATH=./lib ; ./runUnitTests"
