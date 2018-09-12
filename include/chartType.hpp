#ifndef CHARTTYPE_HPP
#define CHARTTYPE_HPP

#include <iostream>

enum class ChartType: char
  {
   EXPERIMENTAL = 'a',
   THEORETICAL = 'b',
   ALL = 'c'
  };

/// We print this value into the option file so need to overload
static std::ostream& operator<<(std::ostream& os, const ChartType& ct)
{
  return os << static_cast<char>(ct);
}

#endif // CHARTTYPE_HPP
