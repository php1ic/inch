#include "inch/magicNumbers.hpp"

#include "fmt/format.h"

#include <cstddef>
#include <utility>


std::string MagicNumbers::EPSWriteProtonNumber(const int number) const
{
  const Number position = coords.at(number);

  const int min = (position.min_n - Nmin < 0) ? min_val : position.min_n - Nmin;

  const int max = ((position.max_n - Nmin) > (Nmax - Nmin + max_val)) ? Nmax - Nmin + max_val : position.max_n - Nmin;

  return fmt::format("%Z={0}\n"
                     "{1} {2} m {3} 0 rl\n"
                     "{1} {4} m {3} 0 rl st\n",
                     number,
                     min,
                     (number - Zmin + 1),
                     (max - min),
                     (number - Zmin));
}


std::string MagicNumbers::EPSWriteNeutronNumber(const int number) const
{
  const Number position = coords.at(number);

  const int min = (position.min_z - Zmin < 0) ? min_val : position.min_z - Zmin;

  const int max = (position.max_z - Zmin > (Zmax - Zmin + max_val)) ? Zmax - Zmin + max_val : position.max_z - Zmin;

  return fmt::format("%N={0}\n"
                     "{1} {2} m 0 {3} rl\n"
                     "{4} {2} m 0 {3} rl st\n",
                     number,
                     (number - Nmin + 1),
                     min,
                     (max - min),
                     (number - Nmin));
}
