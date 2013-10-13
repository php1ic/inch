#include "include/functions.h"

void printBanner(inputs *draw)
{
  std::cout << "\n"
	    << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
	    << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
	    << " ~~                                              ~~\n"
	    << " ~~   Welcome to the Interactive Nuclear CHart   ~~\n"
	    << " ~~                 It is great                  ~~\n"
	    << " ~~               You are rubbish                ~~\n"
	    << " ~~        The Creator Ian Cullen is OK          ~~\n"
	    << " ~~                                              ~~\n"
	    << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
	    << " " << std::setfill('~') << std::setw(50) << draw->version << "\n" << std::endl;
}
