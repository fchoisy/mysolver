#include "catch_amalgamated.hpp"

#include <Algorithms.hpp>

TEST_CASE("Dummy test", "[dummy]")
{
    REQUIRE(dummy(0) == 0);
    REQUIRE(dummy(2) == 9);
}