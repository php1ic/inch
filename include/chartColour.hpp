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
  /// The latin alphabet (lower case) starts at int/unicode value 97,
  /// add this to the casted underlying type and cast the result
  /// so we print a char (i.e. what the user is asked for)
  constexpr int first_abc = 97;

  return os << static_cast<char>(first_abc + static_cast<int>(cc));
}

#endif // CHARTCOLOUR_HPP
