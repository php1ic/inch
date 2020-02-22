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

  Options options;
  io.saveConsoleArguments(options, arguments);

  MassTable table(options.data_path, options.personal_isotopes, options.year, options.AME);

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
      const UI interface(table.theTable, options);
      interface.askQuestions();
    }

  options.showChartOptions();

  Partition part(options.chart_colour);
  /// Define what colours and values will be used to differentiate the nuclei.
  part.setDefaultColours();

  table.setIsotopeAttributes(part, options);

  /// Write the chart
  const Chart theChart;
  theChart.write(table.theTable, options, part);

  options.writeOptionFile();

  fmt::print("Enjoy!\n");

  return 0;
}
