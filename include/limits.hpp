#ifndef LIMITS_HPP
#define LIMITS_HPP

#include <ostream>

/// Min and max values of N & Z, define here, and only here
enum Limits
{
  MIN_Z = 0,
  MAX_Z = 118,
  MIN_N = 0,
  MAX_N = 177
};


/// We print this value so need to overload
inline std::ostream& operator<<(std::ostream& os, const Limits& lim) // NOLINT (fuchsia-overloaded-operator)
{
  /// The latin alphabet (lower case) starts at int/unicode value 97,
  /// add this to the casted underlying type and cast the result
  /// so we print a char (i.e. what the user is asked for)
  constexpr int first_abc = 97;

  return os << static_cast<char>(first_abc + static_cast<int>(lim));
}

#endif // LIMITS_HPP
