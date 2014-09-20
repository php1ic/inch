#include "include/functions.h"

std::map<std::string, std::string> readOptionFile(const std::string &inputFilename)
{
  std::map<std::string, std::string> values;

  std::ifstream infile(inputFilename.c_str());

  std::cout << "Reading " << inputFilename << " for input values {--";

  if ( !infile.is_open() )
    {
      std::cout << "\n***ERROR***: " << inputFilename
		<< " couldn't be opened, does it exist?\n" << std::endl;
    }

  bool repeat=false;
  std::string line;

  while (getline(infile,line))
    {
      /*
	TODO: The order of these conditions is critical, otherwise causes
	a seg fault when accessing zeroth element of an empty line.
	Make this more robust and order independent
      */
      //Let lines starting with '#' be comments
      if ( !line.compare("") || line.at(0) == '#' )
	continue;

      int i=0;
      std::string part;
      std::string theLine[2];
      std::stringstream stream(line);

      while ( getline(stream, part, '=') )
	{
	  if (part.find('#') != std::string::npos)
	    part.erase(part.find('#'));

	  if (part.find(' ') != std::string::npos)
	    part.erase(std::remove(part.begin(), part.end(), ' '), part.end());

	  if (part.find('\t') != std::string::npos)
	    part.erase(std::remove(part.begin(), part.end(), '\t'), part.end());

	  theLine[i] = part;
	  ++i;
	}

      if (values.count(theLine[0]))
	{
	  std::cout << "\n**WARNING**: Already have a value for " << theLine[0]
		    << ", will use new value.";

	  repeat=true;
	}

      values[theLine[0]] = theLine[1];
    }

  infile.close();

  std::cout << (repeat ? "\n" : "")  << "--} done" << std::endl;

  return values;
}
