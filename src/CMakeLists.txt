cmake_minimum_required( VERSION 3.10 )


# We put all source files (except main) into a separate library
# this Library is then linked to the main target -- makes testing simpler
add_library( cpp_project_generator_lib STATIC )

file(GLOB SOURCE_FILES "*.cpp")
target_sources(
	cpp_project_generator_lib
PUBLIC
	${SOURCE_FILES}
	${CMAKE_CURRENT_LIST_DIR}/${arch}/arch.cpp
)

target_link_libraries(
	cpp_project_generator_lib
PUBLIC
	cxxopts::cxxopts
	# just a guess that we ar using libstdc++: gcc 8 requires the filesystem library to be linked explicitly
	$<IF:$<BOOL:${WIN32}>,,stdc++fs>
)

target_include_directories( cpp_project_generator_lib INTERFACE . )
add_library( MBa::cpp_project_generator_lib ALIAS cpp_project_generator_lib )