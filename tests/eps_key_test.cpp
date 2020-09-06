#include "inch/eps_key.hpp"

#include <catch2/catch.hpp>

TEST_CASE("EPS key setting the txt correctly", "[EPSKey]")
{
  const EPSKey tester;
  Partition part(ChartColour::MASSEXCESSERROR);
  part.setDefaultColours();
  tester.SetText(part);

  const auto output{ "1 TR (Stable : ) TotalWidth sh\n"
                     "1 S (d) TotalWidth sh\n"
                     "1 TR (m < 3.0 keV) TotalWidth sh TestWidth\n" };

  REQUIRE_THAT(output, Catch::Equals(part.values.at(0).keyText));
}


TEST_CASE("EPS key setup", "[EPSKey]")
{
  const EPSKey tester;

  const auto output = fmt::format("\n%-------\n"
                                  "%- Key -\n"
                                  "%-------\n"
                                  "/ResetWidth {{/StringLength 0 def}} def\n"
                                  "/TotalWidth {{dup stringwidth pop StringLength add /StringLength exch def}} def\n"
                                  "/KeyWidth 0 def\n"
                                  "/TestWidth {{StringLength KeyWidth gt {{/KeyWidth StringLength def}} if}} def\n\n"
                                  "%-lower left corner of the key-\n");

  REQUIRE_THAT(output, Catch::Equals(tester.Setup()));
}


TEST_CASE("EPS key placement", "[EPSKey]")
{
  Options options;
  EPSKey tester;

  auto output{ "15 75 translate\n"
               "3.0 dup scale\n" };


  SECTION("Full chart is drawn")
  {
    // Don't use memeber function to set the scale
    tester.scale = 3.0;

    std::string output{};

    SECTION("Mass excess error placement")
    {
      output = "15 75 translate\n3.0 dup scale\n";
      REQUIRE_THAT(output, Catch::Equals(tester.PlaceKey(options)));
    }

    SECTION("Relative mass excess error")
    {
      options.chart_colour = ChartColour::REL_MASSEXCESSERROR;
      output               = "12 81 translate\n3.0 dup scale\n";
      REQUIRE_THAT(output, Catch::Equals(tester.PlaceKey(options)));
    }

    SECTION("Ground state decay mode")
    {
      options.chart_colour = ChartColour::GS_DECAYMODE;
      output               = "9 60 translate\n3.0 dup scale\n";
      REQUIRE_THAT(output, Catch::Equals(tester.PlaceKey(options)));
    }

    SECTION("Ground state half life")
    {
      options.chart_colour = ChartColour::GS_HALFLIFE;
      output               = "9 75 translate\n3.0 dup scale\n";
      REQUIRE_THAT(output, Catch::Equals(tester.PlaceKey(options)));
    }

    SECTION("First isomer energy")
    {
      options.chart_colour = ChartColour::FIRST_ISOMERENERGY;
      output               = "15 75 translate\n3.0 dup scale\n";
      REQUIRE_THAT(output, Catch::Equals(tester.PlaceKey(options)));
    }
  }

  SECTION("A subsection of the chart is drawn")
  {
    tester.scale = 1.0;

    SECTION("Chart is shorter than the key")
    {
      // Hackety hack hack! Satisfy [[nodiscard]] and compiler warnings
      bool isset = options.limits.setZmin(56);
      isset      = options.limits.setZmax(57);
      if (isset)
        {
          options.chart_selection = ChartSelection::SUB_CHART;
        }

      output = "179 0.0 translate\n1.0 dup scale\n";
      REQUIRE_THAT(output, Catch::Equals(tester.PlaceKey(options)));
    }

    SECTION("Key is shorter then the chart")
    {
      bool isset = options.limits.setZmin(10);
      isset      = options.limits.setZmax(100);
      if (isset)
        {
          options.chart_selection = ChartSelection::SUB_CHART;
        }

      output = "179 45.25 translate\n1.0 dup scale\n";
      REQUIRE_THAT(output, Catch::Equals(tester.PlaceKey(options)));
    }
  }
}


const auto hl_setup = fmt::format("\n/printUnit{{gs\n"
                                  "1 S (t) sh\n"
                                  "0.5 TR 0 -0.15 rmoveto (1/2) sh\n"
                                  "gr}} def\n\n");

const auto me_setup = fmt::format("\n/exponent{{\n"
                                  "/e1 ed\n"
                                  "/e2 ed\n"
                                  "1 TR e2 5 string cvs TotalWidth sh\n"
                                  "0.75 TR (x) TotalWidth sh\n"
                                  "1 TR (10) TotalWidth sh\n"
                                  "gs\n"
                                  "0.75 TR\n"
                                  "0 0.4 rmoveto e1 2 string cvs TotalWidth sh\n"
                                  "gr\n"
                                  "}} def\n\n"
                                  "/printUnit{{\n"
                                  "1 TR (   < ) TotalWidth sh\n"
                                  "1 S (d) TotalWidth sh\n"
                                  "1 TR (m/m < ) TotalWidth sh\n"
                                  "}} def\n\n");


TEST_CASE("EPS ME setup", "[EPSKey]")
{
  const EPSKey tester;


  REQUIRE_THAT(me_setup, Catch::Equals(tester.MassExcessSetup()));
}


TEST_CASE("EPS Half-life setup", "[EPSKey]")
{
  const EPSKey tester;

  REQUIRE_THAT(hl_setup, Catch::Equals(tester.HalLifeSetup()));
}


TEST_CASE("EPS appropriate additional function is called", "[EPSKey]")
{
  const EPSKey tester;

  auto colour = ChartColour::REL_MASSEXCESSERROR;

  SECTION("Mass excess setup") { REQUIRE_THAT(me_setup, Catch::Equals(tester.AdditionalFunctions(colour))); }

  SECTION("Half-life setup")
  {
    colour = ChartColour::GS_HALFLIFE;

    REQUIRE_THAT(hl_setup, Catch::Equals(tester.AdditionalFunctions(colour)));
  }
}


TEST_CASE("EPS draw the box around the key", "EPSKey")
{
  const EPSKey tester;

  auto output = fmt::format("\n"
                            "%Draw a box around the key\n"
                            "0.1 u div sl\n"
                            "0 0 m\n"
                            "KeyWidth 3 add 0 rl\n"
                            "0 {} rl\n"
                            "KeyWidth 3 add neg 0 rl\n"
                            "closepath\n"
                            "st\n\n",
                            tester.height);

  REQUIRE_THAT(output, Catch::Equals(tester.SurroundingBox()));
}
