#ifndef MASSTABLE_HPP
#define MASSTABLE_HPP

#include "nuclide.hpp"
#include "inputs.hpp"

#include <vector>
#include <string>
#include <fstream>

class MassTable
{
public:
  //Constructors
  //default
  MassTable(const std::string &path, const int year);
  explicit MassTable(const std::string &path);
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

  int table_year = 3;

  std::string data_path;
  std::string mass_table_NUBASE;
  std::string mass_table_AME;
  std::string user_isotopes;

  std::vector<Nuclide> theTable;

  inline void setTableYear(const int year) { table_year = year; }
  inline void setDataPath(const std::string &path) { data_path = path; }
  void populateInternalMassTable();
  void setFilePaths(const int tableYear);

private:
  bool readAME(const std::string &table);
  bool readNUBASE(const std::string &table);
  bool readOWN(const std::string &myNuclei);
};

#endif // MASSTABLE_HPP
