#include <catch/catch.hpp>

#include <sstream>

#include "grid.hpp"

const Grid grid;

const std::string gridtext =
R"(
%Square grid
gs
0.75 setgray
2 TR
0.1 u div sl

1 1 100{
/i ed
i 1 m
gs
i 10 mod 0 eq {
i 3 string cvs sh
i 100 m i 3 string cvs sh
} if
gr
0 98 rl
} for

1 1 100{
/i ed
1 i m
gs
i 10 mod 0 eq {
i 3 string cvs sh
100 i m i 3 string cvs sh
} if
gr
98 0 rl
} for

st
gr
%---------------

)";

TEST_CASE("Create an eps grid", "[Grid]")
{
  std::ostringstream ss;
  grid.EPSDrawGrid(ss, 100, 100, 1);
  REQUIRE( ss.str() == gridtext );
}
