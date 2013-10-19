#include "include/functions.h"

void drawChart(std::vector<Nuclide> &nuc,
		inputs *draw,
		partition *part
		)
{
  std::cout << "\nCreating " << draw->outfile << "\n|--";

  if (draw->file_type == 0)
    writeEPS(nuc,draw,part);
  else if (draw->file_type == 1)
    writeSVG(nuc,draw,part);
  else if (draw->file_type == 2)
    writeTIKZ(nuc,draw,part);

  std::cout << "--| done\n" << std::endl;
}
