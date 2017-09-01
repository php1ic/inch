#include "createChart.h"

createChart::createChart(inputs *draw,
                         partition *part,
                         std::vector<Nuclide> &nuc,
                         std::vector<std::string> &arguments)
{
  //-Check and validate arguments
  if ( !validateInputArguments(nuc,draw,arguments) )
    {
      displaySection(nuc,draw);
    }

  draw->constructFullyQualifiedPaths();

  populateInternalMassTable(draw,nuc);

  draw->showChartOptions();

  constructChart(draw,part,nuc);
}


createChart::createChart(inputs *draw,
                         partition *part,
                         std::vector<Nuclide> &nuc)
{
  draw->constructFullyQualifiedPaths();

  populateInternalMassTable(draw,nuc);

  draw->constructOutputFilename();

  //-Ask how the chart should be displayed
  displaySection(nuc,draw);

  draw->showChartOptions();

  constructChart(draw,part,nuc);
}

createChart::~createChart()
{
}
