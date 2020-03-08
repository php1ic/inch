/**
 *
 * \class DripLine
 *
 * \brief Container class for the driplines
 *
 * The date storage and creation of all four drip lines is identical, so
 * wrap it up here for convenience
 *
 */
#ifndef DRIPLINE_HPP
#define DRIPLINE_HPP

#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <string_view>

class Options;


/**
 * \enum LineType
 *
 * \brief What drip line are we dealing with
 */
enum class LineType
{
  singleneutron = 0,
  doubleneutron = 1,
  singleproton  = 2,
  doubleproton  = 3
};


class DripLine
{
public:
  DripLine(double nMass, double pMass, int _Zmin, int _Zmax, int _Nmin, int _Nmax, LineType line) :
      neutron_mass(nMass),
      proton_mass(pMass),
      Zmin(_Zmin),
      Zmax(_Zmax),
      Nmin(_Nmin),
      Nmax(_Nmax),
      the_line(line)
  {
  }

  DripLine(const DripLine&) = default;
  DripLine(DripLine&&)      = default;

  /// Delete both due to const members
  DripLine& operator=(const DripLine&) = delete;
  DripLine& operator=(DripLine&&) = delete;

  ~DripLine() = default;

  /// We use the neutron mass a lot so store as part of the class
  mutable double neutron_mass = 0.0;
  /// We use the proton mass a lot so store as part of the class
  mutable double proton_mass = 0.0;

  /// Store tha min proton value used to create the chart
  const int Zmin = 0;
  /// Store tha max proton value used to create the chart
  const int Zmax = 0;
  /// Store tha min neutron value used to create the chart
  const int Nmin = 0;
  /// Store tha max neutron value used to create the chart
  const int Nmax = 0;

  /// Which dripline does this instance of the class represent
  mutable LineType the_line = LineType::singleneutron;

  /// The file used to create the drip line
  mutable std::filesystem::path FRDM_file;
  /// The file that the drip line is stored in
  mutable std::filesystem::path drip_file;
  /// The colour of the drip line when drawn
  mutable std::string line_colour;

  /// Lowest N and Z values for the single neutron drip line
  static constexpr std::pair<int, int> single_n_lower_limits{ 17, 8 };
  /// Lowest N and Z values for the single proton drip line
  static constexpr std::pair<int, int> single_p_lower_limits{ 8, 11 };

  /// Lowest N and Z values for the double neutron drip line
  static constexpr std::pair<int, int> double_n_lower_limits{ 20, 8 };
  /// Lowest N and Z values for the double proton drip line
  static constexpr std::pair<int, int> double_p_lower_limits{ 8, 14 };


  /**
   * Allow the drip line to be any colour
   *
   * \param colour The colour the line should be
   *
   * \return Nothing
   */
  inline void setDripLineColour(std::string_view colour) const noexcept { line_colour = colour; }

  /**
   * Create the data for the drip line and store it in a file, if the file does not exist.
   *
   * \param file The file name to use when creating the file
   */
  int createFile(const std::filesystem::path& file) const noexcept;

  /**
   * Read the necessary file for data and output eps code into the chart being created
   *
   * \param outFile An std::ostream representing the chart that is being drawn
   *
   * \return 0 Success
   * \return 1 Faliure
   */
  int EPSWriteLine(std::ostream& outFile) const noexcept;

  /**
   * Which data file should be read to get the necessary data
   *
   * \param draw An instance of the Options class
   *
   * \return Nothing
   */
  void setDripLineFile(const Options& draw) const noexcept;
};

#endif // DRIPLINE_HPP
