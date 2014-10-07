#include "include/functions.h"

bool validateInputArguments(const std::vector<Nuclide> &nuc,
			    inputs *draw,
			    const std::vector<std::string> &arguments
			    )
{
  //Ignore arguments after the 6th, counting starts at 0 not 1.
  const size_t MAX_ARGUMENTS=7;

  size_t numArguments = arguments.size();

  if (numArguments > MAX_ARGUMENTS)
    {
      std::cout << "**WARNING**: Too many arguments given.\n"
		<< "Ignoring: ";

      for (size_t i=MAX_ARGUMENTS; i<numArguments; ++i)
	std::cout << arguments[i] << " ";

      std::cout << "\nContinue ";

      char ignore='n';
      do
	{
	  std::cout << "[y/n]: ";
	  std::cin  >> ignore;

	  if (ignore == 'n')
	    {
	      std::cout << "\nExiting...\n" << std::endl;
	      exit(-1);
	    }
	  else
	    std::cout << "That wasn't y or n. Try again" << std::endl;
	}
      while (ignore != 'n' && ignore !='y');

      numArguments = MAX_ARGUMENTS;
    }

  bool validInput=false;
  bool validOutput=false;
  bool validFileType=false;
  bool attemptedToSetFileType=false;

  //Read option via << -flag file >> so, including the executable, we need
  //an odd number of arguments
  if (numArguments%2 == 1)
    {
      for (size_t i=1; i<numArguments-1; ++i)
	{
	  if (arguments[i] == "-i")
	    {
	      validInput = validateInputFile(nuc,draw,arguments[i+1]);

	      if ( !validInput )
		std::cout << "***ERROR***: Bad inputfile - "
			  << arguments[i+1] << std::endl;
	    }
	  else if (arguments[i] == "-o")
	    {
	      //Checking of the output file is dependent on if the output
	      //type is specified. Look at other arguments to see if it is.
	      if ( !attemptedToSetFileType )
		{
		  for (size_t j=1; j<numArguments-1; ++j)
		    {
		      if (arguments[j] == "-f")
			{
			  validFileType = validateFileType(draw,arguments[j+1]);
			  attemptedToSetFileType=true;

			  if ( !validFileType && attemptedToSetFileType )
			    std::cout << "**WARNING**: Bad file type - "
				      << arguments[j+1] << std::endl;
			}
		    }
		}

	      validOutput = validateOutputFile(draw,arguments[i+1]);

	      if ( !validOutput )
		std::cout << "***ERROR***: Bad outfile - "
			  << arguments[i+1] << std::endl;
	    }
	  else if (arguments[i] == "-f" && !attemptedToSetFileType)
	    {
	      std::cout << "Setting file type" << std::endl;
	      validFileType = validateFileType(draw,arguments[i+1]);
	      attemptedToSetFileType=true;

	      if ( !validFileType )
		std::cout << "**WARNING**: Bad file type - "
			  << arguments[i+1] << std::endl;
	    }
	}

      //Validating the output file either exits the code or returns true
      //Thus if validOutput is false, we will not have checked the output file
      if (!validOutput)
	draw->constructOutputFilename();
    }
  else
    {
      std::cout << "\n"
		<< "***ERROR***: An odd number of arguments is not allowed\n"
		<< std::endl;

      draw->constructOutputFilename();

      return false;
    }

  //If the input file was bad we need to ask what to display
  return validInput;
}
