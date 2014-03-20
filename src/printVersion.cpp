#include "include/functions.h"

void printVersion(inputs *draw)
{
  std::cout << "Interactive Nuclear CHart version " << draw->version << "\n"
	    << "Copyright (C) 2014 Me.\n"
	    << "Interactive Nuclear CHart comes with ABSOLUTELY NO WARRANTY.\n"
	    << "You may redistribute copies under the terms of the\n"
	    << "GNU General Public License\n"
	    << "For more information about these matters, see the file named COPYING."
	    << std::endl;
}
