#include "inch/grid.hpp"

#include <catch2/catch.hpp>
#include <fmt/format.h>

#include <iostream>
#include <sstream>

std::string gridtext = fmt::format("\n%Square grid\n"
                                   "gs\n"
                                   "0.75 setgray\n"
                                   "2 TR\n"
                                   "0.1 u div sl\n"
                                   "1 1 100 {{\n"
                                   "/i ed\n"
                                   "i 1 m\n"
                                   "gs\n"
                                   "i 10 mod 0 eq {{\n"
                                   "i 3 string cvs sh\n"
                                   "i 100 m i 3 string cvs sh\n"
                                   "}} if\n"
                                   "gr\n"
                                   "0 98 rl\n"
                                   "}} for\n"
                                   "1 1 100 {{\n"
                                   "/i ed\n"
                                   "1 i m\n"
                                   "gs\n"
                                   "i 10 mod 0 eq {{\n"
                                   "i 3 string cvs sh\n"
                                   "100 i m i 3 string cvs sh\n"
                                   "}} if\n"
                                   "gr\n"
                                   "98 0 rl\n"
                                   "}} for\n"
                                   "st\n"
                                   "gr\n"
                                   "%---------------\n\n");

TEST_CASE("Create an eps grid", "[Grid]")
{
  REQUIRE(Grid::EPSDrawGrid(100, 100, 1) == gridtext);
}
