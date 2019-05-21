/**
 *
 * \class Options
 *
 * \brief Storage class for all of the options that are used to create the chart
 *
 * This has become a bit of a catch all class, it may disappear as refactoring occurs
 */
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
  Options() { constructAbsolutePaths(); }

  Options(const Options& other)     = default;
  Options(Options&& other) noexcept = default;

  Options& operator=(const Options& other) = default;
  Options& operator=(Options&& other) noexcept = default;

  virtual ~Options() noexcept = default;

  /**
   * Does a file exist at the path provided
   *
   * \param Absolute path to query
   *
   * \return Does the file exist yes/no true/false
   */
  inline bool checkFileExists(const std::string& file) const noexcept
  {
    struct stat buffer
    {
    };
    return (stat(file.c_str(), &buffer) == 0);
  }

  /**
   * Convert the relative paths to absolute so the necessary file can be opened
   *
   * \param Nothing
   *
   * \return Nothing
   */
  void constructAbsolutePaths() const;

  /**
   * Make sure the selected output file name does not exist
   *
   * \param Nothing
   *
   * \return Nothing
   */
  void setOutputFilename() const;

  /**
   * Validate the selected file name and append the appropriate extension
   *
   * \param Nothing
   *
   * \return Nothing
   */
  void constructOutputFilename() const;

  /**
   * Output (to screen) the various paramaters that will be used to create the chart
   *
   * \param Nothing
   *
   * \return Nothing
   */
  void showChartOptions() const;

  /**
   * Write the options used so the same chart can be recreated
   *
   * \param Nothing
   *
   * \return Nothing
   */
  void writeOptionFile() const;

  /**
   * Check that the input file given as an argument contains the minimum number of options
   *
   * No validation of the values is done here, we just check that enough are present so a chart
   * can be created.
   *
   * \param The chart options as read from the input file
   *
   * \return Are there enough options yes/no true/false
   */
  bool checkInputFileOptions(const std::map<std::string, std::string>& values) const;

  /**
   * Validate that the options read in checkInputFileOptions() make sense when taken all together
   *
   * \param The vector of isotopes to which the options will be applied
   *
   * \return Are the options valid yes/no true/false
   */
  bool validateInputFileOptions(const std::vector<Nuclide>& isotope_vector) const;

  /**
   * Having read the mass table and set proton limits, set the appropriate neutron limits
   *
   * \param The mass table that has been read
   *
   * \return Nothing
   */
  void setNeutronLimits(const std::vector<Nuclide>& table) const;

  /**
   * Set which format the file will be created in
   *
   * \param A std::string representing the extension to be used
   *
   * \return Nothing
   */
  void setFileType(const std::string& type) const;

  /// Which file format will the chart be
  mutable FileType filetype = FileType::EPS;

  /// Lower bound on the proton number
  mutable int Zmin = Limits::MAX_Z;
  /// Upper bound on the proton number
  mutable int Zmax = Limits::MIN_Z;
  /// Lower bound on the neutron number
  mutable int Nmin = Limits::MAX_N;
  /// Upper bound on the neutron number
  mutable int Nmax = Limits::MIN_N;

  /// Draw a grid to help with positioning
  mutable bool grid = false;
  /// Draw the magic numbers
  mutable bool magic_numbers = true;
  /// Write the isotope in the drawn 'square'
  mutable bool write_isotope = true;
  /// Show the r-process path
  mutable bool r_process = true;
  /// Show the chart's key
  mutable bool key = true;
  /// Use data from the AME table in addition NUBASE data
  mutable bool AME = false;

  /// Which year should we take the table from
  /// After 2000 is assumed so current options are: 3,12,16
  mutable int year = 3;
  /// Which single particle drip line should we draw
  /// 0=none, 1=both, 2=p-only, 3=n-only
  mutable int single_drip_lines = 1;
  /// Which single particle drip line should we draw
  /// 0=none, 1=both, 2=p-only, 3=n-only
  mutable int double_drip_lines = 1;
  /// Should we only draw stable, proton-rich, etc. isotopes
  /// 1=all, 2=p-rich and stable, 3=n-rich and stable, 6=stable only
  mutable int np_rich = 1;

  /// File contain user isotopes to be drawn
  mutable std::string personal_isotopes = "";
  /// File name the chart will be written to, without extension, this is added in the code
  mutable std::string outfile   = "chart";
  /// Input file name
  mutable std::string inputfile = "";

  /// Output file options will be written to
  mutable std::string options          = "options.in";
  /// Absolute path that will be prepended to others files so they can be located
  mutable std::string path             = "./";
  /// R-process data file
  mutable std::string r_proc_path      = "r-process.dat";
  /// Single particle neutron drip line file
  mutable std::string neutron_drip     = "neutron.drip";
  /// Single particle proton drip line file
  mutable std::string proton_drip      = "proton.drip";
  /// Two particle neutron drip line file
  mutable std::string two_neutron_drip = "2neutron.drip";
  /// Two particle proton drip line file
  mutable std::string two_proton_drip  = "2proton.drip";
  /// Finite-range drop model data file
  mutable std::string FRDM             = "FRLDM_ME.tbl";

  /// How much of the chart will be drawn
  mutable ChartSelection chart_selection = ChartSelection::FULL_CHART;

  /// What property will the chart be coloured by
  mutable ChartColour chart_colour = ChartColour::MASSEXCESSERROR;

  /// What range of neutron values should be shown
  mutable AllNeutrons all_neutrons = AllNeutrons::YES;

  /// Should we drawn just experimentally measured isotopes, theoretical or both
  mutable ChartType chart_type = ChartType::ALL;
};

#endif // OPTIONS_HPP
