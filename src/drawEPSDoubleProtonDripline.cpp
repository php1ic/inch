#include "include/functions.h"

void drawEPSDoubleProtonDripline(const std::vector<Nuclide> &nuc,
				 inputs *draw,
				 std::ofstream &out_file
				 )
{
  //Get straight out if max values of N & Z aren't high enough
  if (draw->Nmax < 8 || draw->Zmax < 14)
    return;

  bool b;
  int sz_drip, sn_drip;
  float s_val;
  std::string line;
  std::stringstream in;

  out_file << "\n%------------------------\n"
	   << "%- Two-Proton drip line -\n"
	   << "%------------------------\n"
	   << "gs\n"
	   << "darkgreen rgb\n"
	   << "1 u div sl" << std::endl;

  //-File with proton drip line.
  //-Format: N Z S(2p)
  draw->two_proton_drip.insert(0,draw->path);

  //Check if file contaning drip line data exists
  if (!checkFileExists(draw->two_proton_drip))
    {
      std::cout << "\nWARNING: The drip line file " << draw->two_proton_drip << " does not exist.\n"
		<< "         Creating it now..." << std::endl;

      createDriplineFile(nuc,draw,3);
    }

  std::ifstream p_drip(draw->two_proton_drip.c_str());

  if (p_drip.is_open())
    {
      std::cout << "Reading "
		<< draw->two_proton_drip.substr(draw->two_proton_drip.find_last_of("/")+1)
		<< " and drawing the two-proton drip line";
      b=false;

      while(getline(p_drip,line))
	{
	  if (!line.compare("") || line.at(0) == '#')
	    continue;

	  in.clear();
	  in << line;

	  in >> sn_drip >> sz_drip >> s_val;

	  if (   sz_drip >= draw->Zmin
	      && sz_drip <= draw->Zmax
	      && sn_drip >= draw->Nmin
	      && sn_drip <= draw->Nmax)
	    {
	      out_file << std::setw(3) << sn_drip-draw->Nmin << " "
		       << std::setw(3) << sz_drip-draw->Zmin;

	      if (!b){out_file << " m\n"; b=true;}
	      else    out_file << " l\n";
	    }
	}
      p_drip.close();
    }
  else
    {
      std::cout << "ERROR: " << draw->two_proton_drip
		<< " couldn't be opened to read the two-proton drip line" << std::endl;
      exit(-1);
    }

  out_file << "st\n"
	   << "gr" << std::endl;

  std::cout << " - done\n";
}
