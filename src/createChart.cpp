#include "createChart.h"

createChart::createChart(std::unique_ptr<inputs> &draw,
                         std::unique_ptr<partition> &part,
                         std::vector<Nuclide> &nuc)
{

  std::cout << "\nSetting the path to the data files as:\n"
            << draw->path << "\n" << std::endl;

  populateInternalMassTable(draw,nuc);

  bool logicalInputFile = false;
  if ( draw->valid_inputfile )
    {
      logicalInputFile = validateInputFile(nuc,draw);
    }

  if ( !logicalInputFile )
    {
      displaySection(nuc,draw);
    }

  draw->showChartOptions();

  constructChart(draw,part,nuc);
}


createChart::~createChart()
{
}
