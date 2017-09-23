#include "functions.h"

void drawEPSDripline(const double meN,
                     const double meP,
                     const std::unique_ptr<inputs> &draw,
                     std::ofstream &outFile,
                     const int np
                     )
{
  std::string dripline;

  switch (np)
    {
    case 0: dripline = draw->neutron_drip;      break;
    case 1: dripline = draw->two_neutron_drip;  break;
    case 2: dripline = draw->proton_drip;       break;
    case 3: dripline = draw->two_proton_drip;   break;
    default: return;
    }

  //Check if file contaning drip line data exists
  if ( !checkFileExists(dripline) )
    {
      std::cout << "**WARNING**: The drip line file " << dripline << " does not exist.\n"
                << "             Creating it now ...";

      createDriplineFile(draw,meN,meP,np);

      std::cout << " done" << std::endl;
    }

  //-Format: N Z S(x)
  std::ifstream drip(dripline.c_str());

  if ( !drip )
    {
      std::cout << "***ERROR***: " << dripline
                << " couldn't be opened to read the drip line data" << std::endl;
      return;
    }

  std::cout << "Reading "
            << dripline.substr(dripline.find_last_of("/")+1)
            << " and drawing the drip line";

  bool initial=true;
  std::string line;

  while (getline(drip,line))
    {
      if ( !line.compare("") || line.at(0) == '#' )
        continue;

      int zDrip=0;
      int nDrip=0;
      double dummy=0.0;

      std::istringstream dripData(line);
      dripData >> nDrip >> zDrip >> dummy;

      if (   zDrip >= draw->Zmin
          && zDrip <= draw->Zmax
          && nDrip >= draw->Nmin
          && nDrip <= draw->Nmax)
        {
          outFile << std::setw(3) << nDrip-draw->Nmin << " "
                  << std::setw(3) << zDrip-draw->Zmin << " "
                  << (initial ? "m" : "l") << "\n";

          initial=false;
        }
    }
  drip.close();

  outFile << "st" << std::endl;

  std::cout << " - done\n";
}
