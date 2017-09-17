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
const int MAX_N=177;

const int KEY_YOFFSET=9;

enum class FileType {EPS, SVG, TIKZ};

class inputs
{
public:
  inputs();
  ~inputs();

  bool checkInputOptions(const std::map<std::string, std::string> &values);

  int readConsoleArguments(const std::vector<std::string> &options);
  int saveConsoleArguments();
  int processConsoleArguments();

  void constructFullyQualifiedPaths();
  void constructOutputFilename();
  void setCanvasSize();
  void setExtreme(const std::string &limit);
  void setTableYear(const int year);
  void showBanner() const;
  void showChartOptions() const;
  void showVersion() const;
  void showUsage(const std::string &exe) const;
  void writeOptionFile();
  void setFileType(const std::string &type);
  void setOutputFilename(const std::string &filename);
  void setInputOptions(const std::string &filename);

  int convertSymbolToZ(const std::string &el) const;
  std::string convertZToSymbol(const int Z) const;

  void readOptionFile(const std::string &inputFilename);

  FileType filetype;

  bool grid;
  bool magic_numbers;
  bool write_isotope;
  bool r_process;
  bool key;
  bool own_nuclei;
  bool AME;
  bool key_relative;
  bool valid_inputfile;

  int valid_console;
  int Zmin;
  int Zmax;
  int Nmin;
  int Nmax;
  int size;
  int experimental;
  int np_rich;
  int single_drip_lines;
  int double_drip_lines;
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
  std::string stringfile_type;
  std::string choice;
  std::string required;
  std::string section;
  std::string type;
  std::string options;
  std::string inputfile;
  std::string outfile;
  std::string FRDM;
  std::string version;
  std::string pwd;

  std::vector<std::pair<int,int> > r_process_data;
  std::map<std::string, std::string> arguments;
  std::map<std::string, std::string> inputfile_options;
};

#endif  //INPUTS_H
