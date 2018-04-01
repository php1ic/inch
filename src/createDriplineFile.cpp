#include "functions.hpp"

void createDriplineFile(const std::unique_ptr<inputs> &draw,
                        const double meN,
                        const double meP,
                        const int np
                        )
{
  std::ifstream file(draw->FRDM, std::ios::binary);

  if ( !file )
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
      dripFile.open(draw->neutron_drip, std::ios::binary);
      if ( dripFile )
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
      dripFile.open(draw->two_neutron_drip, std::ios::binary);
      if ( dripFile )
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
      dripFile.open(draw->proton_drip, std::ios::binary);
      if ( dripFile )
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
      dripFile.open(draw->two_proton_drip, std::ios::binary);
      if ( dripFile )
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

  int nucleonWidth = 4;
  int dripWidth = 8;

  // Rather than use a vector of Nuclide(216), create a new
  // smaller(56) struct to store the required values and
  // create a vector of these.
  struct isotope
  {
    int A = 0;
    int Z = 0;
    int N = 0;
    double s_n = 0.0;
    double s_2n = 0.0;
    double s_p = 0.0;
    double s_2p = 0.0;
    double ME = 0.0;
  };
  std::vector<isotope> dripNuc;

  std::string line;
  while( getline(file,line) )
    {
      if ( line.empty() || line[0] == '#' )
        {
          continue;
        }

      std::istringstream dripData(line);
      std::string dummy;

      dripNuc.emplace_back(isotope());
      auto currentIsotope = std::rbegin(dripNuc);

      dripData >> currentIsotope->A >> currentIsotope->Z >> dummy >> currentIsotope->ME;

      currentIsotope->N = currentIsotope->A - currentIsotope->Z;

      /// File is ordered in Z then A, by looping through backwards,
      /// we do not need to traverse the entire contents as we can stop
      /// once values get too far apart to provide data.
      for ( auto otherIsotope = currentIsotope; otherIsotope != std::rend(dripNuc); ++otherIsotope )
        {
          /// Depending on the drip line, we can break when differences
          /// in N or Z get too larger i.e. 2 for single particle and
          /// 3 for double particle lines.
          if (   (np == 0 && currentIsotope->N - otherIsotope->N >= 2)
              || (np == 1 && currentIsotope->N - otherIsotope->N >= 3)
              || (np == 2 && currentIsotope->Z - otherIsotope->Z >= 2)
              || (np == 3 && currentIsotope->Z - otherIsotope->Z >= 3)
              )
            {
              break;
            }

          if ( currentIsotope->A - otherIsotope->A == 1 )
            {
              /// S(n) = M(Z,N-1) - M(Z,N) + M(0,1)
              if ( np == 0 && otherIsotope->Z == currentIsotope->Z && currentSingleProton < currentIsotope->Z )
                {
                  currentIsotope->s_n = otherIsotope->ME - currentIsotope->ME + meN;

                  if ( currentIsotope->s_n < 0.0 )
                    {
                      dripFile << std::fixed
                               << std::setw(nucleonWidth) << currentIsotope->N   << " "
                               << std::setw(nucleonWidth) << currentIsotope->Z   << " "
                               << std::setw(dripWidth) << currentIsotope->s_n << "\n"
                               << std::setw(nucleonWidth) << currentIsotope->N   << " "
                               << std::setw(nucleonWidth) << currentIsotope->Z+1 << " "
                               << std::setw(dripWidth) << currentIsotope->s_n << std::endl;

                      ++currentSingleProton;
                    }
                }

              /// S(p) = M(Z-1,N) - M(Z,N) + M(1,0)
              if ( np == 2 && otherIsotope->N == currentIsotope->N && currentSingleNeutron < currentIsotope->N )
                {
                  currentIsotope->s_p = otherIsotope->ME - currentIsotope->ME + meP;

                  if ( currentIsotope->s_p < 0.0 )
                    {
                      if ( currentIsotope->N != previousSingleNeutron )
                        {
                          dripFile << std::fixed
                                   << std::setw(nucleonWidth) << previousSingleNeutron+1 << " "
                                   << std::setw(nucleonWidth) << previousSingleProton    << " "
                                   << std::setw(dripWidth) << currentIsotope->s_p          << std::endl;
                        }

                      dripFile << std::fixed
                               << std::setw(nucleonWidth) << currentIsotope->N   << " "
                               << std::setw(nucleonWidth) << currentIsotope->Z   << " "
                               << std::setw(dripWidth) << currentIsotope->s_p << std::endl;

                      ++currentSingleNeutron;
                      previousSingleProton  = currentIsotope->Z;
                      previousSingleNeutron = currentIsotope->N;
                    }
                }
            }
          else if ( currentIsotope->A - otherIsotope->A == 2 )
            {
              /// S(2n) = M(Z,N-2) - M(Z,N) + 2*M(0,1)
              if ( np == 1 && otherIsotope->Z == currentIsotope->Z && currentDoubleProton < currentIsotope->Z )
                {
                  currentIsotope->s_2n = otherIsotope->ME - currentIsotope->ME + 2*meN;

                  if ( currentIsotope->s_2n < 0.0 )
                    {
                      dripFile << std::fixed
                               << std::setw(nucleonWidth) << currentIsotope->N    << " "
                               << std::setw(nucleonWidth) << currentIsotope->Z    << " "
                               << std::setw(dripWidth) << currentIsotope->s_2n << "\n"
                               << std::setw(nucleonWidth) << currentIsotope->N    << " "
                               << std::setw(nucleonWidth) << currentIsotope->Z+1  << " "
                               << std::setw(dripWidth) << currentIsotope->s_2n << std::endl;

                      ++currentDoubleProton;
                    }
                }

              /// S(2p) = M(Z-2,N) - M(Z,N) + 2*M(1,0)
              if ( np == 3 && otherIsotope->N == currentIsotope->N && currentDoubleNeutron < currentIsotope->N )
                {
                  currentIsotope->s_2p = otherIsotope->ME - currentIsotope->ME + 2*meP;

                  if ( currentIsotope->s_2p < 0.0 )
                    {
                      if ( currentIsotope->N != previousDoubleNeutron )
                        {
                          dripFile << std::fixed
                                   << std::setw(nucleonWidth) << previousDoubleNeutron+1 << " "
                                   << std::setw(nucleonWidth) << previousDoubleProton    << " "
                                   << std::setw(dripWidth) << currentIsotope->s_2p         << std::endl;
                        }

                      dripFile << std::fixed
                               << std::setw(nucleonWidth) << currentIsotope->N    << " "
                               << std::setw(nucleonWidth) << currentIsotope->Z    << " "
                               << std::setw(dripWidth) << currentIsotope->s_2p << std::endl;

                      ++currentDoubleNeutron;
                      previousDoubleProton  = currentIsotope->Z;
                      previousDoubleNeutron = currentIsotope->N;
                    }
                }
            }
        }
    }

  file.close();

  dripFile.close();
}
