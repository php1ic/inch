#include "functions.h"

void convertFloatToExponent(const double in,
                            std::vector<std::string> &out
                            )
{
  /// 'in' will be of the form [0-9]+\.[0-9]+e-[0-9]+
  std::ostringstream num;

  /// Stop conversion to a normal number
  num << std::scientific << in;

  /// Get the coefficient part i.e. before 'e-'
  out[0] = num.str().erase(num.str().find("e-"));
  /// Get the exponent part i.e. after 'e-'
  out[1] = num.str().substr(num.str().find("e-")+2);

  /// Remove trailing zeros from coefficient
  out[0].erase(out[0].find_last_not_of('0')+1);

  /// Remove decimal point if it's the last character
  if ( *out[0].crbegin() == '.' )
    {
      out[0].pop_back();
    }

  /// Remove leading zeros from exponent
  out[1].erase(0,out[1].find_first_not_of('0'));
}
