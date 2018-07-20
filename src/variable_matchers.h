#pragma once

#include <regex>

namespace mba {

const std::regex regex_prj(			R"--(\$\{\$PROJECT_NAME\$\}\$)--"			, std::regex::optimize );
const std::regex regex_target(		R"--(\$\{\$TARGET_NAME\$\}\$)--"			, std::regex::optimize );
const std::regex regex_ns(			R"--(\$\{\$NAMESPACE\$\}\$)--"				, std::regex::optimize );
const std::regex regex_cmake_ns(	R"--(\$\{\$CMAKE_NAMESPACE\$\}\$)--"		, std::regex::optimize );
const std::regex regex_link_target( R"--(\$\{\$CMAKE_TARGET_LINK_NAME\$\}\$)--"	, std::regex::optimize );

const std::regex regex_project_filename( R"--(PROJECT_NAME)--", std::regex::optimize );
const std::regex regex_target_filename( R"--(TARGET_NAME)--", std::regex::optimize );
const std::regex regex_component_filename( R"--(COMPONENT_NAME)--", std::regex::optimize );

}
