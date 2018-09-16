#ifndef CHARTCOLOUR_HPP
#define CHARTCOLOUR_HPP

#include <iostream>

enum class ChartColour
  {
   MASSEXCESSERROR = 0,
   REL_MASSEXCESSERROR = 1,
   GS_DECAYMODE = 2,
   GS_HALFLIFE = 3,
   FIRST_ISOMERENERGY = 4
  };


/// We print this value into the option file so need to overload
static std::ostream& operator<<(std::ostream& os, const ChartColour& cc)
{
  /// The latin alphabet (lower case) starts at int/unicode value 97,
  /// add this to the casted underlying type and cast the result
  /// so we print a char (i.e. what the user is asked for)
  return os << static_cast<char>(97 + static_cast<int>(cc));
}

#endif // CHARTCOLOUR_HPP
