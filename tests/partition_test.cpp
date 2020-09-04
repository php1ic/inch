#include "inch/partition.hpp"

#include <catch2/catch.hpp>


TEST_CASE("Get the colour via string", "[Partition]")
{
  Partition testPartition(ChartColour::GS_DECAYMODE);
  testPartition.setDefaultColours();
  REQUIRE_THAT(testPartition.getColour("EC"), Catch::Matches("orange"));
}


TEST_CASE("Get the colour via double", "[Partition]")
{
  Partition testPartition(ChartColour::REL_MASSEXCESSERROR);
  testPartition.setDefaultColours();
  REQUIRE_THAT(testPartition.getColour(4.9e-4), Catch::Matches("green"));
}


TEST_CASE("Get the colour via chrono duration", "[Partition]")
{
  Partition testPartition(ChartColour::GS_HALFLIFE);
  testPartition.setDefaultColours();
  const Converter::seconds hl{ 5.0 };
  REQUIRE_THAT(testPartition.getColour(hl), Catch::Matches("yellow"));
}
