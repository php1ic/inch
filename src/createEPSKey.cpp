#include "functions.h"

void createEPSKey(const inputs *draw,
		  std::ofstream &out_file,
		  const float &key_height,
		  const float &key_scale,
		  const std::vector<std::string> &key_string,
		  const std::vector<std::string> &partition_colour,
		  const std::vector<bool> &draw_partition
		  )
{
  unsigned int i=0;
  int full_chart_key_position[5][2]={
    {15,75},
    {12,81},
    {9,60},
    {9,75},
    {15,75}
  };

  if (draw->section == "a")
    {
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
  else
    out_file << (draw->Nmax-draw->Nmin+2) << " " << ((draw->Zmax-draw->Zmin+1)-key_height*key_scale)/2 << " translate\n";

  out_file << key_scale << " dup scale\n" << std::endl;

  if (draw->choice == "b")
    {
      out_file << "\n/e{\n"
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
    }
  else if (draw->choice == "d")
    {
      out_file << "\n/t{gs\n"
	       << "1 S (t) sh\n"
	       << "0.5 TR 0 -0.15 rmoveto (1/2) sh\n"
	       << "gr} def\n" << std::endl;
    }

  float relative_key_position=0.5;
  for (i=0;i<draw_partition.size();++i)
    {
      if (draw_partition[draw_partition.size()-(i+1)])
  	{
  	  out_file << "0 " << partition_colour[11-i] << " 0.5 " << relative_key_position << " curve n\n"
  		   << "2.5 " << relative_key_position+0.2 << " m rw\n"
  		   << key_string[11-i];
  	  relative_key_position+=1.5;
  	}
    }
}
