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
  return os << static_cast<int>(ct);
}

#endif // CHARTTYPE_HPP
