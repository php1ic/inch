#include "inch/limits.hpp"

#include "inch/converter.hpp"

#include <iostream>


bool Limits::setZmin(const int value) const
{
  const bool valid = (value >= MIN_Z && value <= Zmax);

  if (valid)
    {
      Zmin = value;
    }
  else
    {
      fmt::print("\nZmin must be in the range {}<=Z<={}\n\n", MIN_Z, Zmax);
    }

  return valid;
}


bool Limits::setZmax(const int value) const
{
  const bool valid = (value <= MAX_Z && value > Zmin);

  if (valid)
    {
      Zmax = value;
    }
  else
    {
      fmt::print("\nZmax must be in the range {}<Z<={}\n\n", Zmin, MAX_Z);
    }

  return valid;
}


bool Limits::setNmin(const int value) const
{
  const bool valid = (value >= MIN_N && value <= Nmax);

  if (valid)
    {
      Nmin = value;
    }
  else
    {
      fmt::print("\nNmin must be in the range {}<=N<={}\n\n", MIN_N, Nmax);
    }

  return valid;
}


bool Limits::setNmax(const int value) const
{
  const bool valid = (value <= MAX_N && value > Nmin);

  if (valid)
    {
      Nmax = value;
    }
  else
    {
      fmt::print("\nNmax must be in the range {}<N<={}\n\n", Nmin, MAX_N);
    }

  return valid;
}


void Limits::setExtreme(std::string_view limit) const
{
  if (limit != "Zmin" && limit != "Zmax" && limit != "Nmin" && limit != "Nmax")
    {
      fmt::print("**WARNING** - {} is not a valid input\n"
                 "              choose either Zmin, Zmax, Nmin or Nmax\n"
                 "Setting limits to maxima values\n",
                 limit);

      ResetAllLimits();

      return;
    }

  bool valid{ false };

  do
    {
      std::string in;

      fmt::print("{}: ", limit);
      std::cin >> in;

      const int number = Converter::StringToInt(in);
      valid            = true;

      // Validate the number
      if (limit == "Zmin")
        {
          valid = setZmin(number);
        }
      else if (limit == "Zmax")
        {
          valid = setZmax(number);
        }
      else if (limit == "Nmin")
        {
          valid = setNmin(number);
        }
      else if (limit == "Nmax")
        {
          valid = setNmax(number);
        }
    }
  while (!valid);
}
