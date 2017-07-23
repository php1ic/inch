#include "functions.h"

bool readAME(const std::string &table,
             std::vector<Nuclide> &nuc
             )
{
  std::cout << "Reading " << table.substr(table.find_last_of("/")+1)
            << " for updated mass excess values [--";

  std::ifstream file(table.c_str());

  if ( !file )
    {
      std::cout << "\n"
                << "***ERROR***: "
                << table << " couldn't be opened, does it exist?\n" << std::endl;
      return false;
    }

  int i=0;
  std::string line;

  while ( getline(file,line) )
    {
      // Skip the header of the file
      if ( i < AME_HEADER_LENGTH )
        {
          ++i;
          continue;
        }

      int exp=0;
      int A=0;
      int Z=0;

      // Will use mass excess for criteria, the last digit is char 52 so if
      // there is a '#' but it's after this we will still say experimental
      size_t measured = line.find_first_of("#");
      if ( measured == std::string::npos || measured > AME_EXPERIMENTAL_MARKER )
        {
          exp = 1;
        }

      if ( measured != std::string::npos )
        {
          replace(line.begin(),line.end(),'#',' ');
        }

      extractValue(line,AME_START_A,AME_END_A,A);

      extractValue(line,AME_START_Z,AME_END_Z,Z);

      for ( auto &it : nuc )
        {
          if (   it.st == 0
              && it.A  == A
              && it.Z  == Z
              )
            {
              it.setAMEMassExcess(line);

              it.setAMEMassExcessError(line);

              it.setExperimental(exp);

              break;
            }
        }
    }

  file.close();

  std::cout << "--] done" << std::endl;
  return true;
}
