#include "inch/tikz_chart.hpp"

#include <catch2/catch.hpp>

Options options;

TEST_CASE("TikZ sestup", "[TIKZChart]")
{
  const TIKZChart theChart(options);

  const std::string setup{ "\\begin{document}\n"
                           "\\begin{tikzpicture}\n" };

  REQUIRE(theChart.setup() == setup);
}


TEST_CASE("TikZ teardown", "[TIKZChart]")
{
  const TIKZChart theChart(options);
  theChart.size   = 2;
  theChart.height = 3.0;
  theChart.width  = 4.0;

  const std::string teardown{ "\\end{tikzpicture}\n"
                              "\\end{document}\n" };

  REQUIRE(theChart.teardown() == teardown);
}
