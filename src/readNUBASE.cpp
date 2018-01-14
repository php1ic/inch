#include "functions.hpp"

bool readNUBASE(const std::string &table,
                const int year,
                std::vector<Nuclide> &nuc
                )
{
  std::cout << "Reading " << table.substr(table.find_last_of('/')+1)
            << " for nuclear values <--";

  std::ifstream file(table.c_str());

  if ( !file )
    {
      std::cout << "\n"
                << "***ERROR***: " << table
                << " couldn't be opened, does it exist?\n" << std::endl;
      return false;
    }

  std::vector<bool> pnSide(MAX_Z+1, false);
  std::string line;

  while ( getline(file,line) )
    {
      if ( line.find("non-exist") != std::string::npos )
        {
          continue;
        }

      Nuclide isotope(line);

      isotope.setSpinParity();

      isotope.setExperimental();

      isotope.setA();

      isotope.setZ();

      isotope.setN();

      isotope.setState();

      isotope.setNubaseMassExcess();

      isotope.setNubaseMassExcessError();

      if ( isotope.st == 0 && isotope.A > 1 )
        {
          isotope.setSeparationEnergies(nuc);
        }

      isotope.setIsomerEnergy();

      isotope.setHalfLife();

      isotope.setDecayMode(pnSide,year);

      isotope.setNeutronOrProtonRich(pnSide);

      nuc.emplace_back(isotope);
    }

  file.close();

  std::cout << "--> done" << std::endl;
  return true;
}
