#ifndef MASSTABLE_HPP
#define MASSTABLE_HPP

#include "inputs.hpp"
#include "nuclide.hpp"
#include "partition.hpp"

#include <vector>
#include <string>
#include <fstream>
#include <utility>


class MassTable
{
public:
  //Constructors
  //default
  explicit MassTable(std::string path, std::string _user_data, const int year=TABLE_YEAR, const bool ame=false):
    use_AME(ame),
    table_year(year),
    data_path(std::move(path)),
    user_isotopes(std::move(_user_data))
  {
    theTable.reserve(TABLE_SIZE);
  }

  //copy
  MassTable(const MassTable&) = default;
  //move
  MassTable(MassTable&&) noexcept = default;

  //Assignment
  //copy
  MassTable& operator=(const MassTable&) = delete;
  //move
  MassTable& operator=(MassTable&&) noexcept = delete;

  //Destructors
  ~MassTable() = default;

  const bool use_AME = false;

  mutable int table_year = TABLE_YEAR;

  mutable std::string data_path;
  mutable std::string mass_table_NUBASE;
  mutable std::string mass_table_AME;
  mutable std::string user_isotopes;

  std::vector<Nuclide> theTable;

  inline void setTableYear(const int year) const noexcept { table_year = year; }
  inline void setDataPath(const std::string &path) const noexcept { data_path = path; }
  void populateInternalMassTable();
  void setFilePaths(const int tableYear) const noexcept;
  void setIsotopeAttributes(std::unique_ptr<Partition> &part, const std::unique_ptr<inputs> &draw);

private:
  /// Tables sizes (ground state only)
  /// 2003 = 3179
  /// 2012 = 3379
  /// 2016 = 3436
  static constexpr int TABLE_YEAR = 3;
  static constexpr int TABLE_SIZE = 4096;
  static constexpr int AME_HEADER_LENGTH = 39;
  static constexpr int AME_EXPERIMENTAL_MARKER = 52;

  bool readAME(const std::string &ameTable);
  bool readNUBASE(const std::string &nubaseTable);
  bool readOWN(const std::string &ownTable);
};

#endif // MASSTABLE_HPP
