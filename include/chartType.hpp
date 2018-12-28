#ifndef CHARTTYPE_HPP
#define CHARTTYPE_HPP

#include <ostream>

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
  return os << static_cast<char>(97 + static_cast<int>(ct));
}

#endif // CHARTTYPE_HPP
