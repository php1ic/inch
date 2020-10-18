#include "inch/partition.hpp"

#include <catch2/catch.hpp>


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
