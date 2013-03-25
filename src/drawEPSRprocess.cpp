#include "functions.h"

void drawEPSRprocess(inputs *draw, std::ofstream &out_file, const bool shaded)
{
  bool b;
  int n_rp, z_rp;
  std::string line;

  if (shaded)
    {
      out_file << "\n%----------------------------\n"
	       << "%- r-process -- shaded path -\n"
	       << "%----------------------------\n"
	       << "gs\n"
	       << "0.9 setgray" << std::endl;

      draw->r_proc_path.insert(0,draw->path);
    }
  else
    {
      out_file << "\n%-----------------------------\n"
	       << "%- r-process -- path outline -\n"
	       << "%-----------------------------\n"
	       << "black rgb\n"
	       << "0.1 u div sl" << std::endl;
    }

  std::ifstream rp(draw->r_proc_path.c_str());

  if (rp.is_open())
    {
      std::stringstream in;

      if (!shaded)
	std::cout << "Reading "
		  << draw->r_proc_path.substr(draw->path.length(),draw->r_proc_path.length()-draw->path.length())
		  << " for the r-process nuclei";

      b=false;
      while (getline(rp,line))
	{
	  if (line.at(0) != '#')
	    {
	      in.clear();
	      in << line;

	      in >> n_rp >> z_rp;

	      if (   z_rp >= draw->Zmin
		  && z_rp <= draw->Zmax
		  && n_rp >= draw->Nmin
		  && n_rp <= draw->Nmax
		  )
		{
		  out_file << std::setw(3)
			   << n_rp-draw->Nmin << " " << z_rp-draw->Zmin;

		  if (!b)
		    {
		      out_file << " m\n";
		      b=true;
		    }
		  else
		    out_file << " l\n";
		}
	    }
	}
      rp.close();
    }
  else
    {
      std::cout << "ERROR: " << draw->r_proc_path
		<< " couldn't be opened to read the r-process path." << std::endl;

      if (shaded)
	out_file << "fill\ngr\n" << std::endl;
      else
	out_file << "st" << std::endl;

      return;
    }

  if (shaded)
    {
      out_file << "fill\n"
	       << "gr\n" << std::endl;
    }
  else
    {
      out_file << "st" << std::endl;

      std::cout << " - done" << std::endl;
    }
}
