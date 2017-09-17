#include "functions.h"

void writeChart(std::vector<Nuclide> &nuc,
               inputs *draw,
               partition *part
               )
{
  std::cout << "\nCreating " << draw->outfile << "\n|--\n";


  if ( draw->filetype == FileType::EPS )
    {
      writeEPS(nuc,draw,part);
    }
  else if ( draw->filetype == FileType::SVG )
    {
      writeSVG(nuc,draw);
    }
  else if ( draw->filetype == FileType::TIKZ )
    {
      writeTIKZ(nuc,draw);
    }

  std::cout << "--| done\n" << std::endl;
}
