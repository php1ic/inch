#ifndef CHARTTYPE_HPP
#define CHARTTYPE_HPP

/// Which isotopes should we draw, measured, extrapolated or both
enum class ChartType
{
  EXPERIMENTAL = 0,
  THEORETICAL  = 1,
  ALL          = 2
};

#endif // CHARTTYPE_HPP
