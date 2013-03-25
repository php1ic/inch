#include "functions.h"

void drawEPSKey(const inputs *draw,
		std::ofstream &out_file,
		float &key_height,
		float &key_scale,
		const std::vector<std::string> &partition_colour,
		const std::vector<bool> &draw_partition,
		const std::vector<float> &partition_value
		)
{
  unsigned int i=0;
  std::vector<std::string> key_string(draw_partition.size());
  std::cout << "Drawing the key ";
  out_file << "\n%-------\n"
	   << "%- Key -\n"
	   << "%-------\n"
	   << "/rw {/sw 0 def} def\n"
	   << "/tw {dup stringwidth pop sw add /sw exch def} def\n"
	   << "/kx 0 def\n"
	   << "/tx {sw kx gt{/kx sw def}if} def\n\n"
	   << "%-lower left corner of the key-\n";

  //-Construct an array of strings with the text for each part of the key
  if (draw->choice == "a")
    {
      std::ostringstream low,high;

      low<<partition_value[0];

      key_string[0] = "1 TR (Stable \\() tw sh\n1 S (d) tw sh\n1 TR (m < " + low.str() + " keV\\)) tw sh tx\n\n";
      key_string[1] = "1 TR (Stable \\() tw sh\n1 S (d) tw sh\n1 TR (m > " + low.str() + " keV\\)) tw sh tx\n";
      key_string[2] = "1 S (d) tw sh\n1 TR (m < " + low.str() + " keV) tw sh tx\n";

      high<<partition_value[1];

      key_string[3] = "1 TR (  " + low.str() + " keV < ) tw sh\n1 S (d) tw sh\n1 TR (m < " + high.str() + " keV) tw sh tx\n";

      for (i=1;i<4;++i)
	{
	  low.str("");
	  low<<partition_value[i];
	  high.str("");
	  high<<partition_value[i+1];
	  key_string[i+3] = "1 TR (" + low.str() + " keV < ) tw sh\n1 S (d) tw sh\n1 TR (m < " + high.str() + " keV) tw sh tx\n";
	}

      key_string[7] = "1 S (d) tw sh\n1 TR (m > " + high.str() + " keV) tw sh tx\n";
    }
  else if (draw->choice == "b")
    {
      std::vector<std::string> low(2), high(2);

      convertFloatToExponent(partition_value[0],low);
      key_string[0] = "1 S (d) tw sh\n1 TR (m/m < ) tw sh\n" + low[0] + " -" + low[1] + " e tx\n";

      convertFloatToExponent(partition_value[1],high);
      key_string[1] = low[0] + " -" + low[1] + " e f " + high[0] + " -" + high[1] + " e tx\n";

      for (i=1;i<4;++i)
	{
	  convertFloatToExponent(partition_value[i],low);
	  convertFloatToExponent(partition_value[i+1],high);
	  key_string[i+1] = low[0] + " -" + low[1] + " e f " + high[0] + " -" + high[1] + " e tx\n";
	}

      key_string[5] = "1 S (d) tw sh\n1 TR (m/m > ) tw sh\n" + high[0] + " -" + high[1] + " e tx\n";
    }
  else if (draw->choice == "c")
    {
      key_string[0]  = "1 TR (Stable) tw sh tx\n";
      key_string[1]  = "1 TR (Alpha) tw sh tx\n";
      key_string[2]  = "1 S (b) tw sh\n0.5 TR 0 0.55 rmoveto (+) tw sh tx\n";
      key_string[3]  = "1 S (b) tw sh\n0.75 TR 0 0.55 rmoveto (-) tw sh tx\n";
      key_string[4]  = "1 TR (Spontaneous Fission)tw sh tx\n";
      key_string[5]  = "1 TR (n decay) tw sh tx\n";
      key_string[6]  = "1 TR (2n decay) tw sh tx\n";
      key_string[7]  = "1 TR (p decay) tw sh tx\n";
      key_string[8]  = "1 TR (2p decay) tw sh tx\n";
      key_string[9]  = "1 TR (Unknown) tw sh tx\n";
      key_string[10] = "1 TR (Electron Capture) tw sh tx\n";
    }
  else if (draw->choice == "d")
    {
      std::string low,high;

      convertSecondsToHuman(partition_value[0],low);
      key_string[0] = "t 1 TR (     < " + low + ") tw sh tx\n";

      convertSecondsToHuman(partition_value[1],high);
      key_string[1] = "1 TR (" + low + " < ) tw sh t\n(     < " + high + ") tw sh tx\n";

      for (i=1;i<7;++i)
	{
	  low=high;
	  convertSecondsToHuman(partition_value[i],high);
	  key_string[i] = "1 TR (" + low + " < ) tw sh t\n(     < " + high + ") tw sh tx\n";
	}

      key_string[7] = "t 1 TR (     > " + high + ") tw sh tx\n";
    }
  else if (draw->choice == "e")
    {
      std::string low,high;

      setIsomerUnit(partition_value[0],low);
      key_string[0] = "1 TR (E < " + low + ") tw sh tx\n";

      setIsomerUnit(partition_value[1],high);
      key_string[1] = "1 TR (" + low + " < E < " + high + ") tw sh tx\n";

      low=high;
      setIsomerUnit(partition_value[2],high);
      key_string[2] = "1 TR (" + low +" < E < " + high + ") tw sh tx\n";

      low=high;
      setIsomerUnit(partition_value[3],high);
      key_string[3] = "1 TR (" + low +" < E < " + high + ") tw sh tx\n";

      low=high;
      setIsomerUnit(partition_value[4],high);
      key_string[4] = "1 TR (" + low +" < E < " + high + ") tw sh tx\n";

      key_string[5] = "1 TR (E > " + high + ") tw sh tx\n";
      key_string[6] = "1 TR (No known isomer) tw sh tx\n";
    }

  //-Pass the array just created, position and draw the key where necessary.
  createEPSKey(draw,out_file,key_height,key_scale,key_string,partition_colour,draw_partition);

  //-Draw a box around the key
  out_file << "0.1 u div sl\n"
	   << "0 0 m\n"
	   << "kx 3 add 0 rl\n"
	   << "0 " << key_height << " rl\n"
	   << "kx 3 add neg 0 rl\n"
	   << "closepath\n"
	   << "st\n" << std::endl;

  std::cout << "- done" << std::endl;
}
