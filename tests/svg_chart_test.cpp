#include "inch/svg_chart.hpp"

#include <catch2/catch.hpp>


TEST_CASE("SVG setup", "[SVGChart]")
{
  Options options;
  const SVGChart theChart(options);

  const std::string setup{ "<g transform=\"translate(2.0,2.0) scale(4,4)\">\n" };

  REQUIRE(theChart.setup() == setup);
}


TEST_CASE("SVG teardown", "[SVGChart]")
{
  Options options;
  const SVGChart theChart(options);
  theChart.size   = 2;
  theChart.height = 3.0;
  theChart.width  = 4.0;

  const std::string teardown{ "</g>\n"
                              "</svg>\n" };

  REQUIRE(theChart.teardown() == teardown);
}


TEST_CASE("TODO SVG Key setup", "[SVGChart]")
{
  Options options;
  const SVGChart theChart(options);

  REQUIRE_THAT(theChart.KeySetup(1), Catch::Matches(""));
}


TEST_CASE("TODO SVG Key teardoen", "[SVGChart]")
{
  Options options;
  const SVGChart theChart(options);

  REQUIRE_THAT(theChart.KeyTearDown(), Catch::Matches(""));
}
