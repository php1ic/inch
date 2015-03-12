#include "include/functions.h"

void convertSecondsToHuman(const double in,
			   std::string &out
			   )
{
  std::ostringstream time;
  std::string units;

  double localValue = 0.0;

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
      localValue = in/60.0;
      if (localValue > 1.0) units += "s";

      time << localValue;
    }
  else if (in < 86400.0 && in >= 3600.0)
    {
      units = " hr";
      localValue = in/3600.0;
      if (localValue > 1.0) units +="s";

      time << localValue;
    }
  else if (in < 31557600.0 && in >= 86400.0)
    {
      units = " day";
      localValue = in/86400.0;
      if (localValue > 1.0) units += "s";

      time << localValue;
    }
  else
    {
      if (in/31557600.0 >= 1.0e9)
        {
          units = " Gyr";
          localValue = in/31557600.0/1.0e9;
          if (localValue > 1.0) units += "s";

          time << localValue;
        }
      else if (in/31557600.0 >= 1.0e6)
        {
          units = " Myr";
          localValue = in/31557600.0/1.0e6;
          if (localValue > 1.0) units +="s";

          time << localValue;
        }
      else
        {
          units = " yr";
          localValue = in/31557600.0;
          if (localValue > 1.0) units += "s";

          time << localValue;
        }
    }

  std::string value(time.str());

  /// Remove trailing zeros
  value.erase(value.find_last_not_of('0')+1);

  /// Remove decimal point
  if (value.at(value.length()-1) == '.')
    value.erase(value.length()-1);

  out = value + units;
}
