#include "inch/limits.hpp"
#include "inch/magicNumbers.hpp"

#include <catch2/catch.hpp>


const Limits limits;
MagicNumbers magic(limits);

TEST_CASE("Check numbers and limits are populated", "[MagicNumbers]")
{
  REQUIRE(magic.coords.size() == 7);
}


TEST_CASE("EPS setup is correct", "[MagicNumbers]")
{
  const auto setup{ "\n%Magic Numbers\n"
                    "gs\n"
                    "black rgb\n"
                    "1 u div sl\n" };

  REQUIRE_THAT(setup, Catch::Equals(magic.EPSSetup()));
}


TEST_CASE("EPS tear down is correct", "[MagicNumbers]")
{
  const auto teardown{ "gr\n" };

  REQUIRE_THAT(teardown, Catch::Equals(magic.EPSTearDown()));
}


TEST_CASE("Neutron number is displayed correctly", "[MagicNumbers]")
{
  const auto line{ "%N=8\n"
                   "9 0 m 0 22 rl\n"
                   "8 0 m 0 22 rl st\n" };

  REQUIRE_THAT(line, Catch::Equals(magic.EPSWriteNeutronNumber(8)));
}


TEST_CASE("Proton number is displayed correctly", "[Magic Numbers]")
{
  const auto line{ "%Z=50\n"
                   "45 51 m 47 0 rl\n"
                   "45 50 m 47 0 rl st\n" };

  REQUIRE_THAT(line, Catch::Equals(magic.EPSWriteProtonNumber(50)));
}
