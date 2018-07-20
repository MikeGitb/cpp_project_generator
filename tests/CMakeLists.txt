cmake_minimum_required(VERSION 3.10)

########## Generate test executable ##########################################

# search for test source files (have to start with prefix test_)
file(GLOB_RECURSE TEST_FILES test_*.cpp)
add_executable(
	cpp_project_generator_tests
	main.cpp
	${TEST_FILES}
)

# include "libs" directory for #incldue <catch2/catch2>
target_include_directories(cpp_project_generator_tests PRIVATE libs)

find_package(Threads REQUIRED)
target_link_libraries(
	cpp_project_generator_tests
PRIVATE
	Threads::Threads
	MBa::cpp_project_generator_lib
)


## Make ctest run build.
# idea taken from https://stackoverflow.com/questions/733475/cmake-ctest-make-test-doesnt-build-tests
# TODO: DOES NOT WORK with MSVC open folder (${CMAKE_COMMAND} seems to be the problem, but a plain "cmake" doesn't pass the correct incldue directories)

ADD_TEST(NAME build_cpp_project_generator_tests COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target cpp_project_generator_tests)

set(PARSE_CATCH_TESTS_ADD_TO_CONFIGURE_DEPENDS ON)
set(PARSE_CATCH_TESTS_ADD_TARGET_IN_TEST_NAME OFF)

include(libs/catch2/ParseAndAddCatchTests.cmake)

ParseAndAddCatchTests(cpp_project_generator_tests)


