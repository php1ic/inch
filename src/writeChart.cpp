#include "functions.hpp"

void writeChart(std::vector<Nuclide> &nuc,
               std::unique_ptr<inputs> &draw,
               std::unique_ptr<partition> &part
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
