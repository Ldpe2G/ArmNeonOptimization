#!/usr/bin/env bash

set -e
# set -x

basepath=$(cd `dirname $0`/..; pwd)

BUILD_DIR=${basepath}/build_android

BUILD_ANDROID_NDK_HOME=/mnt/d/downloads/android-ndk-r16b
DEPLOY_DIR=/data/local/tmp/assembly
CMAKE_PATH=/mnt/d/downloads/cmake-3.11.4/bin/cmake

# ABI="armeabi-v7a"
ABI="arm64-v8a"
ANDROID_API_LEVEL=android-21
ANDROID_TOOLCHAIN=clang

rm -rf ${BUILD_DIR}
if [[ ! -d ${BUILD_DIR} ]]; then
    mkdir -p ${BUILD_DIR}
fi

cd ${BUILD_DIR}
${CMAKE_PATH} \
-DCMAKE_TOOLCHAIN_FILE=${BUILD_ANDROID_NDK_HOME}/build/cmake/android.toolchain.cmake \
-DANDROID_NDK=${BUILD_ANDROID_NDK_HOME} \
-DANDROID_ABI=${ABI} \
-DANDROID_ARM_NEON=ON \
-DANDROID_NATIVE_API_LEVEL=${ANDROID_API_LEVEL} \
-DANDROID_TOOLCHAIN=${ANDROID_TOOLCHAIN} \
../

make all -j4

##### geenrate disassembly files
DISASSEMBLY_FILES_PATH=${basepath}/datas/disassemble_files
${BUILD_ANDROID_NDK_HOME}/toolchains/aarch64-linux-android-4.9/prebuilt/linux-x86_64/bin/aarch64-linux-android-objdump \
    -d  ${BUILD_DIR}/CMakeFiles/assemblyEx1ArrWeightSum.dir/assemblyEx1ArrWeightSum.cpp.o \
    > ${DISASSEMBLY_FILES_PATH}/assemblyEx1ArrWeightSum_${ABI}.txt

${BUILD_ANDROID_NDK_HOME}/toolchains/aarch64-linux-android-4.9/prebuilt/linux-x86_64/bin/aarch64-linux-android-objdump \
    -d  ${BUILD_DIR}/CMakeFiles/assemblyEx2Rgb2Gray.dir/assemblyEx2Rgb2Gray.cpp.o \
    > ${DISASSEMBLY_FILES_PATH}/assemblyEx2Rgb2Gray_${ABI}.txt

adb shell "mkdir -p ${DEPLOY_DIR}"

##### run arr weight sum experiments
adb push ${BUILD_DIR}/assemblyEx1ArrWeightSum ${DEPLOY_DIR}
adb shell "cd ${DEPLOY_DIR}; ./assemblyEx1ArrWeightSum"

##### run rgb to gray experiments
INPUT_NAME=dragonball.jpg
OUTPUT_NAME=gray_${INPUT_NAME}
INPUT_PATH=${basepath}/datas/images/${INPUT_NAME}
OUTPUT_PATH=${basepath}/datas/results/

adb push ${INPUT_PATH} ${DEPLOY_DIR}
adb push ${BUILD_DIR}/assemblyEx2Rgb2Gray ${DEPLOY_DIR}
adb shell "cd ${DEPLOY_DIR}; ./assemblyEx2Rgb2Gray ${INPUT_NAME} ${OUTPUT_NAME}"
adb pull ${DEPLOY_DIR}/${OUTPUT_NAME} ${OUTPUT_PATH}



