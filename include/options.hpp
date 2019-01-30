#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include "allNeutrons.hpp"
#include "chartColour.hpp"
#include "chartSelection.hpp"
#include "chartType.hpp"
#include "fileType.hpp"
#include "limits.hpp"

#include <map>
#include <string>
#include <sys/stat.h>
#include <vector>

class Nuclide;

class Options
{
public:
  // Default constructor
  Options() { constructAbsolutePaths(); }

  // Copy constructor
  Options(const Options& other) = default;

  // Move constructor
  Options(Options&& other) noexcept = default;

  // Destructor
  virtual ~Options() noexcept = default;

  // Copy assignment operator
  Options& operator=(const Options& other) = default;

  // Move assignment operator
  Options& operator=(Options&& other) noexcept = default;

  inline bool checkFileExists(const std::string& file) const noexcept
  {
    struct stat buffer
    {
    };
    return (stat(file.c_str(), &buffer) == 0);
  }

  void constructAbsolutePaths() const;
  void setOutputFilename() const;
  void constructOutputFilename() const;
  void showChartOptions() const;
  void writeOptionFile() const;
  bool checkInputFileOptions(const std::map<std::string, std::string>& values) const;
  bool validateInputFileOptions(const std::vector<Nuclide>& isotope_vector) const;
  void setNeutronLimits(const std::vector<Nuclide>& table) const;

  void setFileType() const;
  mutable std::string stringfile_type;
  mutable FileType filetype = FileType::EPS;

  mutable int Zmin = Limits::MAX_Z;
  mutable int Zmax = Limits::MIN_Z;
  mutable int Nmin = Limits::MAX_N;
  mutable int Nmax = Limits::MIN_N;

  mutable bool grid          = false;
  mutable bool magic_numbers = true;
  mutable bool write_isotope = true;
  mutable bool r_process     = true;
  mutable bool key           = true;
  mutable bool AME           = false;

  // 3,12,16
  mutable int year = 3;
  // 0=none, 1=both, 2=p-only, 3=n-only
  mutable int single_drip_lines = 1;
  mutable int double_drip_lines = 1;
  // 1=all, 2=p-rich and stable, 3=n-rich and stable, 6=stable only
  mutable int np_rich = 1;

  // File contain user isotopes to be drawn
  mutable std::string personal_isotopes = "";
  // Without extension, this is added in the code
  mutable std::string outfile   = "chart";
  mutable std::string inputfile = "";

  mutable std::string options          = "options.in";
  mutable std::string path             = "./";
  mutable std::string r_proc_path      = "r-process.dat";
  mutable std::string neutron_drip     = "neutron.drip";
  mutable std::string proton_drip      = "proton.drip";
  mutable std::string two_neutron_drip = "2neutron.drip";
  mutable std::string two_proton_drip  = "2proton.drip";
  mutable std::string FRDM             = "FRLDM_ME.tbl";

  mutable ChartSelection chart_selection = ChartSelection::FULL_CHART;

  mutable ChartColour chart_colour = ChartColour::MASSEXCESSERROR;

  mutable AllNeutrons all_neutrons = AllNeutrons::YES;

  mutable ChartType chart_type = ChartType::ALL;
};

#endif // OPTIONS_HPP
