#ifndef INPUTS_HPP
#define INPUTS_HPP

#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <algorithm>

#include "symbolConverter.hpp"

const int MIN_Z=0;
const int MAX_Z=118;
const int MIN_N=0;
const int MAX_N=177;

const int KEY_YOFFSET=9;

enum class FileType {EPS, SVG, TIKZ};

class inputs: public SymbolConverter
{
public:
  //Constructors
  //default
  inputs();
  //copy
  inputs(const inputs&) = default;
  //move
  inputs(inputs&&) = default;

  //Assignment
  //copy
  inputs& operator=(const inputs&) = default;
  //move
  inputs& operator=(inputs&&) = default;

  //Destructors
  ~inputs() override = default;

  bool checkInputOptions(const std::map<std::string, std::string> &values);

  int readConsoleArguments(const std::vector<std::string> &console_options);
  int saveConsoleArguments();
  int processConsoleArguments();

  void constructFullyQualifiedPaths();
  void constructOutputFilename();
  void setCanvasSize();
  void setExtreme(const std::string &limit);
  void showBanner() const;
  void showChartOptions() const;
  void showVersion() const;
  void showUsage(const std::string &exe) const;
  void writeOptionFile();
  void setFileType(const std::string &file_type);
  void setOutputFilename(const std::string &filename);
  void setInputOptions(const std::string &filename);

  void readOptionFile(const std::string &inputFilename);
  bool readRprocessData();

  FileType filetype = FileType::EPS;

  bool grid = false;
  bool magic_numbers = true;
  bool write_isotope = true;
  bool r_process = true;
  bool key = true;
  bool own_nuclei = true;
  bool AME = false;
  bool key_relative = true;
  bool valid_inputfile = false;

  int valid_console = 1;
  int Zmin = MAX_Z;
  int Zmax = MIN_Z;
  int Nmin = MAX_N;
  int Nmax = MIN_N;
  int experimental = 0;
  // 1=all, 2=p-rich and stable, 3=n-rich and stable, 6=stable only
  int np_rich = 1;
  // 0=none, 1=both, 2=p-only, 3=n-only
  int single_drip_lines = 1;
  int double_drip_lines = 1;
  //3,12,16
  int year = 3;
  //-----------------------------------------------------------------------
  //- If the file is to be converted into some other format (eg.jpg,png), -
  //- without any resizing, and the whole chart is drawn. The following  --
  //- values are the largest allowed for the given page size. -------------
  //-----------------------------------------------------------------------
  // A0 -> size = 18
  // A1 -> size = 13
  // A2 -> size =  9
  // A3 -> size =  6
  // A4 -> size =  4
  // A5 -> size =  3
  //-----------------
  int size = 4;

  double curve = 0.25;
  double key_height = 0.5;
  double key_scale = 0.0;
  double chart_height = 0.0;
  double chart_width = 0.0;

  std::string path = "./";
  std::string mass_table = "";
  std::string mass_table_AME = "mass.mas114";
  std::string mass_table_NUBASE = "nubtab03.asc";
  std::string my_nuclei = "my_nuclei.dat";
  std::string r_proc_path = "r-process.dat";
  std::string neutron_drip = "neutron.drip";
  std::string proton_drip = "proton.drip";
  std::string two_neutron_drip = "2neutron.drip";
  std::string two_proton_drip = "2proton.drip";
  std::string options = "options.in";
  // Without extension, this is added in the code
  std::string outfile = "chart";
  std::string FRDM = "FRLDM_ME.tbl";
  std::string version = "0.9.7";
  std::string pwd;
  std::string stringfile_type;
  std::string choice;
  std::string required;
  std::string section;
  std::string type;
  std::string inputfile;

  std::vector<std::pair<int,int> > r_process_data;
  std::map<std::string, std::string> arguments;
  std::map<std::string, std::string> inputfile_options;
};

#endif // INPUTS_HPP
