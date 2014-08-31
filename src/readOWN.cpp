#include "include/functions.h"

bool readOWN(const std::string &my_nuclei,
	     std::vector<Nuclide> &nuc
	     )
{
  std::cout << "Reading " << my_nuclei.substr(my_nuclei.find_last_of("/")+1)
	    << " for user selected nuclei (--";

  if ( !checkFileExists(my_nuclei) )
    {
      std::cout << "\n"
		<< "***ERROR***: File " << my_nuclei
		<< " couldn't be opened." << std::endl;
      return false;
    }

  std::ifstream my_nuc(my_nuclei.c_str());

  if ( !my_nuc.is_open() )
    {
      std::cout << "\n"
		<< "***ERROR***: " <<  my_nuclei
		<< " couldn't be opened, does it exist?\n" << std::endl;
      return false;
    }

  std::string line;
  std::vector<Nuclide>::iterator nuc_it;

  while (getline(my_nuc,line))
    {
      if ( !line.compare("") || line.at(0) == '#' )
	continue;

      int N=0, Z=0, st=0;
      std::stringstream in(line);
      in >> N >> Z >> st;

      for (nuc_it=nuc.begin(); nuc_it!=nuc.end(); ++nuc_it)
	{
	  if(   nuc_it->N  == N
	     && nuc_it->Z  == Z
	     && nuc_it->st == st
	     )
	    {
	      nuc_it->own = true;
	      break;
	    }
	}
    }

  my_nuc.close();

  std::cout << "--) done" << std::endl;

  return true;
}
