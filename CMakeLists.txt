cmake_minimum_required(VERSION 3.14)
project(BGPSimulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(SOURCE_FILES
	src/ASGraph.cpp
)

add_executable(bgp_sim src/main.cpp ${SOURCE_FILES})

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(run_tests
	tests/GraphTests.cpp
	${SOURCE_FILES}
)

#previously target_link_libraries(run_tests ${GTEST_LIBRARIES} pthread)
target_link_libraries(run_tests GTest::gtest GTest::gtest_main pthread)

enable_testing()
add_test(NAME GraphTests COMMAND run_tests)
