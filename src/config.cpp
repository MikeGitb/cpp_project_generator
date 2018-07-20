#include "config.h"

#include "ProjectType.h"
#include "helpers.h"
#include "types.h"
#include <cxxopts.hpp>
#include <filesystem>
#include <sstream>

namespace mba {

namespace fs = std::filesystem;

template<class T>
T get_or( const cxxopts::ParseResult& cmd_line_options, const std::string& key, const T& default_value )
{
	if( cmd_line_options.count( key ) > 0 ) {
		return cmd_line_options[key].as<T>();
	}
	return default_value;
}

Config parse_config( int argc, char* argv[] )
{
	Config cfg;

	cxxopts::Options options( "cpp_project_generator",
							  "A simple tool to create a standard project layout for executables or libraries" );

	// clang-format off
	const std::string type_option_string =
		to_string_short(ProjectType::exec) + " | "	+
		to_string_short(ProjectType::lib) + " | "	+
		to_string_short(ProjectType::lib_header_only);
	options.add_options()
		("h,help",          "print documentation")
		("N,name",          "Name of the project",                                 cxxopts::value<std::string>() )
		("t,type",          "Type of the project ( "+type_option_string+" )",      cxxopts::value<std::string>()->default_value( to_string_short( ProjectType::exec ) ) )
		("T,target",        "target name",                                         cxxopts::value<std::string>() )
		("n,namespace",     "namespace used in the library",                       cxxopts::value<std::string>() )
		("c,cmake_namespace", "namespace for the cmake",                           cxxopts::value<std::string>() )
		("m,module",        "component name inside cmake namespace",               cxxopts::value<std::string>() )
		("l,link_target",   "target name used by cmake to link to the library",    cxxopts::value<std::string>() )
		("g,git",           "creates a git repository (requires git to be installed)" );
	// clang-format on

	auto result = options.parse( argc, argv );

	if( result.count( "help" ) > 0 ) {
		std::cout << options.help();
		exit( 0 );
	}

	cfg.create_git = result.count( "git" ) > 0;

	cfg.prj_type     = parse_ProjectType( result["type"].as<std::string>() ).value();
	cfg.template_dir = get_template_directory();

	// by default, use current directory
	// if project name is specified, create appropriate sub-directory
	if( result.count( "name" ) > 0 ) {
		cfg.names.project = result["name"].as<std::string>();
		cfg.project_dir   = fs::current_path() / cfg.names.project;
	} else {
		cfg.names.project = mba::fs::current_path().stem().u8string();
		cfg.project_dir   = fs::current_path();
	}

	cfg.names                   = create_default_names( cfg.names.project );
	cfg.names.target            = get_or( result, "target", cfg.names.target );
	cfg.names.ns                = get_or( result, "namespace", cfg.names.ns );
	cfg.names.cmake_ns          = get_or( result, "cmake_namespace", cfg.names.cmake_ns );
	cfg.names.component_name    = get_or( result, "module", cfg.names.component_name );
	
	const std::string default_link_name
		= cfg.names.cmake_ns + "::" + cfg.names.component_name + ( cfg.prj_type == ProjectType::exec ? "_lib" : "" );

	cfg.names.cmake_link_target = get_or( result, "l", default_link_name );


	for( int i = 0; i < int(cfg.names.cmake_link_target.size() - 1); ++i ) {
		if( cfg.names.cmake_link_target[i] == ':' && cfg.names.cmake_link_target[i + 1] == ':' ) {
			cfg.names.component_name = cfg.names.cmake_link_target.substr( i + 2 );
			break;
		}
	}

	return cfg;
}

std::string to_string( const Config& cfg )
{
	std::stringstream ss;

	// clang-format off
	ss << "\n Target directory:      " << cfg.project_dir
	   << "\n Project name:          " << cfg.names.project
	   << "\n Template directory :   " << cfg.template_dir
	   << "\n Target name:           " << cfg.names.target
	   << "\n namespace:             " << cfg.names.ns
	   << "\n cmake namespace:       " << cfg.names.cmake_ns
	   << "\n cmake link target:     " << cfg.names.cmake_link_target;
	// clang-format on

	return ss.str();
}

} // namespace mba
