///////////////////////////////////////////////////////////////
//                                                           //
// inch.cpp                                                  //
//                                                           //
///////////////////////////////////////////////////////////////
//                                                           //
// Description: This program will create a nuclear chart     //
//              containing the nuclei specified by the user, //
//              coloured by a user chosen property           //
//                                                           //
// Input:       Required - None                              //
//                                                           //
//              Optional - The name of the output file       //
//                       - An input file containing values   //
//                         used to create the chart          //
//                                                           //
//                                                           //
// Output:      An encapsulated postscript (eps) file        //
//              or a scalable vector graphics (svg) file     //
//              or a latex figure file (tikz) file           //
//                                                           //
///////////////////////////////////////////////////////////////

#include "include/inputs.h"
#include "include/nuclide.h"
#include "include/partition.h"
#include "include/functions.h"

#include "extractValue.cpp"

int main(int argc, char *argv[])
{
  static inputs *draw = new inputs;
  static partition *part = new partition;
  static std::vector<Nuclide> nuc;

  bool inputfile(false);
  std::vector<std::string> arguments(argv,argv+argc);

  printBanner(draw);

  printUsage(arguments);

  constructFilePaths(draw);

  constructFullyQualifiedPaths(draw);

  //--------------------------------
  //- Check and validate arguments -
  //--------------------------------
  validateInputArguments(nuc,draw,arguments,inputfile);

  //-------------------
  //- Read mass table -
  //-------------------
  if (readNUBASE(draw->mass_table_NUBASE,nuc))
    {
      std::cout << "Nuclear data has not been read, exiting..." << std::endl;
      exit(-1);
    }

  if (draw->AME)
    if (readAME(draw->mass_table_AME,nuc))
      std::cout << "Updated values from AME were not read." << std::endl;

  //----------------------------
  //- Read user defined nuclei -
  //----------------------------
  if (draw->own_nuclei)
    {
      if (readOWN(draw->my_nuclei,nuc))
	std::cout << "User defined nuclei have not been read." << std::endl;
    }
  else
    std::cout << "Not drawing any user selected nuclei" << std::endl;

  //================================================================================
  //
  // We now have a vector of nuclei, each element with members
  //
  //  int:    exp     = experiment(0) or theory/extrapolation(1)
  //          st      = state; gs=0, 1st isomer=1 etc
  //          A       = mass number
  //          Z       = proton number
  //          N       = neutron number
  //          pi      = +ve(0), -ve(1), unknown (2)
  //          pi_exp  = experimentally measured (0), extrapolated(1), unknown (2)
  //          J_exp   = experimentally measured (0), extrapolated(1), unknown (2)
  //          J_tent  = definitely assigned(0), tentatively(1), unknown (2)
  //          rich    = 'side' of the chart 6 (stable)
  //                                        2 (p_rich)
  //                                        3 (n_rich)
  //          own     = if user defined nuclei are plotted 0 (not in user file)
  //                                                       1 (in user file)
  //                                                       2 (user file is not used)
  //  float: NUBASE_ME  = mass excess from NUBASE
  //         NUBASE_dME = error on mass excess from NUBASE
  //         AME_ME     = mass excess from AME (optional)
  //         AME_dME    = error on mass excess from AME (optional)
  //         is_nrg  	= isomer energy
  //         dis_nrg 	= error on isomer energy
  //         J       	= spin of state (100 if unknown)
  //         hl      	= half-life of state (seconds)
  //
  //  string: symbol  = element symbol
  //          decay   = decay mode (gs only)
  //
  //================================================================================

  //-----------------------------------------
  //- Ask how the chart should be displayed -
  //-----------------------------------------
  if (!inputfile)
    displaySection(nuc,draw);

  std::cout << "\n===========================\n"
	    << "\nBetween Z = " << draw->Zmin << "(" << convertZToSymbol(draw->Zmin) << ") and Z = "
	    << draw->Zmax << "(" << convertZToSymbol(draw->Zmax) << ")";

  if (draw->section == "a" || (draw->section == "b" && draw->required == "a") )
    std::cout << ", with all relevant nuclei,\n";
  else if (draw->required == "b")
    std::cout << ", N = " << draw->Nmin << " and N = " << draw->Nmax << "\n";

  if      (draw->type == "a") std::cout << "experimentally measured";
  else if (draw->type == "b") std::cout << "theoretical/extrapolated";
  else                        std::cout << "both experimental and theoretical";

  std::cout << " values will be drawn and\nthe chart coloured by ";

  if      (draw->choice == "a") std::cout << "error on mass-excess\n";
  else if (draw->choice == "b") std::cout << "relative error on mass-excess\n";
  else if (draw->choice == "c") std::cout << "major ground-state decay mode\n";
  else if (draw->choice == "d") std::cout << "ground-state half-life\n";
  else                          std::cout << "first isomer energy\n";

  //-Define what colours and values will be used to differentiate the nuclei.
  setColours(part,draw);

  //-Define if a specific nuclei should be drawn and set what should be included in the key.
  showNuclei(nuc,part,draw);

  //-Use the drawn nuclei to decide how large the key should be.
  if (draw->key)
    setKeyScale(draw,part);
  else
    draw->key_scale=0;

  //-Set the size of the canvas
  if (draw->key_height*draw->key_scale > (draw->Zmax-draw->Zmin+2))
    {
      draw->key_relative=true;
      draw->chart_height=(draw->key_height+1.0)*draw->key_scale;
    }
  else
    draw->chart_height=(draw->Zmax-draw->Zmin+2);

  //HACK - When all nuclei are drawn, key is in top left.
  //Below stops extra space being created on the right.
  if (draw->section == "a" || (draw->Zmax-draw->Zmin) == 118)
    draw->chart_width=draw->Nmax-draw->Nmin+2;
  else
    draw->chart_width=draw->Nmax-draw->Nmin+2+14.5*draw->key_scale;

  //-------------------
  //- Write the chart -
  //-------------------
  std::cout << "\nCreating " << draw->outfile << "\n|--";

  if (draw->file_type == 0)
    writeEPS(nuc,draw,part);
  else if (draw->file_type == 1)
    writeSVG(nuc,draw,part);
  else if (draw->file_type == 2)
    writeTIKZ(nuc,draw,part);

  std::cout << "--| done\n" << std::endl;

  //-----------------------------------------------------
  //- Write chart parameters to file that can be resued -
  //-----------------------------------------------------
  std::ofstream opts(draw->options.c_str());

  if (opts)
    {
      std::cout << "Writing user choices to " << draw->options;

      opts << "section=" << draw->section << "\n";

      if (draw->section == "b")
	{
	  opts << "Zmin=" << draw->Zmin << "\n"
	       << "Zmax=" << draw->Zmax << "\n"
	       << "required=" << draw->required << "\n";

	  if (draw->required == "b")
	    opts << "Nmin=" << draw->Nmin << "\n"
		 << "Nmax=" << draw->Nmax << "\n";
	}

      opts << "type=" << draw->type << "\n"
	   << "choice=" << draw->choice << std::endl;
      opts.close();

      std::cout << " - done\n" << std::endl;
    }
  else
    std::cout << "\nERROR: Couldn't open " << draw->options << " to write the options.\n"
	      << "       Not creating any option file." << std::endl;

  std::cout << "Enjoy\n";

  if(!inputfile)
    std::cout << "\nTo run again with the same options: " << argv[0] << " -i options.in\n" << std::endl;

  delete draw;
  delete part;

  std::cout << std::endl;

  return 0;
}
