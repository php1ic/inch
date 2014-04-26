#include "include/functions.h"

void drawEPSDripline(const std::vector<Nuclide> &nuc,
		     const inputs *draw,
		     std::ofstream &out_file,
		     const int &np
		     )
{
  std::string dripline;

  switch (np)
    {
    case 0: dripline = draw->neutron_drip; break;
    case 1: dripline = draw->two_neutron_drip; break;
    case 2: dripline = draw->proton_drip; break;
    case 3: dripline = draw->two_proton_drip; break;
    default:
      return;
    }

  //Check if file contaning drip line data exists
  if (!checkFileExists(dripline))
    {
      std::cout << "\nWARNING: The drip line file " << dripline << " does not exist.\n"
		<< "         Creating it now..." << std::endl;

      createDriplineFile(nuc,draw,np);
    }

  //-Format: N Z S(x)
  std::ifstream drip(dripline.c_str());

  if (drip.is_open())
    {
      std::cout << "Reading "
		<< dripline.substr(dripline.find_last_of("/")+1)
		<< " and drawing the drip line";

      bool initial=true;
      int z_drip, n_drip;
      float value;
      std::string line;
      std::stringstream in;

      while (getline(drip,line))
	{
	  if ( !line.compare("") || line.at(0) == '#' )
	    continue;

	  in.clear();
	  in << line;

	  in >> n_drip >> z_drip >> value;

	  if (   z_drip >= draw->Zmin
	      && z_drip <= draw->Zmax
	      && n_drip >= draw->Nmin
	      && n_drip <= draw->Nmax)
	    {
	      out_file << std::setw(3) << n_drip-draw->Nmin << " "
		       << std::setw(3) << z_drip-draw->Zmin << " "
		       << (initial ? "m" : "l") << "\n";

	      initial=false;
	    }
	}
      drip.close();
    }
  else
    {
      std::cout << "ERROR: " << dripline
		<< " couldn't be opened to read the drip line data" << std::endl;
      exit(-1);
    }

  out_file << "st\n"
	   << "gr" << std::endl;

  std::cout << " - done\n";
}
