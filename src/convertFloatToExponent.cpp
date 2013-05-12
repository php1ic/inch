#include "include/functions.h"

void convertFloatToExponent(const float &in,
			    std::vector<std::string> &out
			    )
{
  std::ostringstream num;
  std::string value;

  num << std::scientific << in;

  value=num.str();
  value=value.substr(0,value.find('e'));

  while (value.at(value.length()-1) == '0')
    value=value.substr(0,value.length()-1);

  if (value.at(value.length()-1) == '.')
    value.erase(value.length()-1,1);

  out[0]=value;

  value=num.str();
  value=value.substr(value.find('-')+1);

  while (value.at(0) == '0')
    value.erase(0,1);

  out[1]=value;
}
