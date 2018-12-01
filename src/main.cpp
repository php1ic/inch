#include "chart.hpp"
#include "massTable.hpp"

int main(int argc, char* argv[])
{
  std::unique_ptr<inputs> draw = std::make_unique<inputs>();
  draw->showBanner();

  draw->valid_console = draw->readConsoleArguments(std::vector<std::string>(argv, argv + argc));

  if (draw->valid_console == 2)
    {
      return 0;
    }

  std::unique_ptr<MassTable> table =
      std::make_unique<MassTable>(draw->path, draw->personal_isotopes, draw->year, draw->AME);

  table->populateInternalMassTable();

  const bool logicalInputFile = (draw->valid_inputfile) ? draw->validateInputFile(table->theTable) : false;

  if (!logicalInputFile)
    {
      draw->displaySection(table->theTable);
    }

  draw->showChartOptions();

  std::unique_ptr<Partition> part = std::make_unique<Partition>(draw->chart_colour);
  /// Define what colours and values will be used to differentiate the nuclei.
  part->setDefaultColours();

  /// Set if an isotope should be drawn along with the necessary part of the key.
  table->setIsotopeAttributes(part, draw);

  /// Write the chart
  const Chart theChart;
  theChart.write(table->theTable, draw, part);

  draw->writeOptionFile();

  std::cout << "Enjoy\n" << std::endl;

  return 0;
}
