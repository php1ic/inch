#include "converter.hpp"

std::string Converter::FloatToNdp(const double number, const int numDP) const
{
  std::string value = std::to_string(number);

  int digits = (numDP < 1) ? -1 : numDP+1;

  return value.substr(0, value.find('.') + digits);
}


std::tuple<std::string, std::string, std::string> Converter::FloatToExponent(const double in) const
{
  /// Force the number to follow the regex: -?\d*\.?\d+e[+-]?\d+
  std::ostringstream num;
  num << std::scientific << in;

  std::regex pieces_regex(R"((-?\d*\.?\d+)e([+-]?)(\d+))");
  std::smatch matches;

  /// Need to convert before passing to regex_match
  std::string number = num.str();

  /// Always get 1dp from the first number
  /// If it's negative take an extra char for the sign
  int digits = ( in < 0.0 ) ? 4 : 3;

  return ( std::regex_match(number, matches, pieces_regex) )
    //                coefficient(1dp)                          exponent sign           exponent
    ? std::make_tuple(std::string(matches[1]).substr(0, digits),std::string(matches[2]),std::string(matches[3]))
    : std::make_tuple(std::string(), std::string(), std::string());
}


std::string Converter::IsomerEnergyToHuman(const double in) const
{
  return ( in < 1.0e3 )
    ? FloatToNdp(in, 1) + " keV"
    : FloatToNdp(in/1.0e3, 1) + " MeV";
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
