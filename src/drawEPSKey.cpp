#include "include/functions.h"

void drawEPSKey(const inputs *draw,
		std::ofstream &out_file,
		const partition *part
		)
{
  unsigned int i=0;
  std::vector<std::string> key_string(part->draw.size());

  std::cout << "Drawing the key ";

  out_file << "\n%-------\n"
	   << "%- Key -\n"
	   << "%-------\n"
	   << "/ResetWidth {/StringLength 0 def} def\n"
	   << "/TotalWidth {dup stringwidth pop StringLength add /StringLength exch def} def\n"
	   << "/KeyWidth 0 def\n"
	   << "/TestWidth {StringLength KeyWidth gt {/KeyWidth StringLength def} if} def\n\n"
	   << "%-lower left corner of the key-\n";

  //-Construct an array of strings with the text for each part of the key
  if (draw->choice == "a")
    {
      std::stringstream low,high;

      low << part->value[0];
      high << part->value[1];

      key_string[0] = "1 TR (Stable \\() TotalWidth sh\n1 S (d) TotalWidth sh\n1 TR (m < " + low.str() + " keV\\)) TotalWidth sh TestWidth\n";
      key_string[1] = "1 TR (Stable \\() TotalWidth sh\n1 S (d) TotalWidth sh\n1 TR (m > " + low.str() + " keV\\)) TotalWidth sh TestWidth\n";
      key_string[2] = "1 S (d) TotalWidth sh\n1 TR (m < " + low.str() + " keV) TotalWidth sh TestWidth\n";
      key_string[3] = "1 TR (  " + low.str() + " keV < ) TotalWidth sh\n1 S (d) TotalWidth sh\n1 TR (m < " + high.str() + " keV) TotalWidth sh TestWidth\n";

      for (i=1;i<4;++i)
	{
	  low.str("");
	  low << part->value[i];
	  high.str("");
	  high << part->value[i+1];
	  key_string[i+3] = "1 TR (" + low.str() + " keV < ) TotalWidth sh\n1 S (d) TotalWidth sh\n1 TR (m < " + high.str() + " keV) TotalWidth sh TestWidth\n";
	}

      key_string[7] = "1 S (d) TotalWidth sh\n1 TR (m > " + high.str() + " keV) TotalWidth sh TestWidth\n";
    }
  else if (draw->choice == "b")
    {
      std::vector<std::string> low(2), high(2);

      convertFloatToExponent(part->value[0],low);
      convertFloatToExponent(part->value[1],high);

      key_string[0] = "1 S (d) TotalWidth sh\n1 TR (m/m < ) TotalWidth sh\n" + low[0] + " -" + low[1] + " exponent TestWidth\n";
      key_string[1] = low[0] + " -" + low[1] + " exponent printUnit " + high[0] + " -" + high[1] + " exponent TestWidth\n";

      for (i=1;i<4;++i)
	{
	  convertFloatToExponent(part->value[i],low);
	  convertFloatToExponent(part->value[i+1],high);
	  key_string[i+1] = low[0] + " -" + low[1] + " exponent printUnit " + high[0] + " -" + high[1] + " exponent TestWidth\n";
	}

      key_string[5] = "1 S (d) TotalWidth sh\n1 TR (m/m > ) TotalWidth sh\n" + high[0] + " -" + high[1] + " exponent TestWidth\n";
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

      key_string[0] = "printUnit 1 TR (     < " + low + ") TotalWidth sh TestWidth\n";
      key_string[1] = "1 TR (" + low + " < ) TotalWidth sh printUnit\n(     < " + high + ") TotalWidth sh TestWidth\n";

      for (i=2;i<7;++i)
	{
	  low=high;
	  convertSecondsToHuman(part->value[i],high);
	  key_string[i] = "1 TR (" + low + " < ) TotalWidth sh printUnit\n(     < " + high + ") TotalWidth sh TestWidth\n";
	}

      key_string[7] = "printUnit 1 TR (     > " + high + ") TotalWidth sh TestWidth\n";
    }
  else if (draw->choice == "e")
    {
      std::string low, high;

      setIsomerUnit(part->value[0],low);
      setIsomerUnit(part->value[1],high);

      key_string[0] = "1 TR (E < " + low + ") TotalWidth sh TestWidth\n";
      key_string[1] = "1 TR (" + low + " < E < " + high + ") TotalWidth sh TestWidth\n";

      for (i=2;i<5;++i)
	{
	  low=high;
	  setIsomerUnit(part->value[i],high);
	  key_string[i] = "1 TR (" + low +" < E < " + high + ") TotalWidth sh TestWidth\n";
	}

      key_string[5] = "1 TR (E > " + high + ") TotalWidth sh TestWidth\n";
      key_string[6] = "1 TR (No known isomer) TotalWidth sh TestWidth\n";
    }

  //-Pass the array just created and draw the key where necessary.
  createEPSKey(draw,out_file,key_string,part);

  //-Draw a dynamically sized box around the key
  out_file << "\n"
	   << "%Draw a box around the key\n"
	   << "0.1 u div sl\n"
	   << "0 0 m\n"
	   << "KeyWidth 3 add 0 rl\n"
	   << "0 " << draw->key_height << " rl\n"
	   << "KeyWidth 3 add neg 0 rl\n"
	   << "closepath\n"
	   << "st\n" << std::endl;

  std::cout << "- done" << std::endl;
}
