#ifndef CHARTSELECTION_HPP
#define CHARTSELECTION_HPP

#include <iostream>

enum class ChartSelection: char
  {
   FULL_CHART = 'a',
   SUB_CHART = 'b'
  };

/// We print this value into the option file so need to overload
static std::ostream& operator<<(std::ostream& os, const ChartSelection& cs)
{
  return os << static_cast<char>(cs);
}

#endif // CHARTSELECTION_HPP
