#include "include/functions.h"

void drawEPSSingleNeutronDripline(const double meN,
                                  const double meP,
                                  const inputs *draw,
                                  std::ofstream &outFile
                                  )
{
  //Get straight out if max values of N or Z aren't high enough
  if (draw->Nmax < 17 || draw->Zmax < 8)
    return;

  outFile << "\n%---------------------\n"
          << "%- Neutron drip line -\n"
          << "%---------------------\n"
          << "gs\n"
          << "purple rgb\n"
          << "1 u div sl" << std::endl;

  drawEPSDripline(meN,meP,draw,outFile,0);
}
