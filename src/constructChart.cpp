#include "include/functions.h"

void constructChart(inputs *draw,
		    partition* part,
		    std::vector<Nuclide> &nuc)
{
  //-Define what colours and values will be used to differentiate the nuclei.
  setColours(part,draw);

  //-Draw the nuclei in the selected range.
  showNuclei(nuc,part,draw);

  //-Use the drawn nuclei to decide how large the key should be.
  setKeyScale(draw,part);

  //-Set the size of the canvas
  setCanvasSize(draw);

  //-Write the chart -
  drawChart(nuc,draw,part);
}
