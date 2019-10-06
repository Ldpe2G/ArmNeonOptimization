#!/usr/bin/env bash

set -e
set -x

basepath=$(cd `dirname $0`/..; pwd)

COMPARE=${basepath}/build_linux/compare
INPUT_PATH=${basepath}/images/jian20_small_salt_pepper_noise.jpg
FILTER_RADIUS=3
ITERATION_NUM=1
MEDIAN_FILTER_OUT_PATH=${basepath}/results/median_filter_radius${FILTER_RADIUS}_iter${ITERATION_NUM}.jpg
SIDE_WINDOW_MEDIAN_FILTER_OUT_PATH=${basepath}/results/side_window_median_filter_radius${FILTER_RADIUS}_iter${ITERATION_NUM}.jpg

${COMPARE} \
    ${INPUT_PATH} \
    ${FILTER_RADIUS} \
    ${ITERATION_NUM} \
    ${MEDIAN_FILTER_OUT_PATH}  \
    ${SIDE_WINDOW_MEDIAN_FILTER_OUT_PATH}
