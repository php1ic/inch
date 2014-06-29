#ifndef INPUTS_H
#define INPUTS_H

#include <string>
#include <iostream>
#include <fstream>
#include <map>

const int MIN_Z=0;
const int MAX_Z=118;
const int MIN_N=0;
const int MAX_N=176;


class inputs
{
private:
  void constructFilePaths();
  void constructFullyQualifiedPaths();

public:
  inputs();
  ~inputs();

  void constructOutputFilename();

  bool checkInputOptions(std::map<std::string, std::string> &);
  void showVersion();
  void showBanner();
  void showChartOptions();
  void writeOptionFile();

  bool
    grid,
    magic_numbers,
    write_isotope,
    r_process,
    key,
    own_nuclei,
    AME,
    key_relative;

  int
    Zmin,
    Zmax,
    Nmin,
    Nmax,
    size,
    experimental,
    np_rich,
    single_drip_lines,
    double_drip_lines,
    file_type;

  double
    curve,
    key_height,
    key_scale,
    chart_height,
    chart_width;

  std::string
    path,
    mass_table,
    mass_table_AME,
    mass_table_NUBASE,
    my_nuclei,
    r_proc_path,
    neutron_drip,
    proton_drip,
    two_neutron_drip,
    two_proton_drip,
    choice,
    required,
    section,
    type,
    options,
    outfile,
    FRDM,
    version,
    pwd,
    ZminSymbol,
    ZmaxSymbol;
};

#endif
