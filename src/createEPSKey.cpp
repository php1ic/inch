#include "include/functions.h"

void createEPSKey(const inputs *draw,
		  std::ofstream &out_file,
		  const std::vector<std::string> &key_string,
		  const partition *part
		  )
{
  if (draw->section == "a")
    {
      int full_chart_key_position[5][2]={
	{15,75},
	{12,81},
	{9,60},
	{9,75},
	{15,75}
      };

      if (draw->choice=="a")
  	out_file << full_chart_key_position[0][0] << " " << full_chart_key_position[0][1] << " translate\n";
      else if (draw->choice=="b")
  	out_file << full_chart_key_position[1][0] << " " << full_chart_key_position[1][1] << " translate\n";
      else if (draw->choice=="c")
  	out_file << full_chart_key_position[2][0] << " " << full_chart_key_position[2][1] << " translate\n";
      else if (draw->choice=="d")
  	out_file << full_chart_key_position[3][0] << " " << full_chart_key_position[3][1] << " translate\n";
      else if (draw->choice=="e")
  	out_file << full_chart_key_position[4][0] << " " << full_chart_key_position[4][1] << " translate\n";
    }
  else if (draw->Zmax-draw->Zmin >= 9)
    {
      out_file << (draw->Nmax-draw->Nmin+2) << " "
	       << ((draw->Zmax-draw->Zmin+1.0)-draw->key_height*draw->key_scale)*0.5 << " translate\n";
    }
  else
    {
      out_file << (draw->Nmax-draw->Nmin+2) << " 0 translate\n";
    }

  out_file << draw->key_scale << " dup scale\n" << std::endl;

  if (draw->choice == "b")
    {
      out_file << "\n/exponent{\n"
	       << "/e1 ed\n"
	       << "/e2 ed\n"
	       << "1 TR e2 2 string cvs TotalWidth sh\n"
	       << "0.75 TR (x) TotalWidth sh\n"
	       << "1 TR (10) TotalWidth sh\n"
	       << "gs\n"
	       << "0.75 TR\n"
	       << "0 0.4 rmoveto e1 2 string cvs TotalWidth sh\n"
	       << "gr\n"
	       << "} def\n\n"
	       << "/printUnit{\n"
	       << "1 TR (   < ) TotalWidth sh\n"
	       << "1 S (d) TotalWidth sh\n"
	       << "1 TR (m/m < ) TotalWidth sh\n"
	       << "} def\n" << std::endl;
    }
  else if (draw->choice == "d")
    {
      out_file << "\n/printUnit{gs\n"
	       << "1 S (t) sh\n"
	       << "0.5 TR 0 -0.15 rmoveto (1/2) sh\n"
	       << "gr} def\n" << std::endl;
    }

  double relative_key_position=0.5;
  for (unsigned int i=0; i<part->draw.size(); ++i)
    {
      if (part->draw[part->draw.size()-(i+1)])
  	{
	  out_file << "0 " << part->colour[11-i] << " 0.5 " << relative_key_position << " curve Nucleus\n"
		   << "2.5 " << relative_key_position+0.2 << " m ResetWidth\n"
		   << key_string[11-i];
	  relative_key_position+=1.5;
  	}
    }
}
