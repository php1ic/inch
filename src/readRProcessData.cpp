#include "include/functions.h"

bool readRProcessData(inputs *draw)
{
  std::ifstream rp(draw->r_proc_path.c_str());

  if (rp.is_open())
    {
      std::cout << "Reading "
		<< draw->r_proc_path.substr(draw->r_proc_path.find_last_of("/")+1)
		<< " for the r-process nuclei";

      draw->rProcessData.reserve(countLinesInFile(rp));

      std::string line;

      while (getline(rp,line))
	{
	  if ( !line.compare("") || line.at(0) == '#' )
	    continue;

	  int n_rp=0, z_rp=0;
	  std::stringstream in(line);

	  in >> n_rp >> z_rp;

	  draw->rProcessData.push_back(std::make_pair(n_rp,z_rp));
	}
      rp.close();

      std::cout << " - done" << std::endl;
    }
  else
    {
      std::cout << "***ERROR***: " << draw->r_proc_path
		<< " couldn't be opened to read the r-process path." << std::endl;
      return false;
    }

  return true;
}
