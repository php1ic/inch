#include "inch/tikz_chart.hpp"

#include <catch2/catch.hpp>


TEST_CASE("TikZ sestup", "[TIKZChart]")
{
  Options options;
  const TIKZChart theChart(options);

  const std::string setup{ "\\begin{document}\n"
                           "\\begin{tikzpicture}\n" };

  REQUIRE(theChart.setup() == setup);
}


TEST_CASE("TikZ teardown", "[TIKZChart]")
{
  Options options;
  const TIKZChart theChart(options);
  theChart.size   = 2;
  theChart.height = 3.0;
  theChart.width  = 4.0;

  const std::string teardown{ "\\end{tikzpicture}\n"
                              "\\end{document}\n" };

  REQUIRE(theChart.teardown() == teardown);
}


TEST_CASE("TODO TikZ Key setup", "[TIKZChart]")
{
  Options options;
  const TIKZChart theChart(options);

  REQUIRE_THAT(theChart.KeySetup(1), Catch::Matches(""));
}


TEST_CASE("TODO TiKZ Key teardoen", "[TIKZChart]")
{
  Options options;
  const TIKZChart theChart(options);

  REQUIRE_THAT(theChart.KeyTearDown(), Catch::Matches(""));
}
