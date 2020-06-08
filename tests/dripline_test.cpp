#include <catch2/catch.hpp>

#include "inch/dripline.hpp"
#include "inch/limits.hpp"

const Limits limits;
const DripLine dripline(1.0, 2.0, limits, LineType::singleproton);

TEST_CASE("Set the line colour", "[DripLine]")
{
  dripline.setDripLineColour("blue");
  REQUIRE_THAT( dripline.line_colour, Catch::Matches("blue") );

  dripline.setDripLineColour("green");
  REQUIRE_THAT( dripline.line_colour, Catch::Matches("green") );
}
