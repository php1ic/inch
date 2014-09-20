#include "include/functions.h"

void convertFloatToExponent(const double &in,
			    std::vector<std::string> &out
			    )
{
  /// 'in' will be of the form [0-9]*e-[0-9]*
  std::ostringstream num;
  std::string value;

  /// Stop conversion to a normal number
  num << std::scientific << in;

  /// Get the initial part i.e. before 'e-'
  value=num.str();
  value=value.substr(0,value.find('e'));

  /// Remove trailing zeros
  value.erase(value.find_last_not_of('0')+1);
  /// Remove decimal point
  if (value.at(value.length()-1) == '.')
    value.erase(value.length()-1);

  out[0]=value;

  /// Get the final part i.e. after 'e-'
  value=num.str();
  value=value.substr(value.find('-')+1);

  /// Remove leading zeros
  value.erase(0,value.find_first_not_of('0'));

  out[1]=value;
}
