#include "inch/eps_magicNumbers.hpp"
#include "inch/limits.hpp"

#include <catch2/catch.hpp>


TEST_CASE("Check numbers and limits are populated", "[EPSMagicNumbers]")
{
  const Limits limits;
  EPSMagicNumbers magic(limits);

  REQUIRE(magic.coords.size() == 7);
}


TEST_CASE("EPS setup is correct", "[EPSMagicNumbers]")
{
  const Limits limits;
  EPSMagicNumbers magic(limits);

  const auto setup{ "\n%Magic Numbers\n"
                    "gs\n"
                    "black rgb\n"
                    "1 u div sl\n" };

  REQUIRE_THAT(setup, Catch::Equals(magic.Setup()));
}


TEST_CASE("EPS teardown is correct", "[EPSMagicNumbers]")
{
  const Limits limits;
  EPSMagicNumbers magic(limits);

  const auto teardown{ "gr\n" };

  REQUIRE_THAT(teardown, Catch::Equals(magic.TearDown()));
}


TEST_CASE("Neutron number is displayed correctly", "[EPSMagicNumbers]")
{
  const Limits limits;
  EPSMagicNumbers magic(limits);

  const auto line{ "%N=8\n"
                   "9 0 m 0 22 rl\n"
                   "8 0 m 0 22 rl st\n" };

  REQUIRE_THAT(line, Catch::Equals(magic.WriteNeutronNumber(8)));
}


TEST_CASE("Proton number is displayed correctly", "[EPSMagicNumbers]")
{
  const Limits limits;
  EPSMagicNumbers magic(limits);

  const auto line{ "%Z=50\n"
                   "45 51 m 47 0 rl\n"
                   "45 50 m 47 0 rl st\n" };

  REQUIRE_THAT(line, Catch::Equals(magic.WriteProtonNumber(50)));
}
