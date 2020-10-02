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

#include <string_view>

#include "inch/allNeutrons.hpp"
#include "inch/chartColour.hpp"
#include "inch/chartSelection.hpp"
#include "inch/chartType.hpp"
#include "inch/fileType.hpp"
#include "inch/limits.hpp"

#include <filesystem>
#include <map>
#include <regex>
#include <string>
#include <vector>

class Nuclide;

class Options
{
public:
  Options() = default;

  Options(const Options& other)     = default;
  Options(Options&& other) noexcept = default;

  Options& operator=(const Options& other) = default;
  Options& operator=(Options&& other) noexcept = default;

  virtual ~Options() noexcept = default;

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
   * \return [true] The file does not exist
   * \return [false] The file already exists
   */
  [[nodiscard]] bool validateOutputFilename() const;

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
   * Print the options and values that were read from the input file
   *
   * \param Nothing
   *
   * \return Nothing
   */
  void printInputFileOptions() const;

  /**
   * Validate that the Z & N range are consistent with what is available
   *
   * \param The vector of isotopes to which the options will be applied
   *
   * \return Nothing
   */
  bool validateSelection(const std::vector<Nuclide>& isotope_vector) const;

  /**
   * Validate that an appropriate type has been chosen
   *
   * \param Nothing
   *
   * \return Nothing
   */
  bool validateType() const;

  /**
   * Validate that an appropriate colour has been chosen
   *
   * \param Nothing
   *
   * \return Nothing
   */
  bool validateColour() const;

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
  void setFileType(std::string_view type) const;

  /**
   * Find the absolute path to the location of the data files,
   * indepdent of the filesystem.
   *
   * \param Nothing
   *
   * \return The absolute path to the data_files directory
   */
  [[nodiscard]] inline static std::filesystem::path getAbsolutePath() { return datapath; }

  // Get the location of this source file
  // This will(should) always be - /some/path/inch/include/inch/options.hpp
  // Only the directory separator will be different, depending on OS
  // We want to replace "include/inch/options.hpp" with "data_files/", using the appropriate separator
  inline static const std::filesystem::path datapath =
      std::filesystem::absolute(std::regex_replace(__FILE__, std::regex("(include.inch)(.)(.*)"), "data_files$2"));

  /// Which file format will the chart be
  mutable FileType filetype{ FileType::EPS };

  /// Store the N,Z limits of the chart being created
  mutable Limits limits;

  /// Draw a grid to help with positioning
  mutable bool grid{ false };
  /// Draw the magic numbers
  mutable bool magic_numbers{ true };
  /// Write the isotope in the drawn 'square'
  mutable bool write_isotope{ true };
  /// Show the r-process path
  mutable bool r_process{ true };
  /// Show the chart's key
  mutable bool key{ true };
  /// Use data from the AME table in addition NUBASE data
  mutable bool AME{ false };

  /// Which year should we take the table from
  /// After 2000 is assumed so current options are: 3,12,16
  mutable int year{ 3 };
  /// Which single particle drip line should we draw
  /// 0=none, 1=both, 2=p-only, 3=n-only
  mutable int single_drip_lines{ 1 };
  /// Which single particle drip line should we draw
  /// 0=none, 1=both, 2=p-only, 3=n-only
  mutable int double_drip_lines{ 1 };
  /// Should we only draw stable, proton-rich, etc. isotopes
  /// 1=all, 2=p-rich and stable, 3=n-rich and stable, 6=stable only
  mutable int np_rich{ 1 };

  /// File contain user isotopes to be drawn
  mutable std::string personal_isotopes{ "" };
  /// File name the chart will be written to, without extension, this is added in the code
  mutable std::filesystem::path outfile{ "chart" };
  /// Input file name
  mutable std::string inputfile{ "" };

  /// Output file options will be written to
  mutable std::filesystem::path options{ "options.in" };

  /// How much of the chart will be drawn
  mutable ChartSelection chart_selection{ ChartSelection::FULL_CHART };

  /// What property will the chart be coloured by
  mutable ChartColour chart_colour{ ChartColour::MASSEXCESSERROR };

  /// What range of neutron values should be shown
  mutable AllNeutrons all_neutrons{ AllNeutrons::YES };

  /// Should we drawn just experimentally measured isotopes, theoretical or both
  mutable ChartType chart_type{ ChartType::ALL };
};

#endif // OPTIONS_HPP
