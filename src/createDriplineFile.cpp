#include "functions.h"

// Rather than use a vector of Nuclide(216), create a new
// smaller(56) struct to store the required values and
// create a vector of these.
struct isotope
{
  int A;
  int Z;
  int N;
  double s_n;
  double s_2n;
  double s_p;
  double s_2p;
  double ME;
};


void createDriplineFile(const inputs *draw,
                        const double meN,
                        const double meP,
                        const int np
                        )
{
  /// Check that the FRDM file exists so we can calculate the drip line location
  if ( !checkFileExists(draw->FRDM) )
    {
      std::cout << "\n"
                << "***ERROR***: Can't find " << draw->FRDM
                << "\n" << std::endl;
      return;
    }

  std::ifstream file(draw->FRDM.c_str());

  if ( !file.is_open() )
    {
      std::cout << "\n"
                << "***ERROR***: " << draw->FRDM
                << " couldn't be opened, does it exist?"
                << "\n" << std::endl;
      return;
    }

  /// Open the file and write it's header
  std::ofstream dripFile;

  switch ( np )
    {
    case 0:
      dripFile.open(draw->neutron_drip.c_str());
      if ( dripFile.is_open() )
        {
          dripFile << "#Neutron drip line\n"
                   << "#calculated using the FRLDM\n"
                   << "#  N    Z   Sn[MeV]\n";
        }
      else
        {
          std::cout << "**WARNING**: The file " << draw->neutron_drip
                    << " could not be opened. Not creating drip line file." << std::endl;
          return;
        }
      break;
    case 1:
      dripFile.open(draw->two_neutron_drip.c_str());
      if ( dripFile.is_open() )
        {
          dripFile << "#Two neutron drip line\n"
                   << "#calculated using the FRLDM\n"
                   << "#  N    Z   S2n[MeV]\n";
        }
      else
        {
          std::cout << "**WARNING**: The file " << draw->two_neutron_drip
                    << " could not be opened. Not creating drip line file." << std::endl;
          return;
        }
      break;
    case 2:
      dripFile.open(draw->proton_drip.c_str());
      if ( dripFile.is_open() )
        {
          dripFile << "#Proton drip line\n"
                   << "#calculated using the FRLDM\n"
                   << "#  N    Z   Sp[MeV]\n";
        }
      else
        {
          std::cout << "**WARNING**: The file " << draw->proton_drip
                    << " could not be opened. Not creating drip line file." << std::endl;
          return;
        }
      break;
    case 3:
      dripFile.open(draw->two_proton_drip.c_str());
      if ( dripFile.is_open() )
        {
          dripFile << "#Two proton drip line\n"
                   << "#calculated using the FRLDM\n"
                   << "#  N    Z   S2p[MeV]\n";
        }
      else
        {
          std::cout << "**WARNING**: The file " << draw->two_proton_drip
                    << " could not be opened. Not creating drip line file." << std::endl;
          return;
        }
      break;
    default:
      return;
    }

  dripFile << "#------------------" << std::endl;
  dripFile.precision(4);

  ///Starting values come from the FRLDM file
  int currentSingleProton   = 7;
  int currentSingleNeutron  = 7;
  int previousSingleNeutron = 8;
  int previousSingleProton  = 0;

  int currentDoubleProton   = 7;
  int currentDoubleNeutron  = 7;
  int previousDoubleNeutron = 8;
  int previousDoubleProton  = 0;

  int i=0;
  std::string line;

  std::vector<isotope> dripNuc;
  dripNuc.resize(countLinesInFile(file));

  while( getline(file,line) )
    {
      if ( !line.compare("") || line[0] == '#' )
        {
          continue;
        }

      sscanf(line.c_str(), "%d %d %*s %lf", &dripNuc[i].A, &dripNuc[i].Z, &dripNuc[i].ME);

      dripNuc[i].N = dripNuc[i].A - dripNuc[i].Z;

      /// File is ordered in Z then A, by looping through backwards,
      /// we do not need to traverse the entire contents as we can stop
      /// once values get too far apart to provide data.
      for ( int j=i; j>0; --j )
        {
          /// Depending on the drip line, we can break when differences
          /// in N or Z get too larger i.e. 2 for single particle and
          /// 3 for double particle lines.
          if (   (np == 0 && dripNuc[i].N - dripNuc[j].N >= 2)
              || (np == 1 && dripNuc[i].N - dripNuc[j].N >= 3)
              || (np == 2 && dripNuc[i].Z - dripNuc[j].Z >= 2)
              || (np == 3 && dripNuc[i].Z - dripNuc[j].Z >= 3)
              )
            {
              break;
            }

          if ( dripNuc[i].A - dripNuc[j].A == 1 )
            {
              /// S(n) = M(Z,N-1) - M(Z,N) + M(0,1)
              if ( np == 0 && dripNuc[j].Z == dripNuc[i].Z && currentSingleProton < dripNuc[i].Z )
                {
                  dripNuc[i].s_n = dripNuc[j].ME - dripNuc[i].ME + meN;

                  if ( dripNuc[i].s_n < 0 )
                    {
                      dripFile << std::fixed
                               << std::setw(4) << dripNuc[i].N   << " "
                               << std::setw(4) << dripNuc[i].Z   << " "
                               << std::setw(8) << dripNuc[i].s_n << "\n"
                               << std::setw(4) << dripNuc[i].N   << " "
                               << std::setw(4) << dripNuc[i].Z+1 << " "
                               << std::setw(8) << dripNuc[i].s_n << std::endl;

                      ++currentSingleProton;
                    }
                }

              /// S(p) = M(Z-1,N) - M(Z,N) + M(1,0)
              if ( np == 2 && dripNuc[j].N == dripNuc[i].N && currentSingleNeutron < dripNuc[i].N )
                {
                  dripNuc[i].s_p = dripNuc[j].ME - dripNuc[i].ME + meP;

                  if ( dripNuc[i].s_p < 0 )
                    {
                      if ( dripNuc[i].N != previousSingleNeutron )
                        {
                          dripFile << std::fixed
                                   << std::setw(4) << previousSingleNeutron+1 << " "
                                   << std::setw(4) << previousSingleProton    << " "
                                   << std::setw(8) << dripNuc[i].s_p          << std::endl;
                        }

                      dripFile << std::fixed
                               << std::setw(4) << dripNuc[i].N   << " "
                               << std::setw(4) << dripNuc[i].Z   << " "
                               << std::setw(8) << dripNuc[i].s_p << std::endl;

                      ++currentSingleNeutron;
                      previousSingleProton  = dripNuc[i].Z;
                      previousSingleNeutron = dripNuc[i].N;
                    }
                }
            }
          else if ( dripNuc[i].A - dripNuc[j].A == 2 )
            {
              /// S(2n) = M(Z,N-2) - M(Z,N) + 2*M(0,1)
              if ( np == 1 && dripNuc[j].Z == dripNuc[i].Z && currentDoubleProton < dripNuc[i].Z )
                {
                  dripNuc[i].s_2n = dripNuc[j].ME - dripNuc[i].ME + 2*meN;

                  if ( dripNuc[i].s_2n < 0 )
                    {
                      dripFile << std::fixed
                               << std::setw(4) << dripNuc[i].N    << " "
                               << std::setw(4) << dripNuc[i].Z    << " "
                               << std::setw(8) << dripNuc[i].s_2n << "\n"
                               << std::setw(4) << dripNuc[i].N    << " "
                               << std::setw(4) << dripNuc[i].Z+1  << " "
                               << std::setw(8) << dripNuc[i].s_2n << std::endl;

                      ++currentDoubleProton;
                    }
                }

              /// S(2p) = M(Z-2,N) - M(Z,N) + 2*M(1,0)
              if ( np == 3 && dripNuc[j].N == dripNuc[i].N && currentDoubleNeutron < dripNuc[i].N )
                {
                  dripNuc[i].s_2p = dripNuc[j].ME - dripNuc[i].ME + 2*meP;

                  if ( dripNuc[i].s_2p < 0 )
                    {
                      if ( dripNuc[i].N != previousDoubleNeutron )
                        {
                          dripFile << std::fixed
                                   << std::setw(4) << previousDoubleNeutron+1 << " "
                                   << std::setw(4) << previousDoubleProton    << " "
                                   << std::setw(8) << dripNuc[i].s_2p         << std::endl;
                        }

                      dripFile << std::fixed
                               << std::setw(4) << dripNuc[i].N    << " "
                               << std::setw(4) << dripNuc[i].Z    << " "
                               << std::setw(8) << dripNuc[i].s_2p << std::endl;

                      ++currentDoubleNeutron;
                      previousDoubleProton  = dripNuc[i].Z;
                      previousDoubleNeutron = dripNuc[i].N;
                    }
                }
            }
        }

      ++i;
    }

  file.close();

  dripFile.close();
}
