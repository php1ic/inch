#include "inch/eps_chart.hpp"

#include <catch2/catch.hpp>


TEST_CASE("EPS setup", "[EPSChart]")
{
  Options options;
  const EPSChart theChart(options);

  const std::string setup{ "u dup scale\n"
                           "0.5 dup translate\n" };

  REQUIRE(theChart.setup() == setup);
}


TEST_CASE("EPS relative key setup", "[EPSChart]")
{
  Options options;
  const EPSChart theChart(options);
  std::string setup{};

  SECTION("Do use a relative key")
  {
    theChart.key_relative = true;
    theChart.height       = 5.0;

    setup = "\n%Shift coordinates so chart is vertically centered\n"
            "gs\n"
            "0 -3.5 translate\n";

    REQUIRE(theChart.KeySetup(10) == setup);
  }

  SECTION("Do NOT use a relative key")
  {
    theChart.key_relative = false;

    setup = "";

    REQUIRE(theChart.KeySetup(10) == setup);
  }
}


TEST_CASE("EPS relative key teardown", "[EPSChart]")
{
  Options options;
  const EPSChart theChart(options);
  std::string teardown{};

  SECTION("Relative key was not used")
  {
    theChart.key_relative = true;
    teardown              = "\n%Put coordinates back now that chart is drawn\n"
               "gr\n";

    REQUIRE(theChart.KeyTearDown() == teardown);
  }

  SECTION("Relative key was used")
  {
    theChart.key_relative = false;
    teardown              = "";

    REQUIRE(theChart.KeyTearDown() == teardown);
  }
}


TEST_CASE("EPS teardown", "[EPSChart]")
{
  Options options;
  const EPSChart theChart(options);
  theChart.size   = 2;
  theChart.height = 3.0;
  theChart.width  = 4.0;

  const std::string teardown{ "end grestore\n"
                              "\n"
                              "%%Trailer\n"
                              "%%BoundingBox: 0 0 8.0 6.0\n"
                              "%%EOF\n" };

  REQUIRE(theChart.teardown() == teardown);
}
