#include "include/functions.h"

void constructChart(inputs *draw,
		    partition* part,
		    std::vector<Nuclide> &nuc)
{
  //-Define what colours and values will be used to differentiate the nuclei.
  setColours(part,draw);

  //-Set if an isotope should be drawn along with the necessary part of the key.
  setIsotopeAttributes(nuc,part,draw);

  //-Use the drawn nuclei to decide how large the key should be.
  setKeyScale(draw,part);

  //-Set the size of the canvas
  setCanvasSize(draw);

  //-Write the chart
  writeChart(nuc,draw,part);
}
