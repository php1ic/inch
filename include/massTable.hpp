#ifndef MASSTABLE_HPP
#define MASSTABLE_HPP

#include "nuclide.hpp"
#include "inputs.hpp"

#include <vector>
#include <string>
#include <fstream>
#include <utility>


class MassTable
{
public:
  //Constructors
  //default
  explicit MassTable(std::string path, const int year=3, const bool ame=false);
  //copy
  MassTable(const MassTable&) = default;
  //move
  MassTable(MassTable&&) = default;

  //Assignment
  //copy
  MassTable& operator=(const MassTable&) = default;
  //move
  MassTable& operator=(MassTable&&) = default;

  //Destructors
  ~MassTable() = default;

  bool use_AME = false;

  mutable int table_year = 3;

  mutable std::string data_path;
  mutable std::string mass_table_NUBASE;
  mutable std::string mass_table_AME;
  mutable std::string user_isotopes;

  std::vector<Nuclide> theTable;

  inline void setTableYear(const int year) const noexcept { table_year = year; }
  inline void setDataPath(const std::string &path) const noexcept { data_path = path; }
  void populateInternalMassTable();
  void setFilePaths(const int tableYear) const noexcept;

private:
  /// Tables sizes (ground state only)
  /// 2003 = 3179
  /// 2012 = 3379
  /// 2016 = 3436
  static constexpr int TABLE_SIZE = 4096;
  static constexpr int AME_HEADER_LENGTH = 39;
  static constexpr int AME_EXPERIMENTAL_MARKER = 52;

  bool readAME(const std::string &ameTable);
  bool readNUBASE(const std::string &nubaseTable);
  bool readOWN(const std::string &ownTable);
};

#endif // MASSTABLE_HPP
