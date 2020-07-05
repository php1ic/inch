#include "inch/chart.hpp"

#include <catch2/catch.hpp>

TEST_CASE("EPS sestup", "[Chart]")
{
  const Chart theChart;

  const std::string setup{ "u dup scale\n"
                           "0.5 dup translate\n" };

  REQUIRE(theChart.EPSsetup() == setup);
}


TEST_CASE("EPS relative key setup", "[Chart]")
{
  const Chart theChart;
  std::string setup{};

  SECTION("Do use a relative key")
  {
    theChart.key_relative = true;
    theChart.height       = 5.0;

    setup = "\n%Shift coordinates so chart is vertically centered\n"
            "gs\n"
            "0 -3.5 translate\n";

    REQUIRE(theChart.EPSRelativeKeySetup(10) == setup);
  }

  SECTION("Do NOT use a relative key")
  {
    theChart.key_relative = false;

    setup = "";

    REQUIRE(theChart.EPSRelativeKeySetup(10) == setup);
  }
}


TEST_CASE("EPS relative key teardown", "[Chart]")
{
  const Chart theChart;
  std::string teardown{};

  SECTION("Relative key was not used")
  {
    theChart.key_relative = true;
    teardown              = "\n%Put coordinates back now that chart is drawn\n"
               "gr\n";

    REQUIRE(theChart.EPSRelativeKeyTearDown() == teardown);
  }

  SECTION("Relative key was used")
  {
    theChart.key_relative = false;
    teardown              = "";

    REQUIRE(theChart.EPSRelativeKeyTearDown() == teardown);
  }
}


TEST_CASE("EPS teardown", "[Chart]")
{
  const Chart theChart;
  theChart.size   = 2;
  theChart.height = 3.0;
  theChart.width  = 4.0;

  const std::string teardown{ "end grestore\n"
                              "\n"
                              "%%Trailer\n"
                              "%%BoundingBox: 0 0 8.0 6.0\n"
                              "%%EOF\n" };

  REQUIRE(theChart.EPSteardown() == teardown);
}


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
