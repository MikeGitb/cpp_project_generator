cmake_minimum_required(VERSION 3.10)
project(${$PROJECT_NAME$}$  LANGUAGES CXX)

########## General Settings for the whole project ############################
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
	# Set warning level (CMAKE adds /W3 and msvc produces a warning, when we would just add /W4 )
	STRING(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	add_compile_options(
		/permissive-
		/D_SCL_SECURE_NO_WARNINGS
		/D_CRT_SECURE_NO_WARNINGS
		/D_SILENCE_PARALLEL_ALGORITHMS_EXPERIMENTAL_WARNING
	)
else()
	add_compile_options(-Wall -Wextra)
endif()

########## Detect and Build Libraries ########################################
file(GLOB LIBS "libs/*/CMakeLists.txt")
set(library_list "")
foreach( lib_cmake_file IN LISTS LIBS )
	get_filename_component(lib_dir ${lib_cmake_file} DIRECTORY)
	get_filename_component(lib_name ${lib_dir} NAME)

	add_subdirectory(${lib_dir} EXCLUDE_FROM_ALL)
	list(APPEND library_list ${lib_name})
endforeach()

message("Detected libraries: ${library_list}")

########## Generate Executable  ##############################################

########## Generate Proxy Library ####
# We put all source files (except main) into a separate library
# this Library is then linked to the main target -- makes testing simpler

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")
if(SOURCE_FILES)
	add_library				( ${$TARGET_NAME$}$_lib	STATIC	${SOURCE_FILES}	)
	target_link_libraries	( ${$TARGET_NAME$}$_lib PUBLIC	${library_list} )
else()
	add_library				( ${$TARGET_NAME$}$_lib INTERFACE )
	target_link_libraries	( ${$TARGET_NAME$}$_lib INTERFACE	${library_list} )
endif()

target_include_directories	( ${$TARGET_NAME$}$_lib INTERFACE	src )
# add further source files here
# target_sources(${$TARGET_NAME$}$_lib PUBLIC further_source files)

# create link target (e.g. for testing)
add_library( ${$CMAKE_TARGET_LINK_NAME$}$ ALIAS ${$TARGET_NAME$}$_lib )

######### generate the actual Executable

add_executable(	${$TARGET_NAME$}$ main.cpp )
target_link_libraries( ${$TARGET_NAME$}$ ${$CMAKE_TARGET_LINK_NAME$}$)

########## Testing ###########################################################
option(${$PROJECT_NAME$}$_INCLUDE_TESTS "Generate targets in test directory" ON)

if(${$PROJECT_NAME$}$_INCLUDE_TESTS)
	enable_testing()
	add_subdirectory(tests)
endif()