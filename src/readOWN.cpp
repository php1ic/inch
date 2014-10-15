#include "include/functions.h"

bool readOWN(const std::string &myNuclei,
	     std::vector<Nuclide> &nuc
	     )
{
  std::cout << "Reading " << myNuclei.substr(myNuclei.find_last_of("/")+1)
	    << " for user selected nuclei (--";

  if ( !checkFileExists(myNuclei) )
    {
      std::cout << "\n"
		<< "***ERROR***: File " << myNuclei
		<< " couldn't be opened." << std::endl;
      return false;
    }

  std::ifstream inFile(myNuclei.c_str());

  if ( !inFile.is_open() )
    {
      std::cout << "\n"
		<< "***ERROR***: " <<  myNuclei
		<< " couldn't be opened, does it exist?\n" << std::endl;
      return false;
    }

  std::string line;

  while (getline(inFile,line))
    {
      if ( !line.compare("") || line.at(0) == '#' )
	continue;

      int N=0;
      int Z=0;
      int st=0;

      sscanf(line.c_str(), "%d %d %d", &N, &Z, &st);

      for (std::vector<Nuclide>::iterator it=nuc.begin(); it!=nuc.end(); ++it)
	{
	  if(   it->N  == N
	     && it->Z  == Z
	     && it->st == st
	     )
	    {
	      it->setOwn(true);
	      break;
	    }
	}
    }

  inFile.close();

  std::cout << "--) done" << std::endl;

  return true;
}
