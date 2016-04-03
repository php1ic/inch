#include "functions.h"

void convertFloatToExponent(const double in,
                            std::vector<std::string> &out
                            )
{
  /// 'in' will be of the form [0-9]*e-[0-9]*
  std::ostringstream num;

  /// Stop conversion to a normal number
  num << std::scientific << in;

  /// Get the initial part i.e. before 'e-'
  out[0]=num.str().erase(num.str().find('e'));

  /// Remove trailing zeros
  out[0].erase(out[0].find_last_not_of('0')+1);
  /// Remove decimal point if it's the last character
  if (out[0].at(out[0].length()-1) == '.')
    out[0].erase(out[0].length()-1);

  /// Get the final part i.e. after 'e-'
  out[1]=num.str().erase(0,num.str().find('-')+1);

  /// Remove leading zeros
  out[1].erase(0,out[1].find_first_not_of('0'));
}
