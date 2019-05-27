#include "options.hpp"

#include "converter.hpp"
#include "nuclide.hpp"

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
  path = std::regex_replace(__FILE__, re, "data_files$2");

  std::cout << "\nSetting the path to the data files as:\n" << path << "\n";

  FRDM.insert(0, path);

  neutron_drip.insert(0, path);

  two_neutron_drip.insert(0, path);

  proton_drip.insert(0, path);

  two_proton_drip.insert(0, path);

  r_proc_path.insert(0, path);
}


void Options::constructOutputFilename() const
{
  // Remove the extension if given
  if (outfile.find('.') == outfile.length() - 4 && outfile.find('.') != std::string::npos)
    {
      std::cout << "\nThe extension is added depending on the chosen file type\n";

      outfile.erase(outfile.rfind('.'), 4);
    }

  // Check output file is not a directory.
  if (outfile.empty() || outfile.at(outfile.length() - 1) == '/')
    {
      std::cout << "\n"
                << "***ERROR***: " << outfile << " is a directory, can't use that as a file name\n"
                << std::endl;

      outfile = "chart";
    }

  std::cout << "Using <" << outfile << "> as the base of the file name." << std::endl;

  // Add the necessary extension
  switch (filetype)
    {
      case FileType::EPS:
        outfile.append(".eps");
        break;
      case FileType::SVG:
        outfile.append(".svg");
        break;
      case FileType::TIKZ:
        outfile.append(".tex");
        break;
    }
}


void Options::setOutputFilename() const
{
  constructOutputFilename();

  if (!checkFileExists(outfile) || outfile.compare(0, 6, "chart.") == 0)
    {
      std::cout << "Will write chart to " << outfile << std::endl;
      return;
    }

  int f = 0;

  bool overwrite = false;
  char replace;
  char rereplace;
  std::cout << "\n**WARNING**: The file " << outfile << " already exists.\n"
            << "Overwrite ";
  do
    {
      std::cout << "[y/n]: ";
      std::cin >> replace;

      if (replace == 'y')
        {
          std::cout << "\n" << outfile << " will be overwritten\n" << std::endl;
        }
      else if (replace == 'n')
        {
          do
            {
              std::cout << "New filename (without extension): ";
              std::cin >> outfile;

              constructOutputFilename();

              if (checkFileExists(outfile))
                {
                  std::cout << "This file also exists" << std::endl;

                  do
                    {
                      std::cout << "Overwrite this file [y/n]: ";
                      std::cin >> rereplace;

                      if (rereplace == 'y')
                        {
                          overwrite = true;
                          std::cout << "\nWill write chart to " << outfile << "\n" << std::endl;
                        }
                      else if (rereplace != 'y' && rereplace != 'n')
                        {
                          std::cout << "That wasn't y or n. Try again" << std::endl;
                        }
                    }
                  while (rereplace != 'y' && rereplace != 'n' && !overwrite);
                }
              else
                {
                  std::cout << "\nWill write chart to " << outfile << "\n" << std::endl;
                }
            }
          while (checkFileExists(outfile) && !overwrite);
        }
      else
        {
          if (f > 1)
            {
              std::cout << "\nThere are 2 options, you've chosen neither on 3 occasions.\n\n"
                        << "Perhaps this is running in a script.\nExiting..." << std::endl;
              exit(-1);
            }

          std::cout << "That wasn't y or n. Try again" << std::endl;
          ++f;
        }
    }
  while (replace != 'y' && replace != 'n');
}


void Options::setFileType(const std::string& type) const
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
  std::cout << "===========================\n"
            << "\nBetween Z = " << Zmin << "(" << converter.convertZToSymbol(Zmin) << ") and Z = " << Zmax << "("
            << converter.convertZToSymbol(Zmax) << ")";

  if (chart_selection == ChartSelection::FULL_CHART
      || (chart_selection == ChartSelection::SUB_CHART && all_neutrons == AllNeutrons::YES))
    {
      std::cout << ", with all relevant nuclei,\n";
    }
  else if (all_neutrons == AllNeutrons::NO)
    {
      std::cout << ", N = " << Nmin << " and N = " << Nmax << "\n";
    }

  if (chart_type == ChartType::EXPERIMENTAL)
    {
      std::cout << "experimentally measured";
    }
  else if (chart_type == ChartType::THEORETICAL)
    {
      std::cout << "theoretical/extrapolated";
    }
  else
    {
      std::cout << "both experimental and theoretical";
    }

  std::cout << " values will be drawn and\nthe chart coloured by ";

  if (chart_colour == ChartColour::MASSEXCESSERROR)
    {
      std::cout << "error on mass-excess\n";
    }
  else if (chart_colour == ChartColour::REL_MASSEXCESSERROR)
    {
      std::cout << "relative error on mass-excess\n";
    }
  else if (chart_colour == ChartColour::GS_DECAYMODE)
    {
      std::cout << "major ground-state decay mode\n";
    }
  else if (chart_colour == ChartColour::GS_HALFLIFE)
    {
      std::cout << "ground-state half-life\n";
    }
  else
    {
      std::cout << "first isomer energy\n";
    }
}


void Options::writeOptionFile() const
{
  // Match the options filename to that of the output chart
  if (outfile.find("chart") == std::string::npos)
    {
      options = outfile.substr(0, outfile.rfind('.')) + ".in";
    }

  std::ofstream opts(options, std::ios::binary);

  std::cout << "Writing user choices to " << options;

  if (!opts)
    {
      std::cout << "\n"
                << "***ERROR***: Couldn't open " << options << " to write the options.\n"
                << "             Not creating an option file." << std::endl;

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

  std::cout << " - done\n" << std::endl;
}


bool Options::checkInputFileOptions(const std::map<std::string, std::string>& values) const
{
  int linesRead = 0;

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
              std::cerr << "***ERROR***: " << it.second << " is not a valid choice for 'type'" << std::endl;
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
              std::cerr << "***ERROR***: " << it.second << " is not a valid choice for 'choice'" << std::endl;
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
              std::cerr << "\n***ERROR***: " << it.second << " is not a valid choice for 'Zmin'" << std::endl;
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
              std::cerr << "\n***ERROR***: " << it.second << " is not a valid choice for 'Zmax'" << std::endl;
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
              std::cerr << "***\nERROR***: " << it.second << " is not a valid choice for 'Nmin'" << std::endl;
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
              std::cerr << "***ERROR***: " << it.second << " is not a valid choice for 'Nmax'" << std::endl;
              return false;
            }

          linesRead++;
        }
      else
        {
          std::cout << "**WARNING**: " << it.first << " is not a valid option. Ignoring." << std::endl;
        }
    }

  if (linesRead < 3)
    {
      std::cout << "Not enough inputs have been read from the file." << std::endl;
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
          std::cout << "**WARNING**\n"
                    << "The option file contains a Z range but specifies that all nuclei should be drawn.\n"
                    << "The input range will be ignored, set section=b to select a range in Z.\n"
                    << "***********\n"
                    << std::endl;
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
          std::cout << "***ERROR***: " << all_neutrons << " is not a valid option for the 'required' field.\n"
                    << "            Ignoring input file.\n"
                    << std::endl;
          return false;
        }

      if (Zmin > Zmax)
        {
          std::cout << "***ERROR***: Zmax(" << Zmax << ") cannot be less than Zmin(" << Zmin << ")\n"
                    << "            Ignoring input file.\n"
                    << std::endl;
          return false;
        }

      if (Nmin > Nmax)
        {
          std::cout << "***ERROR***: Nmax(" << Nmax << ") cannot be less than Nmin(" << Nmin << ")\n"
                    << "            Ignoring input file.\n"
                    << std::endl;
          return false;
        }
    }
  else
    {
      std::cout << "***ERROR***: " << chart_selection << " is not a valid option for the 'section' field.\n"
                << "            Ignoring input file.\n"
                << std::endl;
      return false;
    }

  if (chart_type != ChartType::EXPERIMENTAL && chart_type != ChartType::THEORETICAL && chart_type != ChartType::ALL)
    {
      std::cout << "***ERROR***: " << chart_type << " is not a valid option for the 'type' field.\n"
                << "            Ignoring input file.\n"
                << std::endl;
      return false;
    }

  // clang-format off
  if (   chart_colour != ChartColour::MASSEXCESSERROR
      && chart_colour != ChartColour::REL_MASSEXCESSERROR
      && chart_colour != ChartColour::GS_DECAYMODE
      && chart_colour != ChartColour::GS_HALFLIFE
      && chart_colour != ChartColour::FIRST_ISOMERENERGY
      )
    //clang-format on
    {
      std::cout << "***ERROR***: " << chart_colour
                << " is not a valid option for the 'choice' field.\n"
                << "            Ignoring input file.\n" << std::endl;
      return false;
    }

  std::cout << "Read values:\n"
            << "section: " << chart_selection  << "\n";

  if ( chart_selection == ChartSelection::SUB_CHART )
    {
      std::cout << "Zmin: "     << Zmin << "\n"
                << "Zmax: "     << Zmax << "\n"
                << "required: " << all_neutrons << "\n";

      if ( all_neutrons == AllNeutrons::NO )
        {
          std::cout << "Nmin: " << Nmin << "\n"
                    << "Nmax: " << Nmax << "\n";
        }
    }

  std::cout << "type: " << chart_type << "\n"
            << "choice: " << chart_colour << std::endl;

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
