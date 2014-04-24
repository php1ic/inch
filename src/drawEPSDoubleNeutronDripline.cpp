#include "include/functions.h"

void drawEPSDoubleNeutronDripline(const std::vector<Nuclide> &nuc,
				 inputs *draw,
				 std::ofstream &out_file
				 )
{
  //Get straight out if max values of N or Z aren't high enough
  if (draw->Nmax < 20 || draw->Zmax < 8)
    return;

  out_file << "\n%-------------------------\n"
	   << "%- Two-Neutron drip line -\n"
	   << "%-------------------------\n"
	   << "gs\n"
	   << "darkgreen rgb\n"
	   << "1 u div sl" << std::endl;

  drawEPSDripline(nuc,draw,out_file,1);
}
