cmake_minimum_required(VERSION 3.10)

########## Generate test executable ##########################################

# search for test source files (have to start with prefix test_)
file(GLOB_RECURSE TEST_FILES test_*.cpp)
add_executable(
	${$TARGET_NAME$}$_tests
	main.cpp
	${TEST_FILES}
)

# include "libs" directory for #incldue <catch2/catch2>
target_include_directories(${$TARGET_NAME$}$_tests PRIVATE libs)

find_package(Threads REQUIRED)
target_link_libraries(
	${$TARGET_NAME$}$_tests
PRIVATE
	Threads::Threads
	${$CMAKE_TARGET_LINK_NAME$}$
)


## Make ctest run build.
# idea taken from https://stackoverflow.com/questions/733475/cmake-ctest-make-test-doesnt-build-tests
# TODO: DOES NOT WORK with MSVC open folder (${CMAKE_COMMAND} seems to be the problem, but a plain "cmake" doesn't pass the correct incldue directories)

ADD_TEST(NAME build_${$TARGET_NAME$}$_tests COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target ${$TARGET_NAME$}$_tests)

set(PARSE_CATCH_TESTS_ADD_TO_CONFIGURE_DEPENDS ON)
set(PARSE_CATCH_TESTS_ADD_TARGET_IN_TEST_NAME OFF)

include(libs/catch2/ParseAndAddCatchTests.cmake)

ParseAndAddCatchTests(${$TARGET_NAME$}$_tests)

