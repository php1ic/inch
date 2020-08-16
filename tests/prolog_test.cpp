#include "inch/prolog.hpp"

#include <catch2/catch.hpp>

#include <iostream>
#include <sstream>

const Limits limits;

TEST_CASE("Creation time", "[Prolog]")
{
  // Get the time using inch
  Prolog startBit(5, limits);

  auto when = std::time(nullptr);
  auto test = fmt::format("{:%Y-%m-%dT%H:%M:%S}", *std::localtime(&when));

  REQUIRE(startBit.getTime() == test);
}
