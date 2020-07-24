#include "inch/prolog.hpp"

#include <catch2/catch.hpp>

#include <iostream>
#include <sstream>

const Limits limits;

TEST_CASE("Creation time", "[Prolog]")
{
  // This seems overly complicated

  // Get the time using inch
  Prolog startBit(5, limits);

  // Get the time ... raw
  auto theTime = std::time(nullptr);
  auto t       = *std::localtime(&theTime);

  // Convert inch time to a string
  std::ostringstream oss;
  oss << startBit.getTime();
  auto when = oss.str();

  // Convert the raw time to a string
  std::ostringstream ss;
  ss << std::put_time(&t, "%Y-%m-%dT%H:%M:%S");
  auto now = ss.str();

  // Compare the strings
  REQUIRE_THAT(when, Catch::Matches(now));
}
