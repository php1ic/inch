#include "functions.h"

void drawDoubleDriplines(std::vector<Nuclide> &nuc, inputs *draw, std::ofstream &out_file)
{
  //-May only want one of the drip lines
  //------------------------------------------
  // double_drip_lines = 1 -> both
  // double_drip_lines = 2 -> only p drip line
  // double_drip_lines = 3 -> only n drip line
  //------------------------------------------

  bool b;
  int sz_drip, sn_drip;
  float s_val;
  std::string line;
  std::stringstream in;

  struct stat drip, FRDM;

  out_file << "\n%-------------------------\n"
	   << "%- Two particle Drip Lines -\n"
	   << "%---------------------------\n"
	   << "gs\n"
	   << "darkgreen rgb\n"
	   << "1 u div sl" << std::endl;

  if (draw->double_drip_lines != 3)
    {
      out_file << "\n%------------------------\n"
	       << "%- Two-Proton drip line -\n"
	       << "%------------------------\n";

      //-File with proton drip line.
      //-Format: N Z S(2p)
      draw->two_proton_drip.insert(0,draw->path);

      //Check if file contaning drip line data exists
      if (stat(draw->two_proton_drip.c_str(), &drip))
	{
	  std::cout << "\nWARNING: The drip line file " << draw->two_proton_drip << " does not exist.\n"
		    << "         Creating it now..." << std::endl;

	  if (stat(draw->FRDM.c_str(), &FRDM))
	    {
	      std::cout << "\nERROR: The file containing drop model masses: " << draw->FRDM << " does not exist,\n"
			<< "       Not drawing the drip lines." << std::endl;
	      return;
	    }

	  createDriplineFile(nuc,draw,3);
	}

      std::ifstream p_drip(draw->two_proton_drip.c_str());

      if (p_drip)
	{
	  std::cout << "Reading "
		    << draw->two_proton_drip.substr(draw->path.length(),draw->two_proton_drip.length()-draw->path.length())
		    << " and drawing the two-proton drip line";
	  b=false;

	  while(getline(p_drip,line))
	    {
	      if (line.at(0) != '#')
		{
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
	    }
	  p_drip.close();
	}
      else
	{
	  std::cout << "ERROR: " << draw->two_proton_drip
		    << " couldn't be opened to read the two-proton drip line" << std::endl;
	  exit(-1);
	}

      out_file << "st" << std::endl;

      std::cout << " - done\n";
    }
  else
    std::cout << "Not drawing the two-proton drip line" << std::endl;

  if (draw->double_drip_lines != 2)
    {
      out_file << "\n%-------------------------\n"
	       << "%- Two-Neutron drip line -\n"
	       << "%-------------------------\n";

      //-File with neutron drip line.
      //-Format: N Z S(2n)
      draw->two_neutron_drip.insert(0,draw->path);

      //Check if file contaning drip line data exists
      if (stat(draw->two_neutron_drip.c_str(), &drip))
	{
	  std::cout << "\nWARNING: The drip line file " << draw->two_neutron_drip << " does not exist.\n"
		    << "         Creating it now..." << std::endl;

	  if (stat(draw->FRDM.c_str(), &FRDM))
	    {
	      std::cout << "\nERROR: The file containing drop model masses: " << draw->FRDM << " does not exist,\n"
			<< "       Not drawing the drip lines." << std::endl;
	      return;
	    }

	  createDriplineFile(nuc,draw,1);
	}

      std::ifstream n_drip(draw->two_neutron_drip.c_str());

      if (n_drip)
	{
	  std::cout << "Reading "
		    << draw->two_neutron_drip.substr(draw->path.length(),draw->two_neutron_drip.length()-draw->path.length())
		    << " and drawing the two-neutron drip line";
	  b=false;

	  while (getline(n_drip,line))
	    {
	      if (line.at(0) != '#')
		{
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
	    }
	  n_drip.close();
	}
      else
	{
	  std::cout << "ERROR: " << draw->two_neutron_drip
		    << " couldn't be opened to read the two-neutron drip line" << std::endl;
	  exit(-1);
	}

      out_file << "st" << std::endl;

      std::cout << " - done\n";
    }
  else
    std::cout << "Not drawing the two-neutron drip line" << std::endl;

  out_file << "gr" << std::endl;
}
