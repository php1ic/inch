#include "include/functions.h"

void setEPSKeyText(const inputs *draw,
		   const partition *part,
		   std::vector<std::string> &key_string
		   )
{
  if (draw->choice == "a")
    {
      std::stringstream low,high;

      low << part->value[0];
      high << part->value[1];

      key_string[0] = "1 TR (Stable \\() TotalWidth sh\n1 S (d) TotalWidth sh\n1 TR (m < ";
      key_string[0] += low.str();
      key_string[0] += " keV\\)) TotalWidth sh TestWidth\n";

      key_string[1] = "1 TR (Stable \\() TotalWidth sh\n1 S (d) TotalWidth sh\n1 TR (m > ";
      key_string[1] += low.str();
      key_string[1] += " keV\\)) TotalWidth sh TestWidth\n";

      key_string[2] = "1 S (d) TotalWidth sh\n1 TR (m < ";
      key_string[2] += low.str();
      key_string[2] += " keV) TotalWidth sh TestWidth\n";

      key_string[3] = "1 TR (  ";
      key_string[3] += low.str();
      key_string[3] += " keV < ) TotalWidth sh\n1 S (d) TotalWidth sh\n1 TR (m < ";
      key_string[3] += high.str();
      key_string[3] += " keV) TotalWidth sh TestWidth\n";

      for (int i=1;i<4;++i)
	{
	  low.str("");
	  low << part->value[i];

	  high.str("");
	  high << part->value[i+1];

	  key_string[i+3] = "1 TR (";
	  key_string[i+3] += low.str();
	  key_string[i+3] += " keV < ) TotalWidth sh\n1 S (d) TotalWidth sh\n1 TR (m < ";
	  key_string[i+3] += high.str();
	  key_string[i+3] += " keV) TotalWidth sh TestWidth\n";
	}

      key_string[7] = "1 S (d) TotalWidth sh\n1 TR (m > ";
      key_string[7] += high.str();
      key_string[7] += " keV) TotalWidth sh TestWidth\n";
    }
  else if (draw->choice == "b")
    {
      std::vector<std::string> low(2), high(2);

      convertFloatToExponent(part->value[0],low);
      convertFloatToExponent(part->value[1],high);

      key_string[0] = "1 S (d) TotalWidth sh\n1 TR (m/m < ) TotalWidth sh\n";
      key_string[0] += low[0];
      key_string[0] += " -";
      key_string[0] += low[1];
      key_string[0] += " exponent TestWidth\n";

      key_string[1] = low[0];
      key_string[1] += " -";
      key_string[1] += low[1];
      key_string[1] += " exponent printUnit ";
      key_string[1] += high[0];
      key_string[1] += " -";
      key_string[1] += high[1];
      key_string[1] += " exponent TestWidth\n";

      for (int i=1;i<4;++i)
	{
	  convertFloatToExponent(part->value[i],low);
	  convertFloatToExponent(part->value[i+1],high);

	  key_string[i+1] = low[0];
	  key_string[i+1] += " -";
	  key_string[i+1] += low[1];
	  key_string[i+1] += " exponent printUnit ";
	  key_string[i+1] += high[0];
	  key_string[i+1] += " -";
	  key_string[i+1] += high[1];
	  key_string[i+1] += " exponent TestWidth\n";
	}

      key_string[5] = "1 S (d) TotalWidth sh\n1 TR (m/m > ) TotalWidth sh\n";
      key_string[5] += high[0];
      key_string[5] += " -";
      key_string[5] += high[1];
      key_string[5] += " exponent TestWidth\n";
    }
  else if (draw->choice == "c")
    {
      key_string[0]  = "1 TR (Stable) TotalWidth sh TestWidth\n";
      key_string[1]  = "1 TR (Alpha) TotalWidth sh TestWidth\n";
      key_string[2]  = "1 S (b) TotalWidth sh\n0.5 TR 0 0.55 rmoveto (+) TotalWidth sh TestWidth\n";
      key_string[3]  = "1 S (b) TotalWidth sh\n0.75 TR 0 0.55 rmoveto (-) TotalWidth sh TestWidth\n";
      key_string[4]  = "1 TR (Spontaneous Fission)TotalWidth sh TestWidth\n";
      key_string[5]  = "1 TR (n decay) TotalWidth sh TestWidth\n";
      key_string[6]  = "1 TR (2n decay) TotalWidth sh TestWidth\n";
      key_string[7]  = "1 TR (p decay) TotalWidth sh TestWidth\n";
      key_string[8]  = "1 TR (2p decay) TotalWidth sh TestWidth\n";
      key_string[9]  = "1 TR (Unknown) TotalWidth sh TestWidth\n";
      key_string[10] = "1 TR (Electron Capture) TotalWidth sh TestWidth\n";
    }
  else if (draw->choice == "d")
    {
      std::string low, high;

      convertSecondsToHuman(part->value[0],low);
      convertSecondsToHuman(part->value[1],high);

      key_string[0] = "printUnit 1 TR (     < ";
      key_string[0] += low;
      key_string[0] += ") TotalWidth sh TestWidth\n";

      key_string[1] = "1 TR (";
      key_string[1] += low;
      key_string[1] += " < ) TotalWidth sh printUnit\n(     < ";
      key_string[1] += high;
      key_string[1] += ") TotalWidth sh TestWidth\n";

      for (int i=2;i<7;++i)
	{
	  low=high;
	  convertSecondsToHuman(part->value[i],high);

	  key_string[i] = "1 TR (";
	  key_string[i] += low;
	  key_string[i] += " < ) TotalWidth sh printUnit\n(     < ";
	  key_string[i] += high;
	  key_string[i] += ") TotalWidth sh TestWidth\n";
	}

      key_string[7] = "printUnit 1 TR (     > ";
      key_string[7] += high;
      key_string[7] += ") TotalWidth sh TestWidth\n";
    }
  else if (draw->choice == "e")
    {
      std::string low, high;

      convertIsomerEnergyToHuman(part->value[0],low);
      convertIsomerEnergyToHuman(part->value[1],high);

      key_string[0] = "1 TR (E < ";
      key_string[0] += low;
      key_string[0] += ") TotalWidth sh TestWidth\n";

      key_string[1] = "1 TR (";
      key_string[1] += low;
      key_string[1] += " < E < ";
      key_string[1] += high;
      key_string[1] += ") TotalWidth sh TestWidth\n";

      for (int i=2;i<5;++i)
	{
	  low=high;
	  convertIsomerEnergyToHuman(part->value[i],high);

	  key_string[i] = "1 TR (";
	  key_string[i] += low;
	  key_string[i] +=" < E < ";
	  key_string[i] += high;
	  key_string[i] += ") TotalWidth sh TestWidth\n";
	}

      key_string[5] = "1 TR (E > ";
      key_string[5] += high;
      key_string[5] += ") TotalWidth sh TestWidth\n";

      key_string[6] = "1 TR (No known isomer) TotalWidth sh TestWidth\n";
    }
}
