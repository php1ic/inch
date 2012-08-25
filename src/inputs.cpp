#include "inputs.h"

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

inputs::inputs()
{
  Zmax=Nmax=0;
  Zmin=Nmin=200;

  e_or_t=0;

  size       = 3;  // See comment above

  drip_lines = 1;  // 0=none, 1=both, 2=p-only, 3=n-only
  np_rich    = 1;  // 1=all, 2=p-rich and stable, 3=n-rich and stable, 6=stable only

  // off/on - 0/1
  grid          = 0;
  magic_numbers = 0;
  write_isotope = 1;
  r_process     = 1;
  key           = 0;
  own_nuclei    = 1;
  AME           = 0;

  curve = 0.25;  // 0->1=square->circle

  choice=required=section=type="";
  options=outfile = "";

  path              = "./";
  mass_table_NUBASE = "nubtab03.asc";
  mass_table_AME    = "mass.mas114";
  r_proc_path       = "r-process.dat";
  proton_drip       = "proton.drip";
  neutron_drip      = "neutron.drip";

  my_nuclei         = "my_nuclei.dat";

  
  outfile 	    = "chart"; // Without extension, this is added in the code
  file_type         = 0;       // 0=eps, 1=svg;

  options 	    = "options.in";
}

inputs::~inputs()
{
}
