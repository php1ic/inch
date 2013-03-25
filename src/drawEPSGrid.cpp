#include "functions.h"

void drawEPSGrid(const inputs *draw,
		 std::ofstream &out_file
		 )
{
  out_file << "\n%-Grid----------\n"
	   << "gs\n"
	   << "0.75 setgray\n"
	   << "2 TR\n"
	   << "0.1 u div sl\n"
	   << "\n5 5 " << draw->Nmax-draw->Nmin << "{\n"
	   << "/i ed\n"
	   << "i 1 m\n"
	   << "gs\n"
	   << "i 10 mod 0 eq {\n"
	   << "i 3 string cvs sh\n"
	   << "i " << draw->Zmax-draw->Zmin << " m i 3 string cvs sh\n"
	   << "} if\n"
	   << "gr\n"
	   << "0 " << draw->Zmax-draw->Zmin << " rl\n"
	   << "} for\n"
	   << "\n5 5 " << draw->Zmax-draw->Zmin << "{\n"
	   << "/i ed\n"
	   << "1 i m\n"
	   << "gs\n"
	   << "i 10 mod 0 eq {\n"
	   << "i 3 string cvs sh\n"
	   << "" << draw->Nmax-draw->Nmin-2 << " i m i 3 string cvs sh\n"
	   << "} if\n"
	   << "gr\n"
	   << draw->Nmax-draw->Nmin << " 0 rl\n"
	   << "} for\n"
	   << "\nst\n"
	   << "gr\n"
	   << "%---------------\n" << std::endl;
}
