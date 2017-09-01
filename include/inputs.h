#ifndef INPUTS_H
#define INPUTS_H

#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <algorithm>

const int MIN_Z=0;
const int MAX_Z=118;
const int MIN_N=0;
const int MAX_N=176;

const int KEY_YOFFSET=9;

class inputs
{
public:
  inputs();
  ~inputs();

  bool checkInputOptions(std::map<std::string, std::string> &values);
  void constructFullyQualifiedPaths();
  void constructOutputFilename();
  void setCanvasSize();
  void setExtreme(const std::string &limit);
  void setTableYear(const int year);
  void showBanner() const;
  void showChartOptions() const;
  void showVersion() const;
  void showUsage(std::string exe) const;
  void writeOptionFile();

  int convertSymbolToZ(const std::string &el) const;
  std::string convertZToSymbol(const int Z) const;

  bool grid;
  bool magic_numbers;
  bool write_isotope;
  bool r_process;
  bool key;
  bool own_nuclei;
  bool AME;
  bool key_relative;

  int Zmin;
  int Zmax;
  int Nmin;
  int Nmax;
  int size;
  int experimental;
  int np_rich;
  int single_drip_lines;
  int double_drip_lines;
  int file_type;
  int year;

  double curve;
  double key_height;
  double key_scale;
  double chart_height;
  double chart_width;

  std::string path;
  std::string mass_table;
  std::string mass_table_AME;
  std::string mass_table_NUBASE;
  std::string my_nuclei;
  std::string r_proc_path;
  std::string neutron_drip;
  std::string proton_drip;
  std::string two_neutron_drip;
  std::string two_proton_drip;
  std::string choice;
  std::string required;
  std::string section;
  std::string type;
  std::string options;
  std::string outfile;
  std::string FRDM;
  std::string version;
  std::string pwd;

  std::vector<std::pair<int,int> > r_process_data;
};

#endif  //INPUTS_H
