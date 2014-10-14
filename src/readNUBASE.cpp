#include "include/functions.h"

bool readNUBASE(const std::string &table,
		std::vector<Nuclide> &nuc
		)
{
  std::cout << "Reading " << table.substr(table.find_last_of("/")+1)
	    << " for nuclear values <--";

  if ( !checkFileExists(table) )
    {
      std::cout << "\n"
		<< "***ERROR***: Mass table " << table
		<< " couldn't be opened." << std::endl;
      return false;
    }

  std::ifstream file(table.c_str());

  if ( !file.is_open() )
    {
      std::cout << "\n"
		<< "***ERROR***: " << table
		<< " couldn't be opened, does it exist?\n" << std::endl;
      return false;
    }


  // Reserving space avoids multiple calls to the copy constructor
  nuc.reserve(countLinesInFile(file));

  std::vector<int> pnSide(MAX_Z+1,0);
  std::string line;

  while (getline(file,line))
    {
      if (line.find("non-exist") != std::string::npos)
	continue;

      Nuclide isotope;

      isotope.full_data = line;

      isotope.setSpinParity();

      isotope.setExperimental();

      isotope.stripHashes();

      isotope.setA();

      isotope.setZ();

      isotope.setN();

      isotope.setState();

      isotope.setMassExcess();

      isotope.setMassExcessError();

      if (isotope.st == 0 && isotope.A > 1)
	isotope.setSeparationEnergies(nuc);

      isotope.setIsomerEnergy();

      isotope.setHalfLife();

      isotope.setDecayMode(pnSide);

      isotope.setNeutronOrProtonRich(pnSide);

      nuc.push_back(isotope);
    }

  file.close();

  std::cout << "--> done" << std::endl;
  return true;
}
