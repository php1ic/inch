#include "include/createChart.h"

createChart::createChart(inputs *draw,
			 partition *part,
			 std::vector<Nuclide> &nuc,
			 std::vector<std::string> &arguments)
{
  buildAllFullFilenames(draw);

  populateInternalMassTable(draw,nuc);

  //-Check and validate arguments
  if ( !validateInputArguments(nuc,draw,arguments) )
    displaySection(nuc,draw);

  printSelection(draw);

  constructChart(draw,part,nuc);
}


createChart::createChart(inputs *draw,
			 partition *part,
			 std::vector<Nuclide> &nuc)
{
  buildAllFullFilenames(draw);

  populateInternalMassTable(draw,nuc);

  constructOutputFilename(draw);

  //-Ask how the chart should be displayed
  displaySection(nuc,draw);

  printSelection(draw);

  constructChart(draw,part,nuc);
}

createChart::~createChart()
{
}
