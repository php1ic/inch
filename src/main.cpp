#include "inch/eps_chart.hpp"
#include "inch/io.hpp"
#include "inch/massTable.hpp"
#include "inch/options.hpp"
#include "inch/partition.hpp"
#include "inch/svg_chart.hpp"
#include "inch/tikz_chart.hpp"
#include "inch/ui.hpp"

#include <fmt/core.h>

#include <filesystem>
#include <map>
#include <string>
#include <vector>

int main(int argc, char* argv[])
{
  fmt::print("{}", IO::showBanner());

  const IO io;
  const auto arguments = io.readConsoleArguments(std::vector<std::string>(argv, argv + argc));

  if (arguments.size() == 1 && arguments.count("HELP") == 1)
    {
      return 0;
    }

  Options runOptions = IO::saveConsoleArguments(arguments);

  fmt::print("Reading to the data files as : {}\n", Options::getAbsolutePath().string());

  MassTable table(runOptions);

  // If we haven't read the file, might as well just exit
  if (!table.populateInternalMassTable())
    {
      fmt::print("\n"
                 "***ERROR***: Mass table has not been populated, exiting\n"
                 "\n");
      return 1;
    }

  bool logicalInputFile{ false };
  if (!arguments.empty() && !runOptions.inputfile.empty())
    {
      const auto fileOptions = IO::readOptionFile(runOptions.inputfile);

      if (runOptions.checkInputFileOptions(fileOptions))
        {
          logicalInputFile = runOptions.validateInputFileOptions(table.theTable);
        }
    }

  const auto valid_outputfile = runOptions.setOutputFilename();
  if (!valid_outputfile)
    {
      return 1;
    }

  if (!logicalInputFile)
    {
      const UI ui(runOptions);
      ui.askQuestions(table);
    }

  runOptions.showChartOptions();

  Partition part(runOptions.chart_colour);
  /// Define what colours and values will be used to differentiate the nuclei.
  part.setDefaultColours();

  table.setIsotopeAttributes(part, runOptions);

  /// Write the chart
  switch (runOptions.filetype)
    {
      case FileType::EPS:
      default:
        {
          const EPSChart epsChart(runOptions);
          epsChart.write(table.theTable, part);
          break;
        }
      case FileType::SVG:
        {
          const SVGChart svgChart(runOptions);
          svgChart.write(table.theTable, part);
          break;
        }
      case FileType::TIKZ:
        {
          const TIKZChart tikzChart(runOptions);
          tikzChart.write(table.theTable, part);
          break;
        }
    }

  runOptions.writeOptionFile();

  fmt::print("Enjoy!\n");

  return 0;
}
