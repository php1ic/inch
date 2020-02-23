#include "inch/options.hpp"

#include "inch/converter.hpp"
#include "inch/nuclide.hpp"

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <fstream>
#include <iostream>
#include <regex>


void Options::constructAbsolutePaths() const
{
  // Where are the data files

  // Get the location of this source file
  // This will always be - /some/path/inch/src/IO.cpp
  // Only the directory separator/specifier will be different
  // We want to replace "src/IO.cpp" with "data_files/", using the appropriate separator
  std::regex re("(src)(.)(.*)");
  const auto absolute_path = std::regex_replace(__FILE__, re, "data_files$2");

  data_path = std::filesystem::path(absolute_path);

  fmt::print("\nSetting the path to the data files as:\n{}\n", data_path);

  FRDM = data_path / FRDM;

  neutron_drip = data_path / neutron_drip;

  two_neutron_drip = data_path / two_neutron_drip;

  proton_drip = data_path / proton_drip;

  two_proton_drip = data_path / two_proton_drip;

  r_proc_path = data_path / r_proc_path;
}


void Options::constructOutputFilename() const
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

  fmt::print("Using <{}> as the base of the file name\n", outfile);

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
}


void Options::setOutputFilename() const
{
  constructOutputFilename();

  if (!std::filesystem::exists(outfile) || outfile.stem() == "chart")
    {
      fmt::print("Will write chart to {}\n", outfile);
      return;
    }

  int f = 0;

  bool overwrite = false;
  char replace;
  char rereplace;

  fmt::print("\n**WARNING**: The file {} already exists\n"
             "Overwrite ",
             outfile);

  do
    {
      fmt::print("[y/n]: ");
      std::cin >> replace;

      if (replace == 'y')
        {
          fmt::print("\n{} will be overwritten\n", outfile);
        }
      else if (replace == 'n')
        {
          do
            {
              fmt::print("New filename (without extension): ");
              std::cin >> outfile;

              constructOutputFilename();

              if (std::filesystem::exists(outfile))
                {
                  fmt::print("This file also exists");

                  do
                    {
                      fmt::print("Overwrite this file [y/n]: ");
                      std::cin >> rereplace;

                      if (rereplace == 'y')
                        {
                          overwrite = true;
                          fmt::print("\nWill write chart to {}\n", outfile);
                        }
                      else if (rereplace != 'y' && rereplace != 'n')
                        {
                          fmt::print("That wasn't y or n. Try again");
                        }
                    }
                  while (rereplace != 'y' && rereplace != 'n' && !overwrite);
                }
              else
                {
                  fmt::print("\nWill write chart to {}\n", outfile);
                }
            }
          while (std::filesystem::exists(outfile) && !overwrite);
        }
      else
        {
          if (f > 1)
            {
              fmt::print("\nThere are 2 options, you've chosen neither on 3 occasions.\n\n"
                         "Perhaps this is running in a script.\nExiting...\n");
              exit(-1);
            }

          fmt::print("That wasn't y or n. Try again");
          ++f;
        }
    }
  while (replace != 'y' && replace != 'n');
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
  const Converter converter;

  fmt::print("===========================\n"
             "\nBetween Z = {}({}) and Z = {}({})",
             Zmin,
             converter.convertZToSymbol(Zmin),
             Zmax,
             converter.convertZToSymbol(Zmax));

  if (chart_selection == ChartSelection::FULL_CHART
      || (chart_selection == ChartSelection::SUB_CHART && all_neutrons == AllNeutrons::YES))
    {
      fmt::print(", with all relevant nuclei,\n");
    }
  else if (all_neutrons == AllNeutrons::NO)
    {
      fmt::print(", N = {} and N = {}\n", Nmin, Nmax);
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

  if (!opts)
    {
      fmt::print("\n***ERROR***: Couldn't open {} to write the options\n"
                 "               Not creating an option file.\n",
                 options);

      return;
    }

  opts << "section=" << chart_selection << "\n";

  if (chart_selection == ChartSelection::SUB_CHART)
    {
      opts << "Zmin=" << Zmin << "\n"
           << "Zmax=" << Zmax << "\n"
           << "required=" << all_neutrons << "\n";

      if (all_neutrons == AllNeutrons::NO)
        {
          opts << "Nmin=" << Nmin << "\n"
               << "Nmax=" << Nmax << "\n";
        }
    }

  opts << "type=" << chart_type << "\n"
       << "choice=" << chart_colour << std::endl;

  opts.close();

  fmt::print(" - done\n");
}


bool Options::checkInputFileOptions(const std::map<std::string, std::string>& values) const
{
  const int minLinesRequired = 3;
  int linesRead              = 0;

  for (const auto& it : values)
    {
      if (it.first == "section")
        {
          linesRead++;

          chart_selection = (it.second == "a") ? ChartSelection::FULL_CHART : ChartSelection::SUB_CHART;
        }
      else if (it.first == "type")
        {
          linesRead++;

          if (it.second == "a")
            {
              chart_type = ChartType::EXPERIMENTAL;
            }
          else if (it.second == "b")
            {
              chart_type = ChartType::THEORETICAL;
            }
          else if (it.second == "c")
            {
              chart_type = ChartType::ALL;
            }
          else
            {
              --linesRead;
              fmt::print(stderr, "***ERROR***: {} is not a valid choice for 'type'", it.second);
            }
        }
      else if (it.first == "choice")
        {
          linesRead++;

          if (it.second == "a")
            {
              chart_colour = ChartColour::MASSEXCESSERROR;
            }
          else if (it.second == "b")
            {
              chart_colour = ChartColour::REL_MASSEXCESSERROR;
            }
          else if (it.second == "c")
            {
              chart_colour = ChartColour::GS_DECAYMODE;
            }
          else if (it.second == "d")
            {
              chart_colour = ChartColour::GS_HALFLIFE;
            }
          else if (it.second == "e")
            {
              chart_colour = ChartColour::FIRST_ISOMERENERGY;
            }
          else
            {
              --linesRead;
              fmt::print(stderr, "***ERROR***: {} is not a valid choice for 'choice'", it.second);
            }
        }
      else if (it.first == "required")
        {
          all_neutrons = (it.second == "a") ? AllNeutrons::YES : AllNeutrons::NO;
        }
      else if (it.first == "Zmin")
        {
          bool valid = true;
          try
            {
              Zmin = stoi(it.second);
            }
          catch (const std::invalid_argument& ia)
            {
              valid = false;
            }

          if (!valid || Zmin < MIN_Z || Zmin > MAX_Z)
            {
              fmt::print(stderr, "\n***ERROR***: {} is not a valid choice for 'Zmin'", it.second);
              return false;
            }

          linesRead++;
        }
      else if (it.first == "Zmax")
        {
          bool valid = true;
          try
            {
              Zmax = stoi(it.second);
            }
          catch (const std::invalid_argument& ia)
            {
              valid = false;
            }

          if (!valid || Zmax < MIN_Z || Zmax > MAX_Z)
            {
              fmt::print(stderr, "\n***ERROR***: {} is not a valid choice for 'Zmax'", it.second);
              return false;
            }

          linesRead++;
        }
      else if (it.first == "Nmin")
        {
          bool valid = true;
          try
            {
              Nmin = stoi(it.second);
            }
          catch (const std::invalid_argument& ia)
            {
              valid = false;
            }

          if (!valid || Nmin < MIN_N || Nmin > MAX_N)
            {
              fmt::print(stderr, "***\nERROR***: {} is not a valid choice for 'Nmin'", it.second);
              return false;
            }

          linesRead++;
        }
      else if (it.first == "Nmax")
        {
          bool valid = true;
          try
            {
              Nmax = stoi(it.second);
            }
          catch (const std::invalid_argument& ia)
            {
              valid = false;
            }

          if (!valid || Nmax < MIN_N || Nmax > MAX_N)
            {
              fmt::print(stderr, "***ERROR***: {} is not a valid choice for 'Nmax'", it.second);
              return false;
            }

          linesRead++;
        }
      else
        {
          fmt::print("**WARNING**: {} is not a valid option. Ignoring.", it.first);
        }
    }

  if (linesRead < minLinesRequired)
    {
      fmt::print("Not enough inputs have been read from the file.");
      return false;
    }

  return true;
}


bool Options::validateInputFileOptions(const std::vector<Nuclide>& isotope_vector) const
{
  // Check that the options as a whole make sense.
  if (chart_selection == ChartSelection::FULL_CHART)
    {
      if (Zmin != MAX_Z && Zmax != MIN_Z)
        {
          fmt::print("**WARNING**\n"
                     "The option file contains a Z range but specifies that all nuclei should be drawn.\n"
                     "The input range will be ignored, set section=b to select a range in Z.\n"
                     "***********\n");
        }

      Zmin = MIN_Z;
      Zmax = MAX_Z;
      Nmin = MIN_N;
      Nmax = MAX_N;
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
                     all_neutrons);
          return false;
        }

      if (Zmin > Zmax)
        {
          fmt::print("***ERROR***: Zmax({}) cannot be less than Zmin({})\n"
                     "            Ignoring input file.\n",
                     Zmax,
                     Zmin);
          return false;
        }

      if (Nmin > Nmax)
        {
          fmt::print("***ERROR***: Nmax({}) cannot be less than Nmin({})\n"
                     "            Ignoring input file.\n",
                     Nmax,
                     Nmin);
          return false;
        }
    }
  else
    {
      fmt::print("***ERROR***: {} is not a valid option for the 'section' field.\n"
                 "            Ignoring input file.\n",
                 chart_selection);
      return false;
    }

  if (chart_type != ChartType::EXPERIMENTAL && chart_type != ChartType::THEORETICAL && chart_type != ChartType::ALL)
    {
      fmt::print("***ERROR***: {} is not a valid option for the 'type' field.\n"
                 "            Ignoring input file.\n",
                 chart_type);
      return false;
    }

  if (chart_colour != ChartColour::MASSEXCESSERROR && chart_colour != ChartColour::REL_MASSEXCESSERROR
      && chart_colour != ChartColour::GS_DECAYMODE && chart_colour != ChartColour::GS_HALFLIFE
      && chart_colour != ChartColour::FIRST_ISOMERENERGY)
    {
      fmt::print("***ERROR***: {} is not a valid option for the 'choice' field.\n"
                 "            Ignoring input file.\n",
                 chart_colour);
      return false;
    }

  fmt::print("Read values:\n"
             "section: {}\n",
             chart_selection);

  if (chart_selection == ChartSelection::SUB_CHART)
    {
      fmt::print("Zmin: {}\n"
                 "Zmax: {}\n"
                 "required: {}\n",
                 Zmin,
                 Zmax,
                 all_neutrons);

      if (all_neutrons == AllNeutrons::NO)
        {
          fmt::print("Nmin: {}\n"
                     "Nmax: {}\n",
                     Nmin,
                     Nmax);
        }
    }

  fmt::print("type: {}\n"
             "choice: {}",
             chart_type,
             chart_colour);

  return true;
}


void Options::setNeutronLimits(const std::vector<Nuclide>& table) const
{
  Nmin = Limits::MAX_N;
  Nmax = Limits::MIN_N;

  for (const auto& it : table)
    {
      if (it.Z >= Zmin && it.Z <= Zmax && it.rich % np_rich == 0)
        {
          if (it.N < Nmin)
            {
              Nmin = it.N;
            }

          if (it.N > Nmax)
            {
              Nmax = it.N;
            }
        }
    }
}
