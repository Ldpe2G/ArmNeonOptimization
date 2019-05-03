include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(SRCS
    boxFilter.cpp
)

add_library(boxfilter SHARED ${SRCS})