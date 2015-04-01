#include "include/functions.h"

void drawEPSDoubleDriplines(const double meN,
                            const double meP,
                            const inputs *draw,
                            std::ofstream &outFile
                            )
{
  //-May only want one of the drip lines
  //------------------------------------------
  // double_drip_lines = 1 -> both
  // double_drip_lines = 2 -> only p drip line
  // double_drip_lines = 3 -> only n drip line
  //------------------------------------------

  if (draw->double_drip_lines != 3)
    drawEPSDoubleProtonDripline(meN,meP,draw,outFile);
  else
    std::cout << "Not drawing the two-proton drip line" << std::endl;

  if (draw->double_drip_lines != 2)
    drawEPSDoubleNeutronDripline(meN,meP,draw,outFile);
  else
    std::cout << "Not drawing the two-neutron drip line" << std::endl;
}
