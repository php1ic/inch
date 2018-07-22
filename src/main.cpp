#include "chart.hpp"

int main(int argc, char *argv[])
{
  std::unique_ptr<inputs> draw = std::make_unique<inputs>();
  draw->showBanner();

  draw->valid_console = draw->readConsoleArguments(std::vector<std::string> (argv,argv+argc));

  if ( draw->valid_console == 2 )
    {
      return 0;
    }

  std::unique_ptr<MassTable> table = std::make_unique<MassTable>(draw->path, draw->year, draw->AME);

  std::cout << "\nSetting the path to the data files as:\n"
            << draw->path << "\n" << std::endl;

  table->populateInternalMassTable();

  bool logicalInputFile = false;
  if ( draw->valid_inputfile )
    {
      logicalInputFile = draw->validateInputFile(table->theTable);
    }

  if ( !logicalInputFile )
    {
      draw->displaySection(table->theTable);
    }

  draw->showChartOptions();

  std::unique_ptr<Partition> part = std::make_unique<Partition>(draw->choice);
  /// Define what colours and values will be used to differentiate the nuclei.
  part->setDefaultColours();

  /// Set if an isotope should be drawn along with the necessary part of the key.
  setIsotopeAttributes(table->theTable, part, draw);

  /// Write the chart
  const Chart theChart;
  theChart.write(table->theTable, draw, part);

  draw->writeOptionFile();

  std::cout << "Enjoy\n" << std::endl;

  return 0;
}
