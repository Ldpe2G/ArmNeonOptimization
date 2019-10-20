#!/usr/bin/env bash

set -e
set -x

basepath=$(cd `dirname $0`/..; pwd)

RUN_MEDIAN_FILTERS=${basepath}/build_linux/run_median_filters
INPUT_PATH=${basepath}/images/jian20_salt_pepper_noise.jpg
FILTER_RADIUS=3
ITERATION_NUM=1
MEDIAN_FILTER_OUT_PATH_PREFIX=${basepath}/results/median_filter_radius${FILTER_RADIUS}_iter${ITERATION_NUM}

${RUN_MEDIAN_FILTERS} \
        ${INPUT_PATH} \
        ${FILTER_RADIUS} \
        ${ITERATION_NUM} \
        ${MEDIAN_FILTER_OUT_PATH_PREFIX}