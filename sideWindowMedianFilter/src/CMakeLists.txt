include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(SRCS
    median_filter.cpp
    side_window_median_filter.cpp
)

add_library(medianFilter SHARED ${SRCS})