//#include "cpp_project_generator.h"

#include <catch2/catch.hpp>

#include <iostream>


TEST_CASE("say_hello", "[cpp_project_generator_tests]")
{
	std::cout << "Hello" << std::endl;
	CHECK( 1 == 1 );
}

