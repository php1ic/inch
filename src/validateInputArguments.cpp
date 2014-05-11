#include "include/functions.h"

bool validateInputArguments(const std::vector<Nuclide> &nuc,
			    inputs *draw,
			    const std::vector<std::string> &arguments
			    )
{
  //Ignore arguments after the 4th, counting starts at 0 not 1.
  const int MAX_ARGUMENTS=5;

  bool validOptions=false;
  int numArguments=arguments.size();

  if (numArguments > MAX_ARGUMENTS)
    {
      char ignore;

      std::cout << "**WARNING**: Too many arguments given.\n"
		<< "Ignoring: ";

      for (int i=MAX_ARGUMENTS; i<numArguments; ++i)
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

      numArguments = MAX_ARGUMENTS;
    }

  //Read option via << -flag file >> so, including the executable, we need
  //an odd number of arguments
  if (numArguments%2 == 1)
    {
      for (int i=1; i<numArguments-1; ++i)
	{
	  if (arguments[i] == "-i")
	    {
	      validOptions = validateInputFile(nuc,draw,arguments[i+1]);

	      if ( !validOptions )
		std::cout << "***ERROR***: Bad inputfile - " << arguments[i+1] << std::endl;
	    }

	  if (arguments[i] == "-o")
	    {
	      validateOutputFile(draw,arguments[i+1]);
	    }
	}
    }
  else
    {
      std::cout << "\n"
		<< "***ERROR***: An odd number of arguments is not allowed\n"
		<< std::endl;

      return false;
    }

  return validOptions;
}
