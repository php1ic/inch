#include "inch/chart.hpp"
#include "inch/io.hpp"
#include "inch/massTable.hpp"
#include "inch/options.hpp"
#include "inch/partition.hpp"
#include "inch/ui.hpp"

#include <fmt/format.h>

#include <iostream>
#include <memory>
#include <string>
#include <vector>

int main(int argc, char* argv[])
{
  const IO io;

  io.showBanner();

  const std::map<std::string, std::string> arguments =
      io.readConsoleArguments(std::vector<std::string>(argv, argv + argc));

  if (arguments.size() == 1 && arguments.count("HELP") == 1)
    {
      return 0;
    }

  Options runOptions;
  io.saveConsoleArguments(runOptions, arguments);

  MassTable table(runOptions.data_path, runOptions.personal_isotopes, runOptions.year, runOptions.AME);

  table.populateInternalMassTable();

  bool logicalInputFile{ false };
  if (!arguments.empty() && !runOptions.inputfile.empty())
    {
      const std::map<std::string, std::string> fileOptions = io.readOptionFile(runOptions.inputfile);

      if (runOptions.checkInputFileOptions(fileOptions))
        {
          logicalInputFile = runOptions.validateInputFileOptions(table.theTable);
        }
    }

  runOptions.setOutputFilename();

  if (!logicalInputFile)
    {
      const UI ui(table.theTable, runOptions);
      ui.askQuestions();
    }

  runOptions.showChartOptions();

  Partition part(runOptions.chart_colour);
  /// Define what colours and values will be used to differentiate the nuclei.
  part.setDefaultColours();

  table.setIsotopeAttributes(part, runOptions);

  /// Write the chart
  const Chart theChart;
  theChart.write(table.theTable, runOptions, part);

  runOptions.writeOptionFile();

  fmt::print("Enjoy!\n");

  return 0;
}
