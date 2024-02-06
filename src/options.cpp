#include "inch/options.hpp"

#include "inch/converter.hpp"
#include "inch/nuclide.hpp"
#include "inch/ui.hpp"

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <fmt/std.h>

#include <fstream>
#include <iostream>


bool Options::validateOutputFilename() const
{
  // Remove the extension if given
  if (outfile.extension() != "")
    {
      fmt::print("\nThe extension is added depending on the chosen file type\n");
      outfile.replace_extension("");
    }

  // Check output file is not a directory.
  if (std::filesystem::is_directory(outfile))
    {
      fmt::print("\n***ERROR***: {} is a directory, can't use that as a file name\n\n", outfile);
      outfile = "chart";
    }

  fmt::print("Using {} as the base of the file name\n", outfile);

  // Add the necessary extension
  switch (filetype)
    {
      case FileType::EPS:
        outfile.replace_extension(".eps");
        break;
      case FileType::SVG:
        outfile.replace_extension(".svg");
        break;
      case FileType::TIKZ:
        outfile.replace_extension(".tex");
        break;
    }

  // Assume an exsiting file cannot be written to
  return !std::filesystem::exists(outfile);
}


void Options::setOutputFilename() const
{
  if (validateOutputFilename() || outfile.stem() == "chart")
    {
      fmt::print("Will write chart to {}\n", outfile);
      return;
    }

  fmt::print("\n**WARNING**: The file {} already exists\n", outfile);

  if (UI::yesNoQuestion("Overwrite", "y"))
    {
      fmt::print("\n{} will be overwritten\n", outfile);
      return;
    }

  int question_counter{ 0 };
  constexpr int question_limit{ 50 };

  while (true)
    {
      fmt::print("New filename (without extension): ");
      std::cin >> outfile;

      if (validateOutputFilename())
        {
          fmt::print("\nWill write chart to {}\n", outfile);
          return;
        }

      fmt::print("{} also exists\n", outfile);
      if (UI::yesNoQuestion("Overwrite this file", "y"))
        {
          fmt::print("\n{} will be overwritten\n", outfile);
          return;
        }

      ++question_counter;

      if (question_counter > question_limit)
        {
          fmt::print(stderr,
                     "\nSomething seems off, this question has now been asked {} times.\n\n"
                     "Perhaps this is running in a script and we are in an infinite loop.\n"
                     "Exiting...\n\n",
                     question_limit);
          exit(-1);
        }
    }
}


void Options::setFileType(std::string_view type) const
{
  if (type == "svg")
    {
      filetype = FileType::SVG;
    }
  else if (type == "tikz")
    {
      filetype = FileType::TIKZ;
    }
  else
    {
      filetype = FileType::EPS;
    }
}


void Options::showChartOptions() const
{
  fmt::print("===========================\n"
             "\nBetween Z = {}({}) and Z = {}({})",
             limits.Zmin,
             Converter::ZToSymbol(limits.Zmin),
             limits.Zmax,
             Converter::ZToSymbol(limits.Zmax));

  if (chart_selection == ChartSelection::FULL_CHART
      || (chart_selection == ChartSelection::SUB_CHART && all_neutrons == AllNeutrons::YES))
    {
      fmt::print(", with all relevant nuclei,\n");
    }
  else if (all_neutrons == AllNeutrons::NO)
    {
      fmt::print(", N = {} and N = {}\n", limits.Nmin, limits.Nmax);
    }

  if (chart_type == ChartType::EXPERIMENTAL)
    {
      fmt::print("experimentally measured");
    }
  else if (chart_type == ChartType::THEORETICAL)
    {
      fmt::print("theoretical/extrapolated");
    }
  else
    {
      fmt::print("both experimental and theoretical");
    }

  fmt::print(" values will be drawn and\nthe chart coloured by ");

  if (chart_colour == ChartColour::MASSEXCESSERROR)
    {
      fmt::print("error on mass-excess\n");
    }
  else if (chart_colour == ChartColour::REL_MASSEXCESSERROR)
    {
      fmt::print("relative error on mass-excess\n");
    }
  else if (chart_colour == ChartColour::GS_DECAYMODE)
    {
      fmt::print("major ground-state decay mode\n");
    }
  else if (chart_colour == ChartColour::GS_HALFLIFE)
    {
      fmt::print("ground-state half-life\n");
    }
  else
    {
      fmt::print("first isomer energy\n");
    }
}


void Options::writeOptionFile() const
{
  // Match the options filename to that of the output chart
  if (outfile.stem() != "chart")
    {
      options = outfile;
      options.replace_extension(".in");
    }

  std::ofstream opts(options, std::ios::binary);

  fmt::print("Writing user choices to {}", options);

  if (!opts.is_open())
    {
      fmt::print("\n***ERROR***: Couldn't open {} to write the options\n"
                 "               Not creating an option file.\n",
                 options);

      return;
    }

  fmt::print(opts, "section={}\n", static_cast<int>(chart_selection));

  if (chart_selection == ChartSelection::SUB_CHART)
    {
      fmt::print(opts,
                 "Zmin={}\n"
                 "Zmax={}\n"
                 "required={}\n",
                 limits.Zmin,
                 limits.Zmax,
                 static_cast<int>(all_neutrons));

      if (all_neutrons == AllNeutrons::NO)
        {
          fmt::print(opts,
                     "Nmin={}\n"
                     "Nmax={}\n",
                     limits.Nmin,
                     limits.Nmax);
        }
    }

  fmt::print(opts,
             "type={}\n"
             "choice={}\n",
             static_cast<int>(chart_type),
             static_cast<int>(chart_colour));

  opts.close();

  fmt::print(" - done\n");
}


bool Options::checkInputFileOptions(const std::map<std::string, std::string>& values) const
{
  constexpr int minLinesRequired{ 3 };
  int linesRead{ 0 };

  for (const auto& it : values)
    {
      if (it.first == "section")
        {
          linesRead++;

          chart_selection = (it.second == "0") ? ChartSelection::FULL_CHART : ChartSelection::SUB_CHART;
        }
      else if (it.first == "type")
        {
          linesRead++;

          if (it.second == "0")
            {
              chart_type = ChartType::EXPERIMENTAL;
            }
          else if (it.second == "1")
            {
              chart_type = ChartType::THEORETICAL;
            }
          else if (it.second == "2")
            {
              chart_type = ChartType::ALL;
            }
          else
            {
              --linesRead;
              fmt::print(stderr, "***ERROR***: {} is not a valid choice for 'type'\n", it.second);
            }
        }
      else if (it.first == "choice")
        {
          linesRead++;

          if (it.second == "0")
            {
              chart_colour = ChartColour::MASSEXCESSERROR;
            }
          else if (it.second == "1")
            {
              chart_colour = ChartColour::REL_MASSEXCESSERROR;
            }
          else if (it.second == "2")
            {
              chart_colour = ChartColour::GS_DECAYMODE;
            }
          else if (it.second == "3")
            {
              chart_colour = ChartColour::GS_HALFLIFE;
            }
          else if (it.second == "4")
            {
              chart_colour = ChartColour::FIRST_ISOMERENERGY;
            }
          else
            {
              --linesRead;
              fmt::print(stderr, "***ERROR***: {} is not a valid choice for 'choice'\n", it.second);
            }
        }
      else if (it.first == "required")
        {
          all_neutrons = (it.second == "0") ? AllNeutrons::YES : AllNeutrons::NO;
        }
      else if (it.first == "Zmin")
        {
          limits.Zmin = Converter::StringToInt(it.second);

          if (limits.Zmin < Limits::MIN_Z || limits.Zmin > Limits::MAX_Z)
            {
              fmt::print(stderr, "***ERROR***: {} is not a valid choice for 'Zmin'\n", it.second);
              return false;
            }

          linesRead++;
        }
      else if (it.first == "Zmax")
        {
          limits.Zmax = Converter::StringToInt(it.second);

          if (limits.Zmax < Limits::MIN_Z || limits.Zmax > Limits::MAX_Z)
            {
              fmt::print(stderr, "***ERROR***: {} is not a valid choice for 'Zmax'\n", it.second);
              return false;
            }

          linesRead++;
        }
      else if (it.first == "Nmin")
        {
          limits.Nmin = Converter::StringToInt(it.second);

          if (limits.Nmin < Limits::MIN_N || limits.Nmin > Limits::MAX_N)
            {
              fmt::print(stderr, "***ERROR***: {} is not a valid choice for 'Nmin'\n", it.second);
              return false;
            }

          linesRead++;
        }
      else if (it.first == "Nmax")
        {
          limits.Nmax = Converter::StringToInt(it.second);

          if (limits.Nmax < Limits::MIN_N || limits.Nmax > Limits::MAX_N)
            {
              fmt::print(stderr, "***ERROR***: {} is not a valid choice for 'Nmax'\n", it.second);
              return false;
            }

          linesRead++;
        }
      else
        {
          fmt::print("**WARNING**: {} is not a valid option. Ignoring.\n", it.first);
        }
    }

  if (linesRead < minLinesRequired)
    {
      fmt::print("Not enough inputs have been read from the file.\n");
      return false;
    }

  return true;
}


bool Options::validateInputFileOptions(const std::vector<Nuclide>& isotope_vector) const
{
  // Check that the options as a whole make sense.
  if (!validateSelection(isotope_vector))
    {
      return false;
    }

  printInputFileOptions();

  return true;
}


bool Options::validateSelection(const std::vector<Nuclide>& isotope_vector) const
{
  if (chart_selection == ChartSelection::FULL_CHART)
    {
      if (limits.Zmin != Limits::MIN_Z && limits.Zmax != Limits::MAX_Z)
        {
          fmt::print("**WARNING**\n"
                     "The option file contains a Z range but specifies that all nuclei should be drawn.\n"
                     "The input range will be ignored, set section=1 to select a range in Z.\n"
                     "***********\n");
        }

      limits.ResetAllLimits();
    }
  else if (chart_selection == ChartSelection::SUB_CHART)
    {
      if (all_neutrons == AllNeutrons::YES)
        {
          setNeutronLimits(isotope_vector);
        }
      else if (all_neutrons != AllNeutrons::NO)
        {
          fmt::print("***ERROR***: {} is not a valid option for the 'required' field.\n"
                     "            Ignoring input file.\n",
                     fmt::underlying(all_neutrons));
          return false;
        }

      if (limits.Zmin > limits.Zmax)
        {
          fmt::print("***ERROR***: Zmax({}) cannot be less than Zmin({})\n"
                     "            Ignoring input file.\n",
                     limits.Zmax,
                     limits.Zmin);
          return false;
        }

      if (limits.Nmin > limits.Nmax)
        {
          fmt::print("***ERROR***: Nmax({}) cannot be less than Nmin({})\n"
                     "            Ignoring input file.\n",
                     limits.Nmax,
                     limits.Nmin);
          return false;
        }
    }
  else
    {
      fmt::print("***ERROR***: {} is not a valid option for the 'section' field.\n"
                 "            Ignoring input file.\n",
                 static_cast<int>(chart_selection));
      return false;
    }

  return true;
}


void Options::printInputFileOptions() const
{
  fmt::print("Read values:\n"
             "section: {}\n",
             static_cast<int>(chart_selection));

  if (chart_selection == ChartSelection::SUB_CHART)
    {
      fmt::print("Zmin: {}\n"
                 "Zmax: {}\n"
                 "required: {}\n",
                 limits.Zmin,
                 limits.Zmax,
                 static_cast<int>(all_neutrons));

      if (all_neutrons == AllNeutrons::NO)
        {
          fmt::print("Nmin: {}\n"
                     "Nmax: {}\n",
                     limits.Nmin,
                     limits.Nmax);
        }
    }

  fmt::print("type: {}\n"
             "choice: {}\n",
             static_cast<int>(chart_type),
             static_cast<int>(chart_colour));
}


void Options::setNeutronLimits(const std::vector<Nuclide>& table) const
{
  limits.Nmin = Limits::MAX_N;
  limits.Nmax = Limits::MIN_N;

  for (const auto& it : table)
    {
      if (limits.inZRange(it.Z) && it.rich % np_rich == 0)
        {
          if (it.N < limits.Nmin)
            {
              limits.Nmin = it.N;
            }

          if (it.N > limits.Nmax)
            {
              limits.Nmax = it.N;
            }
        }
    }
}
