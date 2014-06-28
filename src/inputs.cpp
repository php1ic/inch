#include "include/inputs.h"

#include <iostream>
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

inputs::inputs():
  //bool
  grid(false),
  magic_numbers(true),
  write_isotope(true),
  r_process(true),
  key(true),
  own_nuclei(true),
  AME(false),
  key_relative(true),                  // Not an option, just default value
  //int
  Zmin(200),
  Zmax(0),
  Nmin(200),
  Nmax(0),
  size(4),                             // See comment above
  experimental(0),
  np_rich(1),                          // 1=all, 2=p-rich and stable, 3=n-rich and stable, 6=stable only
  single_drip_lines(1),                // 0=none, 1=both, 2=p-only, 3=n-only
  double_drip_lines(1),
  file_type(0),                        // 0=eps, 1=svg, 2=tikz
  //double
  curve(0.25),
  key_height(0.5),
  key_scale(0.0),
  chart_height(0.0),
  chart_width(0.0),
  //std::string
  path("./"),
  mass_table(""),
  mass_table_AME("mass.mas114"),
  mass_table_NUBASE("nubtab03.asc"),
  my_nuclei("my_nuclei.dat"),
  r_proc_path("r-process.dat"),
  neutron_drip("neutron.drip"),
  proton_drip("proton.drip"),
  two_neutron_drip("2neutron.drip"),
  two_proton_drip("2proton.drip"),
  choice(""),
  required(""),
  section(""),
  type(""),
  options("options.in"),
  outfile("chart"),                    // Without extension, this is added in the code
  FRDM("FRLDM_ME.tbl"),
  version("0.9.7"),
  pwd("")
{
  constructFilePaths();

  constructFullyQualifiedPaths();
}

inputs::~inputs()
{
}

void inputs::constructFilePaths()
{
  pwd = getenv("PWD");
  pwd.append("/");

  path = LOCAL_PATH;
  path.append("/");

  path.append("data_files/");

  std::cout << "\nSetting the path to the required files as:\n"
	    << path << "\n" << std::endl;
}

void inputs::constructFullyQualifiedPaths()
{
  FRDM.insert(0,path);

  my_nuclei.insert(0,path);

  mass_table_AME.insert(0,path);

  mass_table_NUBASE.insert(0,path);

  if (AME)
    mass_table = mass_table_AME;
  else
    mass_table = mass_table_NUBASE;

  neutron_drip.insert(0,path);

  two_neutron_drip.insert(0,path);

  proton_drip.insert(0,path);

  two_proton_drip.insert(0,path);

  options.insert(0,pwd);
}
