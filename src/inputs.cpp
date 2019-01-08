#include "inputs.hpp"

#include "converter.hpp"
#include "nuclide.hpp"
#include "version.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>


// Read and store the console arguments, process if --help or --version, return if
// a 'bad' number of arguments are given.
int inputs::readConsoleArguments(const std::vector<std::string>& console_options) const
{
  // Check for help or version option, print as requrired and exit.
  if (console_options.size() == 2)
    {
      if (console_options.back() == std::string("--version") || console_options.back() == std::string("-v"))
        {
          showVersion();
        }
      else if (console_options.back() == std::string("--help") || console_options.back() == std::string("-h"))
        {
          showUsage(console_options.front());
        }
      else
        {
          std::cerr << "\n***ERROR***: Unkown single option " << console_options.back() << " exiting...\n";
          showUsage(console_options.front());
        }

      return 2;
    }

  // Read options via << -flag value >> so, including the executable, we need
  // an odd number of arguments. Don't process any in the even case, as we can't know
  // what the user was trying to do.
  if (console_options.size() % 2 == 0)
    {
      std::cerr << "\n"
                << "***ERROR***: An odd number of arguments is not allowed\n"
                << std::endl;

      return 1;
    }

  // Don't want initial element as it's the executable, <<-flag option>> format means we
  // want every other element as the key of our map, therfore don't read the final element as
  // even if it was a key, there would be no value with it.
  for (auto it = std::next(std::cbegin(console_options)); it != std::cend(console_options); std::advance(it, 2))
    {
      arguments.emplace(*it, *std::next(it));
    }

  return saveConsoleArguments();
}


// Assign console arguments to the necessary members
int inputs::saveConsoleArguments() const
{
  for (const auto& flag : arguments)
    {
      /// Keep list alphabetical to avoid duplication
      if (flag.first == "-a" || flag.first == "--ame")
        {
          AME = flag.second == "ON";
        }
      else if (flag.first == "-b" || flag.first == "--betarich")
        {
          np_rich = stoi(flag.second);
        }
      else if (flag.first == "-f" || flag.first == "--filetype")
        {
          stringfile_type = flag.second;
        }
      else if (flag.first == "-g" || flag.first == "--grid")
        {
          grid = flag.second == "ON";
        }
      else if (flag.first == "-i" || flag.first == "--infile")
        {
          inputfile = flag.second;
        }
      else if (flag.first == "-k" || flag.first == "--key")
        {
          key = flag.second == "ON";
        }
      else if (flag.first == "-m" || flag.first == "--magicnumbers")
        {
          magic_numbers = flag.second == "ON";
        }
      else if (flag.first == "-o" || flag.first == "--outfile")
        {
          outfile = flag.second;
        }
      else if (flag.first == "-p" || flag.first == "--personal")
        {
          personal_isotopes = flag.second;
        }
      else if (flag.first == "-r" || flag.first == "--rprocess")
        {
          r_process = flag.second == "ON";
        }
      else if (flag.first == "-sdl" || flag.first == "--singleDripLines")
        {
          single_drip_lines = stoi(flag.second);
        }
      else if (flag.first == "-ddl" || flag.first == "--doubleDripLines")
        {
          double_drip_lines = stoi(flag.second);
        }
      else if (flag.first == "-w" || flag.first == "--writesotopes")
        {
          write_isotope = flag.second == "ON";
        }
      else if (flag.first == "-y" || flag.first == "--year")
        {
          year = stoi(flag.second);
        }
      else
        {
          std::cerr << "\n**WARNING**: The flag " << flag.first << " is not currently supported. "
                    << "Ignoring this flag and it's associated option " << flag.second << std::endl;
        }
    }

  return processConsoleArguments();
}


// Validate the console arguments
int inputs::processConsoleArguments() const
{
  setFileType(stringfile_type);

  setOutputFilename(outfile);

  if (!inputfile.empty())
    {
      setInputOptions(inputfile);
    }

  return 0;
}


void inputs::setInputOptions(const std::string& filename) const
{
  readOptionFile(filename);

  valid_inputfile = checkInputOptions(inputfile_options);
}


void inputs::setOutputFilename(const std::string& filename) const
{
  outfile = filename;

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


void inputs::setFileType(const std::string& file_type) const
{
  if (file_type == "svg")
    {
      filetype = FileType::SVG;
    }
  else if (file_type == "tikz")
    {
      filetype = FileType::TIKZ;
    }
  else
    {
      filetype = FileType::EPS;
    }
}


void inputs::showVersion() const
{
  const std::string date = __DATE__;

  std::cout << "Interactive Nuclear CHart (INCH) version " << INCH_VERSION << "\n"
            << "Copyright (C) " << date.substr(date.rfind(' ')) << " Me.\n"
            << "INCH comes with ABSOLUTELY NO WARRANTY.\n"
            << "You may redistribute copies of INCH\n"
            << "under the terms of the GNU General Public License\n"
            << "For more information about these matters, see the file named COPYING." << std::endl;
}


void inputs::showUsage(const std::string& exe) const
{
  std::cout << "\n"
            << "Usage: " << exe.substr(exe.find_last_of('/') + 1) << " [with any or non of the options below]\n"
            << "\n"
            << " -h | --help\n"
            << "         Print this help and exit\n"
            << "\n"
            << " -v | --version\n"
            << "         Print version information and exit\n"
            << "\n"
            << "\n"
            << " -a | --ame <ON/OFF>\n"
            << "         Colour chart with data from AME rather an NUBASE if it exists [default: OFF]\n"
            << "\n"
            << " -b | --betarich <1/2/3/6>\n"
            << "         Set what part of the chart is drawn:\n"
            << "            1 = All [default]\n"
            << "            2 = Proton rich and stable isotopes\n"
            << "            3 = Neutron rich and stable isotoeps\n"
            << "            6 = Stable isotopes only\n"
            << "\n"
            << " -f | --filetype <filetype>\n"
            << "         Set the output file type [default:eps, svg, tikz]\n"
            << "\n"
            << " -g | --grid <ON/OFF>\n"
            << "         Draw a grid on the chart [default: OFF]\n"
            << "\n"
            << " -i | --infile <inputFilename>\n"
            << "         Use the provided file as options to create the chart\n"
            << "\n"
            << " -k | --key <ON/OFF>\n"
            << "         Draw the key [default: ON]\n"
            << "\n"
            << " -m | --magicnumbers <ON/OFF>\n"
            << "         Show  the magic numbers [default: ON]\n"
            << "\n"
            << " -o | --outfile <outputName>\n"
            << "         Set the root of the output filename i.e. without extension [default: chart]\n"
            << "\n"
            << " -p | --personal <userIsotopes>\n"
            << "         Set the file to be read to mark user defined isotopes\n"
            << "\n"
            << " -r | --rprocess <ON/OFF>\n"
            << "         Show the R-process path [default: ON]\n"
            << "\n"
            << " -sdl | --singleDripLine <0/1/2/3>\n"
            << "           Set which single particle drip lines are drawn:\n"
            << "              0 = Neither\n"
            << "              1 = Both [default]\n"
            << "              2 = Proton only\n"
            << "              3 = Neutron only\n"
            << "\n"
            << " -ddl | --doubleDripLine <0/1/2/3>\n"
            << "           Set which double particle drip lines are drawn:\n"
            << "              0 = Neither\n"
            << "              1 = Both [default]\n"
            << "              2 = Proton only\n"
            << "              3 = Neutron only\n"
            << "\n"
            << " -w | --writesotopes <ON/OFF>\n"
            << "         Write the isotope number and symbol [default: ON]\n"
            << "\n"
            << " -y | --year <3/12/16>\n"
            << "         Set the year from which data is taken [default: 3]\n"
            << std::endl;
}


void inputs::showBanner() const
{
  std::cout << "\n"
            << "         +---+---+---+---+---+---+---+\n"
            << "         |In |Te |Ra | C |Ti | V | E |\n"
            << "         +---+---+---+---+---+---+---+\n"
            << "             | N | U |Cl | E |Ar |\n"
            << "         +---+---+---+---+---+---+\n"
            << "         | C | H |Ar | T |\n"
            << "         +---+---+---+---v" << INCH_VERSION << "\n"
            << std::endl;
}


void inputs::constructFullyQualifiedPaths() const
{
  /// Where are the data files
  /// Get the location of this source file
  path = __FILE__;
  /// This will always be - /some/path/inch/src/inputs.cpp
  /// Remove from /src/ to the end
  path.erase(path.rfind("/src/"));
  /// Add on the folder within the project containing the files
  path.append("/data_files/");

  std::cout << "\nSetting the path to the data files as:\n" << path << "\n";

  FRDM.insert(0, path);

  neutron_drip.insert(0, path);

  two_neutron_drip.insert(0, path);

  proton_drip.insert(0, path);

  two_proton_drip.insert(0, path);

  r_proc_path.insert(0, path);
}


void inputs::readOptionFile(const std::string& inputFilename) const
{
  std::ifstream infile(inputFilename, std::ios::binary);

  std::cout << "Reading " << inputFilename << " for input values {--";

  if (!infile)
    {
      std::cout << "\n***ERROR***: " << inputFilename << " couldn't be opened, does it exist?\n" << std::endl;

      return;
    }

  std::string line;

  while (std::getline(infile, line))
    {
      // Skip empty lines
      // Let lines starting with '#' be comments
      if (line.empty() || line.at(0) == '#')
        {
          continue;
        }

      // Clear any part of the string after and including a '#'.
      // We can't get here if the string starts with '#' so no issue
      // of creating an empty string at this point.
      if (line.find('#') != std::string::npos)
        {
          line.erase(line.find('#'));
        }

      int i = 0;
      std::string part;
      std::vector<std::string> theLine{ "", "" };
      std::istringstream stream(line);

      while (std::getline(stream, part, '='))
        {
          /// Remove any and all 'white-space' characters
          part.erase(std::remove_if(
                         part.begin(), part.end(), [](char x) { return std::isspace(static_cast<unsigned char>(x)); }),
                     part.end());

          theLine.at(i) = part;
          ++i;
        }

      if (inputfile_options.count(theLine.front()) > 0)
        {
          std::cout << "\n**WARNING**: Already have a value for " << theLine.front() << " ("
                    << inputfile_options.at(theLine.front()) << ")"
                    << ", will use new value (" << theLine.back() << ").\n";
        }

      inputfile_options.emplace(theLine.front(), theLine.back());
    }

  infile.close();

  std::cout << "--} done" << std::endl;
}


bool inputs::validateInputFile(const std::vector<Nuclide>& isotope_vector) const
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


bool inputs::checkInputOptions(const std::map<std::string, std::string>& values) const
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


void inputs::setNeutronLimits(const std::vector<Nuclide>& isotope_vector) const
{
  Nmin = MAX_N;
  Nmax = MIN_N;

  for (const auto& it : isotope_vector)
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


void inputs::setExtreme(const std::string& limit) const
{
  if (limit != "Zmin" && limit != "Zmax" && limit != "Nmin" && limit != "Nmax")
    {
      std::cout << "**WARNING** - " << limit << " is not a valid input\n"
                << "              choose either Zmin, Zmax, Nmin or Nmax\n"
                << "Setting limits to maxima values." << std::endl;

      Zmin = MIN_Z;
      Zmax = MAX_Z;
      Nmin = MIN_N;
      Nmax = MAX_N;

      return;
    }

  bool valid = false;

  do
    {
      int number = 0;
      std::string in;

      std::cout << limit << ": ";
      std::cin >> in;

      // Read the entered value and convert symbol->Z if necessary
      try
        {
          number = stoi(in);
        }
      catch (const std::invalid_argument& ia)
        {
          const Converter converter;
          number = (limit.at(0) == 'Z') ? converter.convertSymbolToZ(in) : -1;
        }

      // Validate the number
      if (limit == "Zmin")
        {
          Zmin  = number;
          valid = true;
          if (Zmin < MIN_Z || Zmin > MAX_Z)
            {
              std::cout << "\n"
                        << "Zmin must be in the range " << MIN_Z << "<Z<" << MAX_Z << "\n"
                        << std::endl;
              valid = false;
            }
        }
      else if (limit == "Zmax")
        {
          Zmax  = number;
          valid = true;
          if (Zmax < Zmin || Zmax > MAX_Z)
            {
              std::cout << "\n"
                        << "Zmax must be in the range " << Zmin << "<Z<" << MAX_Z << "\n"
                        << std::endl;
              valid = false;
            }
        }
      else if (limit == "Nmin")
        {
          Nmin  = number;
          valid = true;
          if (Nmin < MIN_N || Nmin > MAX_N)
            {
              std::cout << "\n"
                        << "Nmin must be in the range " << MIN_N << "<N<" << MAX_N << "\n"
                        << std::endl;
              valid = false;
            }
        }
      else if (limit == "Nmax")
        {
          Nmax  = number;
          valid = true;
          if (Nmax < Nmin || Nmax > MAX_N)
            {
              std::cout << "\n"
                        << "Nmax must be in the range " << Nmin << "<N<" << MAX_N << "\n"
                        << std::endl;
              valid = false;
            }
        }
    }
  while (!valid);
}


void inputs::showChartOptions() const
{
  const Converter converter;
  std::cout << "===========================\n"
            << "\nBetween Z = " << Zmin << "(" << converter.convertZToSymbol(Zmin) << ") and Z = " << Zmax << "("
            << converter.convertZToSymbol(Zmax) << ")";

  if (chart_selection == ChartSelection::FULL_CHART ||
      (chart_selection == ChartSelection::SUB_CHART && all_neutrons == AllNeutrons::YES))
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


void inputs::constructOutputFilename() const
{
  //-Remove the extension if given
  if (outfile.find('.') == outfile.length() - 4 && outfile.find('.') != std::string::npos)
    {
      std::cout << "\nThe extension is added depending on the chosen file type\n";

      outfile.erase(outfile.rfind('.'), 4);
    }

  //-Check output file is not a directory.
  if (outfile.empty() || outfile.at(outfile.length() - 1) == '/')
    {
      std::cout << "\n"
                << "***ERROR***: " << outfile << " is a directory, can't use that as a file name\n"
                << std::endl;

      outfile = "chart";
    }

  std::cout << R"(Using ")" << outfile << R"(" as the base of the file name.)" << std::endl;

  //-Add the necessary extension
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


void inputs::displaySection(const std::vector<Nuclide>& isotope_vector) const
{
  int stblZmin = MAX_N;
  int stblZmax = MIN_N;
  int NminZmin = MAX_N;
  int NminZmax = MAX_N;
  int NmaxZmin = MIN_N;
  int NmaxZmax = MIN_N;

  bool validChoice = false;

  std::cout << "\n---------------------------\n"
            << "Draw a) The entire chart\n"
            << "     b) A section\n";

  do
    {
      std::string section;

      do
        {
          validChoice = true;
          std::cout << "[a,b]: ";
          std::cin >> section;

          if (section != "a" && section != "b")
            {
              validChoice = false;
              std::cout << "\nThat wasn't one of the options. Try again." << std::endl;
            }
        }
      while (!validChoice);

      chart_selection = (section == "a") ? ChartSelection::FULL_CHART : ChartSelection::SUB_CHART;

      if (chart_selection == ChartSelection::FULL_CHART)
        {
          Zmin = MIN_Z;
          Nmin = MIN_N;
          Zmax = MAX_Z;
          Nmax = MAX_N;
        }
      else if (chart_selection == ChartSelection::SUB_CHART)
        {
          std::cout << "---------------------------\n"
                    << "Enter range of Z, by symbol [n,Ei] or number [0," << MAX_Z << "]\n";

          setExtreme("Zmin");

          setExtreme("Zmax");

          std::cout << "---------------------------\n"
                    << "Draw a) All required N\n"
                    << "     b) A section\n";

          std::string required;
          do
            {
              std::cout << "[a,b]: ";
              std::cin >> required;

              if (required == "a")
                {
                  all_neutrons = AllNeutrons::YES;
                  setNeutronLimits(isotope_vector);
                }
              else if (required == "b")
                {
                  all_neutrons = AllNeutrons::NO;
                  for (const auto& it : isotope_vector)
                    {
                      // Set N range for Zmin
                      if (it.Z == Zmin)
                        {
                          if (it.N < NminZmin)
                            {
                              NminZmin = it.N;
                            }
                          else if (it.N > NmaxZmin)
                            {
                              NmaxZmin = it.N;
                            }
                        }
                      // Set N range for Zmax
                      else if (it.Z == Zmax)
                        {
                          if (it.N < NminZmax)
                            {
                              NminZmax = it.N;
                            }
                          else if (it.N > NmaxZmax)
                            {
                              NmaxZmax = it.N;
                            }
                        }
                    }

                  // Set high/low stable N for Zmax/Zmin
                  for (const auto& it : isotope_vector)
                    {
                      if (it.N >= NminZmin && it.N <= NmaxZmax && it.decay == "stable")
                        {
                          if (it.Z == Zmin && it.N < stblZmin)
                            {
                              stblZmin = it.N;
                            }

                          if (it.Z == Zmax && it.N > stblZmax)
                            {
                              stblZmax = it.N;
                            }
                        }
                    }

                  const Converter converter;
                  std::cout << "---------------------------\n"
                            << "Enter range of N [0," << MAX_N << "]\n"
                            << converter.convertZToSymbol(Zmin) << "(" << Zmin << ") has N from " << NminZmin << " to "
                            << NmaxZmin;

                  if (Zmin > 83 || Zmin == 43 || Zmin == 0)
                    {
                      std::cout << " with no stable isotope\n";
                    }
                  else
                    {
                      std::cout << " and the lightest stable isotope has N=" << stblZmin << "\n";
                    }

                  setExtreme("Nmin");

                  std::cout << converter.convertZToSymbol(Zmax) << "(" << Zmax << ") has N from " << NminZmax << " to "
                            << NmaxZmax;

                  if (Zmax > 83 || Zmax == 43 || Zmax == 0)
                    {
                      std::cout << " with no stable isotope\n";
                    }
                  else
                    {
                      std::cout << " and the heaviest stable isotope has N=" << stblZmax << "\n";
                    }

                  setExtreme("Nmax");
                }
              else
                {
                  std::cout << "\nThat wasn't one of the options. Try again." << std::endl;
                }
            }
          while (required != "a" && required != "b");
        }
      else
        {
          std::cout << "\nThat wasn't one of the options. Try again." << std::endl;
        }
    }
  while (!validChoice);

  std::cout << "---------------------------\n"
            << "Display which nuclei?\n"
            << "a) Experimentally measured only\n"
            << "b) Theoretical/Extrapolated values only\n"
            << "c) Both\n";

  validChoice = false;

  do
    {
      validChoice = true;
      std::string type;
      std::cout << "Which: ";
      std::cin  >> type;

      chart_type = [&type]()
                   {
                     if ( type == "a" )
                       {
                         return ChartType::EXPERIMENTAL;
                       }

                     if ( type == "b" )
                       {
                         return ChartType::THEORETICAL;
                       }

                     return ChartType::ALL;
                   }();

      if ( type != "a" && type != "b" && type != "c" )
        {
          std::cout << "\nThat wasn't one of the options. Try again" << std::endl;
          validChoice = false;
        }
   }
  while ( !validChoice );

  std::cout << "---------------------------\n"
            << "Colour by which property?\n"
            << "a) Error on Mass-Excess\n"
            << "b) Relative Error on Mass-Excess (dm/m)\n";

  if ( !AME )
    {
      std::cout << "c) Major Ground-State Decay Mode\n"
                << "d) Ground-State Half-Life\n";

      if ( chart_type != ChartType::THEORETICAL )
        {
          std::cout << "e) First Isomer Energy\n";
        }
    }

  validChoice = false;
  while ( !validChoice )
    {
      validChoice = true;
      std::string choice;
      std::cout << "Choice: ";
      std::cin  >> choice;

      if ( choice == "a" )
        {
          chart_colour = ChartColour::MASSEXCESSERROR;
        }
      else if ( choice == "b" )
        {
          chart_colour = ChartColour::REL_MASSEXCESSERROR;
        }
      else if ( choice == "c" )
        {
          chart_colour = ChartColour::GS_DECAYMODE;
        }
      else if ( choice == "d" )
        {
          chart_colour = ChartColour::GS_HALFLIFE;
        }
      else if ( choice == "e" )
        {
          chart_colour = ChartColour::FIRST_ISOMERENERGY;
        }
      else
        {
          validChoice = false;
        }

      if ( AME )
        {
          if ( choice != "a" && choice != "b" )
            {
              std::cout << "\nThat wasn't one of the options. Try again" << std::endl;
              validChoice = false;
            }
        }
      else
        {
          if ( chart_type != ChartType::THEORETICAL && !validChoice )
            {
              std::cout << "\nThat wasn't one of the options. Try again" << std::endl;
              validChoice = false;
            }
          else if ( chart_type == ChartType::THEORETICAL && chart_colour == ChartColour::FIRST_ISOMERENERGY )
            {
              std::cout << "\nThat wasn't one of the options. Try again" << std::endl;
              validChoice = false;
            }
        }
    }

  /*
  std::cout << "\n^^^^^^^^^^^^^^^^^^^^^^^^^^^\n"
            << "Z = " << Zmin << " -> " << Zmax << "\n"
            << "N = " << Nmin << " -> " << Nmax << "\n"
            << "===========================\n"
            << Zmin << " : " << NminZmin << " " << stblZmin << " " << NmaxZmin << "\n"
            << Zmax << " : " << NminZmax << " " << stblZmax << " " << NmaxZmax << "\n"
            << "===========================\n"
            << Zmin << " : " << Zmin+Nmin << "  " << Zmin+NmaxZmin << "\n"
            << Zmax << " : " << Zmax+Nmin << "  " << Zmax+NmaxZmax << "\n"
            << "___________________________\n" << std::endl;
  */
}


void inputs::writeOptionFile() const
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
