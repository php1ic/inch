#include "createChart.hpp"

createChart::createChart(std::unique_ptr<inputs> &draw,
                         std::unique_ptr<partition> &part,
                         std::unique_ptr<MassTable> &table)
{

  std::cout << "\nSetting the path to the data files as:\n"
            << draw->path << "\n" << std::endl;

  table->populateInternalMassTable();

  bool logicalInputFile = false;
  if ( draw->valid_inputfile )
    {
      logicalInputFile = validateInputFile(table->theTable,draw);
    }

  if ( !logicalInputFile )
    {
      displaySection(table->theTable,draw);
    }

  draw->showChartOptions();

  constructChart(draw,part,table->theTable);
}
