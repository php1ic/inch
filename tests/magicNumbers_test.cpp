#include <catch2/catch.hpp>

#include "inch/limits.hpp"
#include "inch/magicNumbers.hpp"


const Limits limits;
MagicNumbers magic(limits);

TEST_CASE("Check numbers and limits are populated", "[Magic Numbers]")
{
  REQUIRE(magic.coords.size() == 7);
}
