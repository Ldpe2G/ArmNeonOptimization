if (CMAKE_TOOLCHAIN_FILE)
    set(LIBRARY_OUTPUT_PATH_ROOT ${CMAKE_BINARY_DIR} CACHE PATH "root for library output, set this to change where android libs are compiled to")
    # get absolute path, but get_filename_component ABSOLUTE only refer with source dir, so find_file here :(
    get_filename_component(CMAKE_TOOLCHAIN_FILE_NAME ${CMAKE_TOOLCHAIN_FILE} NAME)
    find_file(CMAKE_TOOLCHAIN_FILE ${CMAKE_TOOLCHAIN_FILE_NAME} PATHS ${CMAKE_SOURCE_DIR} NO_DEFAULT_PATH)
    message(STATUS "CMAKE_TOOLCHAIN_FILE = ${CMAKE_TOOLCHAIN_FILE}")
elseif (ANDROID)
    message(FATAL_ERROR "Please specify a toolchain file.")
endif ()

cmake_minimum_required(VERSION 3.5)
project(sideWindowBoxFilter)

set(CMAKE_CXX_STANDARD 11)

option(USE_NEON "Whether to enable neon acceleration." ON)

if(USE_NEON)
  message(STATUS "use neon")
  add_definitions(-DUSE_NEON_INTRINSIC)
endif()

include_directories(${PROJECT_SOURCE_DIR}/src)

# Only usefull in gcc
if(CMAKE_C_COMPILER_ID MATCHES "GNU")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")
endif ()

add_definitions(-fPIC)
add_definitions(-Ofast)
add_definitions(-ffast-math)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Android 5.0 以上需要在此处设置 PIE
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIE")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fPIE -pie")

add_subdirectory(src)
add_subdirectory(third_party/googletest/googletest)
add_subdirectory(unit_test)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/src )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imageHelper )

add_executable(run_median_filters
               run_median_filters.cpp)

target_link_libraries(run_median_filters gtest medianFilter)
