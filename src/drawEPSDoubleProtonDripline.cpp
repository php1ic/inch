#include "include/functions.h"

void drawEPSDoubleProtonDripline(const std::vector<Nuclide> &nuc,
				 const inputs *draw,
				 std::ofstream &outFile
				 )
{
  //Get straight out if max values of N or Z aren't high enough
  if (draw->Nmax < 8 || draw->Zmax < 14)
    return;

  outFile << "\n%------------------------\n"
	  << "%- Two-Proton drip line -\n"
	  << "%------------------------\n"
	  << "gs\n"
	  << "darkgreen rgb\n"
	  << "1 u div sl" << std::endl;

  drawEPSDripline(nuc,draw,outFile,3);
}
