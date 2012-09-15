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
//                                                           //
///////////////////////////////////////////////////////////////

#include "inputs.h"
#include "nuclide.h"
#include "functions.h"

#include "extractValue.cpp"

int main(int argc, char *argv[])
{
  static inputs *draw = new inputs;
  static std::vector<Nuclide> nuc;
  std::vector<Nuclide>::iterator nuc_it;
  unsigned short i;

  std::cout << "\n"
	    << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
	    << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
	    << " ~~                                              ~~\n"
	    << " ~~   Welcome to the Interactive Nuclear CHart   ~~\n"
	    << " ~~                 It is great                  ~~\n"
	    << " ~~               You are rubbish                ~~\n"
	    << " ~~        The Creator Ian Cullen is OK          ~~\n"
	    << " ~~                                              ~~\n"
	    << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
	    << " " << std::setfill('~') << std::setw(50) << draw->version << "\n"
	    << "\n"
	    << "  USAGE: " << argv[0] << "\n"
	    << "    OR:  " << argv[0] << " -i <input_file>\n"
	    << "    OR:  " << argv[0] << " -o <outfile without extension>\n"
	    << "    OR:  " << argv[0] << " -i <input_file> -o <outfile without extension>\n" << std::endl;

  std::string pwd = getenv("PWD");
  pwd.append("/");

  draw->path = LOCAL_PATH;
  draw->path.append("/");

  std::string input_data="data_files/";
  draw->path = draw->path.append(input_data);

  std::cout << "\nSetting the path to the required files as:\n"
	    << draw->path << "\n" << std::endl;

  draw->mass_table_AME.insert(0,draw->path);

  draw->mass_table_NUBASE.insert(0,draw->path);

  if (draw->AME)
    draw->mass_table = draw->mass_table_AME;
  else
    draw->mass_table = draw->mass_table_NUBASE;

  struct stat mass_t;

  if (stat(draw->mass_table_NUBASE.c_str(),&mass_t))
    {
      std::cout << "\nERROR: Mass table " << draw->mass_table_NUBASE << " couldn't be opened.\n"
		<< "\nExiting...\n" << std::endl;
      exit(-1);
    }

  if (draw->AME && stat(draw->mass_table_AME.c_str(),&mass_t))
    {
      std::cout << "\nERROR: Mass table " << draw->mass_table_AME << " couldn't be opened.\n"
		<< "\nExiting...\n" << std::endl;
      exit(-1);
    }

  //-------------------
  //- Read mass table -
  //-------------------
  std::cout << "Reading "
	    << draw->mass_table_NUBASE.substr(draw->path.length(),draw->mass_table_NUBASE.length()-draw->path.length())
	    << " for nuclear values <--";

  readNUBASE(draw->mass_table_NUBASE,nuc);

  if (draw->AME)
    {
      std::cout << "\nReading "
		<< draw->mass_table_AME.substr(draw->path.length(),draw->mass_table_AME.length()-draw->path.length())
		<< " for newer mass excess data [--";

      readAME(draw->mass_table_AME,nuc);

      std::cout << "--] updated\n";
    }

  std::cout << "--> done" << std::endl;

  //-----------------------------
  //- Check and valid arguments -
  //-----------------------------
  short arguments=argc;
  bool inputfile=false;

  if (arguments > 5)
    {
      char ignore;

      std::cout << "WARNING: Too many arguments given.\n"
		<< "Ignoring: ";

      for (i=5;i<arguments;i++)
	std::cout << argv[i] << " ";

      std::cout << "\nContinue ";

      do
	{
	  std::cout << "[y/n]: ";
	  std::cin  >> ignore;

	  if (ignore == 'y')
	    {
	      std::cout << "\nContinuing..." << std::endl;
	    }
	  else if (ignore == 'n')
	    {
	      std::cout << "\nExiting...\n" << std::endl;
	      exit(-1);
	    }
	  else
	    std::cout << "That wasn't y or n. Try again" << std::endl;
	}
      while (ignore != 'n' && ignore !='y');

      arguments=5;
    }

  if (arguments%2 != 1)
    {
      std::cout << "\n\n"
		<< "  ERROR: An odd number of arguments is not allowed\n\n"
		<< "  USAGE: " << argv[0] << "\n"
		<< "    OR:  " << argv[0] << " -i <input_file>\n"
		<< "    OR:  " << argv[0] << " -o <outfile without extension>\n"
		<< "    OR:  " << argv[0] << " -i <input_file> -o <outfile without extension>\n\n" << std::endl;

      exit(-1);
    }
  else if (arguments == 1)
    {
      draw->outfile.insert(0,pwd);

      // Append necessary file extension
      if (draw->file_type == 0)
	draw->outfile.append(".eps");
      else if (draw->file_type == 1)
	draw->outfile.append(".svg");
    }
  else if (arguments != 1)
    {
      for (i=1;i<arguments;i++)
	{
	  if (!strcmp(argv[i],"-i"))
	    {
	      inputfile=true;

	      std::ifstream infile(argv[i+1]);

	      if (infile.is_open())
		{
		  std::cout << "Reading " << argv[i+1] << " for input values {--";

		  char zx[9];
		  short ax=0,lines_read=0;
		  std::string *line = new std::string;
		  std::string temp;

		  while (getline(infile,*line))
		    {
		      if (line->find("#") == std::string::npos)
			{
			  if (line->find("section=") != std::string::npos)
			    {
			      draw->section=line->erase(0,line->size()-1);
			      lines_read++;
			    }
			  else if (line->find("type=") != std::string::npos)
			    {
			      draw->type=line->erase(0,line->size()-1);

			      if (draw->type == "a")
				{
				  draw->e_or_t=1;
				  lines_read++;
				}
			      else if (draw->type == "b")
				{
				  draw->e_or_t=0;
				  lines_read++;
				}
			      else if (draw->type == "c")
				{
				  draw->e_or_t=2;
				  lines_read++;
				}
			      else
				std::cout << "\nERROR: " << draw->type << " is not a valid choice for 'type'"<< std::endl;
			    }
			  else if (line->find("choice=") != std::string::npos)
			    {
			      draw->choice=line->erase(0,line->size()-1);
			      lines_read++;
			    }
			  else if (line->find("required=") != std::string::npos)
			    {
			      draw->required=line->erase(0,line->size()-1);
			      lines_read++;
			    }
			  else if (line->find("Zmin=") != std::string::npos)
			    {
			      temp=*line;
			      ax=line->erase(0,line->find('=')+1).size();
			      line->copy(zx,ax,0);
			      zx[ax] = '\0';
			      draw->Zmin=atoi(zx);

			      if ((!atoi(zx) && temp!="Zmin=0") || (draw->Zmin<0 && draw->Zmin>118))
				{
				  std::cout << "\nERROR: " << zx << " is not a valid choice for 'Zmin'" << std::endl;
				  inputfile=false;
				}
			      else
				lines_read++;
			    }
			  else if (line->find("Zmax=") != std::string::npos)
			    {
			      temp=*line;
			      ax=line->erase(0,line->find('=')+1).size();
			      line->copy(zx,ax,0);
			      zx[ax] = '\0';
			      draw->Zmax=atoi(zx);

			      if ((!atoi(zx) && temp!="Zmax=0") || (draw->Zmax<0 && draw->Zmax>118))
				{
				  std::cout << "\nERROR: " << zx << " is not a valid choice for 'Zmax'" << std::endl;
				  inputfile=false;
				}
			      else
				lines_read++;
			    }
			  else if (line->find("Nmin=") != std::string::npos)
			    {
			      temp=*line;
			      ax=line->erase(0,line->find('=')+1).size();
			      line->copy(zx,ax,0);
			      zx[ax] = '\0';
			      draw->Nmin=atoi(zx);

			      //extractValue(&line->erase(0,line->find('=')+1),0,line->length(),draw->Nmin);
			      if ((!atoi(zx) && temp!="Nmin=0") || (draw->Nmin<0 && draw->Nmin>176))
				{
				  std::cout << "\nERROR: " << zx << " is not a valid choice for 'Nmin'" << std::endl;
				  inputfile=false;
				}
			      else
				lines_read++;
			    }
			  else if (line->find("Nmax=") != std::string::npos)
			    {
			      temp=*line;
			      ax=line->erase(0,line->find('=')+1).size();
			      line->copy(zx,ax,0);
			      zx[ax] = '\0';
			      draw->Nmax=atoi(zx);

			      //extractValue(&line->erase(0,line->find('=')+1),0,line->length(),draw->Nmax);
			      if ((!atoi(zx) && temp!="Nmax=0") || (draw->Nmax<0 && draw->Nmax>176))
			      {
				  std::cout << "\nERROR: " << zx << " is not a valid choice for 'Nmax'" << std::endl;
				  inputfile=false;
				}
			      else
				lines_read++;
			    }
			  else
			    {
			      std::cout << "\nWARNING: " << line << " is not a valid input line. Ignoring." << std::endl;
			    }
			}
		    }
		  infile.close();
		  delete line;

		  bool output=false;
		  for (int j=1; j<arguments; j++)
		    {
		      if ((strcmp(argv[i],"-o")))
			output=true;
		    }

		  if (output)
		    {
		      draw->outfile.insert(0,pwd);

		      if (draw->file_type == 0)
			draw->outfile.append(".eps");
		      else if (draw->file_type == 1)
			draw->outfile.append(".svg");
		    }

		  if (lines_read < 3)
		    {
		      std::cout << "Not enough inputs have been read from the file." << std::endl;
		      inputfile=false;
		    }

		  if (inputfile)
		    {
		      if (draw->section == "a")
			{
			  if (draw->Zmin != 200 && draw->Zmax != 0)
			    std::cout << "\n**WARNING**\nThe option file contains a Z range but specifies that all nuclei should be drawn.\n"
				      << "The input range will be ignored, set section=b to select a range in Z.\n"
				      << "***********\n" << std::endl;

			  draw->Zmin=0;
			  draw->Zmax=118;
			  draw->Nmin=0;
			  draw->Nmax=176;
			}
		      else if (draw->section == "b")
			{
			  if (draw->required == "a")
			    setNlimits(nuc,draw);
			  else if (draw->required != "b")
			    {
			      std::cout << "\nERROR: " << draw->required << " is not a valid option for the 'required' field.\n"
					<< "       Ignoring input file.\n" << std::endl;
			      inputfile=false;
			    }

			  if (draw->Zmin>draw->Zmax)
			    {
			      std::cout << "\nERROR: Zmax(" << draw->Zmax << ") cannot be less than Zmin(" << draw->Zmin<< ")\n"
					<< "       Ignoring input file.\n" << std::endl;
			      inputfile=false;
			    }

			  if (draw->Nmin>draw->Nmax)
			    {
			      std::cout << "\nERROR: Nmax(" << draw->Nmax << ") cannot be less than Nmin(" << draw->Nmin<< ")\n"
					<< "       Ignoring input file.\n" << std::endl;
			      inputfile=false;
			    }
			}
		      else
			{
			  std::cout << "\nERROR: " << draw->section << " is not a valid option for the 'section' field.\n"
				    << "       Ignoring input file.\n" << std::endl;
			  inputfile=false;
			}

		      if (draw->type != "a" && draw->type != "b" && draw->type != "c")
			{
			  std::cout << "\nERROR: " << draw->type << " is not a valid option for the 'section' field.\n"
				    << "       Ignoring input file.\n" << std::endl;
			  inputfile=false;
			}

		      if (draw->choice != "a" && draw->choice != "b" && draw->choice != "c" && draw->choice != "d" && draw->choice != "e")
			{
			  std::cout << "\nERROR: " << draw->type << " is not a valid option for the 'section' field.\n"
				    << "       Ignoring input file.\n" << std::endl;
			  inputfile=false;
			}

		      if (inputfile)
			{
			  std::cout << "--} done\n"
				    <<"Read values:\n";

			  std::cout << "section: " << draw->section  << "\n";

			  if (draw->section == "b")
			    {
			      std::cout << "Zmin: "     << draw->Zmin << "\n"
					<< "Zmax: "     << draw->Zmax << "\n"
					<< "required: " << draw->required << "\n"
					<< "Nmin: "     << draw->Nmin << "\n"
					<< "Nmin: "     << draw->Nmax << "\n";
			    }

			  std::cout << "type: " << draw->type << "\n"
				    << "choice: " << draw->choice << std::endl;
			}
		    }
		}
	      else
		{
		  std::cout << "\n\nERROR: " << argv[i+1] << " couldn't be opened, does it exist?\n" << std::endl;
		  inputfile=false;
		}
	    }

	  if (!strcmp(argv[i],"-o"))
	    {
	      short f=0;
	      bool r=false;
	      char replace, rereplace;
	      draw->outfile = argv[i+1];

	      if (   draw->outfile.find(".") == draw->outfile.length()-4
		  && draw->outfile.find(".") != std::string::npos
		  )
		{
		  std::cout << "\nThe extension is added depending on the chosen file type\n";

		  draw->outfile.erase(draw->outfile.rfind("."),4);

		  std::cout << "Using " << draw->outfile << " as the base of the file name." << std::endl;
		}

	      draw->outfile.insert(0,pwd);

	      if (draw->file_type == 0)
		draw->outfile.append(".eps");
	      else if (draw->file_type == 1)
		draw->outfile.append(".svg");

	      if (   draw->path+draw->outfile == draw->mass_table_AME
		  || draw->path+draw->outfile == draw->mass_table_NUBASE
		  )
		{
		  std::cout << "\nERROR: You can't overwrite the mass table(s) you are using\n"
			    << "\n" << draw->mass_table_NUBASE << " is ALWAYS read on execution and\n"
			    << draw->mass_table_AME << " is read if the relevant option is chosen.\n"
			    << "\nExiting...\n" << std::endl;
		  exit(-1);
		}

	      struct stat out;

	      if (!stat(draw->outfile.c_str(),&out))
		{
		  std::cout << "\nWARNING: The file " << argv[i+1];

		  if (draw->file_type == 0)
		    std::cout << ".eps already exists.\nOverwrite ";
		  else if (draw->file_type == 1)
		    std::cout << ".svg already exists.\nOverwrite ";

		  do
		    {
		      std::cout << "[y/n]: ";
		      std::cin  >> replace;

		      if (replace == 'y')
			std::cout << "\n" << argv[i+1] << " will be overwritten\n" << std::endl;
		      else if (replace == 'n')
			{
			  do
			    {
			      std::cout << "New filename (without extension): ";
			      std::cin  >> draw->outfile;

			       draw->outfile.insert(0,pwd);

			       if (draw->file_type == 0)
				 draw->outfile.append(".eps");
			       else if (draw->file_type == 1)
				 draw->outfile.append(".svg");

			      if (   draw->path+draw->outfile == draw->mass_table_AME
				  || draw->path+draw->outfile == draw->mass_table_NUBASE
				  )
				{
				  std::cout << "Writing over the mass table: " << draw->outfile
					    << " is not a good idea" << std::endl;
				}
			      else if (!stat(draw->outfile.c_str(),&out))
				{
				  std::cout << "This file also exists!" << std::endl;

				  do
				    {
				      std::cout << "Overwrite this file [y/n]: ";
				      std::cin  >> rereplace;

				      if (rereplace == 'y')
					{
					  r=true;
					  std::cout << "\nWill write chart to " << draw->outfile << "\n" << std::endl;
					}
				      else if (rereplace != 'y' && rereplace != 'n')
					std::cout << "That wasn't y or n. Try again" << std::endl;
				    }
				  while (rereplace != 'y' && rereplace != 'n' && !r);
				}
			      else
				std::cout << "\nWill write chart to " << draw->outfile << "\n" << std::endl;
			    }
			  while (   draw->outfile != draw->mass_table_AME
				 && draw->outfile != draw->mass_table_NUBASE
				 && !stat(draw->outfile.c_str(),&out)
				 && !r
				 );
			}
		      else
			{
			  if (f>1)
			    {
			      std::cout << "\n\nThere are 2 options, you've chosen neither on 3 occasions.\n\n"
					<< argv[0] << " file.eps < options.in\n"
					<< "\nUses BASH shell functionality so currently creates an infinite loop.\n"
					<< "Watch this space for the implementation of an input file.\n" << std::endl;
			      exit(-1);
			    }

			  std::cout << "That wasn't y or n. Try again" << std::endl;
			  f++;
			}
		    }
		  while (replace != 'y' && replace != 'n');
		}
	      else
		std::cout << "Will write chart to " << draw->outfile << "\n" << std::endl;
	    }
	}
    }

  //----------------------------
  //- Read user defined nuclei -
  //----------------------------
  if (draw->own_nuclei)
    {
      draw->my_nuclei.insert(0,draw->path);

      std::cout << "Reading "
		<< draw->my_nuclei.substr(draw->path.length(),draw->my_nuclei.length()-draw->path.length())
		<< " for user selected nuclei (--";

      readOWN(draw->my_nuclei,nuc);

      std::cout << "--) done" << std::endl;
    }
  else
    {
      for (nuc_it=nuc.begin(); nuc_it!=nuc.end(); nuc_it++)
	nuc_it->own=2;

      std::cout << "Not drawing any user selected nuclei" << std::endl;
    }

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

  if (draw->section == "a" || (draw->section == "b" && draw->required == "a"))
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

  //-------------------
  //- Write the chart -
  //-------------------

  std::cout << "\nCreating ";
  std::cout << draw->outfile << " |--";

  if (draw->file_type == 0)
    writeEPS(nuc,draw);
  else if (draw->file_type == 1)
    writeSVG(nuc,draw);

  std::cout << "--| done\n" << std::endl;

  //-----------------------------------------------------
  //- Write chart parameters to file that can be resued -
  //-----------------------------------------------------
  draw->options.insert(0,pwd);
  std::ofstream opts(draw->options.c_str());

  if (opts)
    {
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
    }
  else
    {
      std::cout << "\nERROR: Couldn't open " << draw->options << " to write the options." << std::endl;
      exit(-1);
    }

  std::cout << "Enjoy\n";

  if(!inputfile)
    std::cout << "\nTo run again with the same options: " << argv[0] << " -i options.in\n" << std::endl;

  delete draw;

  std::cout << std::endl;

  return 0;
}
