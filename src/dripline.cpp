#include "dripline.hpp"

#include "options.hpp"

#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <sys/stat.h>
#include <vector>


int DripLine::createFile(const std::string& file) const noexcept
{
  std::ifstream modelFile(FRDM_file, std::ios::binary);

  if (!modelFile)
    {
      std::cerr << "\n"
                << "***ERROR***: " << FRDM_file << " couldn't be opened, does it exist?"
                << "\n"
                << std::endl;
      return 1;
    }

  std::ofstream dripFile(file, std::ios::binary);

  if (!dripFile)
    {
      std::cerr << "\n"
                << "***ERROR***: " << file << " couldn't be opened to write to"
                << "\n"
                << std::endl;
      return 2;
    }

  dripFile << "#  N    Z   drip[MeV]\n"
           << "#------------------" << std::endl;
  dripFile.precision(4);

  // Starting values come from the FRLDM file
  int currentSingleProton   = 7;
  int currentSingleNeutron  = 7;
  int previousSingleNeutron = 8;
  int previousSingleProton  = 0;

  int currentDoubleProton   = 7;
  int currentDoubleNeutron  = 7;
  int previousDoubleNeutron = 8;
  int previousDoubleProton  = 0;

  int nucleonWidth = 4;
  int dripWidth    = 8;

  // Rather than use a vector of Nuclide(216), create a new
  // smaller(56) struct to store the required values and
  // create a vector of these.
  struct isotope
  {
    int A = 0;
    int Z = 0;
    int N = 0;

    double s_n  = 0.0;
    double s_2n = 0.0;
    double s_p  = 0.0;
    double s_2p = 0.0;
    double ME   = 0.0;
  };
  std::vector<isotope> dripNuc;

  std::string dataline;
  while (std::getline(modelFile, dataline))
    {
      if (dataline.empty() || dataline.at(0) == '#')
        {
          continue;
        }

      std::istringstream dripData(dataline);
      std::string dummy;

      dripNuc.emplace_back(isotope());
      auto currentIsotope = std::rbegin(dripNuc);

      dripData >> currentIsotope->A >> currentIsotope->Z >> dummy >> currentIsotope->ME;

      currentIsotope->N = currentIsotope->A - currentIsotope->Z;

      // File is ordered in Z then A, by looping through backwards,
      // we do not need to traverse the entire contents as we can stop
      // once values get too far apart to provide data.
      for (auto otherIsotope = currentIsotope; otherIsotope != std::rend(dripNuc); ++otherIsotope)
        {
          // Depending on the drip line, we can break when differences
          // in N or Z get too larger i.e. 2 for single particle and
          // 3 for double particle lines.
          if ((the_line == LineType::singleneutron && currentIsotope->N - otherIsotope->N >= 2)
              || (the_line == LineType::doubleneutron && currentIsotope->N - otherIsotope->N >= 3)
              || (the_line == LineType::singleproton && currentIsotope->Z - otherIsotope->Z >= 2)
              || (the_line == LineType::doubleproton && currentIsotope->Z - otherIsotope->Z >= 3))
            {
              break;
            }

          if (currentIsotope->A - otherIsotope->A == 1)
            {
              // S(n) = M(Z,N-1) - M(Z,N) + M(0,1)
              if (the_line == LineType::singleneutron && otherIsotope->Z == currentIsotope->Z
                  && currentSingleProton < currentIsotope->Z)
                {
                  currentIsotope->s_n = otherIsotope->ME - currentIsotope->ME + neutron_mass;

                  if (currentIsotope->s_n < 0.0)
                    {
                      dripFile << std::fixed << std::setw(nucleonWidth) << currentIsotope->N << " "
                               << std::setw(nucleonWidth) << currentIsotope->Z << " " << std::setw(dripWidth)
                               << currentIsotope->s_n << "\n"
                               << std::setw(nucleonWidth) << currentIsotope->N << " " << std::setw(nucleonWidth)
                               << currentIsotope->Z + 1 << " " << std::setw(dripWidth) << currentIsotope->s_n
                               << std::endl;

                      ++currentSingleProton;
                    }
                }

              // S(p) = M(Z-1,N) - M(Z,N) + M(1,0)
              if (the_line == LineType::singleproton && otherIsotope->N == currentIsotope->N
                  && currentSingleNeutron < currentIsotope->N)
                {
                  currentIsotope->s_p = otherIsotope->ME - currentIsotope->ME + proton_mass;

                  if (currentIsotope->s_p < 0.0)
                    {
                      if (currentIsotope->N != previousSingleNeutron)
                        {
                          dripFile << std::fixed << std::setw(nucleonWidth) << previousSingleNeutron + 1 << " "
                                   << std::setw(nucleonWidth) << previousSingleProton << " " << std::setw(dripWidth)
                                   << currentIsotope->s_p << std::endl;
                        }

                      dripFile << std::fixed << std::setw(nucleonWidth) << currentIsotope->N << " "
                               << std::setw(nucleonWidth) << currentIsotope->Z << " " << std::setw(dripWidth)
                               << currentIsotope->s_p << std::endl;

                      ++currentSingleNeutron;
                      previousSingleProton  = currentIsotope->Z;
                      previousSingleNeutron = currentIsotope->N;
                    }
                }
            }
          else if (currentIsotope->A - otherIsotope->A == 2)
            {
              // S(2n) = M(Z,N-2) - M(Z,N) + 2*M(0,1)
              if (the_line == LineType::doubleneutron && otherIsotope->Z == currentIsotope->Z
                  && currentDoubleProton < currentIsotope->Z)
                {
                  currentIsotope->s_2n = otherIsotope->ME - currentIsotope->ME + 2.0 * neutron_mass;

                  if (currentIsotope->s_2n < 0.0)
                    {
                      dripFile << std::fixed << std::setw(nucleonWidth) << currentIsotope->N << " "
                               << std::setw(nucleonWidth) << currentIsotope->Z << " " << std::setw(dripWidth)
                               << currentIsotope->s_2n << "\n"
                               << std::setw(nucleonWidth) << currentIsotope->N << " " << std::setw(nucleonWidth)
                               << currentIsotope->Z + 1 << " " << std::setw(dripWidth) << currentIsotope->s_2n
                               << std::endl;

                      ++currentDoubleProton;
                    }
                }

              // S(2p) = M(Z-2,N) - M(Z,N) + 2*M(1,0)
              if (the_line == LineType::doubleproton && otherIsotope->N == currentIsotope->N
                  && currentDoubleNeutron < currentIsotope->N)
                {
                  currentIsotope->s_2p = otherIsotope->ME - currentIsotope->ME + 2.0 * proton_mass;

                  if (currentIsotope->s_2p < 0.0)
                    {
                      if (currentIsotope->N != previousDoubleNeutron)
                        {
                          dripFile << std::fixed << std::setw(nucleonWidth) << previousDoubleNeutron + 1 << " "
                                   << std::setw(nucleonWidth) << previousDoubleProton << " " << std::setw(dripWidth)
                                   << currentIsotope->s_2p << std::endl;
                        }

                      dripFile << std::fixed << std::setw(nucleonWidth) << currentIsotope->N << " "
                               << std::setw(nucleonWidth) << currentIsotope->Z << " " << std::setw(dripWidth)
                               << currentIsotope->s_2p << std::endl;

                      ++currentDoubleNeutron;
                      previousDoubleProton  = currentIsotope->Z;
                      previousDoubleNeutron = currentIsotope->N;
                    }
                }
            }
        }
    }

  modelFile.close();

  dripFile.close();

  return 0;
}


void DripLine::setDripLineFile(const Options& draw) const noexcept
{
  drip_file = [&]() {
    switch (the_line)
      {
        case LineType::singleneutron:
          return draw.neutron_drip;
        case LineType::doubleneutron:
          return draw.two_neutron_drip;
        case LineType::singleproton:
          return draw.proton_drip;
        case LineType::doubleproton:
          return draw.two_proton_drip;
        default:
          return std::string("NoFile");
      }
  }();

  // Check the file exists, and create it if it doesn't
  struct stat buffer
  {
  };

  if (stat(drip_file.c_str(), &buffer) != 0)
    {
      FRDM_file = draw.FRDM;
      createFile(drip_file);
    }
}


int DripLine::EPSWriteLine(std::ostream& outFile) const noexcept
{
  // Format: N Z S(x)
  std::ifstream drip(drip_file, std::ios::binary);

  if (!drip)
    {
      std::cerr << "***ERROR***: " << drip_file << " couldn't be opened to read the drip line data" << std::endl;
      return 1;
    }

  std::cout << "Reading " << drip_file << " and drawing the drip line";

  switch (the_line)
    {
      case LineType::singleneutron:
        outFile << "\n%Neutron Drip Line\n";
        break;
      case LineType::doubleneutron:
        outFile << "\n%Two Neutron Drip Line\n";
        break;
      case LineType::singleproton:
        outFile << "\n%Proton Drip Line\n";
        break;
      case LineType::doubleproton:
        outFile << "\n%Two Proton Drip Line\n";
        break;
    }
  outFile << "gs\n"
          << line_colour << " rgb\n"
          << "1 u div sl" << std::endl;

  bool initial = true;
  std::string line;

  while (std::getline(drip, line))
    {
      if (line.empty() || line.at(0) == '#')
        {
          continue;
        }

      int zDrip = 0;
      int nDrip = 0;

      double dummy = 0.0;

      std::istringstream dripData(line);
      dripData >> nDrip >> zDrip >> dummy;

      if (zDrip >= Zmin && zDrip <= Zmax && nDrip >= Nmin && nDrip <= Nmax)
        {
          outFile << std::setw(3) << nDrip - Nmin << " " << std::setw(3) << zDrip - Zmin << " " << (initial ? 'm' : 'l')
                  << '\n';

          if (initial)
            {
              initial = false;
            }
        }
    }
  drip.close();

  outFile << "st\n"
          << "gr" << std::endl;

  std::cout << std::endl;
  return 0;
}
