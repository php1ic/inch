#include "functions.h"

bool readOWN(const std::string &myNuclei,
             std::vector<Nuclide> &nuc
             )
{
  std::cout << "Reading " << myNuclei.substr(myNuclei.find_last_of('/')+1)
            << " for user selected nuclei (--";

  std::ifstream inFile(myNuclei.c_str());

  if ( !inFile )
    {
      std::cout << "\n"
                << "***ERROR***: " <<  myNuclei
                << " couldn't be opened.\n" << std::endl;
      return false;
    }

  std::string line;

  while ( getline(inFile,line) )
    {
      if ( !line.compare("") || line.at(0) == '#' )
        {
          continue;
        }

      int N=0;
      int Z=0;
      int st=0;

      std::istringstream ownData(line);
      ownData >> N >> Z >> st;

      for ( auto &it : nuc )
        {
          if(   it.N  == N
             && it.Z  == Z
             && it.st == st
             )
            {
              it.setOwn(true);
              break;
            }
        }
    }

  inFile.close();

  std::cout << "--) done" << std::endl;

  return true;
}
