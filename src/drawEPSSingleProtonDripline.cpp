#include "functions.h"

void drawEPSSingleProtonDripline(const double meN,
                                 const double meP,
                                 const std::unique_ptr<inputs> &draw,
                                 std::ofstream &outFile
                                 )
{
  //Get straight out if max values of N or Z aren't high enough
  if ( draw->Nmax < 8 || draw->Zmax < 11 )
    {
      return;
    }

  outFile << "\n%--------------------\n"
          << "%- Proton drip line -\n"
          << "%--------------------\n"
          << "gs\n"
          << "purple rgb\n"
          << "1 u div sl" << std::endl;

  drawEPSDripline(meN,meP,draw,outFile,2);

  outFile << "gr" << std::endl;
}
