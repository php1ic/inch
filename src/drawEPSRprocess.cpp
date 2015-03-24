#include "include/functions.h"

void drawEPSRprocess(inputs *draw,
                     std::ofstream &outFile,
                     const bool shaded
                     )
{
  //r-process data starts at Z=26.
  //no need to do anything if we aren't displaying above that.
  if ( draw->Zmax < 26 )
    return;

  if ( shaded )
    {
      readRProcessData(draw);

      outFile << "\n%----------------------------\n"
              << "%- r-process -- shaded path -\n"
              << "%----------------------------\n"
              << "gs\n"
              << "0.9 setgray" << std::endl;
    }
  else
    {
      outFile << "\n%-----------------------------\n"
              << "%- r-process -- path outline -\n"
              << "%-----------------------------\n"
              << "black rgb\n"
              << "0.1 u div sl" << std::endl;
    }

  bool initial=true;
  std::vector<std::pair<int,int> >::iterator it;

  for (it=draw->r_process_data.begin(); it!=draw->r_process_data.end(); ++it)
    {
      //it->first = N
      //it->second = Z
      if (   it->second >= draw->Zmin
          && it->second <= draw->Zmax
          && it->first  >= draw->Nmin
          && it->first  <= draw->Nmax
          )
        {
          outFile << std::setw(3) << it->first-draw->Nmin << " "
                  << std::setw(3) << it->second-draw->Zmin << " "
                  << (initial ? "m" : "l") << "\n";

          initial=false;
        }
    }

  if ( shaded )
    {
      outFile << "fill\n"
              << "gr\n" << std::endl;
    }
  else
    {
      outFile << "st" << std::endl;
    }
}
