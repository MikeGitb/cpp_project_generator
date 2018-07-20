cmake_minimum_required(VERSION 3.10)
project(${$PROJECT_NAME$}$ LANGUAGES CXX)

option(${$PROJECT_NAME$}$_INCLUDE_TESTS "Generate targets in test directory" ON)

add_library(
	${$TARGET_NAME$}$
	INTERFACE
)

target_include_directories(
	${$TARGET_NAME$}$
INTERFACE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)

target_compile_features(
	${$TARGET_NAME$}$
INTERFACE
	cxx_std_17
)

if(${$PROJECT_NAME$}$_INCLUDE_TESTS)
	enable_testing()
	add_subdirectory(tests)
endif()

add_library(${$CMAKE_TARGET_LINK_NAME$}$ ALIAS ${$TARGET_NAME$}$)
