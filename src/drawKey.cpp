#include "functions.h"

void drawKey(inputs *draw,
	     std::ofstream &out_file,
	     float &s,
	     const std::vector<std::string> &partition_colour,
	     const std::vector<bool> &draw_partition,
	     const std::vector<float> &partition_value
	     )
{
  int i=0;
  float y=0;
  std::cout << "Drawing the key ";
  out_file << "\n%-------\n"
	   << "%- Key -\n"
	   << "%-------\n"
	   << "/rw {/sw 0 def} def\n"
	   << "/tw {dup stringwidth pop sw add /sw exch def} def\n"
	   << "/kx 0 def\n"
	   << "/tx {sw kx gt{/kx sw def}if} def\n\n"
	   << "%-lower left corner of the key-\n";

  if (draw->choice == "a")
    {
      std::vector<std::string> key_string(draw_partition.size());
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

      y=0.5;
      for (i=0;i<12;++i)
	{
	  if (draw_partition[11-i]==1)
	    y+=1.5;
	}

      if ((draw->Zmax-draw->Zmin)/y>3.0)
	s=3;
      else
	s=(draw->Zmax-draw->Zmin)/y;

      if (draw->section=="a" || draw->Zmax-draw->Zmin==118)
	{
	  s=3;
	  out_file << "15 75 translate\n";
	}
      else
	out_file << (draw->Nmax-draw->Nmin+2) << " " << ((draw->Zmax-draw->Zmin+1)-y*s)/2 << " translate\n";

      out_file << s << " dup scale\n" << std::endl;
      y=0.5;
      for (i=0;i<12;++i)
	{
	  if (draw_partition[11-i]==1)
	    {
	      out_file << "0 " << partition_colour[11-i] << " 0.5 " << y << " curve n\n"
		       << "2.5 " << y+0.2 << " m rw\n"
		       << key_string[11-i];
	      y+=1.5;
	    }
	}
    }
  else if (draw->choice == "b")
    {
      std::vector<std::string> low(2), high(2);
      std::string key_string[6];

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

      y=0.5;
      for (i=0;i<12;++i)
	{
	  if (draw_partition[11-i]==1)
	    y+=1.5;
	}

      if ((draw->Zmax-draw->Zmin)/y>3.0)
	s=3;
      else
	s=(draw->Zmax-draw->Zmin)/y;

      if (draw->section=="a" || draw->Zmax-draw->Zmin==118)
	{
	  s=3;
	  out_file << "12 81 translate\n";
	}
      else
	out_file << (draw->Nmax-draw->Nmin+2) << " " << ((draw->Zmax-draw->Zmin+1)-y*s)/2 << " translate\n";

      y=0.5;
      out_file << s << " dup scale\n"
	       << "\n/e{\n"
	       << "/e1 ed\n"
	       << "/e2 ed\n"
	       << "1 TR e2 2 string cvs tw sh\n"
	       << "0.75 TR (x) tw sh\n"
	       << "1 TR (10) tw sh\n"
	       << "gs\n"
	       << "0.75 TR\n"
	       << "0 0.4 rmoveto e1 2 string cvs tw sh\n"
	       << "gr\n"
	       << "} def\n\n"
	       << "/f{\n"
	       << "1 TR (   < ) tw sh\n"
	       << "1 S (d) tw sh\n"
	       << "1 TR (m/m < ) tw sh\n"
	       << "} def\n" << std::endl;
      for (i=0;i<12;++i)
	{
	  if (draw_partition[11-i]==1)
	    {
	      out_file << "0 " << partition_colour[11-i] << " 0.5 " << y << " curve n\n"
		       << "2.5 " << y+0.2 << " m rw\n"
		       << key_string[11-i];
	      y+=1.5;
	    }
	}
    }
  else if (draw->choice == "c")
    {
      std::vector<std::string> key_string(11);

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

      y=0.5;
      for (i=0;i<12;++i)
	{
	  if (draw_partition[11-i]==1)
	    y+=1.5;
	}

      if ((draw->Zmax-draw->Zmin)/y>3.0)
	s=3;
      else
	s=(draw->Zmax-draw->Zmin)/y;

      if (draw->section=="a" || draw->Zmax-draw->Zmin==118)
	{
	  s=3;
	  out_file << "9 60 translate\n";
	}
      else
	out_file << (draw->Nmax-draw->Nmin+2) << " " << ((draw->Zmax-draw->Zmin+1)-y*s)/2 << " translate\n";

      out_file << s << " dup scale\n" << std::endl;
      y=0.5;
      for (i=0;i<12;++i)
	{
	  if (draw_partition[11-i]==1)
	    {
	      out_file << "0 " << partition_colour[11-i] << " 0.5 " << y << " curve n\n"
		       << "2.5 " << y+0.2 << " m rw\n"
		       << key_string[11-i];
	      y+=1.5;
	    }
	}
    }
  else if (draw->choice == "d")
    {
      std::string key_string[8];
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

      key_string[7] 	= "t 1 TR (     > " + high + ") tw sh tx\n";

      y=0.5;
      for (i=0;i<12;++i)
	{
	  if (draw_partition[11-i]==1)
	    y+=1.5;
	}

      if ((draw->Zmax-draw->Zmin)/y>3.0)
	s=3;
      else
	s=(draw->Zmax-draw->Zmin)/y;

      if (draw->section=="a" || draw->Zmax-draw->Zmin==118)
	{
	  s=3;
	  out_file << "9 75 translate\n";
	}
      else
	out_file << (draw->Nmax-draw->Nmin+2) << " " << ((draw->Zmax-draw->Zmin+1)-y*s)/2 << " translate\n";

      y=0.5;
      out_file << s << " dup scale\n"
	       << "\n/t{gs\n"
	       << "1 S (t) sh\n"
	       << "0.5 TR 0 -0.15 rmoveto (1/2) sh\n"
	       << "gr} def\n" << std::endl;
      for (i=0;i<12;++i)
	{
	  if (draw_partition[11-i]==1)
	    {
	      out_file << "0 " << partition_colour[11-i] << " 0.5 " << y << " curve n\n"
		       << "2.5 " << y+0.2 << " m rw\n"
		       << key_string[11-i];
	      y+=1.5;
	    }
	}
    }
  else if (draw->choice == "e")
    {
      std::string key_string[7];
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

      y=0.5;
      for (i=0;i<12;++i)
	{
	  if (draw_partition[11-i]==1)
	    y+=1.5;
	}

      if ((draw->Zmax-draw->Zmin)/y>3.0)
	s=3;
      else
	s=(draw->Zmax-draw->Zmin)/y;

      if (draw->section=="a" || draw->Zmax-draw->Zmin==118)
	{
	  s=3;
	  out_file << "15 75 translate\n";
	}
      else
	out_file << (draw->Nmax-draw->Nmin+2) << " " << ((draw->Zmax-draw->Zmin+1)-y*s)/2 << " translate\n";

      out_file << s << " dup scale\n" << std::endl;
      y=0.5;
      for (i=0;i<12;++i)
	{
	  if (draw_partition[11-i]==1)
	    {
	      out_file << "0 " << partition_colour[11-i] << " 0.5 " << y << " curve n\n"
		       << "2.5 " << y+0.2 << " m rw\n"
		       << key_string[11-i];
	      y+=1.5;
	    }
	}
    }

  out_file << "0.1 u div sl\n"
	   << "0 0 m\n"
	   << "kx 3 add 0 rl\n"
	   << "0 " << y << " rl\n"
	   << "kx 3 add neg 0 rl\n"
	   << "closepath\n"
	   << "st\n" << std::endl;

  std::cout << "- done" << std::endl;
}

