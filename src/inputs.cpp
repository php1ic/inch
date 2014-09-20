#include "include/inputs.h"

/*N.B.
inputs::convertZToSymbol() and inputs::convertSymbolToZ() definitions are in their own files.
 */


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
  pwd(""),
  r_process_data()
{
}


inputs::~inputs()
{
}


void inputs::showVersion()
{
  std::cout << "Interactive Nuclear CHart (INCH) version " << version << "\n"
	    << "Copyright (C) 2014 Me.\n"
	    << "INCH comes with ABSOLUTELY NO WARRANTY.\n"
	    << "You may redistribute copies if INCH\n"
	    << "under the terms of the GNU General Public License\n"
	    << "For more information about these matters, see the file named COPYING."
	    << std::endl;
}


void inputs::showBanner()
{
  std::cout << "\n"
	    << "	 +---+---+---+---+---+---+---+\n"
	    << "	 |In |Te |Ra | C |Ti | V | E |\n"
	    << "	 +---+---+---+---+---+---+---+\n"
	    << "	     | N | U |Cl | E |Ar |\n"
	    << "	 +---+---+---+---+---+---+\n"
	    << "	 | C | H |Ar | T |\n"
	    << "	 +---+---+---+---v" << version << "\n"
	    << "\n"
	    << "  USAGE: inch\n"
	    << "     OR: inch -i <input_file>\n"
	    << "     OR: inch -o <outfile without extension>\n"
	    << "     OR: inch -i <input_file> -o <outfile without extension>\n"
	    << std::endl;
}


void inputs::constructFullyQualifiedPaths()
{
  pwd = getenv("PWD");
  pwd.append("/");

  path = LOCAL_PATH;
  path.append("/data_files/");

  std::cout << "\nSetting the path to the required files as:\n"
	    << path << "\n" << std::endl;

  FRDM.insert(0,path);

  my_nuclei.insert(0,path);

  mass_table_AME.insert(0,path);

  mass_table_NUBASE.insert(0,path);

  mass_table = AME ? mass_table_AME : mass_table_NUBASE;

  neutron_drip.insert(0,path);

  two_neutron_drip.insert(0,path);

  proton_drip.insert(0,path);

  two_proton_drip.insert(0,path);

  r_proc_path.insert(0,path);

  options.insert(0,pwd);
}


bool inputs::checkInputOptions(std::map<std::string, std::string> &values)
{
  std::map<std::string, std::string>::iterator it;
  int linesRead=0;

  for (it=values.begin(); it!=values.end(); ++it)
    {
      if (it->first == "section")
	{
	  section=it->second;
	  linesRead++;
	}
      else if (it->first == "type")
	{
	  type=it->second;

	  if (type == "a")
	    {
	      experimental=1;
	      linesRead++;
	    }
	  else if (type == "b")
	    {
	      experimental=0;
	      linesRead++;
	    }
	  else if (type == "c")
	    {
	      experimental=2;
	      linesRead++;
	    }
	  else
	    {
	      std::cout << "***ERROR***: " << type
			<< " is not a valid choice for 'type'" << std::endl;
	    }
	}
      else if (it->first == "choice")
	{
	  choice=it->second;
	  linesRead++;
	}
      else if (it->first == "required")
	{
	  required=it->second;
	}
      else if (it->first == "Zmin")
	{
	  Zmin=atoi(it->second.c_str());

	  if (   (!atoi(it->second.c_str()) && it->second!="0")
	      || (Zmin<MIN_Z || Zmin>MAX_Z)
	      )
	    {
	      std::cout << "***ERROR***: " << it->second
			<< " is not a valid choice for 'Zmin'" << std::endl;
	      return false;
	    }
	  else
	    linesRead++;
	}
      else if (it->first == "Zmax")
	{
	  Zmax=atoi(it->second.c_str());

	  if (   (!atoi(it->second.c_str()) && it->second!="0")
	      || (Zmax<MIN_Z || Zmax>MAX_Z)
	      )
	    {
	      std::cout << "***ERROR***: " << it->second
			<< " is not a valid choice for 'Zmax'" << std::endl;
	      return false;
	    }
	  else
	    linesRead++;
	}
      else if (it->first == "Nmin")
	{
	  Nmin=atoi(it->second.c_str());

	  if (   (!atoi(it->second.c_str()) && it->second!="0")
	      || (Nmin<MIN_N || Nmin>MAX_N)
	      )
	    {
	      std::cout << "***ERROR***: " << it->second
			<< " is not a valid choice for 'Nmin'" << std::endl;
	      return false;
	    }
	  else
	    linesRead++;
	}
      else if (it->first == "Nmax")
	{
	  Nmax=atoi(it->second.c_str());

	  if (   (!atoi(it->second.c_str()) && it->second!="0")
	      || (Nmax<MIN_N || Nmax>MAX_N)
	      )
	    {
	      std::cout << "***ERROR***: " << it->second
			<< " is not a valid choice for 'Nmax'" << std::endl;
	      return false;
	    }
	  else
	    linesRead++;
	}
      else
	{
	  std::cout << "**WARNING**: " << it->first
		    <<" is not a valid option. Ignoring." << std::endl;
	}
    }

  if (linesRead < 3)
    {
      std::cout << "Not enough inputs have been read from the file." << std::endl;
      return false;
    }

  return true;
}


void inputs::setCanvasSize()
{
  if (key_height*key_scale > (Zmax-Zmin+2))
    {
      key_relative=true;
      chart_height=(key_height+1.0)*key_scale;
    }
  else
    chart_height=Zmax-Zmin+2;

  //HACKS
  //- When all nuclei are drawn, key is in top left.
  //Below stops extra space being created on the right.
  //- 14.5*key_scale extends the width to fit the widest key
  //this should really be set as a function of the variable
  //used to colour the isotopes. Either way, this cannot be
  //set dynamically in the file so we need to use 'magic numbers'
  if (section == "a" || (Zmax-Zmin) == MAX_Z)
    chart_width=Nmax-Nmin+2;
  else
    chart_width=Nmax-Nmin+2 + 14.5*key_scale;
}


void inputs::setExtreme(const std::string &limit)
{
  if (limit != "Zmin" && limit != "Zmax" && limit != "Nmin" && limit != "Nmax")
    {
      std::cout << "**WARNING** - " << limit << " is not a valid input\n"
		<< "              choose either Zmin, Zmax, Nmin or Nmax\n"
		<< "Setting limits to maxima values." << std::endl;

      Zmin=MIN_Z;
      Zmax=MAX_Z;
      Nmin=MIN_N;
      Nmax=MAX_N;

      return;
    }

  bool valid=false;

  do
    {
      int number=0;
      std::string in;

      std::cout << limit << ": ";
      std::cin >> in;

      //Read the entered value and convert symbol->Z if necessary
      if (in == "0")
	number = 0;
      else
	{
	  if (limit.at(0) == 'Z')
	    number = atoi(in.c_str()) ? atoi(in.c_str()) : convertSymbolToZ(in);
	  else if ( !atoi(in.c_str()) )
	    number = -1;
	  else
	    number = atoi(in.c_str());
	}

      //Validate the number
      if (limit == "Zmin")
	{
	  Zmin = number;
	  valid=true;
	  if (Zmin < MIN_Z || Zmin > MAX_Z)
	    {
	      std::cout << "\n"
			<< "Zmin must be in the range " << MIN_Z << "<Z<" << MAX_Z
			<< "\n" << std::endl;
	      valid=false;
	    }
	}
      else if (limit == "Zmax")
	{
	  Zmax = number;
	  valid=true;
	  if (Zmax < Zmin || Zmax > MAX_Z)
	    {
	      std::cout << "\n"
			<<"Zmax must be in the range " << Zmin << "<Z<" << MAX_Z
			<< "\n" << std::endl;
	      valid=false;
	    }
	}
      else if (limit == "Nmin")
	{
	  Nmin = number;
	  valid=true;
	  if (Nmin < MIN_N || Nmin > MAX_N)
	    {
	      std::cout << "\n"
			<< "Nmin must be in the range " << MIN_N << "<N<" << MAX_N
			<< "\n" << std::endl;
	      valid=false;
	    }
	}
      else if (limit == "Nmax")
	{
	  Nmax = number;
	  valid=true;
	  if (Nmax < Nmin || Nmax > MAX_N)
	    {
	      std::cout << "\n"
			<< "Nmax must be in the range " << Nmin << "<N<" << MAX_N
			<< "\n" << std::endl;
	      valid=false;
	    }
	}
    }
  while (!valid);
}


void inputs::showChartOptions()
{
  std::cout << "===========================\n"
	    << "\nBetween Z = " << Zmin << "(" << convertZToSymbol(Zmin)
	    << ") and Z = " << Zmax << "(" << convertZToSymbol(Zmax) << ")";

  if (section == "a" || (section == "b" && required == "a") )
    std::cout << ", with all relevant nuclei,\n";
  else if (required == "b")
    std::cout << ", N = " << Nmin << " and N = " << Nmax << "\n";

  if      (type == "a") std::cout << "experimentally measured";
  else if (type == "b") std::cout << "theoretical/extrapolated";
  else                  std::cout << "both experimental and theoretical";

  std::cout << " values will be drawn and\nthe chart coloured by ";

  if      (choice == "a") std::cout << "error on mass-excess\n";
  else if (choice == "b") std::cout << "relative error on mass-excess\n";
  else if (choice == "c") std::cout << "major ground-state decay mode\n";
  else if (choice == "d") std::cout << "ground-state half-life\n";
  else                    std::cout << "first isomer energy\n";
}


void inputs::constructOutputFilename()
{
  //-Remove the extension if given
  if (   outfile.find(".") == outfile.length()-4
      && outfile.find(".") != std::string::npos
      )
    {
      std::cout << "\nThe extension is added depending on the chosen file type\n";

      outfile.erase(outfile.rfind("."),4);
    }

  //-Remove the CWD if given
  if ( outfile.find(pwd) != std::string::npos )
    {
      std::cout << "\nThe current working directory is added\n";

      outfile.erase(0,outfile.rfind("/")+1);
    }

  //-Check input file is not a directory.
  if (   outfile.empty()
      || outfile.at(outfile.length()-1) == '/'
      )
    {
      std::cout << "\n"
		<< "***ERROR***: "
		<< outfile << " is a directory, can't use that as a file name\n"
		<< std::endl;
      exit(-1);
    }
  else
    {
      std::cout << "Using \"" << outfile << "\" as the base of the file name." << std::endl;

      //-Add the necessary extension
      if (file_type == 0)
	outfile.append(".eps");
      else if (file_type == 1)
	outfile.append(".svg");
      else if (file_type == 2)
	outfile.append(".tex");
    }
}


void inputs::writeOptionFile()
{
  std::ofstream opts(options.c_str());

  if (opts.is_open())
    {
      std::cout << "Writing user choices to " << options;

      opts << "section=" << section << "\n";

      if (section == "b")
	{
	  opts << "Zmin=" << Zmin << "\n"
	       << "Zmax=" << Zmax << "\n"
	       << "required=" << required << "\n";

	  if (required == "b")
	    opts << "Nmin=" << Nmin << "\n"
		 << "Nmax=" << Nmax << "\n";
	}

      opts << "type=" << type << "\n"
	   << "choice=" << choice << std::endl;

      opts.close();

      std::cout << " - done\n" << std::endl;
    }
  else
    std::cout << "\n"
	      << "***ERROR***: Couldn't open " << options << " to write the options.\n"
	      << "             Not creating an option file." << std::endl;
}
