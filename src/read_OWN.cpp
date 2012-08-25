#include "functions.h"

void read_OWN(const std::string &my_nuclei, std::vector<Nuclide> &nuc)
{
  bool k=false;
  unsigned short
    j=0,
    *N = new unsigned short[3];
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
      exit(-1);
    }

  delete line;
  delete[] N;

  //-Set member own to 0 if nuclei not in user's file
  for (nuc_it=nuc.begin();nuc_it!=nuc.end();nuc_it++)
    {
      k=false;
      for (j=0; j<own_N.size(); j++)
	{
	  if(    own_N.at(j) == nuc_it->N
	     &&  own_Z.at(j) == nuc_it->Z
	     && own_IS.at(j) == nuc_it->st)
	    {
	      nuc_it->own = 1;
	      k=true;
	      break;
	    }
	}

      if (!k)
	nuc_it->own = 0;
    }
}
