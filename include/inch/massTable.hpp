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

#include <filesystem>
#include <memory>
#include <string>
#include <utility>
#include <vector>

/// TODO
/// Nuclide is an incomplete type so can't forward declare
/// Can we make is complete?
#include "inch/nuclide.hpp"

class Partition;
class Options;

class MassTable
{
public:
  explicit MassTable(std::filesystem::path path,
                     std::filesystem::path _user_data,
                     const int year = TABLE_YEAR,
                     const bool ame = false) :
      use_AME(ame), table_year(year), data_path(std::move(path)), user_isotopes(std::move(_user_data))
  {
    theTable.reserve(TABLE_SIZE);
  }

  MassTable(const MassTable&)     = default;
  MassTable(MassTable&&) noexcept = default;

  /// Delete both due to const members
  MassTable& operator=(const MassTable&) = delete;
  MassTable& operator=(MassTable&&) noexcept = delete;

  ~MassTable() = default;

  /// Should we read data from the Atomic Mass Evaulation data
  const bool use_AME{ false };

  /// Which year's table should we read
  mutable int table_year{ TABLE_YEAR };

  /// The base path of the data files
  mutable std::filesystem::path data_path{};
  /// The NUBASE table file path
  mutable std::filesystem::path mass_table_NUBASE{};
  /// The AME table file path
  mutable std::filesystem::path mass_table_AME{};
  /// The user isotopes file path
  mutable std::filesystem::path user_isotopes{};

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
   * Set the root path of all data files
   *
   * \param The file path
   *
   * \return Nothing
   */
  inline void setDataPath(const std::filesystem::path& path) const noexcept { data_path = path; }

  /**
   * Fill the main container with the data that will be used to create the chart
   *
   * \param Nothing
   *
   * \return Nothing
   */
  void populateInternalMassTable();

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

private:
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

  /**
   * Read the AME datafile for isotopic values
   *
   * \param The absolute path to the data file
   *
   * \return Nothing
   */
  bool readAME(const std::filesystem::path& ameTable);

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
  bool readOWN(const std::filesystem::path& ownTable);

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
