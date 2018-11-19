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
	)
else()
	add_compile_options(-Wall -Wextra)
endif()

set(arch $<IF:$<BOOL:${WIN32}>,win,linux>)

########## Lookup libraries #################################################
find_package(cxxopts REQUIRED)

########## Generate Executable  ##############################################
########## Generate Proxy Library
add_subdirectory(src)

######### generate the actual Executable
add_executable(	cpp_project_generator main.cpp )
target_link_libraries( cpp_project_generator MBa::cpp_project_generator_lib)
set_target_properties( cpp_project_generator PROPERTIES OUTPUT_NAME "cpp_project")

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

