#include <catch2/catch.hpp>

#include "inch/key.hpp"
#include "inch/options.hpp"
#include "inch/partition.hpp"
#include "inch/chartColour.hpp"

Key key;
Options options;
Partition partition(ChartColour::MASSEXCESSERROR);

TEST_CASE("The key scales as required", "[Key]")
{
  SECTION("No key sets the scale appropriately")
    {
      options.key = false;
      key.scale = 1.0;

      key.setScale(options, partition);

      REQUIRE(key.scale == Approx(0.0));
    }
}
