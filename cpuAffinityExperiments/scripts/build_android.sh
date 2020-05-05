#!/usr/bin/env bash

set -e
# set -x

basepath=$(cd `dirname $0`/..; pwd)

BUILD_DIR=${basepath}/build_android

BUILD_ANDROID_NDK_HOME=/mnt/d/downloads/android-ndk-r15c
DEPLOY_DIR=/data/local/tmp/ldp
CMAKE_PATH=/mnt/d/downloads/cmake-3.11.4/bin/cmake

rm -rf ${BUILD_DIR}
if [[ ! -d ${BUILD_DIR} ]]; then
    mkdir -p ${BUILD_DIR}
fi

cd ${BUILD_DIR}
${CMAKE_PATH} \
-DCMAKE_TOOLCHAIN_FILE=${BUILD_ANDROID_NDK_HOME}/build/cmake/android.toolchain.cmake \
-DANDROID_NDK=${BUILD_ANDROID_NDK_HOME} \
-DANDROID_ABI=armeabi-v7a with NEON \
-DANDROID_NATIVE_API_LEVEL=9 \
-DANDROID_TOOLCHAIN=gcc \
-DANDROID_ARM_NEON=TRUE \
../

make all -j4

adb shell "mkdir -p ${DEPLOY_DIR}/lib"
adb push ${BUILD_DIR}/getCPUid ${DEPLOY_DIR}

for i in {1..10};
do
    adb shell "cd ${DEPLOY_DIR}; ./getCPUid"
done
