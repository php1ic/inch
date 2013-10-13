#include "include/functions.h"

void printUsage(const std::vector<std::string> &arguments)
{
  std::cout << "  USAGE: " << arguments.at(0) << "\n"
	    << "     OR: " << arguments.at(0) << " -i <input_file>\n"
	    << "     OR: " << arguments.at(0) << " -o <outfile without extension>\n"
	    << "     OR: " << arguments.at(0) << " -i <input_file> -o <outfile without extension>\n" << std::endl;
}
