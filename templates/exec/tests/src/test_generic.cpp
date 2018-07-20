//#include "${$PROJECT_NAME$}$.h"

#include <catch2/catch.hpp>

#include <iostream>


TEST_CASE("say_hello", "[${$TARGET_NAME$}$_tests]")
{
	std::cout << "Hello" << std::endl;
	CHECK( 1 == 1 );
}