#include "include/functions.h"

void printBanner(inputs *draw)
{
  std::cout << "\n"
	    << "	 +---+---+---+---+---+---+---+\n"
	    << "	 |In |Te |Ra | C |Ti | V | E |\n"
	    << "	 +---+---+---+---+---+---+---+\n"
	    << "	     | N | U |Cl | E |Ar |\n"
	    << "	 +---+---+---+---+---+---+\n"
	    << "	 | C | H |Ar | T |\n"
	    << "	 +---+---+---+---v" << draw->version << "\n"
	    << "\n"
	    << "  USAGE: inch\n"
	    << "     OR: inch -i <input_file>\n"
	    << "     OR: inch -o <outfile without extension>\n"
	    << "     OR: inch -i <input_file> -o <outfile without extension>\n"
	    << std::endl;
}
