#include "include/functions.h"

bool readOWN(const std::string &my_nuclei,
	     std::vector<Nuclide> &nuc
	     )
{
  std::cout << "Reading " << my_nuclei << " for user selected nuclei (--";

  if(!checkFileExists(my_nuclei))
    {
      std::cout << "\nERROR: File " << my_nuclei << " couldn't be opened." << std::endl;
      return false;
    }

  std::string line("");
  std::vector<int> own_N, own_Z, own_IS;
  std::vector<Nuclide>::iterator nuc_it;

  std::ifstream my_nuc(my_nuclei.c_str());
  std::stringstream in;

  if (my_nuc.is_open())
    {
      while (getline(my_nuc,line))
	{
	  int N[3];
	  in.clear();
	  in << line;

	  in >> N[0] >> N[1] >> N[2];

	  own_N.push_back(N[0]);
	  own_Z.push_back(N[1]);
	  own_IS.push_back(N[2]);
	}
      my_nuc.close();
    }
  else
    {
      std::cout << "\nERROR: " <<  my_nuclei << " couldn't be opened, does it exist?\n" << std::endl;
      return false;
    }

  //-Set member own to 0 if nuclei not in user's file
  for (nuc_it=nuc.begin(); nuc_it!=nuc.end(); ++nuc_it)
    {
      bool k=false;
      for (unsigned int i=0; i<own_N.size(); ++i)
	{
	  if(    own_N.at(i) == nuc_it->N
	     &&  own_Z.at(i) == nuc_it->Z
	     && own_IS.at(i) == nuc_it->st)
	    {
	      nuc_it->own = true;
	      k++;
	      break;
	    }
	}

      if (!k)
	nuc_it->own = false;
    }
  std::cout << "--) done" << std::endl;

  return true;
}
