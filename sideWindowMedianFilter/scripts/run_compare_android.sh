#!/usr/bin/env bash

set -e
set -x

basepath=$(cd `dirname $0`/..; pwd)

BUILD_DIR=${basepath}/build_android
DEPLOY_DIR=/data/local/tmp/ldp

FILTER_RADIUS=3
ITERATION_NUM=10

INPUT_NAME=jian20_salt_pepper_noise.jpg

MEDIAN_FILTER_OUT_NAME=median_filter_radius${FILTER_RADIUS}_iter${ITERATION_NUM}.jpg
SIDE_WINDOW_MEDIAN_FILTER_OUT_NAME=side_window_median_filter_radius${FILTER_RADIUS}_iter${ITERATION_NUM}.jpg

INPUT_PATH=${basepath}/images/${INPUT_NAME}
OUTPUT_PATH=${basepath}/results/

adb push ${INPUT_PATH} ${DEPLOY_DIR}

adb shell "cd ${DEPLOY_DIR}; export LD_LIBRARY_PATH=./lib ; ./compare ${INPUT_NAME} ${FILTER_RADIUS} ${ITERATION_NUM} ${MEDIAN_FILTER_OUT_NAME} ${SIDE_WINDOW_MEDIAN_FILTER_OUT_NAME}"

adb pull ${DEPLOY_DIR}/${MEDIAN_FILTER_OUT_NAME} ${OUTPUT_PATH}
adb pull ${DEPLOY_DIR}/${SIDE_WINDOW_MEDIAN_FILTER_OUT_NAME} ${OUTPUT_PATH}
