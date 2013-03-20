#include "functions.h"

void validateInputArguments(const std::vector<Nuclide> &nuc,
			    inputs *draw,
			    const std::vector<std::string> &arguments,
			    bool &inputfile,
			    const std::string &pwd,
			    int &numArguments)
{
  int i=0;
  if (numArguments > 5)
    {
      char ignore;

      std::cout << "WARNING: Too many arguments given.\n"
		<< "Ignoring: ";

      for (i=5;i<numArguments;++i)
	std::cout << arguments[i] << " ";

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

      numArguments=5;
    }

  if (numArguments%2 != 1)
    {
      std::cout << "\n\n"
		<< "  ERROR: An odd number of arguments is not allowed\n\n"
		<< "  USAGE: " << arguments[0] << "\n"
		<< "     OR: " << arguments[0] << " -i <input_file>\n"
		<< "     OR: " << arguments[0] << " -o <outfile without extension>\n"
		<< "     OR: " << arguments[0] << " -i <input_file> -o <outfile without extension>\n\n" << std::endl;

      exit(-1);
    }
  else if (numArguments == 1)
    {
      constructOutputFilename(draw,pwd);
    }
  else if (numArguments != 1)
    {
      for (i=1;i<numArguments;++i)
	{
	  if (arguments[i] == "-i")
	    {
	      validateInputFile(nuc,draw,arguments[i+1],inputfile);
	    }

	  if (arguments[i] == "-o")
	    {
	      validateOutputFile(draw,arguments[i+1],pwd);
	    }
	}
    }

  //-Add the necessary extension
  if (draw->file_type == 0)
    draw->outfile.append(".eps");
  else if (draw->file_type == 1)
    draw->outfile.append(".svg");
  else if (draw->file_type == 2)
    draw->outfile.append(".tex");
}
