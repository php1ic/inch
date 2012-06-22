#include "functions.h"

void read_AME(const std::string &table, std::vector<Nuclide> &nuc)
{
  std::ifstream file(table.c_str());

  unsigned short
    *i = new unsigned short,
    *exp = new unsigned short,
    *A = new unsigned short,
    *Z = new unsigned short,
    *num = new unsigned short;
  std::string *line = new std::string;
  char *c = new char[13];
  std::vector<Nuclide>::iterator nuc_it;

  if (file.is_open())
    {
      *i=0;
      while (getline(file,*line))
	{
	  if (*i>38)
	    {
	      if (line->find("#") == std::string::npos)
		*exp=0;
	      else
		{
		  *exp=1;

		  do line->replace(line->find("#"),1," ");
		  while (line->find("#") != std::string::npos);
		}

	      *num = line->copy(c,3,16);
	      c[*num] = '\0';
	      *A = atoi(c);

	      *num = line->copy(c,3,11);
	      c[*num] = '\0';
	      *Z = atoi(c);

	      for (nuc_it=nuc.begin(); nuc_it!=nuc.end(); nuc_it++)
		{
		  if (   nuc_it->exp == *exp
		      && nuc_it->A == *A
		      && nuc_it->Z == *Z)
		    {
		      //-Store mass excess in member AME_ME
		      *num = line->copy(c,12,29);
		      c[*num] = '\0';
		      nuc_it->AME_ME = atof(c);

		      //-Store error on mass excess in member AME_dME
		      *num = line->copy(c,11,42);
		      c[*num] = '\0';
		      nuc_it->AME_dME = atof(c);

		      break;
		    }
		}
	    }
	  (*i)++;
	}
      file.close();
    }
  else
    {
      std::cout << "\n\nERROR: " << table << " couldn't be opened, does it exist?\n" << std::endl;
      exit(-1);
    }

  delete[] c;
  delete i;
  delete exp;
  delete A;
  delete Z;
  delete num;
  delete line;
}
