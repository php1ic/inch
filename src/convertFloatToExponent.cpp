#include "functions.hpp"

std::pair<std::string, std::string> convertFloatToExponent(const double in)
{
  /// 'in' will be of the form [0-9]+\.[0-9]+e-[0-9]+
  std::ostringstream num;

  /// Stop conversion to a normal number
  num << std::scientific << in;

  std::pair<std::string, std::string> out;

  /// Get the coefficient part i.e. before 'e-'
  out.first = num.str().erase(num.str().find("e-"));
  /// Get the exponent part i.e. after 'e-'
  out.second = num.str().substr(num.str().find("e-")+2);

  /// Remove trailing zeros from coefficient
  out.first.erase(out.first.find_last_not_of('0')+1);

  /// Remove decimal point if it's the last character
  if ( out.first.back() == '.' )
    {
      out.first.pop_back();
    }

  /// Remove leading zeros from exponent
  out.second.erase(0,out.second.find_first_not_of('0'));

  return out;
}
