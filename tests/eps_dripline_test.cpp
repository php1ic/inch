#include "inch/eps_dripline.hpp"

#include <catch2/catch.hpp>

#include <ostream>

const Limits limits;

TEST_CASE("EPS dripline setup is correct", "[EPSDripLine]")
{
  const EPSDripLine dripline(1.0, 2.0, limits, LineType::singleproton, "green");
  const auto setup{ "gs\n"
                    "green rgb\n"
                    "1 u div sl\n" };

  REQUIRE_THAT(setup, Catch::Equals(dripline.Setup()));
}


TEST_CASE("EPS dripline teardown is correct", "[EPSDripLine]")
{
  const EPSDripLine dripline(1.0, 2.0, limits, LineType::doubleneutron, "black");

  const auto teardown{ "st\n"
                       "gr\n" };

  REQUIRE_THAT(teardown, Catch::Equals(dripline.TearDown()));
}

TEST_CASE("EPS dripline create file if necessary", "[EPSDripLine]")
{
  EPSDripLine dripline(1.0, 2.0, limits, LineType::singleneutron, "black");
  EPSDripLine moved_line = std::move(dripline);
  std::ofstream temp("file.temp");
  moved_line.drip_file = "random.file";

  REQUIRE(moved_line.WriteLine(temp) == 1);
}
