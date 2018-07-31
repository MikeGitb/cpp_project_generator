#include "src/helpers.h"
#include "src/ProjectType.h"
#include "src/git.h"
#include "src/config.h"

#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <cassert>

#include <filesystem>
#include <stdexcept>

namespace mba {
namespace fs = std::filesystem;

void install_project( const fs::path&      template_dir,
					  const mba::fs::path& project_dir,
					  mba::ProjectType     prj_type,
					  const Names&         names )
{
	fs::create_directories( project_dir );
	install_recursive( template_dir / "common", project_dir, names );
	switch( prj_type ) {
		case ProjectType::exec:
			install_recursive( template_dir / "exec", project_dir, names );
			// create some empty
			fs::create_directories( project_dir / "src" );
			fs::create_directories( project_dir / "libs" );
			break;
		case ProjectType::lib:
			install_recursive( template_dir / "lib-common", project_dir, names );
			install_recursive( template_dir / "lib", project_dir, names );
			break;
		case ProjectType::lib_header_only:
			install_recursive( template_dir / "lib-common", project_dir, names );
			install_recursive( template_dir / "lib-header", project_dir, names );
			break;
		default: assert( false );
	}
}

} // namespace mba

using namespace mba;

const std::string post_build_message
	= "\n################################################"
	  "\n### Project creation completed successfully! ###"
	  "\n################################################"
	  "\n#"
	  "\n# To build your project and run test cases "
	  "\n# you can perform the following steps:"
	  "\n#"
	  "\n# - Go to the \"build\" subdirectory (create it if neccessary)"
	  "\n# - cmake .. "
	  "\n# - cmake --build ."
	  "\n# - ctest .		# or for MSVC: ctest . -C Debug"
	  "\n#"
	  "\n################################################\n\n";


// example command : cpp_project_generator.exe -N flat_map -t lib -T mba_flat_map -n mba -c MBa -m flat_map -g
int main( int argc, char** argv )
{
	try {
		Config cfg = parse_config( argc, const_cast<const char**>( argv ) );

		std::cout << "This will create a \"" << to_string( cfg.prj_type )
				  << "\" project with the following configuration:\n"
				  << to_string( cfg ) << std::endl;

		std::cout << "\nProceede? (y/n)";

		char answer{};
		std::cin >> answer;
		if( answer != 'y' ) {
			return 1;
		}

		try {
			install_project( cfg.template_dir, cfg.project_dir, cfg.prj_type, cfg.names );

			std::cout << post_build_message  << std::endl;

			if( cfg.create_git ) {
				mba::git_init_dir( cfg.project_dir );
			}

		} catch( const std::exception& e ) {
			std::cout << "Error during project creation: \"" << e.what() << "\"" << std::endl;
		}

	} catch( const std::exception& e ) {
		std::cout << "Error: " << e.what() << std::endl;
	}
}
