#include "functions.h"

void drawEPSDoubleNeutronDripline(const double meN,
                                  const double meP,
                                  const inputs *draw,
                                  std::ofstream &outFile
                                  )
{
  //Get straight out if max values of N or Z aren't high enough
  if (draw->Nmax < 20 || draw->Zmax < 8)
    {
      return;
    }

  outFile << "\n%-------------------------\n"
          << "%- Two-Neutron drip line -\n"
          << "%-------------------------\n"
          << "gs\n"
          << "darkgreen rgb\n"
          << "1 u div sl" << std::endl;

  drawEPSDripline(meN,meP,draw,outFile,1);

  outFile << "gr" << std::endl;
}
