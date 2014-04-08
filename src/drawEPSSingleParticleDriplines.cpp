#include "include/functions.h"

void drawEPSSingleDriplines(const std::vector<Nuclide> &nuc,
			    inputs *draw,
			    std::ofstream &out_file
			    )
{
  //-May only want one of the drip lines
  //------------------------------------------
  // single_drip_lines = 1 -> both
  // single_drip_lines = 2 -> only p drip line
  // single_drip_lines = 3 -> only n drip line
  //------------------------------------------

  bool b;
  int sz_drip, sn_drip;
  float s_val;
  std::string line;
  std::stringstream in;

  out_file << "\n%--------------\n"
	   << "%- Drip Lines -\n"
	   << "%--------------\n"
	   << "gs\n"
	   << "purple rgb\n"
	   << "1 u div sl" << std::endl;

  if (draw->single_drip_lines != 3)
    {
      out_file << "\n%--------------------\n"
	       << "%- Proton drip line -\n"
	       << "%--------------------\n";

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

      out_file << "st" << std::endl;

      std::cout << " - done\n";
    }
  else
    std::cout << "Not drawing the one-proton drip line" << std::endl;

  if (draw->single_drip_lines != 2)
    {
      out_file << "\n%---------------------\n"
	       << "%- Neutron drip line -\n"
	       << "%---------------------\n";

      //-File with neutron drip line.
      //-Format: N Z S(n)
      draw->neutron_drip.insert(0,draw->path);

      //Check if file contaning drip line data exists
      if (!checkFileExists(draw->neutron_drip))
	{
	  std::cout << "\nWARNING: The drip line file " << draw->neutron_drip << " does not exist.\n"
		    << "         Creating it now..." << std::endl;

	  createDriplineFile(nuc,draw,0);
	}

      std::ifstream n_drip(draw->neutron_drip.c_str());

      if (n_drip.is_open())
	{
	  std::cout << "Reading "
		    << draw->neutron_drip.substr(draw->neutron_drip.find_last_of("/")+1)
		    << " and drawing the one-neutron drip line";
	  b=false;

	  while (getline(n_drip,line))
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
	  n_drip.close();
	}
      else
	{
	  std::cout << "ERROR: " << draw->neutron_drip
		    << " couldn't be opened to read the one-neutron drip line" << std::endl;
	  exit(-1);
	}

      out_file << "st" << std::endl;

      std::cout << " - done\n";
    }
  else
    std::cout << "Not drawing the one-neutron drip line" << std::endl;

  out_file << "gr" << std::endl;
}
