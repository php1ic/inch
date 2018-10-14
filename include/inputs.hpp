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

  bool checkInputOptions(const std::map<std::string, std::string> &values) const;
  inline bool checkFileExists(const std::string &file) const noexcept
  {
    struct stat buffer{};
    return ( stat(file.c_str(), &buffer) == 0 );
  }

  int readConsoleArguments(const std::vector<std::string> &console_options) const;
  int saveConsoleArguments() const;
  int processConsoleArguments() const;

  void constructFullyQualifiedPaths() const;
  void constructOutputFilename() const;
  void setExtreme(const std::string &limit) const;
  void showBanner() const;
  void showChartOptions() const;
  void showVersion() const;
  void showUsage(const std::string &exe) const;
  void writeOptionFile() const;
  void setFileType(const std::string &file_type) const;
  void setOutputFilename(const std::string &filename) const;
  void setInputOptions(const std::string &filename) const;
  void setNeutronLimits(const std::vector<Nuclide> &isotope_vector) const;

  void readOptionFile(const std::string &inputFilename) const;
  bool validateInputFile(const std::vector<Nuclide> &isotope_vector) const;

  void displaySection(const std::vector<Nuclide> &isotope_vector) const;

  /// Options that can be set via command line
  mutable FileType filetype = FileType::EPS;

  mutable bool grid = false;
  mutable bool magic_numbers = true;
  mutable bool write_isotope = true;
  mutable bool r_process = true;
  mutable bool key = true;
  mutable bool AME = false;

  //3,12,16
  mutable int year = 3;
  // 0=none, 1=both, 2=p-only, 3=n-only
  mutable int single_drip_lines = 1;
  mutable int double_drip_lines = 1;
  // 1=all, 2=p-rich and stable, 3=n-rich and stable, 6=stable only
  mutable int np_rich = 1;

  // File contain user isotopes to be drawn
  mutable std::string personal_isotopes = "";
  // Without extension, this is added in the code
  mutable std::string outfile = "chart";
  mutable std::string inputfile;
  mutable std::string stringfile_type;
  /////////////////////////////

  mutable bool valid_inputfile = false;

  int valid_console = 1;
  mutable int Zmin = MAX_Z;
  mutable int Zmax = MIN_Z;
  mutable int Nmin = MAX_N;
  mutable int Nmax = MIN_N;

  mutable std::string options = "options.in";
  mutable std::string path = "./";
  mutable std::string r_proc_path = "r-process.dat";
  mutable std::string neutron_drip = "neutron.drip";
  mutable std::string proton_drip = "proton.drip";
  mutable std::string two_neutron_drip = "2neutron.drip";
  mutable std::string two_proton_drip = "2proton.drip";
  mutable std::string FRDM = "FRLDM_ME.tbl";

  mutable ChartSelection chart_selection = ChartSelection::FULL_CHART;

  mutable ChartColour chart_colour = ChartColour::MASSEXCESSERROR;

  mutable AllNeutrons all_neutrons = AllNeutrons::YES;

  mutable ChartType chart_type = ChartType::ALL;

  mutable std::map<std::string, std::string> arguments;
  mutable std::map<std::string, std::string> inputfile_options;
};

#endif // INPUTS_HPP
