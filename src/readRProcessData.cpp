#include "functions.hpp"

bool readRProcessData(std::unique_ptr<inputs> &draw)
{
  std::cout << "Reading "
            << draw->r_proc_path.substr(draw->r_proc_path.find_last_of('/')+1)
            << " for the r-process nuclei";

  std::ifstream rp(draw->r_proc_path.c_str());

  if ( !rp )
    {
      std::cout << "***ERROR***: " << draw->r_proc_path
                << " couldn't be opened to read the r-process path." << std::endl;
      return false;
    }

  std::string line;

  while ( getline(rp,line) )
    {
      if ( line.empty() || line.at(0) == '#' )
        {
          continue;
        }

      int n=0;
      int z=0;

      std::istringstream rData(line);
      rData >> n >> z;

      draw->r_process_data.emplace_back(n,z);
    }

  rp.close();

  std::cout << " - done" << std::endl;

  return true;
}
