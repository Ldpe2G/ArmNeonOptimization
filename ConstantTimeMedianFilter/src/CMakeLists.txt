include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(SRCS
    normal_median_filter_float.cpp
    normal_median_filter_uint16.cpp
    constant_time_median_filter_uint16.cpp
)

add_library(medianFilter SHARED ${SRCS})
