#include "inch/grid.hpp"

#include "fmt/format.h"

std::string Grid::EPSDrawGrid(const int width, const int height, const int spacing)
{
  return fmt::format("\n%Square grid\n"
                     "gs\n"
                     "0.75 setgray\n"
                     "2 TR\n"
                     "0.1 u div sl\n"
                     "{0} {0} {1} {{\n"
                     "/i ed\n"
                     "i 1 m\n"
                     "gs\n"
                     "i 10 mod 0 eq {{\n"
                     "i 3 string cvs sh\n"
                     "i {2} m i 3 string cvs sh\n"
                     "}} if\n"
                     "gr\n"
                     "0 {3} rl\n"
                     "}} for\n"
                     "{0} {0} {2} {{\n"
                     "/i ed\n"
                     "1 i m\n"
                     "gs\n"
                     "i 10 mod 0 eq {{\n"
                     "i 3 string cvs sh\n"
                     "{1} i m i 3 string cvs sh\n"
                     "}} if\n"
                     "gr\n"
                     "{4} 0 rl\n"
                     "}} for\n"
                     "st\n"
                     "gr\n"
                     "%---------------\n\n",
                     spacing,
                     width,
                     height,
                     height - 2,
                     width - 2);
}
