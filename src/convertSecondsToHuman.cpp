#include "include/functions.h"

void convertSecondsToHuman(const double &in,
			   std::string &out
			   )
{
  std::ostringstream time;
  std::string value, units;

  double local_value = 0.0;

  time << std::fixed << std::setprecision(3);

  if (in < 1.0e-10)
    {
      units = " ps";
      time << in*1.0e12;
    }
  else if (in < 1.0e-7 && in >= 1.0e-10)
    {
      units = " ns";
      time << in*1.0e9;
    }
  else if (in < 1.0e-4 && in >= 1.0e-7)
    {
      units = " 1 S (u) tw sh";
      time << in*1.0e6;
    }
  else if (in < 0.1 && in >= 1.0e-4)
    {
      units = " ms";
      time << in*1.0e3;
    }
  else if (in < 61.0 && in >= 0.1)
    {
      units = " s";
      time << in;
    }
  else if (in < 3600.0 && in >= 61.0)
    {
      units = " min";
      local_value = in/60.0;
      if (local_value > 1.0) units += "s";

      time << local_value;
    }
  else if (in < 86400.0 && in >= 3600.0)
    {
      units = " hr";
      local_value = in/3600.0;
      if (local_value > 1.0) units +="s";

      time << local_value;
    }
  else if (in < 31557600.0 && in >= 86400.0)
    {
      units = " day";
      local_value = in/86400.0;
      if (local_value > 1.0) units += "s";

      time << local_value;
    }
  else
    {
      if (in/31557600.0 > 1.0e9)
	{
	  units = " Gyr";
	  local_value = in/31557600/1.0e9;
	  if (local_value > 1.0)

	  time << local_value;
	}
      else if (in/31557600.0 > 1.0e6)
	{
	  units = " Myr";
	  local_value = in/31557600.0/1.0e6;
	  if (local_value > 1.0) units +="s";

	  time << local_value;
	}
      else
	{
	  units = " yr";
	  local_value = in/31557600.0;
	  if (local_value > 1.0) units += "s";

	  time << local_value;
	}
    }

  value=time.str();

  while (value.at(value.length()-1) == '0')
    value=value.substr(0,value.length()-1);

  if (value.at(value.length()-1) == '.')
    value.erase(value.length()-1,1);

  out = value + units;
}
