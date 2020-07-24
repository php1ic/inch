#include "inch/partition.hpp"

#include <catch2/catch.hpp>

Partition testPartition(ChartColour::GS_DECAYMODE);

TEST_CASE("Get the colour", "[Partition]")
{
  testPartition.setDefaultColours();
  REQUIRE_THAT(testPartition.getColour("EC"), Catch::Matches("orange"));
}
