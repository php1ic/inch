#ifndef CHARTSELECTION_HPP
#define CHARTSELECTION_HPP

#include <iostream>

enum class ChartSelection
  {
   FULL_CHART = 0,
   SUB_CHART = 1
  };

/// We print this value into the option file so need to overload
static std::ostream& operator<<(std::ostream& os, const ChartSelection& ct) // NOLINT (fuchsia-overloaded-operator)
{
  /// The latin alphabet (lower case) starts at int/unicode value 97,
  /// add this to the casted underlying type and cast the result
  /// so we print a char (i.e. what the user is asked for)
  return os << static_cast<char>(97 + static_cast<int>(ct));
}

#endif // CHARTSELECTION_HPP
