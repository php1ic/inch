#include "chart.hpp"
#include "io.hpp"
#include "massTable.hpp"
#include "options.hpp"
#include "partition.hpp"
#include "ui.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

int main(int argc, char* argv[])
{
  Options options;
  const IO io;

  io.showBanner();

  const std::map<std::string, std::string> arguments =
      io.readConsoleArguments(std::vector<std::string>(argv, argv + argc));

  if (arguments.size() == 1 && arguments.count("HELP") == 1)
    {
      return 0;
    }

  io.saveConsoleArguments(options, arguments);

  MassTable table(options.path, options.personal_isotopes, options.year, options.AME);

  table.populateInternalMassTable();

  bool logicalInputFile = false;
  if (!arguments.empty() && !options.inputfile.empty())
    {
      const std::map<std::string, std::string> fileOptions = io.readOptionFile(options.inputfile);

      if (options.checkInputFileOptions(fileOptions))
        {
          logicalInputFile = options.validateInputFileOptions(table.theTable);
        }
    }

  options.setOutputFilename();

  if (!logicalInputFile)
    {
      UI interface(table.theTable, options);
      interface.askQuestions();
    }

  options.showChartOptions();

  std::unique_ptr<Partition> part = std::make_unique<Partition>(options.chart_colour);
  /// Define what colours and values will be used to differentiate the nuclei.
  part->setDefaultColours();

  /// Set if an isotope should be ion along with the necessary part of the key.
  table.setIsotopeAttributes(part, options);

  // Write the chart
  const Chart theChart;
  theChart.write(table.theTable, options, part);

  options.writeOptionFile();

  std::cout << "Enjoy\n" << std::endl;

  return 0;
}
