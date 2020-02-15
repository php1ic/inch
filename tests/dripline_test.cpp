#include <catch/catch.hpp>

#include "inch/dripline.hpp"

const DripLine dripline(1.0, 2.0, 0, 118, 0, 180, LineType::singleproton);

TEST_CASE("Set the line colour", "[DripLine]")
{
  dripline.setDripLineColour("blue");
  REQUIRE_THAT( dripline.line_colour, Catch::Matches("blue") );

  dripline.setDripLineColour("green");
  REQUIRE_THAT( dripline.line_colour, Catch::Matches("green") );
}
