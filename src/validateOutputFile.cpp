#include "include/functions.h"

void validateOutputFile(inputs *draw,
			const std::string &outputFilename
			)
{
  int f=0;
  bool r=false;
  char replace, rereplace;
  draw->outfile = outputFilename;

  constructOutputFilename(draw);

  if (   draw->outfile == draw->mass_table_AME
      || draw->outfile == draw->mass_table_NUBASE
      )
    {
      std::cout << "\nERROR: You can't overwrite the mass table(s) you are using\n"
		<< "\n" << draw->mass_table_NUBASE << " is ALWAYS read on execution and\n"
		<< draw->mass_table_AME << " is read if the relevant option is chosen.\n"
		<< "\nExiting...\n" << std::endl;
      exit(-1);
    }

  if (checkFileExists(draw->outfile))
    {
      std::cout << "\nWARNING: The file " << draw->outfile << " already exists.\n"
		<< "Overwrite ";
      do
	{
	  std::cout << "[y/n]: ";
	  std::cin  >> replace;

	  if (replace == 'y')
	    {
	      std::cout << "\n" << draw->outfile << " will be overwritten\n" << std::endl;
	    }
	  else if (replace == 'n')
	    {
	      do
		{
		  std::cout << "New filename (without extension): ";
		  std::cin  >> draw->outfile;

		  constructOutputFilename(draw);

		  if (   draw->outfile == draw->mass_table_AME
		      || draw->outfile == draw->mass_table_NUBASE
		      )
		    {
		      std::cout << "Writing over the mass table: " << draw->outfile
				<< " is not a good idea" << std::endl;
		    }
		  else if (checkFileExists(draw->outfile))
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
		     && checkFileExists(draw->outfile)
		     && !r
		     );
	    }
	  else
	    {
	      if (f>1)
		{
		  std::cout << "\n\nThere are 2 options, you've chosen neither on 3 occasions.\n\n"
			    << "Perhaps this is running in a script.\nExiting..." << std::endl;
		  exit(-1);
		}

	      std::cout << "That wasn't y or n. Try again" << std::endl;
	      ++f;
	    }
	}
      while (replace != 'y' && replace != 'n');
    }
  else
    std::cout << "Will write chart to " << draw->outfile << "\n" << std::endl;
}
