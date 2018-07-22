#include "inputs.hpp"
#include "functions.hpp"


//Read and store the console arguments, process if --help or --version, return if
//a 'bad' number of arguments are given.
int inputs::readConsoleArguments(const std::vector<std::string> &console_options)
{
  //Check for help or version option, print as requrired and exit.
  if ( console_options.size() == 2 )
    {
      if ( console_options.back() == std::string("--version") || console_options.back() == std::string("-v") )
        {
          showVersion();
        }
      else if ( console_options.back() == std::string("--help") || console_options.back() == std::string("-h") )
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

  //Read options via << -flag value >> so, including the executable, we need
  //an odd number of arguments. Don't process any in the even case, as we can't know
  //what the user was trying to do.
  if ( console_options.size()%2 == 0 )
    {
      std::cerr << "\n"
                << "***ERROR***: An odd number of arguments is not allowed\n"
                << std::endl;

      return 1;
    }

  //Don't want initial element as it's the executable, <<-flag option>> format means we
  //want every other element as the key of our map, therfore don't read the final element as
  //even if it was a key, there would be no value with it.
  for ( auto it = std::next(std::cbegin(console_options)); it != std::cend(console_options); std::advance(it,2) )
    {
      arguments.emplace( *it, *std::next(it) );
    }

  return saveConsoleArguments();
}


//Assign console arguments to the necessary members
int inputs::saveConsoleArguments()
{
  for ( auto flag: arguments )
    {
      if ( flag.first == "-y" )
        {
          year = stoi(flag.second);
        }
      else if ( flag.first == "-o" )
        {
          outfile = flag.second;
        }
      else if ( flag.first == "-i" )
        {
          inputfile = flag.second;
        }
      else if ( flag.first == "-f" )
        {
          stringfile_type = flag.second;
        }
      else
        {
          std::cerr << "\n**WARNING**: The flag " << flag.first
                    << " is not currently supported. "
                    << "Ignoring this flag and it's associated option " << flag.second
                    << std::endl;
        }
    }

  return processConsoleArguments();
}


//Validate the console arguments
int inputs::processConsoleArguments()
{
  setFileType(stringfile_type);

  setOutputFilename(outfile);

  if ( !inputfile.empty() )
    {
      setInputOptions(inputfile);
    }

  return 0;
}


void inputs::setInputOptions(const std::string &filename)
{
  readOptionFile(filename);

  valid_inputfile = checkInputOptions(inputfile_options);
}


void inputs::setOutputFilename(const std::string &filename)
{
  outfile = filename;

  constructOutputFilename();

  if ( !checkFileExists(outfile) || outfile.compare(0, 6, "chart.") == 0 )
    {
      std::cout << "Will write chart to " << outfile << std::endl;
      return;
    }

  int f=0;
  bool overwrite=false;
  char replace;
  char rereplace;
  std::cout << "\n**WARNING**: The file " << outfile
            << " already exists.\n"
            << "Overwrite ";
  do
    {
      std::cout << "[y/n]: ";
      std::cin  >> replace;

      if ( replace == 'y' )
        {
          std::cout << "\n"
                    << outfile << " will be overwritten\n" << std::endl;
        }
      else if ( replace == 'n' )
        {
          do
            {
              std::cout << "New filename (without extension): ";
              std::cin  >> outfile;

              constructOutputFilename();

              if ( checkFileExists(outfile) )
                {
                  std::cout << "This file also exists" << std::endl;

                  do
                    {
                      std::cout << "Overwrite this file [y/n]: ";
                      std::cin  >> rereplace;

                      if ( rereplace == 'y' )
                        {
                          overwrite=true;
                          std::cout << "\nWill write chart to "
                                    << outfile << "\n" << std::endl;
                        }
                      else if ( rereplace != 'y' && rereplace != 'n' )
                        {
                          std::cout << "That wasn't y or n. Try again" << std::endl;
                        }
                    }
                  while ( rereplace != 'y' && rereplace != 'n' && !overwrite );
                }
              else
                {
                  std::cout << "\nWill write chart to "
                            << outfile << "\n" << std::endl;
                }
            }
          while ( checkFileExists(outfile) && !overwrite );
        }
      else
        {
          if ( f>1 )
            {
              std::cout << "\nThere are 2 options, you've chosen neither on 3 occasions.\n\n"
                        << "Perhaps this is running in a script.\nExiting..." << std::endl;
              exit(-1);
            }

          std::cout << "That wasn't y or n. Try again" << std::endl;
          ++f;
        }
    }
  while ( replace != 'y' && replace != 'n' );
}


void inputs::setFileType(const std::string &file_type)
{
  if ( file_type == "svg" )
    {
      filetype = FileType::SVG;
    }
  else if ( file_type == "tikz" )
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
  std::string date = __DATE__;

  std::cout << "Interactive Nuclear CHart (INCH) version " << version << "\n"
            << "Copyright (C) " << date.substr(date.rfind(' ')) << " Me.\n"
            << "INCH comes with ABSOLUTELY NO WARRANTY.\n"
            << "You may redistribute copies of INCH\n"
            << "under the terms of the GNU General Public License\n"
            << "For more information about these matters, see the file named COPYING."
            << std::endl;
}


void inputs::showUsage(const std::string &exe) const
{
  std::cout << "\n"
            << "Usage: " << exe.substr(exe.find_last_of('/')+1) << " [with any or non of the options below]\n"
            << "\n"
            << " -h | --help\n"
            << "         Print this help and exit\n"
            << "\n"
            << " -v | --version\n"
            << "         Print version information and exit\n"
            << "\n"
            << " -i | --infile <inputFilename>\n"
            << "         Use the provided file as options to create the chart\n"
            << "\n"
            << " -o | --outfile <outputName>\n"
            << "         Set the root of the output filename i.e. without extension (default:chart)\n"
            << "\n"
            << " -f | --filetype <filetype>\n"
            << "         Set the output file type [default:eps, svg, tikz]\n"
            << "\n"
            << " -y | --year <tableYear>\n"
            << "         Set the year from which data is taken [default:3, 12, 16]\n"
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
            << "         +---+---+---+---v" << version << "\n"
            << std::endl;
}


void inputs::constructFullyQualifiedPaths()
{
  path = LOCAL_PATH;
  path.append("/data_files/");

  FRDM.insert(0,path);

  neutron_drip.insert(0,path);

  two_neutron_drip.insert(0,path);

  proton_drip.insert(0,path);

  two_proton_drip.insert(0,path);

  r_proc_path.insert(0,path);

  pwd = getenv("PWD");
  pwd.append("/");

  options.insert(0,pwd);
}


void inputs::readOptionFile(const std::string &inputFilename)
{
  std::ifstream infile(inputFilename, std::ios::binary);

  std::cout << "Reading " << inputFilename << " for input values {--";

  if ( !infile )
    {
      std::cout << "\n***ERROR***: " << inputFilename
                << " couldn't be opened, does it exist?\n" << std::endl;

      return;
    }

  std::string line;

  while ( getline(infile,line) )
    {
      // Skip empty lines
      // Let lines starting with '#' be comments
      if ( line.empty() || line.at(0) == '#' )
        {
          continue;
        }

      // Clear any part of the string after and including a '#'.
      // We can't get here if the string starts with '#' so no issue
      // of creating an empty string at this point.
      if ( line.find('#') != std::string::npos )
        {
          line.erase(line.find('#'));
        }

      int i=0;
      std::string part;
      std::vector<std::string> theLine(2);
      std::istringstream stream(line);

      while ( getline(stream, part, '=') )
        {
          /// Remove any and all 'white-space' characters
          part.erase(std::remove_if(part.begin(),
                                    part.end(),
                                    [](char x) {return std::isspace(x);} ),
                     part.end()
                     );

          theLine.at(i) = part;
          ++i;
        }

      if ( inputfile_options.count(theLine.at(0)) > 0 )
        {
          std::cout << "\n**WARNING**: Already have a value for " << theLine.at(0)
                    << " (" << theLine.at(1) << ")"
                    << ", will use new value.\n";
        }

      inputfile_options.emplace( theLine.front(), theLine.back() );
    }

  infile.close();

  std::cout << "--} done" << std::endl;
}


bool inputs::validateInputFile(const std::vector<Nuclide> &isotope_vector)
{
  //Check that the options as a whole make sense.
  if ( section == "a" )
    {
      if ( Zmin != MAX_Z && Zmax != MIN_Z )
        {
          std::cout << "**WARNING**\n"
                    << "The option file contains a Z range but specifies that all nuclei should be drawn.\n"
                    << "The input range will be ignored, set section=b to select a range in Z.\n"
                    << "***********\n" << std::endl;
        }

      Zmin=MIN_Z;
      Zmax=MAX_Z;
      Nmin=MIN_N;
      Nmax=MAX_N;
    }
  else if ( section == "b" )
    {
      if ( required == "a" )
        {
          setNeutronLimits(isotope_vector);
        }
      else if ( required != "b" )
        {
          std::cout << "***ERROR***: " << required
                    << " is not a valid option for the 'required' field.\n"
                    << "            Ignoring input file.\n" << std::endl;
          return false;
        }

      if ( Zmin > Zmax )
        {
          std::cout << "***ERROR***: Zmax(" << Zmax
                    << ") cannot be less than Zmin(" << Zmin<< ")\n"
                    << "            Ignoring input file.\n" << std::endl;
          return false;
        }

      if ( Nmin > Nmax )
        {
          std::cout << "***ERROR***: Nmax(" << Nmax
                    << ") cannot be less than Nmin(" << Nmin<< ")\n"
                    << "            Ignoring input file.\n" << std::endl;
          return false;
        }
    }
  else
    {
      std::cout << "***ERROR***: " << section
                << " is not a valid option for the 'section' field.\n"
                << "            Ignoring input file.\n" << std::endl;
      return false;
    }

  if (   type != "a"
      && type != "b"
      && type != "c"
      )
    {
      std::cout << "***ERROR***: " << type
                << " is not a valid option for the 'type' field.\n"
                << "            Ignoring input file.\n" << std::endl;
      return false;
    }

  if (   choice != "a"
      && choice != "b"
      && choice != "c"
      && choice != "d"
      && choice != "e"
      )
    {
      std::cout << "***ERROR***: " << choice
                << " is not a valid option for the 'choice' field.\n"
                << "            Ignoring input file.\n" << std::endl;
      return false;
    }

  std::cout << "Read values:\n"
            << "section: " << section  << "\n";

  if ( section == "b" )
    {
      std::cout << "Zmin: "     << Zmin << "\n"
                << "Zmax: "     << Zmax << "\n"
                << "required: " << required << "\n";

      if ( required == "b" )
        {
          std::cout << "Nmin: " << Nmin << "\n"
                    << "Nmax: " << Nmax << "\n";
        }
    }

  std::cout << "type: " << type << "\n"
            << "choice: " << choice << std::endl;

  return true;
}


bool inputs::checkInputOptions(const std::map<std::string, std::string> &values)
{
  int linesRead=0;

  for ( const auto &it : values )
    {
      if ( it.first == "section" )
        {
          section = it.second;
          linesRead++;
        }
      else if ( it.first == "type" )
        {
          type=it.second;
          linesRead++;

          if ( type == "a" )
            {
              experimental = 1;
            }
          else if ( type == "b" )
            {
              experimental = 0;
            }
          else if ( type == "c" )
            {
              experimental = 2;
            }
          else
            {
              --linesRead;
              std::cerr << "***ERROR***: " << type
                        << " is not a valid choice for 'type'" << std::endl;
            }
        }
      else if ( it.first == "choice" )
        {
          choice = it.second;
          linesRead++;
        }
      else if ( it.first == "required" )
        {
          required = it.second;
        }
      else if ( it.first == "Zmin" )
        {
          bool valid = true;
          try
            {
              Zmin = stoi(it.second);
            }
          catch ( const std::invalid_argument& ia )
            {
              valid = false;
            }

          if ( !valid || Zmin < MIN_Z || Zmin > MAX_Z )
            {
              std::cerr << "\n***ERROR***: " << it.second
                        << " is not a valid choice for 'Zmin'" << std::endl;
              return false;
            }

          linesRead++;
        }
      else if ( it.first == "Zmax" )
        {
          bool valid = true;
          try
            {
              Zmax = stoi(it.second);
            }
          catch ( const std::invalid_argument& ia )
            {
              valid = false;
            }

          if ( !valid || Zmax < MIN_Z || Zmax > MAX_Z )
            {
              std::cerr << "\n***ERROR***: " << it.second
                        << " is not a valid choice for 'Zmax'" << std::endl;
              return false;
            }

          linesRead++;
        }
      else if ( it.first == "Nmin" )
        {
          bool valid = true;
          try
            {
              Nmin = stoi(it.second);
            }
          catch ( const std::invalid_argument& ia )
            {
              valid = false;
            }

          if ( !valid || Nmin < MIN_N || Nmin > MAX_N )
            {
              std::cerr << "***\nERROR***: " << it.second
                        << " is not a valid choice for 'Nmin'" << std::endl;
              return false;
            }

          linesRead++;
        }
      else if ( it.first == "Nmax" )
        {
          bool valid = true;
          try
            {
              Nmax = stoi(it.second);
            }
          catch ( const std::invalid_argument& ia )
            {
              valid = false;
            }

          if ( !valid || Nmax < MIN_N || Nmax > MAX_N )
            {
              std::cerr << "***ERROR***: " << it.second
                        << " is not a valid choice for 'Nmax'" << std::endl;
              return false;
            }

          linesRead++;
        }
      else
        {
          std::cout << "**WARNING**: " << it.first
                    <<" is not a valid option. Ignoring." << std::endl;
        }
    }

  if ( linesRead < 3 )
    {
      std::cout << "Not enough inputs have been read from the file." << std::endl;
      return false;
    }

  return true;
}


void inputs::setCanvasSize(const double scale, const double height)
{
  if ( height*scale > (Zmax-Zmin+2) )
    {
      key_relative = true;
      chart_height = (height+1.0)*scale;
    }
  else
    {
      chart_height = Zmax-Zmin+2;
    }

  //HACKS
  //- When all nuclei are drawn, key is in top left.
  //Below stops extra space being created on the right.
  //- 14.5*scale extends the width to fit the widest key
  //this should really be set as a function of the variable
  //used to colour the isotopes. Either way, this cannot be
  //set dynamically in the file so we need to use 'magic numbers'
  if ( section == "a" || (Zmax-Zmin) == MAX_Z )
    {
      chart_width = Nmax-Nmin+2;
    }
  else
    {
      chart_width = (Nmax-Nmin+2) + (14.5*scale);
    }
}


void inputs::setNeutronLimits(const std::vector<Nuclide> &isotope_vector)
{
  Nmin = MAX_N;
  Nmax = MIN_N;

  for ( const auto &it : isotope_vector )
    {
      if (   it.Z >= Zmin
          && it.Z <= Zmax
          && it.rich % np_rich == 0
          )
        {
          if ( it.N < Nmin )
            {
              Nmin = it.N;
            }

          if ( it.N > Nmax )
            {
              Nmax = it.N;
            }
        }
    }
}


void inputs::setExtreme(const std::string &limit)
{
  if ( limit != "Zmin" && limit != "Zmax" && limit != "Nmin" && limit != "Nmax" )
    {
      std::cout << "**WARNING** - " << limit << " is not a valid input\n"
                << "              choose either Zmin, Zmax, Nmin or Nmax\n"
                << "Setting limits to maxima values." << std::endl;

      Zmin=MIN_Z;
      Zmax=MAX_Z;
      Nmin=MIN_N;
      Nmax=MAX_N;

      return;
    }

  bool valid=false;

  do
    {
      int number=0;
      std::string in;

      std::cout << limit << ": ";
      std::cin >> in;

      //Read the entered value and convert symbol->Z if necessary
      try
        {
          number = stoi(in);
        }
      catch ( const std::invalid_argument& ia )
        {
          const Converter converter;
          number = ( limit.at(0) == 'Z' ) ? converter.convertSymbolToZ(in) : -1 ;
        }

      //Validate the number
      if ( limit == "Zmin" )
        {
          Zmin = number;
          valid = true;
          if ( Zmin < MIN_Z || Zmin > MAX_Z )
            {
              std::cout << "\n"
                        << "Zmin must be in the range " << MIN_Z << "<Z<" << MAX_Z
                        << "\n" << std::endl;
              valid = false;
            }
        }
      else if ( limit == "Zmax" )
        {
          Zmax = number;
          valid = true;
          if ( Zmax < Zmin || Zmax > MAX_Z )
            {
              std::cout << "\n"
                        <<"Zmax must be in the range " << Zmin << "<Z<" << MAX_Z
                        << "\n" << std::endl;
              valid = false;
            }
        }
      else if ( limit == "Nmin" )
        {
          Nmin = number;
          valid = true;
          if ( Nmin < MIN_N || Nmin > MAX_N )
            {
              std::cout << "\n"
                        << "Nmin must be in the range " << MIN_N << "<N<" << MAX_N
                        << "\n" << std::endl;
              valid = false;
            }
        }
      else if ( limit == "Nmax" )
        {
          Nmax = number;
          valid = true;
          if ( Nmax < Nmin || Nmax > MAX_N )
            {
              std::cout << "\n"
                        << "Nmax must be in the range " << Nmin << "<N<" << MAX_N
                        << "\n" << std::endl;
              valid=false;
            }
        }
    }
  while ( !valid );
}


void inputs::showChartOptions() const
{
  const Converter converter;
  std::cout << "===========================\n"
            << "\nBetween Z = " << Zmin << "(" << converter.convertZToSymbol(Zmin)
            << ") and Z = " << Zmax << "(" << converter.convertZToSymbol(Zmax) << ")";

  if ( section == "a" || (section == "b" && required == "a") )
    {
      std::cout << ", with all relevant nuclei,\n";
    }
  else if ( required == "b" )
    {
      std::cout << ", N = " << Nmin << " and N = " << Nmax << "\n";
    }

  if ( type == "a" )
    {
      std::cout << "experimentally measured";
    }
  else if ( type == "b" )
    {
      std::cout << "theoretical/extrapolated";
    }
  else
    {
      std::cout << "both experimental and theoretical";
    }

  std::cout << " values will be drawn and\nthe chart coloured by ";

  if ( choice == "a" )
    {
      std::cout << "error on mass-excess\n";
    }
  else if ( choice == "b" )
    {
      std::cout << "relative error on mass-excess\n";
    }
  else if ( choice == "c" )
    {
      std::cout << "major ground-state decay mode\n";
    }
  else if ( choice == "d" )
    {
      std::cout << "ground-state half-life\n";
    }
  else
    {
      std::cout << "first isomer energy\n";
    }
}


void inputs::constructOutputFilename()
{
  //-Remove the extension if given
  if (   outfile.find('.') == outfile.length()-4
      && outfile.find('.') != std::string::npos
      )
    {
      std::cout << "\nThe extension is added depending on the chosen file type\n";

      outfile.erase(outfile.rfind('.'),4);
    }

  //-Remove the CWD if given
  if ( outfile.find(pwd) != std::string::npos )
    {
      std::cout << "\nThe current working directory is added\n";

      outfile.erase(0,outfile.rfind('/')+1);
    }

  //-Check output file is not a directory.
  if (   outfile.empty()
      || outfile.at(outfile.length()-1) == '/'
      )
    {
      std::cout << "\n"
                << "***ERROR***: "
                << outfile << " is a directory, can't use that as a file name\n"
                << std::endl;

      outfile = "chart";
    }

  std::cout << R"(Using ")" << outfile << R"(" as the base of the file name.)" << std::endl;

  //-Add the necessary extension
  switch ( filetype )
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


void inputs::displaySection(const std::vector<Nuclide> &isotope_vector)
{
  int stblZmin = MAX_N;
  int stblZmax = MIN_N;
  int NminZmin = MAX_N;
  int NminZmax = MAX_N;
  int NmaxZmin = MIN_N;
  int NmaxZmax = MIN_N;

  std::cout << "\n---------------------------\n"
            << "Draw a) The entire chart\n"
            << "     b) A section\n";
  do
    {
      std::cout << "[a,b]: ";
      std::cin  >> section;

      if ( section == "a" )
        {
          Zmin = MIN_Z;
          Nmin = MIN_N;
          Zmax = MAX_Z;
          Nmax = MAX_N;
        }
      else if ( section == "b" )
        {
          std::cout << "---------------------------\n"
                    << "Enter range of Z, by symbol [n,Ei] or number [0," << MAX_Z << "]\n";

          setExtreme("Zmin");

          setExtreme("Zmax");

          std::cout << "---------------------------\n"
                    << "Draw a) All required N\n"
                    << "     b) A section\n";
          do
            {
              std::cout << "[a,b]: ";
              std::cin  >> required;

              if ( required == "a" )
                {
                  setNeutronLimits(isotope_vector);
                }
              else if ( required == "b" )
                {
                  for ( const auto &it : isotope_vector )
                    {
                      //Set N range for Zmin
                      if ( it.Z == Zmin )
                        {
                          if ( it.N < NminZmin )
                            {
                              NminZmin = it.N;
                            }
                          else if ( it.N > NmaxZmin )
                            {
                              NmaxZmin = it.N;
                            }
                        }
                      //Set N range for Zmax
                      else if ( it.Z == Zmax )
                        {
                          if ( it.N < NminZmax )
                            {
                              NminZmax = it.N;
                            }
                          else if ( it.N > NmaxZmax )
                            {
                              NmaxZmax = it.N;
                            }
                        }
                    }

                  //Set high/low stable N for Zmax/Zmin
                  for ( const auto &it : isotope_vector )
                    {
                      if (    it.N >= NminZmin
                           && it.N <= NmaxZmax
                           && it.decay == "stable"
                          )
                        {
                          if ( it.Z == Zmin && it.N < stblZmin )
                            {
                              stblZmin = it.N;
                            }

                          if ( it.Z == Zmax && it.N > stblZmax )
                            {
                              stblZmax = it.N;
                            }
                        }
                    }

                  const Converter converter;
                  std::cout << "---------------------------\n"
                            << "Enter range of N [0," << MAX_N << "]\n"
                            << converter.convertZToSymbol(Zmin) << "(" << Zmin << ") has N from "
                            << NminZmin << " to " << NmaxZmin;

                  if ( Zmin > 83 || Zmin == 43 || Zmin == 0 )
                    {
                      std::cout << " with no stable isotope\n";
                    }
                  else
                    {
                      std::cout << " and the lightest stable isotope has N=" << stblZmin << "\n";
                    }

                  setExtreme("Nmin");

                  std::cout << converter.convertZToSymbol(Zmax) << "(" << Zmax << ") has N from "
                            << NminZmax << " to " << NmaxZmax;

                  if ( Zmax > 83 || Zmax == 43 || Zmax == 0 )
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
          while ( required != "a" && required != "b" );
        }
      else
        {
          std::cout << "\nThat wasn't one of the options. Try again." << std::endl;
        }
    }
  while ( section != "a" && section != "b" );

  std::cout << "---------------------------\n"
            << "Display which nuclei?\n"
            << "a) Experimentally measured only\n"
            << "b) Theoretical/Extrapolated values only\n"
            << "c) Both\n";
  do
    {
      std::cout << "Which: ";
      std::cin  >> type;

      if ( type != "a" && type != "b" && type != "c" )
        {
          std::cout << "\nThat wasn't one of the options. Try again" << std::endl;
        }
    }
  while ( type != "a" && type != "b" && type != "c" );

  if ( type == "a" )
    {
      experimental = 0;
    }
  else if ( type == "b" )
    {
      experimental = 1;
    }
  else
    {
      experimental = 2;
    }

  std::cout << "---------------------------\n"
            << "Colour by which property?\n"
            << "a) Error on Mass-Excess\n"
            << "b) Relative Error on Mass-Excess (dm/m)\n";

  if ( !AME )
    {
      std::cout << "c) Major Ground-State Decay Mode\n"
                << "d) Ground-State Half-Life\n";

      if ( type!="b" )
        {
          std::cout << "e) First Isomer Energy\n";
        }
    }

  bool validChoice=false;
  while ( !validChoice )
    {
      std::cout << "Choice: ";
      std::cin  >> choice;

      if ( AME )
        {
          if ( choice != "a" && choice != "b" )
            {
              std::cout << "\nThat wasn't one of the options. Try again" << std::endl;
            }
          else if ( choice == "a" || choice == "b" )
            {
              validChoice = true;
            }
        }
      else
        {
          if (  (type == "a" || type== "c")
              && choice != "a"
              && choice != "b"
              && choice != "c"
              && choice != "d"
              && choice != "e"
              )
            {
              std::cout << "\nThat wasn't one of the options. Try again" << std::endl;
            }
          else if (   type == "b"
                   && choice != "a"
                   && choice != "b"
                   && choice != "c"
                   && choice != "d"
                   )
            {
              std::cout << "\nThat wasn't one of the options. Try again" << std::endl;
            }
          else
            {
              validChoice = true;
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


void inputs::writeOptionFile()
{
  //Match the options filename to that of the output chart
  if ( outfile.find("chart") == std::string::npos )
    {
      options = outfile.substr(0, outfile.rfind('.')) + ".in";
    }

  std::ofstream opts(options, std::ios::binary);

  std::cout << "Writing user choices to " << options;

  if ( !opts )
    {
      std::cout << "\n"
                << "***ERROR***: Couldn't open " << options << " to write the options.\n"
                << "             Not creating an option file." << std::endl;

      return;
    }

  opts << "section=" << section << "\n";

  if ( section == "b" )
    {
      opts << "Zmin=" << Zmin << "\n"
           << "Zmax=" << Zmax << "\n"
           << "required=" << required << "\n";

      if ( required == "b" )
        {
          opts << "Nmin=" << Nmin << "\n"
               << "Nmax=" << Nmax << "\n";
        }
    }

  opts << "type=" << type << "\n"
       << "choice=" << choice << std::endl;

  opts.close();

  std::cout << " - done\n" << std::endl;
}
