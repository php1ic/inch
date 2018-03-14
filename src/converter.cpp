#include "converter.hpp"

std::pair<std::string, std::string> Converter::FloatToExponent(const double in) const
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


std::string Converter::IsomerEnergyToHuman(const double in) const
{
  std::string out;
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

  return out;
}


std::string Converter::SecondsToHuman(const double in) const
{
  std::ostringstream time;
  std::string units;

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
      if (in/60.0 > 1.0)
        {
          units += "s";
        }

      time << in/60.0;
    }
  else if (in < 86400.0 && in >= 3600.0)
    {
      units = " hr";
      if (in/3600.0 > 1.0)
        {
          units +="s";
        }

      time << in/3600.0;
    }
  else if (in < 31557600.0 && in >= 86400.0)
    {
      units = " day";
      if (in/86400.0 > 1.0)
        {
          units += "s";
        }

      time << in/86400.0;
    }
  else
    {
      if (in/31557600.0 >= 1.0e9)
        {
          units = " Gyr";
          if (in/31557600.0e9 > 1.0)
            {
              units += "s";
            }

          time << in/31557600.0e9;
        }
      else if (in/31557600.0 >= 1.0e6)
        {
          units = " Myr";
          if (in/31557600.0e6 > 1.0)
            {
              units += "s";
            }

          time << in/31557600.0e6;
        }
      else
        {
          units = " yr";
          if (in/31557600.0 > 1.0)
            {
              units += "s";
            }

          time << in/31557600.0;
        }
    }

  std::string value = time.str();

  /// Remove trailing zeros
  value.erase(value.find_last_not_of('0')+1);

  /// Remove decimal point if it's the last character
  if ( value.back() == '.' )
    {
      value.pop_back();
    }

  return value + units;
}
