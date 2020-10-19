#include "inch/partition.hpp"

#include <catch2/catch.hpp>

#include <algorithm>

TEST_CASE("Colours are set correctly", "[Partition]")
{
  Partition testPartition(ChartColour::MASSEXCESSERROR);

  SECTION("Mass Excess")
  {
    testPartition.setDefaultColours();
    REQUIRE_THAT(testPartition.getColour(5.0), Catch::Matches("cyan"));
  }

  SECTION("Relative Mass Excess")
  {
    testPartition.setScheme(ChartColour::REL_MASSEXCESSERROR);
    testPartition.setDefaultColours();
    REQUIRE_THAT(testPartition.getColour(4.9e-4), Catch::Matches("green"));
  }

  SECTION("Ground-state Decay mode")
  {
    testPartition.setScheme(ChartColour::GS_DECAYMODE);
    testPartition.setDefaultColours();
    REQUIRE_THAT(testPartition.getColour("EC"), Catch::Matches("orange"));
  }

  SECTION("Ground-state half life")
  {
    testPartition.setScheme(ChartColour::GS_HALFLIFE);
    testPartition.setDefaultColours();
    const Converter::seconds hl{ 5.0 };
    REQUIRE_THAT(testPartition.getColour(hl), Catch::Matches("yellow"));
  }

  SECTION("First isomer energy")
  {
    testPartition.setScheme(ChartColour::FIRST_ISOMERENERGY);
    testPartition.setDefaultColours();
    REQUIRE_THAT(testPartition.getColour(75.0), Catch::Matches("yellow"));
  }
}


TEST_CASE("EPS decay mode key text", "[Partition]")
{
  Partition testPartition(ChartColour::GS_DECAYMODE);

  testPartition.setDefaultDecayColours();
  testPartition.populateEPSDecayModeKeyText();

  const std::vector<std::string> text{ "1 TR (Stable) TotalWidth sh TestWidth\n",
                                       "1 TR (Alpha) TotalWidth sh TestWidth\n",
                                       "1 S (b) TotalWidth sh\n0.5 TR 0 0.55 rmoveto (+) TotalWidth sh TestWidth\n",
                                       "1 S (b) TotalWidth sh\n0.75 TR 0 0.55 rmoveto (-) TotalWidth sh TestWidth\n",
                                       "1 TR (Spontaneous Fission)TotalWidth sh TestWidth\n",
                                       "1 TR (n decay) TotalWidth sh TestWidth\n",
                                       "1 TR (2n decay) TotalWidth sh TestWidth\n",
                                       "1 TR (p decay) TotalWidth sh TestWidth\n",
                                       "1 TR (2p decay) TotalWidth sh TestWidth\n",
                                       "1 TR (Unknown) TotalWidth sh TestWidth\n",
                                       "1 TR (Electron Capture) TotalWidth sh TestWidth\n" };

  REQUIRE(std::equal(text.begin(),
                     text.end(),
                     testPartition.values.begin(),
                     testPartition.values.end(),
                     [](const auto& t, const auto& s) { return t == s.keyText; }));
}
