#include "functions.h"

void drawEPSSingleDriplines(const double meN,
                            const double meP,
                            const inputs *draw,
                            std::ofstream &outFile
                            )
{
  //-May only want one of the drip lines
  //------------------------------------------
  // single_drip_lines = 1 -> both
  // single_drip_lines = 2 -> only p drip line
  // single_drip_lines = 3 -> only n drip line
  //------------------------------------------

  if (draw->single_drip_lines != 3)
    drawEPSSingleProtonDripline(meN,meP,draw,outFile);
  else
    std::cout << "Not drawing the one-proton drip line" << std::endl;

  if (draw->single_drip_lines != 2)
    drawEPSSingleNeutronDripline(meN,meP,draw,outFile);
  else
    std::cout << "Not drawing the one-neutron drip line" << std::endl;
}
