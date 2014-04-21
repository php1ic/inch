#include "include/functions.h"

void drawEPSDoubleDriplines(const std::vector<Nuclide> &nuc,
			    inputs *draw,
			    std::ofstream &out_file
			    )
{
  //-May only want one of the drip lines
  //------------------------------------------
  // double_drip_lines = 1 -> both
  // double_drip_lines = 2 -> only p drip line
  // double_drip_lines = 3 -> only n drip line
  //------------------------------------------

  if (draw->double_drip_lines != 3)
    drawEPSDoubleProtonDripline(nuc, draw, out_file);
  else
    std::cout << "Not drawing the two-proton drip line" << std::endl;

  if (draw->double_drip_lines != 2)
    drawEPSDoubleNeutronDripline(nuc, draw, out_file);
  else
    std::cout << "Not drawing the two-neutron drip line" << std::endl;
}
