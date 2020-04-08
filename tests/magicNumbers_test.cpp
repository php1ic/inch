#include <catch2/catch.hpp>

#include "inch/limits.hpp"
#include "inch/magicNumbers.hpp"


MagicNumbers magic(Limits::MIN_Z, Limits::MAX_Z, Limits::MIN_N, Limits::MAX_N);

TEST_CASE("Check numbers and limits are populated", "[Magic Numbers]")
{
  REQUIRE(magic.coords.size() == 7);
}
