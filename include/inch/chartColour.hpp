#ifndef CHARTCOLOUR_HPP
#define CHARTCOLOUR_HPP

#include <ostream>

/// What property should the chart be coloured by
enum class ChartColour
{
  MASSEXCESSERROR     = 0,
  REL_MASSEXCESSERROR = 1,
  GS_DECAYMODE        = 2,
  GS_HALFLIFE         = 3,
  FIRST_ISOMERENERGY  = 4
};

/// We print this value into the option file so need to overload
inline std::ostream& operator<<(std::ostream& os, const ChartColour& cc) // NOLINT (fuchsia-overloaded-operator)
{
  return os << static_cast<int>(cc);
}

#endif // CHARTCOLOUR_HPP
