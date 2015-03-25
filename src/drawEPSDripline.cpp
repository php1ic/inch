#include "include/functions.h"

void drawEPSDripline(const std::vector<Nuclide> &nuc,
		     const inputs *draw,
		     std::ofstream &outFile,
		     const int np
		     )
{
  std::string dripline;

  switch (np)
    {
    case 0: dripline = draw->neutron_drip; break;
    case 1: dripline = draw->two_neutron_drip; break;
    case 2: dripline = draw->proton_drip; break;
    case 3: dripline = draw->two_proton_drip; break;
    default: return;
    }

  //Check if file contaning drip line data exists
  if ( !checkFileExists(dripline) )
    {
      std::cout << "**WARNING**: The drip line file " << dripline << " does not exist.\n"
                << "             Creating it now ...";

      createDriplineFile(nuc,draw,np);

      std::cout << " done" << std::endl;
    }

  //-Format: N Z S(x)
  std::ifstream drip(dripline.c_str());

  if ( !drip.is_open() )
    {
      std::cout << "***ERROR***: " << dripline
                << " couldn't be opened to read the drip line data" << std::endl;
      exit(-1);
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
      std::stringstream in(line);

      sscanf(line.c_str(), "%d %d %*s", &nDrip, &zDrip);

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

  outFile << "st\n"
          << "gr" << std::endl;

  std::cout << " - done\n";
}
