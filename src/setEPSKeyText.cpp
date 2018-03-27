#include "functions.hpp"

#include "converter.hpp"

void setEPSKeyText(const std::unique_ptr<inputs> &draw,
                   const std::unique_ptr<partition> &part,
                   std::vector<std::string> &keyString
                   )
{
  Converter convert;

  auto text = std::begin(keyString);

  if ( draw->choice == "a" )
    {
      auto low = convert.FloatToNdp(part->value[0], 1);
      auto high = convert.FloatToNdp(part->value[1], 1);

      *text = "1 TR (Stable \\() TotalWidth sh\n1 S (d) TotalWidth sh\n1 TR (m < ";
      *text += low;
      *text += " keV\\)) TotalWidth sh TestWidth\n";
      std::advance(text, 1);

      *text = "1 TR (Stable \\() TotalWidth sh\n1 S (d) TotalWidth sh\n1 TR (m > ";
      *text += low;
      *text += " keV\\)) TotalWidth sh TestWidth\n";
      std::advance(text, 1);

      *text = "1 S (d) TotalWidth sh\n1 TR (m < ";
      *text += low;
      *text += " keV) TotalWidth sh TestWidth\n";
      std::advance(text, 1);

      *text = "1 TR (  ";
      *text += low;
      *text += " keV < ) TotalWidth sh\n1 S (d) TotalWidth sh\n1 TR (m < ";
      *text += high;
      *text += " keV) TotalWidth sh TestWidth\n";
      std::advance(text, 1);

      int index = 1;
      while ( (text - std::begin(keyString)) < static_cast<int>(part->colour.size() - 1) )
        {
          low = high;
          high = convert.FloatToNdp(part->value[index+1], 1);
          *text = "1 TR (";
          *text += low;
          *text += " keV < ) TotalWidth sh\n1 S (d) TotalWidth sh\n1 TR (m < ";
          *text += high;
          *text += " keV) TotalWidth sh TestWidth\n";
          std::advance(text,1);
          ++index;
        }

      *text = "1 S (d) TotalWidth sh\n1 TR (m > ";
      *text += high;
      *text += " keV) TotalWidth sh TestWidth\n";
    }
  else if ( draw->choice == "b" )
    {
      auto low  = convert.FloatToExponent(part->value[0]);
      auto high = convert.FloatToExponent(part->value[1]);

      *text = "1 S (d) TotalWidth sh\n1 TR (m/m < ) TotalWidth sh\n";
      *text += std::get<0>(low);
      *text += " ";
      *text += std::get<1>(low);
      *text += std::get<2>(low);
      *text += " exponent TestWidth\n";
      std::advance(text, 1);

      *text = std::get<0>(low);
      *text += " ";
      *text += std::get<1>(low);
      *text += std::get<2>(low);
      *text += " exponent printUnit ";
      *text += std::get<0>(high);
      *text += " ";
      *text += std::get<1>(high);
      *text += std::get<2>(high);
      *text += " exponent TestWidth\n";
      std::advance(text, 1);

      int index = 1;
      while ( (text - std::begin(keyString)) < static_cast<int>(part->colour.size() - 1) )
        {
          low = high;
          high = convert.FloatToExponent(part->value[index+1]);

          *text = std::get<0>(low);
          *text += " ";
          *text += std::get<1>(low);;
          *text += std::get<2>(low);
          *text += " exponent printUnit ";
          *text += std::get<0>(high);
          *text += " ";
          *text += std::get<1>(high);
          *text += std::get<2>(high);
          *text += " exponent TestWidth\n";
          std::advance(text, 1);
          ++index;
        }

      *text = "1 S (d) TotalWidth sh\n1 TR (m/m > ) TotalWidth sh\n";
      *text += std::get<0>(high);
      *text += " ";
      *text += std::get<1>(high);
      *text += std::get<2>(high);
      *text += " exponent TestWidth\n";
    }
  else if ( draw->choice == "c" )
    {
      *text = "1 TR (Stable) TotalWidth sh TestWidth\n";
      std::advance(text, 1);
      *text = "1 TR (Alpha) TotalWidth sh TestWidth\n";
      std::advance(text, 1);
      *text = "1 S (b) TotalWidth sh\n0.5 TR 0 0.55 rmoveto (+) TotalWidth sh TestWidth\n";
      std::advance(text, 1);
      *text = "1 S (b) TotalWidth sh\n0.75 TR 0 0.55 rmoveto (-) TotalWidth sh TestWidth\n";
      std::advance(text, 1);
      *text = "1 TR (Spontaneous Fission)TotalWidth sh TestWidth\n";
      std::advance(text, 1);
      *text = "1 TR (n decay) TotalWidth sh TestWidth\n";
      std::advance(text, 1);
      *text = "1 TR (2n decay) TotalWidth sh TestWidth\n";
      std::advance(text, 1);
      *text = "1 TR (p decay) TotalWidth sh TestWidth\n";
      std::advance(text, 1);
      *text = "1 TR (2p decay) TotalWidth sh TestWidth\n";
      std::advance(text, 1);
      *text = "1 TR (Unknown) TotalWidth sh TestWidth\n";
      std::advance(text, 1);
      *text = "1 TR (Electron Capture) TotalWidth sh TestWidth\n";
    }
  else if ( draw->choice == "d" )
    {
      std::string low = convert.SecondsToHuman(part->value[0]);
      std::string high = convert.SecondsToHuman(part->value[1]);

      *text = "printUnit 1 TR (     < ";
      *text += low;
      *text += ") TotalWidth sh TestWidth\n";
      std::advance(text, 1);

      *text = "1 TR (";
      *text += low;
      *text += " < ) TotalWidth sh printUnit\n(     < ";
      *text += high;
      *text += ") TotalWidth sh TestWidth\n";
      std::advance(text, 1);

      int index = 1;
      while ( (text - std::begin(keyString)) < static_cast<int>(part->colour.size() - 1) )
        {
          low = high;
          high = convert.SecondsToHuman(part->value[index+1]);

          *text = "1 TR (";
          *text += low;
          *text += " < ) TotalWidth sh printUnit\n(     < ";
          *text += high;
          *text += ") TotalWidth sh TestWidth\n";
          std::advance(text, 1);
          ++index;
        }

      *text = "printUnit 1 TR (     > ";
      *text += high;
      *text += ") TotalWidth sh TestWidth\n";
    }
  else if ( draw->choice == "e" )
    {
      auto low  = convert.IsomerEnergyToHuman(part->value[0]);
      auto high = convert.IsomerEnergyToHuman(part->value[1]);

      *text = "1 TR (E < ";
      *text += low;
      *text += ") TotalWidth sh TestWidth\n";
      std::advance(text, 1);

      *text = "1 TR (";
      *text += low;
      *text += " < E < ";
      *text += high;
      *text += ") TotalWidth sh TestWidth\n";
      std::advance(text, 1);

      int index = 1;
      while ( (text - std::begin(keyString)) < static_cast<int>(part->colour.size() - 1) )
        {
          low = high;
          high = convert.IsomerEnergyToHuman(part->value[index]);

          *text = "1 TR (";
          *text += low;
          *text +=" < E < ";
          *text += high;
          *text += ") TotalWidth sh TestWidth\n";
          std::advance(text, 1);
          ++index;
        }

      *text = "1 TR (E > ";
      *text += high;
      *text += ") TotalWidth sh TestWidth\n";
      std::advance(text, 1);

      *text = "1 TR (No known isomer) TotalWidth sh TestWidth\n";
    }
}
