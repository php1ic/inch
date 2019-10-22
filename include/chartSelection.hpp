#ifndef CHARTSELECTION_HPP
#define CHARTSELECTION_HPP

#include <ostream>

/// Is all of the chart drawn, i.e. full Z range, or just part of it
enum class ChartSelection
{
  FULL_CHART = 0,
  SUB_CHART  = 1
};

/// We print this value into the option file so need to overload
inline std::ostream& operator<<(std::ostream& os, const ChartSelection& ct) // NOLINT (fuchsia-overloaded-operator)
{
  /// The latin alphabet (lower case) starts at int/unicode value 97,
  /// add this to the casted underlying type and cast the result
  /// so we print a char (i.e. what the user is asked for)
  constexpr int first_abc = 97;

  return os << static_cast<char>(first_abc + static_cast<int>(ct));
}

#endif // CHARTSELECTION_HPP
