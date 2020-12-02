#include "inch/eps_dripline.hpp"

#include <catch2/catch.hpp>

#include <ostream>

const Limits limits;

TEST_CASE("EPS dripline setup", "[EPSDripLine]")
{
  const EPSDripLine dripline(1.0, 2.0, limits, LineType::singleproton, "green");
  const auto setup{ "gs\n"
                    "green rgb\n"
                    "1 u div sl\n" };

  REQUIRE_THAT(setup, Catch::Equals(dripline.Setup()));
}


TEST_CASE("EPS dripline teardown", "[EPSDripLine]")
{
  const EPSDripLine dripline(1.0, 2.0, limits, LineType::doubleneutron, "black");

  const auto teardown{ "st\n"
                       "gr\n" };

  REQUIRE_THAT(teardown, Catch::Equals(dripline.TearDown()));
}


TEST_CASE("EPS dripline comment", "[EPSDripLine]")
{
  EPSDripLine dripline(1.0, 2.0, limits, LineType::doubleneutron, "black");

  SECTION("Double neutron") { REQUIRE_THAT("\n%Two Neutron Drip Line\n", Catch::Equals(dripline.Header())); }

  SECTION("Single proton")
  {
    dripline.the_line = LineType::singleproton;
    REQUIRE_THAT("\n%Proton Drip Line\n", Catch::Equals(dripline.Header()));
  }

  SECTION("Single neutron")
  {
    dripline.the_line = LineType::singleneutron;
    REQUIRE_THAT("\n%Neutron Drip Line\n", Catch::Equals(dripline.Header()));
  }

  SECTION("Double proton")
  {
    dripline.the_line = LineType::doubleproton;
    REQUIRE_THAT("\n%Two Proton Drip Line\n", Catch::Equals(dripline.Header()));
  }
}


TEST_CASE("EPS dripline create file if necessary", "[EPSDripLine]")
{
  EPSDripLine dripline(1.0, 2.0, limits, LineType::singleneutron, "black");
  EPSDripLine moved_line = std::move(dripline);
  std::ofstream temp("file.temp");
  moved_line.drip_file = "random.file";

  REQUIRE(moved_line.WriteLine(temp) == 1);
}
