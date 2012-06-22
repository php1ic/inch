#include "functions.h"

void sec_human(const float &in, std::string &out)
{
  std::ostringstream time;
  std::string value,units;

  if (in < 1.0e-10)
    {
      units = " ps";
      time << std::fixed << std::setprecision(3) << in*1.0e12;
    }
  else if (in < 1.0e-7 && in >= 1.0e-10)
    {
      units = " ns";
      time << std::fixed << std::setprecision(3) << in*1.0e9;
    }
  else if (in < 1.0e-4 && in >= 1.0e-7)
    {
      units = " 1 S (u) tw sh";
      time << std::fixed << std::setprecision(3) << in*1.0e6;
    }
  else if (in < 0.1 && in >= 1.0e-4)
    {
      units = " ms";
      time << std::fixed << std::setprecision(3) << in*1.0e3;
    }
  else if (in < 61.0 && in >= 0.1)
    {
      units = " s";
      time << std::fixed << std::setprecision(3) << in;
    }
  else if (in < 3600.0 && in >= 61.0)
    {
      units = " mins";
      if (in/60.0==1.0) units=" min";
      time << std::fixed << std::setprecision(3) << in/60.0;
    }
  else if (in < 86400.0 && in >= 3600.0)
    {
      units = " hrs";
      if (in/3600.0==1.0) units=" hr";
      time << std::fixed << std::setprecision(3) << in/3600.0;
    }
  else if (in < 31557600.0 && in >= 86400.0)
    {
      units = " days";
      if (in/86400.0==1.0) units=" day";
      time << std::fixed << std::setprecision(3) << in/86400.0;
    }
  else
    {
      if (in/31557600.0 > 1.0e9)
	{
	  time << std::fixed << std::setprecision(3) << in/31557600.0/1.0e9;
	  units = " Gyrs";
	  if (in/31557600.0/1.0e9==1.0) units=" Myr";
	}
      else if (in/31557600.0 > 1.0e6)
	{
	  time << std::fixed << std::setprecision(3) << in/31557600.0/1.0e6;
	  units = " Myrs";
	  if (in/31557600.0/1.0e6==1.0) units=" Myr";
	}
      else
	{
	  time << std::fixed << std::setprecision(3) << in/31557600.0;
	  units = " yrs";
	  if (in/31557600.0==1.0) units=" yr";
	}
    }

  value=time.str();

  while (value.at(value.length()-1) == '0')
    value=value.substr(0,value.length()-1);

  if (value.at(value.length()-1) == '.')
    value.erase(value.length()-1,1);

  out = value + units;
}
