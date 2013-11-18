#include "include/functions.h"

bool readOWN(const std::string &my_nuclei,
	     std::vector<Nuclide> &nuc
	     )
{
  std::cout << "Reading " << my_nuclei << " for user selected nuclei (--";

  if(!checkFileExists(my_nuclei))
    {
      std::cout << "\nERROR: File " << my_nuclei << " couldn't be opened." << std::endl;
      return 1;
    }

  bool
    rValue=0;
  //k=false;
  //unsigned int j=0;
  int *N = new int[3];
  std::string *line = new std::string;
  std::vector<int> own_N, own_Z, own_IS;
  std::vector<Nuclide>::iterator nuc_it;

  std::ifstream my_nuc(my_nuclei.c_str());
  std::stringstream in;

  if (my_nuc.is_open())
    {
      while (getline(my_nuc,*line))
	{
	  in.clear();
	  in << *line;

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
      rValue=1;
    }

  delete line;
  delete[] N;

  if (!rValue)
    {
      rValue=1;
      //-Set member own to 0 if nuclei not in user's file
      for (nuc_it=nuc.begin(); nuc_it!=nuc.end(); ++nuc_it)
	{
	  bool k=false;
	  unsigned int j=0;
	  for (j=0; j<own_N.size(); ++j)
	    {
	      if(    own_N.at(j) == nuc_it->N
		     &&  own_Z.at(j) == nuc_it->Z
		     && own_IS.at(j) == nuc_it->st)
		{
		  nuc_it->own = k = true;
		  break;
		}
	    }

	  if (!k)
	    nuc_it->own = false;
	}
      std::cout << "--) done" << std::endl;
    }

  return rValue;
}
