#!/usr/bin/env bash

set -e
set -x

basepath=$(cd `dirname $0`/..; pwd)

BUILD_DIR=${basepath}/build_android
DEPLOY_DIR=/data/local/tmp/ldp

FILTER_RADIUS=3
ITERATION_NUM=1

INPUT_NAME=jian20_salt_pepper_noise.jpg

MEDIAN_FILTER_OUT_NAME=android_median_filter_radius${FILTER_RADIUS}_iter${ITERATION_NUM}

INPUT_PATH=${basepath}/images/${INPUT_NAME}
OUTPUT_PATH=${basepath}/results/

RUN_MEDIAN_FILTERS=run_median_filters

adb push ${INPUT_PATH} ${DEPLOY_DIR}

adb shell "cd ${DEPLOY_DIR}; export LD_LIBRARY_PATH=./lib ; ./${RUN_MEDIAN_FILTERS} ${INPUT_NAME} ${FILTER_RADIUS} ${ITERATION_NUM} ${MEDIAN_FILTER_OUT_NAME}"

adb pull ${DEPLOY_DIR}/${MEDIAN_FILTER_OUT_NAME}_normal_float_median_filter.jpg ${OUTPUT_PATH}
adb pull ${DEPLOY_DIR}/${MEDIAN_FILTER_OUT_NAME}_normal_uint16_median_filter.jpg ${OUTPUT_PATH}
adb pull ${DEPLOY_DIR}/${MEDIAN_FILTER_OUT_NAME}_const_time_uint16_median_filter.jpg ${OUTPUT_PATH}