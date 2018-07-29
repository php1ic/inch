#include "rProcess.hpp"

bool rProcess::readData()
{
  data.reserve(150);

  std::cout << "Reading "
            << file
            << " for the r-process nuclei";

  std::ifstream rp(file, std::ios::binary);

  if ( !rp )
    {
      std::cerr << "***ERROR***: " << file
                << " couldn't be opened to read the r-process path." << std::endl;
      return false;
    }

  std::string line;

  while ( std::getline(rp,line) )
    {
      if ( line.empty() || line.at(0) == '#' )
        {
          continue;
        }

      int n=0;
      int z=0;

      std::istringstream rData(line);
      rData >> n >> z;

      data.emplace_back(n,z);
    }

  rp.close();

  std::cout << " - done" << std::endl;

  return true;
}


void rProcess::EPSWritePath(std::ofstream &outFile, const bool shaded) const
{
  if ( shaded )
    {
      //readData();

      outFile  << "\n%r-process -- shaded path\n"
              << "gs\n"
              << "0.9 setgray" << std::endl;
    }
  else
    {
      outFile << "\n%r-process -- path outline\n"
              << "black rgb\n"
              << "0.1 u div sl" << std::endl;
    }

  bool initial = true;

  for ( const auto it : data )
    {
      //it.first = N
      //it.second = Z
      if (   it.second >= Zmin
          && it.second <= Zmax
          && it.first  >= Nmin
          && it.first  <= Nmax
          )
        {
          outFile << std::setw(3) << it.first - Nmin << " "
                  << std::setw(3) << it.second - Zmin << " "
                  << (initial ? 'm' : 'l') << '\n';

          initial=false;
        }
    }

  if ( shaded )
    {
      outFile << "fill\n"
              << "gr" << std::endl;
    }
  else
    {
      outFile << "st" << std::endl;
    }
}
