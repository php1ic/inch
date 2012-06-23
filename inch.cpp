////////////////////////////////////////////////////////////////
//                                                            //
//  inch.cpp                                                  //
//                                                            //
////////////////////////////////////////////////////////////////
//                                                            //
//  Description: This program will create a nuclear chart     //
//               containing the nuclei specified by the user, //
//               coloured by a user chosen property           //
//                                                            //
//  Input:       Required - None                              //
//                                                            //
//               Optional - The name of the output file       //
//                                                            //
//                                                            //
//  Output:      An encapsulated postscript (.eps) file       //
//                                                            //
////////////////////////////////////////////////////////////////

#include "inputs.h"
#include "nuclide.h"
#include "functions.h"

using namespace std;

int main(int argc, char *argv[])
{
  inputs *draw = new inputs;
  vector<Nuclide> nuc;
  vector<Nuclide>::iterator nuc_it;
  unsigned short i;

  cout << "\n"
       << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
       << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
       << " ~~                                              ~~\n"
       << " ~~   Welcome to the Interactive Nuclear CHart   ~~\n"
       << " ~~                 It is great                  ~~\n"
       << " ~~               You are rubbish                ~~\n"
       << " ~~        The Creator Ian Cullen is OK          ~~\n"
       << " ~~                                              ~~\n"
       << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
       << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~1.5\n"
       << "\n"
       << "  USAGE: " << argv[0] << "\n"
       << "    OR:  " << argv[0] << " -i <input_file>\n"
       << "    OR:  " << argv[0] << " -o <outfile.eps>\n"
       << "    OR:  " << argv[0] << " -i <input_file> -o <outfile.eps>\n" << endl;

  draw->path = LOCAL_PATH;
  draw->path.append("/");

  string input_data="data_files/";
  draw->path = draw->path.append(input_data);

  cout << "\nSetting the path to the required files as:\n"
       << draw->path << "\n" << endl;

  draw->mass_table_AME.insert(0,draw->path);

  draw->mass_table_NUBASE.insert(0,draw->path);

  if (draw->AME)
    draw->mass_table = draw->mass_table_AME;
  else
    draw->mass_table = draw->mass_table_NUBASE;

  struct stat mass_t;

  if (stat(draw->mass_table_NUBASE.c_str(),&mass_t))
    {
      cout << "\nERROR: Mass table " << draw->mass_table_NUBASE << " couldn't be opened.\n"
	   << "\nExiting...\n" << endl;
      exit(-1);
    }

  if (draw->AME && stat(draw->mass_table_AME.c_str(),&mass_t))
    {
      cout << "\nERROR: Mass table " << draw->mass_table_AME << " couldn't be opened.\n"
	   << "\nExiting...\n" << endl;
      exit(-1);
    }

  short arguments=argc;
  bool inputfile=false;

  if (arguments > 5)
    {
      char ignore;

      cout << "WARNING: Too many arguments given.\n"
	   << "Ignoring: ";

      for (i=5;i<arguments;i++)
	cout << argv[i] << " ";

      cout << "\nContinue ";

      do
	{
	  cout << "[y/n]: ";
	  cin  >> ignore;

	  if (ignore == 'y')
	    {
	      cout << "\nContinuing..." << endl;
	    }
	  else if (ignore == 'n')
	    {
	      cout << "\nExiting...\n" << endl;
	      exit(-1);
	    }
	  else
	    cout << "That wasn't y or n. Try again" << endl;
	}
      while (ignore != 'n' && ignore !='y');

      arguments=5;
    }

  if (arguments%2 != 1)
    {
      cout << "  ERROR: An odd number of arguments is not allowed\n\n"
	   << "  USAGE: " << argv[0] << "\n"
	   << "    OR:  " << argv[0] << " -i <input_file>\n"
	   << "    OR:  " << argv[0] << " -o <outfile.eps>\n"
	   << "    OR:  " << argv[0] << " -i <input_file> -o <outfile.eps>\n" << endl;
    }
  else if (arguments == 1)
    {
      draw->outfile.insert(0,"/");
      draw->outfile.insert(0,getenv("PWD"));
    }
  else if (arguments != 1)
    {
      for (i=1;i<arguments;i++)
	{
	  if (!strcmp(argv[i],"-i"))
	    {
	      inputfile=true;

	      ifstream infile(argv[i+1]);

	      if (infile.is_open())
		{
		  cout << "Reading " << argv[i+1] << " for input values ";
		  
		  char zx[9];
		  short ax=0;
		  string *line = new string;
		  string temp;
		  
		  while (getline(infile,*line))
		    {
		      if (line->find("#") == string::npos)
			{
			  if (line->find("section=") != string::npos)
			    draw->section=line->erase(0,line->size()-1);
			  else if (line->find("type=") != string::npos)
			    {
			      draw->type=line->erase(0,line->size()-1);
			      if (draw->type == "a")
				draw->e_or_t=1;
			      if (draw->type == "b")
				draw->e_or_t=0;
			      if (draw->type == "c")
				draw->e_or_t=2;
			    }
			  else if (line->find("choice=") != string::npos)
			    draw->choice=line->erase(0,line->size()-1);
			  else if (line->find("required=") != string::npos)
			    draw->required=line->erase(0,line->size()-1);
			  else if (line->find("Zmin=") != string::npos)
			    {
			      ax=line->erase(0,line->find('=')+1).size();  
			      line->copy(zx,ax,0);
			      zx[ax] = '\0';
			      draw->Zmin=atoi(zx);
			    }
			  else if (line->find("Zmax=") != string::npos)
			    {
			      ax=line->erase(0,line->find('=')+1).size();  
			      line->copy(zx,ax,0);
			      zx[ax] = '\0';
			      draw->Zmax=atoi(zx);
			    }
			  else if (line->find("Nmin=") != string::npos)
			    {
			      ax=line->erase(0,line->find('=')+1).size();  
			      line->copy(zx,ax,0);
			      zx[ax] = '\0';
			      draw->Nmin=atoi(zx);
			    }
			  else if (line->find("Nmax=") != string::npos)
			    {
			      ax=line->erase(0,line->find('=')+1).size();  
			      line->copy(zx,ax,0);
			      zx[ax] = '\0';
			      draw->Nmax=atoi(zx);
			    }
			}
		    }
		  infile.close();
		  delete line;
		  cout << "- done" << endl;
		  
		  cout << draw->section << " - "
		       << draw->required << " - "
		       << draw->type << " - "
		       << draw->choice << endl;

		  cout << draw->Zmin << " - "
		       << draw->Zmax << " - "
		       << draw->Nmin << " - "
		       << draw->Nmax << " - " << endl;
		  
		}
	      else
		{
		  cout << "\n\nERROR: " << argv[i+1] << " couldn't be opened, does it exist?\n" << endl;
		  inputfile=false;
		}
	      //cout << argv[i+1] << endl;
	    }

	  if (!strcmp(argv[i],"-o"))
	    {
	      short f=0;
	      bool r=false;
	      char replace, rereplace;
	      draw->outfile = argv[i+1];

	      if (   draw->path+draw->outfile == draw->mass_table_AME
		  || draw->path+draw->outfile == draw->mass_table_NUBASE
		  )
		{
		  cout << "\nERROR: You can't overwrite the mass table(s) you are using\n"
		       << "\n" << draw->mass_table_NUBASE << " is ALWAYS read on execution and\n"
		       << draw->mass_table_AME << " is read if the relevant option is chosen.\n"
		       << "\nExiting...\n" << endl;
		  exit(-1);
		}

	      struct stat out;

	      if (!stat(draw->outfile.c_str(),&out))
		{
		  cout << "\nWARNING: The file " << argv[i+1] << " already exists."
		       << "\nOverwrite ";
		  do
		    {
		      cout << "[y/n]: ";
		      cin  >> replace;

		      if (replace == 'y')
			cout << "\n" << argv[i+1] << " will be overwritten\n" << endl;
		      else if (replace == 'n')
			{
			  do
			    {
			      cout << "New filename (including .eps extension): ";
			      cin  >> draw->outfile;

			      if (   draw->path+draw->outfile == draw->mass_table_AME
				  || draw->path+draw->outfile == draw->mass_table_NUBASE
				  )
				{
				  cout << "Writing over the mass table: " << draw->outfile
				       << " is not a good idea" << endl;
				}
			      else if (!stat(draw->outfile.c_str(),&out))
				{
				  cout << "This file also exists!" << endl;

				  do
				    {
				      cout << "Overwrite this file [y/n]: ";
				      cin  >> rereplace;

				      if (rereplace == 'y')
					{
					  r=true;
					  cout << "\nWill write chart to " << draw->outfile << "\n" << endl;
					}
				      else if (rereplace != 'y' && rereplace != 'n')
					cout << "That wasn't y or n. Try again" << endl;
				    }
				  while (rereplace != 'y' && rereplace != 'n' && !r);
				}
			      else
				cout << "\nWill write chart to " << draw->outfile << "\n" << endl;
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
			      cout << "\n\nThere are 2 options, you've chosen neither on 3 occasions.\n\n"
				   << argv[0] << " file.eps < options.in\n"
				   << "\nUses BASH shell functionality so currently creates an infinite loop.\n"
				   << "Watch this space for the implementation of an input file.\n" << endl;
			      exit(-1);
			    }

			  cout << "That wasn't y or n. Try again" << endl;
			  f++;
			}
		    }
		  while (replace != 'y' && replace != 'n');
		}
	      else
		cout << "Will write chart to " << draw->outfile << "\n" << endl;
	      //cout << argv[i+1] << endl;
	    }
	}
    }

  //exit(-1);

  //-------------------
  //- Read mass table -
  //-------------------
  cout << "Reading "
       << draw->mass_table_NUBASE.substr(draw->path.length(),draw->mass_table_NUBASE.length()-draw->path.length())
       << " for nuclear values";

  read_NUBASE(draw->mass_table_NUBASE,nuc);

  if (draw->AME)
    {
      cout << "\nReading "
	   << draw->mass_table_AME.substr(draw->path.length(),draw->mass_table_AME.length()-draw->path.length())
	   << " for newer mass excess data";

      read_AME(draw->mass_table_AME,nuc);

      cout << " - updated";
    }

  cout << " - done" << endl;

  //----------------------------
  //- Read user defined nuclei -
  //----------------------------
  if (draw->own_nuclei)
    {
      draw->my_nuclei.insert(0,draw->path);

      cout << "Reading "
	   << draw->my_nuclei.substr(draw->path.length(),draw->my_nuclei.length()-draw->path.length())
	   << " for user selected nuclei";

      read_OWN(draw->my_nuclei,nuc);

      cout << " - done" << endl;
    }
  else
    {
      for (nuc_it=nuc.begin(); nuc_it!=nuc.end(); nuc_it++)
	nuc_it->own=2;

      cout << "Not drawing any user selected nuclei" << endl;
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
    display_section(nuc,draw);
  
  //exit(-1);

  cout << "\n===========================\n"
       << "\nBetween Z = " << draw->Zmin << "(" << z_el(draw->Zmin) << ") and Z = "
       << draw->Zmax << "(" << z_el(draw->Zmax) << ")";

  if (draw->section == "a" || (draw->section == "b" && draw->required == "a"))
    cout << ", with all relevant nuclei,\n";
  else if (draw->required == "b")
    cout << ", N = " << draw->Nmin << " and N = " << draw->Nmax << "\n";

  if      (draw->type == "a") cout << "experimentally measured";
  else if (draw->type == "b") cout << "theoretical/extrapolated";
  else                        cout << "both experimental and theoretical";

  cout << " values will be drawn and\nthe chart coloured by ";

  if      (draw->choice == "a") cout << "error on mass-excess\n";
  else if (draw->choice == "b") cout << "relative error on mass-excess\n";
  else if (draw->choice == "c") cout << "major ground-state decay mode\n";
  else if (draw->choice == "d") cout << "ground-state half-life\n";
  else                          cout << "first isomer energy\n";

  cout << "\nCreating " << draw->outfile << "\n\n";
  
  if (draw->file_type == 0)
    {
      draw->outfile.append(".eps");
      cout << draw->outfile << "\n\n";
      ofstream out_file(draw->outfile.c_str());
      write_EPS(nuc,draw,out_file);
    }
  else if (draw->file_type == 1)
    {
      draw->outfile.append(".svg");
      cout << draw->outfile << "\n\n";
      ofstream out_file(draw->outfile.c_str());
      write_SVG(nuc,draw,out_file);
    }

  cout << "\n- done\n" << endl;

  draw->options.insert(0,draw->path);
  ofstream opts(draw->options.c_str());

  if (opts)
    {
      opts << draw->section << "\n";

      if (draw->section == "b")
	{
	  opts << draw->Zmin << "\n"
	       << draw->Zmax << "\n"
	       << draw->required << "\n";

	  if (draw->required == "b")
	    opts << draw->Nmin << "\n"
		 << draw->Nmax << "\n";
	}

      opts << draw->type << "\n"
	   << draw->choice << endl;
      opts.close();
    }
  else
    {
      cout << "\nERROR: Couldn't open " << draw->options << " to write the options." << endl;
      exit(-1);
    }

  cout << "Enjoy\n" << endl;
  /*
       << "\nTo run again with the same options: " << argv[0] << " < options.in\n"
       << "\nIf file.eps already, DO NOT RUN: " << argv[0] << " file.eps < options.in\n" << endl;
  */
  delete draw;

  return 0;
}
