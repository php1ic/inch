#include "include/functions.h"

#include "extractValue.cpp"

bool readAME(const std::string &table,
	     std::vector<Nuclide> &nuc
	     )
{
  std::cout << "Reading " << table.substr(table.find_last_of("/")+1) << " for updated mass excess values [--";

  if(!checkFileExists(table))
    {
      std::cout << "\nERROR: Mass table " << table << " couldn't be opened." << std::endl;
      return false;
    }

  std::ifstream file(table.c_str());

  std::string line("");
  std::vector<Nuclide>::iterator nuc_it;

  if (file.is_open())
    {
      int i(0), exp(0), A(0), Z(0);

      while (getline(file,line))
	{
	  //Skip the header of the file
	  if (i<39)
	    {
	      ++i;
	      continue;
	    }

	  if (line.find("#") == std::string::npos)
	    {
	      exp=0;
	    }
	  else
	    {
	      exp=1;
	      replace(line.begin(),line.end(),'#',' ');
	    }

	  extractValue(line,16,19,A);

	  extractValue(line,11,14,Z);

	  for (nuc_it=nuc.begin(); nuc_it!=nuc.end(); ++nuc_it)
	    {
	      if (   nuc_it->exp == exp
		  && nuc_it->A   == A
		  && nuc_it->Z   == Z
		     )
		{
		  //-Store mass excess in member AME_ME
		  extractValue(line,29,41,nuc_it->AME_ME);

		  //-Store error on mass excess in member AME_dME
		  extractValue(line,42,53,nuc_it->AME_dME);

		  break;
		}
	    }
	}
      file.close();
    }
  else
    {
      std::cout << "\n\nERROR: " << table << " couldn't be opened, does it exist?\n" << std::endl;
      return false;
    }

  std::cout << "--] done" << std::endl;
  return true;
}
