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

#include "fmt/format.h"
#include <string_view>

#include "inch/limits.hpp"

#include <filesystem>
#include <fstream>
#include <memory>
#include <string>

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
  DripLine(double nMass, double pMass, Limits _limits, LineType line) :
      neutron_mass(nMass), proton_mass(pMass), limits(_limits), the_line(line)
  {
  }

  DripLine(const DripLine&) = default;
  DripLine(DripLine&&)      = default;

  /// Delete both due to const members
  DripLine& operator=(const DripLine&) = delete;
  DripLine& operator=(DripLine&&) = delete;

  ~DripLine() = default;

  /// We use the neutron mass a lot so store as part of the class
  mutable double neutron_mass{ 0.0 };
  /// We use the proton mass a lot so store as part of the class
  mutable double proton_mass{ 0.0 };

  /// Store the N,Z limits of the chart being created
  Limits limits;

  /// Which dripline does this instance of the class represent
  mutable LineType the_line{};

  /// The file used to create the drip line
  mutable std::filesystem::path FRDM_file{};
  /// The file that the drip line is stored in
  mutable std::filesystem::path drip_file{};
  /// The colour of the drip line when drawn
  mutable std::string line_colour{};

  /// Lowest N and Z values for the single neutron drip line
  static constexpr std::pair<int, int> single_n_lower_limits{ 17, 8 };
  /// Lowest N and Z values for the single proton drip line
  static constexpr std::pair<int, int> single_p_lower_limits{ 8, 11 };

  /// Lowest N and Z values for the double neutron drip line
  static constexpr std::pair<int, int> double_n_lower_limits{ 20, 8 };
  /// Lowest N and Z values for the double proton drip line
  static constexpr std::pair<int, int> double_p_lower_limits{ 8, 14 };

  ///
  static constexpr int lowestN{ 7 };
  ///
  static constexpr int lowestZ{ 7 };

  ///
  struct drop_model_data
  {
    drop_model_data(const int a, const int z, const double me) : A(a), Z(z), N(a - z), ME(me) {}

    int A{ 0 };
    int Z{ 0 };
    int N{ 0 };

    double ME{ 0.0 };
  };

  ///
  static std::vector<drop_model_data> dm_data;

  /**
   * Allow the drip line to be any colour
   *
   * \param colour The colour the line should be
   *
   * \return Nothing
   */
  inline void setDripLineColour(std::string_view colour) const noexcept { line_colour = colour; }

  /**
   * fjkldsaf
   *
   * \param The full path to the Drop Model data file
   *
   * \retrun Nothing
   */
  inline void setDropModelFile(const std::filesystem::path& file) const { FRDM_file = file; }

  /**
   *
   */
  int createFileIfDoesNotExist(const std::filesystem::path& file) const;

  /**
   * Create the data for the drip line and store it in a file.
   *
   * \param Nothing
   */
  int createFile() const;

  /**
   *
   **/
  bool readFRDMFile(const bool overwrite = false) const;

  /**
   *
   */
  int GetDripValue(const int number, std::string_view particle) const;

  /**
   *
   */
  inline int GetNeutronDripValue(const int Z) const { return GetDripValue(Z, "Z"); }

  /**
   *
   */
  inline int GetProtonDripValue(const int N) const { return GetDripValue(N, "N"); }

  /**
   * Output the dripline data in a consistent format
   *
   * \param The N value
   * \param The Z value
   * \param The binding energy for this isotope
   *
   * \return A std::string to be written to the data file
   */
  //[[nodiscard]] inline std::string WriteDataLine(const int N, const int Z, const double value) const
  [[nodiscard]] inline std::string WriteDataLine(const int N, const int Z) const
  {
    // return fmt::format("{0:>3d} {1:>3d} {2:.4f}\n", N, Z, value);
    return fmt::format("{0:>3d} {1:>3d}\n", N, Z);
  }

  /**
   * Do the necesary to prepare for writing dripline data
   *
   * \param Nothing
   *
   * \return The text necessary to start writing the drip line in the eps file
   */
  [[nodiscard]] inline std::string EPSSetup() const
  {
    return fmt::format("gs\n"
                       "{} rgb\n"
                       "1 u div sl\n",
                       line_colour);
  }

  /**
   * Do the necessary to clean up after writing dripline data
   *
   * \param Nothing
   *
   * \return The text necessary to tidy up after writing the drip line in the eps file
   */
  [[nodiscard]] inline std::string EPSTearDown() const
  {
    return fmt::format("st\n"
                       "gr\n");
  }

  /**
   * Read the necessary file for data and output eps code into the chart being created
   *
   * \param outFile An std::ostream representing the chart that is being drawn
   *
   * \return 0 Success
   * \return 1 Faliure
   */
  int EPSWriteLine(std::ostream& outFile) const;

  /**
   * Which data file should be read to get the necessary data
   *
   * \param draw An instance of the Options class
   *
   * \return Nothing
   */
  void setDripLineFile(const Options& draw) const;
};

#endif // DRIPLINE_HPP
