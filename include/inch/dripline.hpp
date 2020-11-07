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

#include <string_view>

#include "inch/limits.hpp"

#include <fmt/format.h>

#include <filesystem>
#include <fstream>
#include <string>
#include <utility>
#include <vector>


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
  DripLine(const double nMass, const double pMass, Limits _limits, const LineType& line, std::string_view colour) :
      neutron_mass(nMass), proton_mass(pMass), limits(std::move(_limits)), the_line(line), line_colour(colour)
  {
    setDripLineFile();
    createFileIfDoesNotExist();
  }

  DripLine(const DripLine&) = default;
  DripLine(DripLine&&)      = default;

  /// Delete both due to const members
  DripLine& operator=(const DripLine&) = delete;
  DripLine& operator=(DripLine&&) = delete;

  virtual ~DripLine() = default;

  /// We use the neutron mass a lot so store as part of the class
  mutable double neutron_mass{ 0.0 };
  /// We use the proton mass a lot so store as part of the class
  mutable double proton_mass{ 0.0 };

  /// Store the N,Z limits of the chart being created
  Limits limits;

  /// Which dripline does this instance of the class represent
  mutable LineType the_line{};

  /// The colour of the drip line when drawn
  /// Use a bad default so it's obvious if it hasn't been set
  mutable std::string line_colour{ "black" };

  /// The file used to create the drip line
  mutable std::filesystem::path FRDM_file{ "FRLDM_ME.tbl" };
  /// The file that the drip line is stored in
  mutable std::filesystem::path drip_file{};

  /// The available files that can be assigned to drip_file
  /// Single particle neutron drip line file
  mutable std::filesystem::path neutron_drip{ "neutron.drip" };
  /// Single particle proton drip line file
  mutable std::filesystem::path proton_drip{ "proton.drip" };
  /// Two particle neutron drip line file
  mutable std::filesystem::path two_neutron_drip{ "2neutron.drip" };
  /// Two particle proton drip line file
  mutable std::filesystem::path two_proton_drip{ "2proton.drip" };


  /// Lowest N and Z values for the single neutron drip line
  static constexpr std::pair<int, int> single_n_lower_limits{ 17, 8 };
  /// Lowest N and Z values for the single proton drip line
  static constexpr std::pair<int, int> single_p_lower_limits{ 8, 11 };

  /// Lowest N and Z values for the double neutron drip line
  static constexpr std::pair<int, int> double_n_lower_limits{ 20, 8 };
  /// Lowest N and Z values for the double proton drip line
  static constexpr std::pair<int, int> double_p_lower_limits{ 8, 14 };

  /// Container to hold each isotope in the drop model data file
  struct drop_model_data
  {
    drop_model_data(const int a, const int z, const double me) : A(a), Z(z), N(a - z), ME(me) {}

    int A{ 0 };
    int Z{ 0 };
    int N{ 0 };

    double ME{ 0.0 };
  };

  /// Storage for the entire drop model data file
  static inline std::vector<drop_model_data> dm_data;

  /**
   * Check if the Nmax value of the currently drawn chart is larger than the start of the dripline data
   *
   * \param Nothing
   *
   * \return [true] The Nmax value is high enough
   * \return [false] The Nmax value is too low
   */
  [[nodiscard]] inline bool isNmaxValueHighEnough() const
  {
    const int limit = (the_line == LineType::singleneutron) ? DripLine::single_n_lower_limits.first
                                                            : DripLine::double_n_lower_limits.first;

    return (limits.Nmax > limit);
  }

  /**
   * Check if the Zmax value of the currently drawn chart is larger than the start of the dripline data
   *
   * \param Nothing
   *
   * \return [true] The Zmax value is high enough
   * \return [false] The Zmax value is too low
   */
  [[nodiscard]] inline bool isZmaxValueHighEnough() const
  {
    const int limit = (the_line == LineType::singleproton) ? DripLine::single_p_lower_limits.second
                                                           : DripLine::double_p_lower_limits.second;

    return (limits.Zmax > limit);
  }

  /**
   * Wrap the N and Z comparisons into one function as they are often called together
   *
   * \param Nothing
   *
   * \return [true] Both Nmax anx Zmax are high enough
   * \return [false] Either Nmax or Zmax are too low
   */
  [[nodiscard]] inline bool areNmaxAndZmaxValuesHighEnough() const
  {
    return isNmaxValueHighEnough() && isZmaxValueHighEnough();
  }

  /**
   * Allow the drip line to be any colour
   *
   * \param colour The colour the line should be
   *
   * \return Nothing
   */
  inline void setDripLineColour(std::string_view colour) const noexcept { line_colour = colour; }

  /**
   * Check that a filename has been set for the drip line and that it exists.
   * If it doesn't exist, call createFile() to create it
   *
   * \param Nothing
   *
   * \return [0] File exists and everything is OK
   */
  int createFileIfDoesNotExist() const;

  /**
   * Create the data for the drip line and store it in a file.
   *
   * \param Nothing
   *
   * \return [0] File created
   * \return [1] Could not open file to write to
   */
  int createFile() const;

  /**
   * Read the Finite Range Drop Model file into an array that will later be used to calculate separation energies and
   * look for particle drip lines.
   *
   * \param Should any exisiting data be overwritten
   *
   * \return [false] The file could not be opened to read
   * \return [true] The file's data has been read
   **/
  bool readFRDMFile(const bool overwrite = false) const;

  /**
   * For a given <particle> N/Z we are looking for Z/N dripline
   * e.g. for Z = <number>, hold that const and vary N to look for a -ve separation energy
   *
   * \param The proton/neutron number to hold constant
   * \param Which particle to hold constant
   *
   * \return The particle number at which the spearation energy is negative
   */
  int GetDripValue(const int number, std::string_view particle) const;

  /**
   * Wrapper around GetDripValue explicitly for Z values and looking for N
   *
   * \param The Z value to get the neutron value for
   *
   * \return The neutron number of the dripline
   */
  inline int GetNeutronDripValue(const int Z) const { return GetDripValue(Z, "Z"); }

  /**
   * Wrapper around GetDripValue explicitly for N values and looking for Z
   *
   * \param The N value to get the proton value for
   *
   * \return The proton number of the dripline
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
  [[nodiscard]] static inline std::string WriteDataLine(const int N, const int Z)
  {
    return fmt::format("{0:>3d} {1:>3d}\n", N, Z);
  }

  /**
   * Do the necesary to prepare for writing dripline data
   *
   * \param Nothing
   *
   * \return The text necessary to start writing the drip line in the eps file
   */
  [[nodiscard]] virtual std::string Setup() const = 0;

  /**
   * Do the necessary to clean up after writing dripline data
   *
   * \param Nothing
   *
   * \return The text necessary to tidy up after writing the drip line in the eps file
   */
  [[nodiscard]] virtual std::string TearDown() const = 0;

  /**
   * Read the necessary file for data and output eps code into the chart being created
   *
   * \param outFile An std::ostream representing the chart that is being drawn
   *
   * \return 0 Success
   * \return 1 Faliure
   */
  virtual int WriteLine(std::ostream& outFile) const = 0;

  /**
   * Which data file should be read to get the necessary data
   *
   * \param Nothing
   *
   * \return Nothing
   */
  void setDripLineFile() const;
};

#endif // DRIPLINE_HPP
