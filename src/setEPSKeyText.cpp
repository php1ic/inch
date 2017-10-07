#include "functions.h"

void setEPSKeyText(const std::unique_ptr<inputs> &draw,
                   const std::unique_ptr<partition> &part,
                   std::vector<std::string> &keyString
                   )
{
  if ( draw->choice == "a" )
    {
      std::ostringstream low;
      std::ostringstream high;

      low << part->value[0];
      high << part->value[1];

      keyString[0] = "1 TR (Stable \\() TotalWidth sh\n1 S (d) TotalWidth sh\n1 TR (m < ";
      keyString[0] += low.str();
      keyString[0] += " keV\\)) TotalWidth sh TestWidth\n";

      keyString[1] = "1 TR (Stable \\() TotalWidth sh\n1 S (d) TotalWidth sh\n1 TR (m > ";
      keyString[1] += low.str();
      keyString[1] += " keV\\)) TotalWidth sh TestWidth\n";

      keyString[2] = "1 S (d) TotalWidth sh\n1 TR (m < ";
      keyString[2] += low.str();
      keyString[2] += " keV) TotalWidth sh TestWidth\n";

      keyString[3] = "1 TR (  ";
      keyString[3] += low.str();
      keyString[3] += " keV < ) TotalWidth sh\n1 S (d) TotalWidth sh\n1 TR (m < ";
      keyString[3] += high.str();
      keyString[3] += " keV) TotalWidth sh TestWidth\n";

      for ( int i=1; i<4; ++i )
        {
          low.str("");
          low << part->value[i];

          high.str("");
          high << part->value[i+1];

          keyString[i+3] = "1 TR (";
          keyString[i+3] += low.str();
          keyString[i+3] += " keV < ) TotalWidth sh\n1 S (d) TotalWidth sh\n1 TR (m < ";
          keyString[i+3] += high.str();
          keyString[i+3] += " keV) TotalWidth sh TestWidth\n";
        }

      keyString[7] = "1 S (d) TotalWidth sh\n1 TR (m > ";
      keyString[7] += high.str();
      keyString[7] += " keV) TotalWidth sh TestWidth\n";
    }
  else if ( draw->choice == "b" )
    {
      std::pair<std::string, std::string> low  = convertFloatToExponent(part->value[0]);
      std::pair<std::string, std::string> high = convertFloatToExponent(part->value[1]);


      keyString[0] = "1 S (d) TotalWidth sh\n1 TR (m/m < ) TotalWidth sh\n";
      keyString[0] += low.first;
      keyString[0] += " -";
      keyString[0] += low.first;
      keyString[0] += " exponent TestWidth\n";

      keyString[1] = low.first;
      keyString[1] += " -";
      keyString[1] += low.first;
      keyString[1] += " exponent printUnit ";
      keyString[1] += high.first;
      keyString[1] += " -";
      keyString[1] += high.first;
      keyString[1] += " exponent TestWidth\n";

      for ( int i=1; i<4; ++i )
        {
          low = convertFloatToExponent(part->value[i]);
          high = convertFloatToExponent(part->value[i+1]);

          keyString[i+1] = low.first;
          keyString[i+1] += " -";
          keyString[i+1] += low.first;
          keyString[i+1] += " exponent printUnit ";
          keyString[i+1] += high.first;
          keyString[i+1] += " -";
          keyString[i+1] += high.first;
          keyString[i+1] += " exponent TestWidth\n";
        }

      keyString[5] = "1 S (d) TotalWidth sh\n1 TR (m/m > ) TotalWidth sh\n";
      keyString[5] += high.first;
      keyString[5] += " -";
      keyString[5] += high.first;
      keyString[5] += " exponent TestWidth\n";
    }
  else if ( draw->choice == "c" )
    {
      keyString[0]  = "1 TR (Stable) TotalWidth sh TestWidth\n";
      keyString[1]  = "1 TR (Alpha) TotalWidth sh TestWidth\n";
      keyString[2]  = "1 S (b) TotalWidth sh\n0.5 TR 0 0.55 rmoveto (+) TotalWidth sh TestWidth\n";
      keyString[3]  = "1 S (b) TotalWidth sh\n0.75 TR 0 0.55 rmoveto (-) TotalWidth sh TestWidth\n";
      keyString[4]  = "1 TR (Spontaneous Fission)TotalWidth sh TestWidth\n";
      keyString[5]  = "1 TR (n decay) TotalWidth sh TestWidth\n";
      keyString[6]  = "1 TR (2n decay) TotalWidth sh TestWidth\n";
      keyString[7]  = "1 TR (p decay) TotalWidth sh TestWidth\n";
      keyString[8]  = "1 TR (2p decay) TotalWidth sh TestWidth\n";
      keyString[9]  = "1 TR (Unknown) TotalWidth sh TestWidth\n";
      keyString[10] = "1 TR (Electron Capture) TotalWidth sh TestWidth\n";
    }
  else if ( draw->choice == "d" )
    {
      std::string low =convertSecondsToHuman(part->value[0]);
      std::string high =convertSecondsToHuman(part->value[1]);

      keyString[0] = "printUnit 1 TR (     < ";
      keyString[0] += low;
      keyString[0] += ") TotalWidth sh TestWidth\n";

      keyString[1] = "1 TR (";
      keyString[1] += low;
      keyString[1] += " < ) TotalWidth sh printUnit\n(     < ";
      keyString[1] += high;
      keyString[1] += ") TotalWidth sh TestWidth\n";

      for ( int i=2; i<7; ++i )
        {
          low=high;
          high = convertSecondsToHuman(part->value[i]);

          keyString[i] = "1 TR (";
          keyString[i] += low;
          keyString[i] += " < ) TotalWidth sh printUnit\n(     < ";
          keyString[i] += high;
          keyString[i] += ") TotalWidth sh TestWidth\n";
        }

      keyString[7] = "printUnit 1 TR (     > ";
      keyString[7] += high;
      keyString[7] += ") TotalWidth sh TestWidth\n";
    }
  else if ( draw->choice == "e" )
    {
      std::string low  = convertIsomerEnergyToHuman(part->value[0]);
      std::string high = convertIsomerEnergyToHuman(part->value[1]);

      keyString[0] = "1 TR (E < ";
      keyString[0] += low;
      keyString[0] += ") TotalWidth sh TestWidth\n";

      keyString[1] = "1 TR (";
      keyString[1] += low;
      keyString[1] += " < E < ";
      keyString[1] += high;
      keyString[1] += ") TotalWidth sh TestWidth\n";

      for ( int i=2; i<5; ++i )
        {
          low=high;
          high = convertIsomerEnergyToHuman(part->value[i]);

          keyString[i] = "1 TR (";
          keyString[i] += low;
          keyString[i] +=" < E < ";
          keyString[i] += high;
          keyString[i] += ") TotalWidth sh TestWidth\n";
        }

      keyString[5] = "1 TR (E > ";
      keyString[5] += high;
      keyString[5] += ") TotalWidth sh TestWidth\n";

      keyString[6] = "1 TR (No known isomer) TotalWidth sh TestWidth\n";
    }
}
