#include "functions.h"

std::map<std::string, std::string> readOptionFile(const std::string &inputFilename)
{
  std::map<std::string, std::string> values;

  std::ifstream infile(inputFilename.c_str());

  std::cout << "Reading " << inputFilename << " for input values {--";

  if ( !infile )
    {
      std::cout << "\n***ERROR***: " << inputFilename
                << " couldn't be opened, does it exist?\n" << std::endl;

      return values;
    }

  std::string line;

  while ( getline(infile,line) )
    {
      // Skip empty lines
      // Have this on it's own and as the first check
      if ( line.empty() )
        {
          continue;
        }
      // Let lines starting with '#' be comments
      // We could 'OR' this with the above empty line check, but as the order
      // of the conditions would be critical, lets keep them separate
      else if ( line.at(0) == '#' )
        {
          continue;
        }
      // Clear any part of the string after and including a '#'.
      // We can't get here if the string starts with '#' so no issue
      // of creating an empty string at this point.
      else if ( line.find('#') != std::string::npos )
        {
          line.erase(line.find('#'));
        }

      int i=0;
      std::string part;
      std::vector<std::string> theLine(2);
      std::stringstream stream(line);

      while ( getline(stream, part, '=') )
        {
          /// Remove any and all 'white-space' characters
          part.erase(std::remove_if(part.begin(),
                                    part.end(),
                                    [](char x) {return std::isspace(x);} ),
                     part.end()
                     );

          theLine.at(i) = part;
          ++i;
        }

      if ( values.count(theLine.at(0)) > 0 )
        {
          std::cout << "\n**WARNING**: Already have a value for " << theLine.at(0)
                    << " (" << theLine.at(1) << ")"
                    << ", will use new value.\n";
        }

      values.insert( std::pair<std::string, std::string>(theLine.at(0), theLine.at(1)) );
    }

  infile.close();

  std::cout << "--} done" << std::endl;

  return values;
}
