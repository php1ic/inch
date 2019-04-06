#ifndef ALLNEUTRONS_HPP
#define ALLNEUTRONS_HPP

#include <ostream>

/// For the given Z range, should all neutrons be drawn
enum AllNeutrons
{
  YES = 0,
  NO  = 1
};

/// We print this value into the option file so need to overload
inline std::ostream& operator<<(std::ostream& os, const AllNeutrons& an) // NOLINT (fuchsia-overloaded-operator)
{
  /// The latin alphabet (lower case) starts at int/unicode value 97,
  /// add this to the casted underlying type and cast the result
  /// so we print a char (i.e. what the user is asked for)
  return os << static_cast<char>(97 + static_cast<int>(an));
}


#endif // ALLNEUTRONS_HPP
