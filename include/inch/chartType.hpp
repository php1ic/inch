#ifndef CHARTTYPE_HPP
#define CHARTTYPE_HPP

#include <ostream>

/// Which isotopes should we draw, measured, extrapolated or both
enum class ChartType
{
  EXPERIMENTAL = 0,
  THEORETICAL  = 1,
  ALL          = 2
};

/// We print this value into the option file so need to overload
inline std::ostream& operator<<(std::ostream& os, const ChartType& ct) // NOLINT (fuchsia-overloaded-operator)
{
  /// The latin alphabet (lower case) starts at int/unicode value 97,
  /// add this to the casted underlying type and cast the result
  /// so we print a char (i.e. what the user is asked for)
  constexpr int first_abc = 97;

  return os << static_cast<char>(first_abc + static_cast<int>(ct));
}

#endif // CHARTTYPE_HPP
