#include "include/inputs.h"

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
  grid(0),
  magic_numbers(1),
  write_isotope(1),
  r_process(1),
  key(1),
  own_nuclei(1),
  AME(1),
  key_relative(0),
  //int
  Zmin(200),
  Zmax(0),
  Nmin(200),
  Nmax(0),
  size(6),                             // See comment above
  experimental(0),
  np_rich(1),                          // 1=all, 2=p-rich and stable, 3=n-rich and stable, 6=stable only
  single_drip_lines(1),                // 0=none, 1=both, 2=p-only, 3=n-only
  double_drip_lines(1),
  file_type(0),                        // 0=eps, 1=svg, 2=tikz
  //float
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
  version("1.9b"),
  pwd("")
{
}

inputs::~inputs()
{
}
