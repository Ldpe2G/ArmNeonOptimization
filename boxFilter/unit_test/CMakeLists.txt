include_directories("${gtest_SOURCE_DIR}/include")
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../src )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../third_party )

add_executable(runUnitTests
        gtest.cpp
        testBoxFilter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../src/boxFilter.cpp)

target_link_libraries(runUnitTests gtest boxfilter)

