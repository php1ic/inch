#include "inch/chart.hpp"

#include <catch2/catch.hpp>


TEST_CASE("Canvas Height", "[Chart]")
{
  Chart theChart;

  SECTION("Z range is large enough to not use a relatively placed chart")
  {
    theChart.key_relative = false;
    const int ZRange{ 50 };
    const double scale{ 1.5 };
    const double height{ 6.0 };

    REQUIRE(theChart.calculateCanvasHeight(scale, height, ZRange) == Approx(52.0));
    REQUIRE(theChart.key_relative == false);
  }

  SECTION("Z range means that the chart will be placed relative to the key")
  {
    theChart.key_relative = false;

    const int ZRange{ 1 };
    const double scale{ 2.0 };
    const double height{ 5.0 };

    REQUIRE(theChart.calculateCanvasHeight(scale, height, ZRange) == Approx(12.0));
    REQUIRE(theChart.key_relative == true);
  }
}


TEST_CASE("Canvas Width", "[Chart]")
{
  Chart theChart;
  Options draw;

  SECTION("Full chart does not have additional width")
  {
    draw.limits.ResetAllLimits();
    draw.chart_selection = ChartSelection::FULL_CHART;

    const double scale{ 2.0 };
    REQUIRE(theChart.calculateCanvasWidth(scale, draw) == Approx(179.0));
  }

  SECTION("Partial chart has additional canvas width")
  {
    draw.limits.Nmin     = 20;
    draw.limits.Nmax     = 50;
    draw.limits.Zmin     = 30;
    draw.limits.Zmax     = 70;
    draw.chart_selection = ChartSelection::SUB_CHART;

    const double scale{ 1.5 };
    REQUIRE(theChart.calculateCanvasWidth(scale, draw) == Approx(53.75));
  }
}


TEST_CASE("Canvas size", "[Chart]")
{
  Chart theChart;
  Options draw;

  draw.limits.Nmin     = 20;
  draw.limits.Nmax     = 50;
  draw.limits.Zmin     = 30;
  draw.limits.Zmax     = 70;
  draw.chart_selection = ChartSelection::SUB_CHART;

  const double scale{ 1.5 };
  const double height{ 5.0 };

  theChart.setCanvasSize(scale, height, draw);

  REQUIRE(theChart.width == Approx(53.75));
  REQUIRE(theChart.height == Approx(42.0));
}
