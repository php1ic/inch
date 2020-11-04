#include "inch/ui.hpp"

#include <catch2/catch.hpp>

// This is a bit of a cheat to get coverage for this file to 100%
TEST_CASE("UI construction", "[UI]")
{
  Options options;
  UI ui(options);

  REQUIRE_FALSE(ui.user_options.AME);
}
