#include "functions.hpp"

void constructChart(std::unique_ptr<inputs> &draw,
                    std::unique_ptr<partition> &part,
                    std::vector<Nuclide> &nuc)
{
  //-Define what colours and values will be used to differentiate the nuclei.
  setColours(part,draw);

  //-Set if an isotope should be drawn along with the necessary part of the key.
  setIsotopeAttributes(nuc,part,draw);

  //-Use the drawn nuclei to decide how large the key should be.
  setKeyScale(draw,part);

  //-Set the size of the canvas
  draw->setCanvasSize();

  //-Write the chart
  writeChart(nuc,draw,part);
}
