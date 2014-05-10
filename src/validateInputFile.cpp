#include "include/functions.h"

bool validateInputFile(const std::vector<Nuclide> &nuc,
		       inputs *draw,
		       const std::string &inputFilename
		       )
{
  std::map<std::string, std::string> options = readOptionFile(inputFilename);

  //Check that there were the min number of
  //validly formatted lines in the file.
  if (options.size() < 3)
    return false;

  //Check that the individual options are valid.
  if( !checkOptions(options, draw) )
    return false;

  //Check that the options as a whole make sense.
  if (draw->section == "a")
    {
      if (draw->Zmin != 200 && draw->Zmax != 0)
	{
	  std::cout << "**WARNING**\n"
		    << "The option file contains a Z range but specifies that all nuclei should be drawn.\n"
		    << "The input range will be ignored, set section=b to select a range in Z.\n"
		    << "***********\n" << std::endl;
	}

      draw->Zmin=0;
      draw->Zmax=MAX_Z;
      draw->Nmin=0;
      draw->Nmax=MAX_N;
    }
  else if (draw->section == "b")
    {
      if (draw->required == "a")
	{
	  setNeutronLimits(nuc,draw);
	}
      else if (draw->required != "b")
	{
	  std::cout << "***ERROR***: " << draw->required
		    << " is not a valid option for the 'required' field.\n"
		    << "            Ignoring input file.\n" << std::endl;
	  return false;
	}

      if (draw->Zmin > draw->Zmax)
	{
	  std::cout << "***ERROR***: Zmax(" << draw->Zmax
		    << ") cannot be less than Zmin(" << draw->Zmin<< ")\n"
		    << "            Ignoring input file.\n" << std::endl;
	  return false;
	}

      if (draw->Nmin > draw->Nmax)
	{
	  std::cout << "***ERROR***: Nmax(" << draw->Nmax
		    << ") cannot be less than Nmin(" << draw->Nmin<< ")\n"
		    << "            Ignoring input file.\n" << std::endl;
	  return false;
	}
    }
  else
    {
      std::cout << "***ERROR***: " << draw->section
		<< " is not a valid option for the 'section' field.\n"
		<< "            Ignoring input file.\n" << std::endl;
      return false;
    }

  if (   draw->type != "a"
      && draw->type != "b"
      && draw->type != "c"
      )
    {
      std::cout << "***ERROR***: " << draw->type
		<< " is not a valid option for the 'type' field.\n"
		<< "            Ignoring input file.\n" << std::endl;
      return false;
    }

  if (   draw->choice != "a"
      && draw->choice != "b"
      && draw->choice != "c"
      && draw->choice != "d"
      && draw->choice != "e"
      )
    {
      std::cout << "***ERROR***: " << draw->choice
		<< " is not a valid option for the 'choice' field.\n"
		<< "            Ignoring input file.\n" << std::endl;
      return false;
    }

  std::cout << "Read values:\n"
	    << "section: " << draw->section  << "\n";

  if (draw->section == "b")
    {
      std::cout << "Zmin: "     << draw->Zmin << "\n"
		<< "Zmax: "     << draw->Zmax << "\n"
		<< "required: " << draw->required << "\n"
		<< "Nmin: "     << draw->Nmin << "\n"
		<< "Nmax: "     << draw->Nmax << "\n";
    }

  std::cout << "type: " << draw->type << "\n"
	    << "choice: " << draw->choice << std::endl;

  return true;
}
