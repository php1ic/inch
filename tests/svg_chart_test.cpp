#include "inch/svg_chart.hpp"

#include <catch2/catch.hpp>

TEST_CASE("SVG setup", "[SVGChart]")
{
  const SVGChart theChart;

  const std::string setup{ "<g transform=\"translate(2,2) scale(4,4)\">\n" };

  REQUIRE(theChart.setup() == setup);
}


TEST_CASE("SVG teardown", "[SVGChart]")
{
  const SVGChart theChart;
  theChart.size   = 2;
  theChart.height = 3.0;
  theChart.width  = 4.0;

  const std::string teardown{ "</g>\n"
                              "</svg>\n" };

  REQUIRE(theChart.teardown() == teardown);
}
