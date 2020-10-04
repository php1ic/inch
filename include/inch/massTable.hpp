/**
 *
 * \class MassTable
 *
 * \brief Read the mass table
 *
 * Read and store the deatils from the necessary mass table file
 */
#ifndef MASSTABLE_HPP
#define MASSTABLE_HPP

#include "inch/nuclide.hpp"

#include <filesystem>
#include <utility>
#include <vector>

class Partition;
class Options;

class MassTable
{
public:
  explicit MassTable(Options& _user_options) : user_options(_user_options)
  {
    theTable.reserve(TABLE_SIZE);
    use_AME       = user_options.AME;
    table_year    = user_options.year;
    user_isotopes = user_options.personal_isotopes;
  }

  MassTable(const MassTable&)     = default;
  MassTable(MassTable&&) noexcept = default;

  /// Delete both due to const members
  MassTable& operator=(const MassTable&) = delete;
  MassTable& operator=(MassTable&&) noexcept = delete;

  ~MassTable() = default;

  /// Should we read data from the Atomic Mass Evaulation data
  mutable bool use_AME{ false };

  /// Which year's table should we read
  mutable int table_year{ TABLE_YEAR };

  /// The NUBASE table file path
  mutable std::filesystem::path mass_table_NUBASE{};
  /// The AME table file path
  mutable std::filesystem::path mass_table_AME{};
  /// The user isotopes file path
  mutable std::filesystem::path user_isotopes{};

  /// User options that effect which table to actually read
  Options& user_options;

  /// Container to store all of the data used to create the file
  std::vector<Nuclide> theTable;

  /**
   * Set the year of the table whos data will be read
   *
   * \param The year table to use
   *
   * \return Nothing
   */
  inline void setTableYear(const int year) const noexcept { table_year = year; }

  /**
   * Fill the main container with the data that will be used to create the chart
   *
   * \param Nothing
   *
   * \return Nothing
   */
  [[nodiscard]] bool populateInternalMassTable();

  /**
   * Use the given year to set the absolute file paths of all data files
   *
   * \param The year of the table so data files can be set
   *
   * \return Nothing
   */
  void setFilePaths(const int tableYear) const noexcept;

  /**
   * Loop through the data container setting the colour each isotope should be drawn,
   * and make sure the necessary part of the key will also be displayed
   *
   * \param An instance of the Partition class
   * \param An instance of the Options class
   *
   * \return Nothing
   */
  void setIsotopeAttributes(Partition& part, const Options& draw);

  /**
   * Get the range of N values for the isotopes with <Z>. Filter on <decayMode> if required.
   * The filter is a regex so default is anything.
   *
   * \param The Z to get the N range for
   * \param The decay mode to filter on
   *
   * \return A std::pair<int,int> with min and max N
   */
  [[nodiscard]] std::pair<int, int> GetNeutronRange(const int Z, const std::string& decayMode = ".") const;

  /**
   * For the isotope with Z=<Z>, get the min and max N values of the stable isotopes.
   *
   * \param The Z value to get the stable N range for
   *
   * \return A std::pair<int,int> with min and max N
   */
  [[nodiscard]] inline std::pair<int, int> GetStableNeutronRange(const int Z) const
  {
    return GetNeutronRange(Z, "stable");
  }

  /**
   * Using the currently set values of Zmin and Zmax, read the mass table
   * and find the assoicated Nmin and Mmax for each Z. Also find the N values
   * of first and last stable isotope for that Z
   *
   * \param Nothing
   *
   * \return Nothing
   */
  void setUserNeutronRange() const;

  /**
   * For the isotope with Z=<Z>, depending on <limit>, extract the N range
   * and ask the user to select a value
   *
   * \param The Z value to set the N limit for
   * \param The limit to set (Nmin or Nmax)
   *
   * \return Nothing
   */
  void SetNeutronLimitForZ(const int Z, std::string_view limit) const;

  // Tables sizes (ground state only)
  // 2003 = 3179
  // 2012 = 3379
  // 2016 = 3436

  /// Which years data should be drawn (after 2000 is assumed)
  static constexpr int TABLE_YEAR{ 3 };
  // We reserve space for the isotope vector
  static constexpr int TABLE_SIZE{ 4096 };
  /// How long is the header in the AME file
  static constexpr int AME_HEADER_LENGTH{ 39 };
  /// Which column marks an isotope as experiemntal in the AME file
  static constexpr int AME_EXPERIMENTAL_MARKER{ 52 };

  /// Don't let the relative error drop below a certain value
  static constexpr double min_relative_error{ 1.0e-7 };

  /**
   * Read the AME datafile for isotopic values
   *
   * \param The absolute path to the data file
   *
   * \return Nothing
   */
  bool readAME(const std::filesystem::path& ameTable) const;

  /**
   * Read the NUBASE datafile for isotopic values
   *
   * \param The absolute path to the data file
   *
   * \return Nothing
   */
  bool readNUBASE(const std::filesystem::path& nubaseTable);

  /**
   * Read the datafile for user defined isotopes
   *
   * \param The absolute path to the data file
   *
   * \return Nothing
   */
  bool readOWN(const std::filesystem::path& ownTable) const;

  /**
   * Convert ... singular file format to csv
   *
   * \param Nothing
   *
   * \return Nothing
   */
  bool outputTableToCSV() const;

  /**
   * Convert ... singular file format to json
   *
   * \param Nothing
   *
   * \return Nothing
   */
  bool outputTableToJSON() const;
};

#endif // MASSTABLE_HPP
