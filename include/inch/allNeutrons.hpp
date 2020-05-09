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
  return os << static_cast<int>(an);
}

#endif // ALLNEUTRONS_HPP
