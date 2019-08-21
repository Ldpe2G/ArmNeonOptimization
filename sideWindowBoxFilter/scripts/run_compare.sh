#!/usr/bin/env bash

set -e
set -x

basepath=$(cd `dirname $0`/..; pwd)

COMPARE=${basepath}/build_linux/compare
INPUT_PATH=${basepath}/images/dragonball_small.jpg
FILTER_RADIUS=3
ITERATION_NUM=30
BOX_FILTER_OUT_PATH=${basepath}/results/box_filter_radius${FILTER_RADIUS}_iter${ITERATION_NUM}.jpg
SIDE_WINDOW_BOX_FILTER_OUT_PATH=${basepath}/results/side_window_box_filter_radius${FILTER_RADIUS}_iter${ITERATION_NUM}.jpg


${COMPARE} \
    ${INPUT_PATH} \
    ${FILTER_RADIUS} \
    ${ITERATION_NUM} \
    ${BOX_FILTER_OUT_PATH}  \
    ${SIDE_WINDOW_BOX_FILTER_OUT_PATH}
