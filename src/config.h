#pragma once

#include "ProjectType.h"
#include "helpers.h"
#include "types.h"
#include <filesystem>

namespace mba {

struct Config {
	ProjectType           prj_type;
	Names                 names;
	std::filesystem::path template_dir;
	std::filesystem::path project_dir;
	bool                  create_git;
};

inline std::filesystem::path get_template_directory()
{
	return get_exec_directory() / "cpp_project_templates";
}

inline Names create_default_names( const std::string& project_name )
{
	Names names;
	names.project           = project_name;
	names.target            = project_name;
	names.ns                = my_tolower( project_name );
	names.cmake_ns          = capitalize_first( project_name );
	names.component_name    = project_name;
	names.cmake_link_target = names.cmake_ns + "::" + names.component_name;
	return names;
}

Config parse_config( int argc, const char** argv );

std::string to_string( const Config& cfg );

} // namespace mba
