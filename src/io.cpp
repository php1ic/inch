#include "io.hpp"

#include "converter.hpp"
#include "nuclide.hpp"
#include "options.hpp"
#include "version.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>


std::map<std::string, std::string> IO::readConsoleArguments(const std::vector<std::string>& console_options) const
{
  std::map<std::string, std::string> arguments;

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

      arguments.emplace("HELP", "HELP");
      return arguments;
    }

  // Read options via << -flag value >> so, including the executable, we need
  // an odd number of arguments. Don't process any in the even case, as we can't know
  // what the user was trying to do.
  if (console_options.size() % 2 == 0)
    {
      std::cerr << "\n"
                << "***ERROR***: An odd number of arguments is not allowed\n"
                << std::endl;

      valid_console = false;
      arguments.emplace("ERROR", "ERROR");
      return arguments;
    }

  // Don't want initial element as it's the executable, <<-flag option>> format means we
  // want every other element as the key of our map, therfore don't read the final element as
  // even if it was a key, there would be no value with it.
  for (auto it = std::next(std::cbegin(console_options)); it != std::cend(console_options); std::advance(it, 2))
    {
      arguments.emplace(*it, *std::next(it));
    }

  return arguments;
}


// Assign console arguments to the necessary members
void IO::saveConsoleArguments(Options& options, const std::map<std::string, std::string>& arguments) const
{
  for (const auto& flag : arguments)
    {
      // Keep list alphabetical to avoid duplication
      if (flag.first == "-a" || flag.first == "--ame")
        {
          options.AME = flag.second == "ON";
        }
      else if (flag.first == "-b" || flag.first == "--betarich")
        {
          options.np_rich = stoi(flag.second);
        }
      else if (flag.first == "-f" || flag.first == "--filetype")
        {
          options.setFileType(flag.second);
        }
      else if (flag.first == "-g" || flag.first == "--grid")
        {
          options.grid = flag.second == "ON";
        }
      else if (flag.first == "-i" || flag.first == "--infile")
        {
          options.inputfile = flag.second;
        }
      else if (flag.first == "-k" || flag.first == "--key")
        {
          options.key = flag.second == "ON";
        }
      else if (flag.first == "-m" || flag.first == "--magicnumbers")
        {
          options.magic_numbers = flag.second == "ON";
        }
      else if (flag.first == "-o" || flag.first == "--outfile")
        {
          options.outfile = flag.second;
        }
      else if (flag.first == "-p" || flag.first == "--personal")
        {
          options.personal_isotopes = flag.second;
        }
      else if (flag.first == "-r" || flag.first == "--rprocess")
        {
          options.r_process = flag.second == "ON";
        }
      else if (flag.first == "-sdl" || flag.first == "--singleDripLines")
        {
          options.single_drip_lines = stoi(flag.second);
        }
      else if (flag.first == "-ddl" || flag.first == "--doubleDripLines")
        {
          options.double_drip_lines = stoi(flag.second);
        }
      else if (flag.first == "-w" || flag.first == "--writesotopes")
        {
          options.write_isotope = flag.second == "ON";
        }
      else if (flag.first == "-y" || flag.first == "--year")
        {
          options.year = stoi(flag.second);
        }
      else
        {
          std::cerr << "\n**WARNING**: The flag " << flag.first << " is not currently supported. "
                    << "Ignoring this flag and it's associated option " << flag.second << std::endl;
        }
    }
}


void IO::showVersion() const
{
  const std::string date = __DATE__;

  std::cout << "Interactive Nuclear CHart (INCH) version " << INCH_VERSION << "\n"
            << "Copyright (C) " << date.substr(date.rfind(' ')) << " Me.\n"
            << "INCH comes with ABSOLUTELY NO WARRANTY.\n"
            << "You may redistribute copies of INCH\n"
            << "under the terms of the GNU General Public License\n"
            << "For more information about these matters, see the file named COPYING." << std::endl;
}


void IO::showUsage(const std::string& exe) const
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


void IO::showBanner() const
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


std::map<std::string, std::string> IO::readOptionFile(const std::string& inputFilename) const
{
  std::ifstream infile(inputFilename, std::ios::binary);

  std::cout << "Reading " << inputFilename << " for input values {--";
  std::map<std::string, std::string> inputfile_options;
  if (!infile)
    {
      std::cout << "\n***ERROR***: " << inputFilename << " couldn't be opened, does it exist?\n" << std::endl;

      return inputfile_options;
    }

  std::string line;

  while (std::getline(infile, line))
    {
      // Skip empty lines
      // Let lines starting with '#' be comments
      if (line.empty() || line.front() == '#')
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
          // Remove any and all 'white-space' characters
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
  return inputfile_options;
}
