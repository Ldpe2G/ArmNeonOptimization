include_directories("${gtest_SOURCE_DIR}/include")
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../src )

add_executable(runUnitTests
                gtest.cpp
                test_side_window_box_filter.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/../src/box_filter.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/../src/side_window_box_filter.cpp)

target_link_libraries(runUnitTests gtest boxfilter)

