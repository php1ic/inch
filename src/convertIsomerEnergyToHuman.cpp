#include "include/functions.h"

void convertIsomerEnergyToHuman(const double in,
                                std::string &out
                                )
{
  std::ostringstream num;

  if ( in < 1.0e3 )
    {
      num << in;
      out = num.str() + " keV";
    }
  else
    {
      num << in/1.0e3;
      out = num.str() + " MeV";
    }
}
