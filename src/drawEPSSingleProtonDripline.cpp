#include "include/functions.h"

void drawEPSSingleProtonDripline(const std::vector<Nuclide> &nuc,
				 inputs *draw,
				 std::ofstream &out_file
				 )
{
  //Get straight out if max values of N & Z aren't high enough
  if (draw->Nmax < 8 || draw->Zmax < 11 )
    return;

  bool b;
  int sz_drip, sn_drip;
  float s_val;
  std::string line;
  std::stringstream in;

  out_file << "\n%--------------------\n"
	   << "%- Proton drip line -\n"
	   << "%--------------------\n"
	   << "gs\n"
	   << "purple rgb\n"
	   << "1 u div sl" << std::endl;

  //-File with proton drip line.
  //-Format: N Z S(p)
  draw->proton_drip.insert(0,draw->path);

  //Check if file contaning drip line data exists
  if (!checkFileExists(draw->proton_drip))
    {
      std::cout << "\nWARNING: The drip line file " << draw->proton_drip << " does not exist.\n"
		<< "         Creating it now..." << std::endl;

      createDriplineFile(nuc,draw,2);
    }

  std::ifstream p_drip(draw->proton_drip.c_str());

  if (p_drip.is_open())
    {
      std::cout << "Reading "
		<< draw->proton_drip.substr(draw->proton_drip.find_last_of("/")+1)
		<< " and drawing the one-proton drip line";
      b=false;

      while(getline(p_drip,line))
	{
	  if ( !line.compare("") || line.at(0) == '#' )
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
      std::cout << "ERROR: " << draw->proton_drip
		<< " couldn't be opened to read the one-proton drip line" << std::endl;
      exit(-1);
    }

  out_file << "st\n"
	   << "gr"<< std::endl;

  std::cout << " - done\n";
}
