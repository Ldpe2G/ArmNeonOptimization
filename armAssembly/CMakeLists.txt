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
project(getCPUid)

set(CMAKE_CXX_STANDARD 11)

# generate compile_commands.json for function jumps
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#include_directories(${PROJECT_SOURCE_DIR}/src)

# Only usefull in gcc
if(CMAKE_C_COMPILER_ID MATCHES "GNU")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")
endif ()

add_definitions(-fPIC)
add_definitions(-Ofast)
add_definitions(-ffast-math)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIE")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fPIE -pie")

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imageHelper )

add_executable(assemblyEx1ArrWeightSum
               assemblyEx1ArrWeightSum.cpp)

add_executable(assemblyEx2Rgb2Gray
               assemblyEx2Rgb2Gray.cpp)



