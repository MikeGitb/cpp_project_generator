#include <${$PROJECT_NAME$}$/${$PROJECT_NAME$}$.h>

#include <catch2/catch.hpp>

#include <iostream>


TEST_CASE("say_hello", "[${$TARGET_NAME$}$_tests]")
{
	std::cout << ${$NAMESPACE$}$::get_hello() << std::endl;
	CHECK( 1 == 1 );
}