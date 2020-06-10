#include "inch/magicNumbers.hpp"

#include <fmt/format.h>

#include <utility>


std::string MagicNumbers::EPSWriteProtonNumber(const int number) const
{
  const Number position = coords.at(number);

  const int start_pos = position.min_n - limits.Nmin;
  const int end_pos   = position.max_n - limits.Nmin;

  const int min = (start_pos < 0) ? min_val : start_pos;
  const int max = (end_pos > (limits.getNRange() + max_val)) ? limits.getNRange() + max_val : end_pos;

  return fmt::format("%Z={0}\n"
                     "{1} {2} m {3} 0 rl\n"
                     "{1} {4} m {3} 0 rl st\n",
                     number,
                     min,
                     (number - limits.Zmin + 1),
                     (max - min),
                     (number - limits.Zmin));
}


std::string MagicNumbers::EPSWriteNeutronNumber(const int number) const
{
  const Number position = coords.at(number);

  const int start_pos = position.min_z - limits.Zmin;
  const int end_pos   = position.max_z - limits.Zmin;

  const int min = (start_pos < 0) ? min_val : start_pos;
  const int max = (end_pos > (limits.getZRange() + max_val)) ? limits.getZRange() + max_val : end_pos;

  return fmt::format("%N={0}\n"
                     "{1} {2} m 0 {3} rl\n"
                     "{4} {2} m 0 {3} rl st\n",
                     number,
                     (number - limits.Nmin + 1),
                     min,
                     (max - min),
                     (number - limits.Nmin));
}
