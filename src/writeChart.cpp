#include "functions.h"

void writeChart(std::vector<Nuclide> &nuc,
               inputs *draw,
               partition *part
               )
{
  std::cout << "\nCreating " << draw->outfile << "\n|--\n";

  if ( draw->file_type == 0 )
    {
      writeEPS(nuc,draw,part);
    }
  else if ( draw->file_type == 1 )
    {
      writeSVG(nuc,draw);
    }
  else if ( draw->file_type == 2 )
    {
      writeTIKZ(nuc,draw);
    }

  std::cout << "--| done\n" << std::endl;
}
