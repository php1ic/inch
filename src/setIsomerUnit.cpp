#include "functions.h"

void setIsomerUnit(const float &in, std::string &out)
{
  std::ostringstream num;
  std::string unit;

  if(in < 1.0e3)
    {
      num << in;
      unit = " keV";
    }
  else
    {
      num << in/1.0e3;
      unit = " MeV";
    }

  out = num.str() + unit;
}
