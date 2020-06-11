#include "inch/limits.hpp"

#include <fmt/core.h>
#include <fmt/format.h>

bool Limits::setZmin(const int value) const
{
  const bool valid = (value < MIN_Z || value > Zmax) ? false : true;

  if (valid)
    {
      Zmin = value;
    }
  else
    {
      fmt::print("\nZmin must be in the range {}<Z<{}\n\n", MIN_Z, Zmax);
    }

  return valid;
}


bool Limits::setZmax(const int value) const
{
  const bool valid = (value > MAX_Z || value < Zmin) ? false : true;

  if (valid)
    {
      Zmax = value;
    }
  else
    {
      fmt::print("\nZmax must be in the range {}<Z<{}\n\n", Zmin, MAX_Z);
    }

  return valid;
}


bool Limits::setNmin(const int value) const
{
  const bool valid = (value < MIN_N || value > Nmax) ? false : true;

  if (valid)
    {
      Nmin = value;
    }
  else
    {
      fmt::print("\nNmin must be in the range {}<N<{}\n\n", MIN_N, Nmax);
    }

  return valid;
}


bool Limits::setNmax(const int value) const
{
  const bool valid = (value > MAX_N || value < Nmin) ? false : true;

  if (valid)
    {
      Nmax = value;
    }
  else
    {
      fmt::print("\nNmax must be in the range {}<N<{}\n\n", Nmin, MAX_N);
    }

  return valid;
}
