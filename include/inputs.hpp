#ifndef INPUTS_HPP
#define INPUTS_HPP

#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <algorithm>
#include <sys/stat.h>

#include "allNeutrons.hpp"
#include "chartColour.hpp"
#include "chartSelection.hpp"
#include "chartType.hpp"
#include "converter.hpp"
#include "fileType.hpp"
#include "nuclide.hpp"
#include "version.hpp"

static constexpr int MIN_Z = 0;
static constexpr int MAX_Z = 118;
static constexpr int MIN_N = 0;
static constexpr int MAX_N = 177;

class inputs
{
public:
  //Constructors
  //default
  inputs() { constructFullyQualifiedPaths(); }
  //copy
  inputs(const inputs&) = default;
  //move
  inputs(inputs&&) = default;

  //Assignment
  //copy
  inputs& operator=(const inputs&) = default;
  //move
  inputs& operator=(inputs&&) = default;

  //Destructors
  ~inputs() = default;

  bool checkInputOptions(const std::map<std::string, std::string> &values);
  inline bool checkFileExists(const std::string &file) const noexcept
  {
    struct stat buffer{};
    return ( stat(file.c_str(), &buffer) == 0 );
  }

  int readConsoleArguments(const std::vector<std::string> &console_options);
  int saveConsoleArguments();
  int processConsoleArguments();

  void constructFullyQualifiedPaths();
  void constructOutputFilename();
  void setExtreme(const std::string &limit);
  void showBanner() const;
  void showChartOptions() const;
  void showVersion() const;
  void showUsage(const std::string &exe) const;
  void writeOptionFile();
  void setFileType(const std::string &file_type);
  void setOutputFilename(const std::string &filename);
  void setInputOptions(const std::string &filename);
  void setNeutronLimits(const std::vector<Nuclide> &isotope_vector);

  void readOptionFile(const std::string &inputFilename);
  bool validateInputFile(const std::vector<Nuclide> &isotope_vector);

  void displaySection(const std::vector<Nuclide> &isotope_vector);

  /// Options that can be set via command line
  FileType filetype = FileType::EPS;

  bool grid = false;
  bool magic_numbers = true;
  bool write_isotope = true;
  bool r_process = true;
  bool key = true;
  bool AME = false;

  //3,12,16
  int year = 3;
  // 0=none, 1=both, 2=p-only, 3=n-only
  int single_drip_lines = 1;
  int double_drip_lines = 1;

  std::string personal_isotopes = "";
  // Without extension, this is added in the code
  std::string outfile = "chart";
  std::string options = "options.in";
  std::string inputfile;
  std::string stringfile_type;
  /////////////////////////////

  bool valid_inputfile = false;

  int valid_console = 1;
  int Zmin = MAX_Z;
  int Zmax = MIN_Z;
  int Nmin = MAX_N;
  int Nmax = MIN_N;
  // 1=all, 2=p-rich and stable, 3=n-rich and stable, 6=stable only
  int np_rich = 1;

  std::string path = "./";
  std::string r_proc_path = "r-process.dat";
  std::string neutron_drip = "neutron.drip";
  std::string proton_drip = "proton.drip";
  std::string two_neutron_drip = "2neutron.drip";
  std::string two_proton_drip = "2proton.drip";
  std::string FRDM = "FRLDM_ME.tbl";

  std::string pwd;

  ChartSelection chart_selection = ChartSelection::FULL_CHART;

  ChartColour chart_colour = ChartColour::MASSEXCESSERROR;

  AllNeutrons all_neutrons = AllNeutrons::YES;

  ChartType chart_type = ChartType::ALL;

  std::map<std::string, std::string> arguments;
  std::map<std::string, std::string> inputfile_options;
};

#endif // INPUTS_HPP
