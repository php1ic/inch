#include "functions.hpp"

void constructChart(std::unique_ptr<inputs> &draw,
                    std::unique_ptr<partition> &part,
                    std::vector<Nuclide> &nuc)
{
  //-Define what colours and values will be used to differentiate the nuclei.
  part->setColours(draw->choice);

  //-Set if an isotope should be drawn along with the necessary part of the key.
  setIsotopeAttributes(nuc,part,draw);

  //-Write the chart
  writeChart(nuc,draw,part);
}
