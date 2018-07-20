cmake_minimum_required(VERSION 3.10)
project(cpp_project_generator  LANGUAGES CXX)

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

file(GLOB SOURCE_FILES "src/*.cpp")
if(SOURCE_FILES)
	add_library( cpp_project_generator_lib	STATIC	${SOURCE_FILES}	)
else()
	add_library( cpp_project_generator_lib INTERFACE )
endif()

target_include_directories	( cpp_project_generator_lib INTERFACE	src )
target_link_libraries		( cpp_project_generator_lib PUBLIC		${library_list})

if(MSVC)
	target_sources(cpp_project_generator_lib PRIVATE src/win/arch.cpp)
else()
	target_sources(cpp_project_generator_lib PRIVATE src/linux/arch.cpp)
endif()

if(NOT WIN32)
	# just a guess that we ar using libstdc++: gcc 8 requires the filesystem library to be linked explicitly
	target_link_libraries(cpp_project_generator_lib PUBLIC stdc++fs)
endif()

# create link target (e.g. for testing)
add_library( MBa::cpp_project_generator_lib ALIAS cpp_project_generator_lib )

######### generate the actual Executable

add_executable(	cpp_project_generator main.cpp )
target_link_libraries( cpp_project_generator MBa::cpp_project_generator_lib)


########## Installation  #####################################################

add_custom_command(TARGET cpp_project_generator POST_BUILD
  COMMAND "${CMAKE_COMMAND}" -E copy_directory
     "${CMAKE_CURRENT_SOURCE_DIR}/templates"
     "$<TARGET_FILE_DIR:cpp_project_generator>/cpp_project_templates"
  COMMENT "Copying to output directory")


INSTALL( DIRECTORY "$<TARGET_FILE_DIR:cpp_project_generator>/cpp_project_templates" DESTINATION "." )
INSTALL( TARGETS cpp_project_generator RUNTIME DESTINATION ".")

########## Testing ###########################################################
option(cpp_project_generator_INCLUDE_TESTS "Generate targets in test directory" ON)

if(cpp_project_generator_INCLUDE_TESTS)
	enable_testing()
	add_subdirectory(tests)
endif()

