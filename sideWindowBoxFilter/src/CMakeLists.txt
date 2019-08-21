include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(SRCS
    box_filter.cpp
    side_window_box_filter.cpp
)

add_library(boxfilter SHARED ${SRCS})