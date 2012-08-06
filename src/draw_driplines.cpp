#include "functions.h"

void draw_driplines(inputs *draw, std::ofstream &out_file)
{
  //-May only want one of the drip lines
  //------------------------------------
  // drip_lines = 1 -> both
  // drip_lines = 2 -> only p drip line
  // drip_lines = 3 -> only n drip line
  //------------------------------------

  bool b;
  short sz_drip, sn_drip;
  float s_val;
  std::string line;
  //std::istringstream in;

  out_file << "\n%--------------\n"
	   << "%- Drip Lines -\n"
	   << "%--------------\n"
	   << "purple rgb\n"
	   << "1 u div sl" << std::endl;

  if (draw->drip_lines != 3)
    {
      out_file << "\n%--------------------\n"
	       << "%- Proton drip line -\n"
	       << "%--------------------\n";

      //-File with proton drip line.
      //-Format: N Z S(p)
      draw->proton_drip.insert(0,draw->path);
      std::ifstream p_drip(draw->proton_drip.c_str());

      if (p_drip)
	{
	  std::cout << "Reading "
		    << draw->proton_drip.substr(draw->path.length(),draw->proton_drip.length()-draw->path.length())
		    << " and drawing the proton drip line";
	  b=false;

	  while(getline(p_drip,line))
	    {
	      if (line.at(0) != '#')
		{
		  std::istringstream in(line);

		  in >> sn_drip >> sz_drip >> s_val;

		  if (   sz_drip >= draw->Zmin
			 && sz_drip <= draw->Zmax
			 && sn_drip >= draw->Nmin
			 && sn_drip <= draw->Nmax)
		    {
		      out_file << std::setw(3) << sn_drip-draw->Nmin << " "
			       << std::setw(3) << sz_drip-draw->Zmin;

		      if (!b){out_file << " m\n"; b=true;}
		      else   out_file << " l\n";
		    }
		}
	    }
	  p_drip.close();
	}
      else
	{
	  std::cout << "ERROR: " << draw->proton_drip
		    << " couldn't be opened to read the proton drip line" << std::endl;
	  exit(-1);
	}

      std::cout << " - done\n";

      out_file << "st" << std::endl;
    }
  else
    std::cout << "Not drawing the proton drip line" << std::endl;

  if (draw->drip_lines != 2)
    {
      out_file << "\n%---------------------\n"
	       << "%- Neutron drip line -\n"
	       << "%---------------------\n";

      //-File with neutron drip line.
      //-Format: N Z S(n)
      draw->neutron_drip.insert(0,draw->path);
      std::ifstream n_drip(draw->neutron_drip.c_str());

      if (n_drip)
	{
	  std::cout << "Reading "
		    << draw->neutron_drip.substr(draw->path.length(),draw->neutron_drip.length()-draw->path.length())
		    << " and drawing the neutron drip line";
	  b=false;

	  while (getline(n_drip,line))
	    {
	      if (line.at(0) != '#')
		{
		  std::istringstream in(line);

		  in >> sn_drip >> sz_drip >> s_val;

		  if (   sz_drip >= draw->Zmin
			 && sz_drip <= draw->Zmax
			 && sn_drip >= draw->Nmin
			 && sn_drip <= draw->Nmax)
		    {
		      out_file << std::setw(3) << sn_drip-draw->Nmin << " "
			       << std::setw(3) << sz_drip-draw->Zmin;

		      if (!b){out_file << " m\n"; b=true;}
		      else   out_file << " l\n";
		    }
		}
	    }
	  n_drip.close();
	}
      else
	{
	  std::cout << "ERROR: " << draw->neutron_drip
		    << " couldn't be opened to read the neutron drip line" << std::endl;
	  exit(-1);
	}

      std::cout << " - done\n";

      out_file << "st" << std::endl;
    }
  else
    std::cout << "Not drawing the neutron drip line" << std::endl;

}
