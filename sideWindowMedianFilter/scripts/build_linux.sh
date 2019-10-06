#!/usr/bin/env bash

set -e
set -x

basepath=$(cd `dirname $0`/..; pwd)

BUILD_DIR=${basepath}/build_linux

CMAKE_PATH=cmake

rm -rf ${BUILD_DIR}
if [[ ! -d ${BUILD_DIR} ]]; then
    mkdir -p ${BUILD_DIR}
fi

cd ${BUILD_DIR}
${CMAKE_PATH} ../

make -j4

${BUILD_DIR}/unit_test/runUnitTests
