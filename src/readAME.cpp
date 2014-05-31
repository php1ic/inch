#include "include/functions.h"

#include "extractValue.cpp"

bool readAME(const std::string &table,
	     std::vector<Nuclide> &nuc
	     )
{
  std::cout << "Reading " << table.substr(table.find_last_of("/")+1)
	    << " for updated mass excess values [--";

  if (!checkFileExists(table))
    {
      std::cout << "\n"
		<< "***ERROR***: Mass table "
		<< table << " couldn't be opened." << std::endl;
      return false;
    }

  std::ifstream file(table.c_str());

  std::string line("");
  std::vector<Nuclide>::iterator nuc_it;

  if (file.is_open())
    {
      int i(0), A(0), Z(0);

      while (getline(file,line))
	{
	  //Skip the header of the file
	  if (i<39)
	    {
	      ++i;
	      continue;
	    }

	  //-Will use mass excess for criteria, the last digit is char 52 so if
	  //-there is a '#' but it's after this we will still say experimental
	  int exp=0;
	  size_t measured = line.find_first_of("#");
	  if (measured == std::string::npos || measured > 52)
	    exp=1;

	  if (measured != std::string::npos)
	    replace(line.begin(),line.end(),'#',' ');

	  extractValue(line,16,19,A);

	  extractValue(line,11,14,Z);

	  for (nuc_it=nuc.begin(); nuc_it!=nuc.end(); ++nuc_it)
	    {
	      if (   nuc_it->st == 0
		  && nuc_it->A  == A
		  && nuc_it->Z  == Z
		  )
		{
		  //-Store mass excess in member AME_ME
		  extractValue(line,29,41,nuc_it->AME_ME);

		  //-Store error on mass excess in member AME_dME
		  extractValue(line,42,53,nuc_it->AME_dME);

		  nuc_it->exp = exp;

		  break;
		}
	    }
	}
      file.close();
    }
  else
    {
      std::cout << "\n"
		<< "***ERROR***: "
		<< table << " couldn't be opened, does it exist?\n" << std::endl;
      return false;
    }

  std::cout << "--] done" << std::endl;
  return true;
}
