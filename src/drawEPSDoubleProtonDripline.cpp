#include "include/functions.h"

void drawEPSDoubleProtonDripline(const std::vector<Nuclide> &nuc,
				 inputs *draw,
				 std::ofstream &out_file
				 )
{
  //Get straight out if max values of N or Z aren't high enough
  if (draw->Nmax < 8 || draw->Zmax < 14)
    return;

  out_file << "\n%------------------------\n"
	   << "%- Two-Proton drip line -\n"
	   << "%------------------------\n"
	   << "gs\n"
	   << "darkgreen rgb\n"
	   << "1 u div sl" << std::endl;

  drawEPSDripline(nuc,draw,out_file,3);
}
