#include "inch/dripline.hpp"

#include <sys/stat.h>

#include "inch/options.hpp"

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <filesystem>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>


// Add definition to allow compilation older versions of gcc (<= 7)
constexpr std::pair<int, int> DripLine::single_n_lower_limits; // NOLINT (readability-redundant-declaration)
constexpr std::pair<int, int> DripLine::single_p_lower_limits; // NOLINT (readability-redundant-declaration)
constexpr std::pair<int, int> DripLine::double_n_lower_limits; // NOLINT (readability-redundant-declaration)
constexpr std::pair<int, int> DripLine::double_p_lower_limits; // NOLINT (readability-redundant-declaration)


int DripLine::createFile(const std::filesystem::path& file) const
{
  std::ifstream modelFile(FRDM_file, std::ios::binary);

  if (!modelFile)
    {
      fmt::print(stderr, "\n***ERROR***: {} couldn't be opened, does it exist?\n", FRDM_file);
      return 1;
    }

  std::ofstream dripFile(file, std::ios::binary);

  if (!dripFile)
    {
      fmt::print(stderr, "\n***ERROR***: {} couldn't be opened to write to\n", file);
      return 2;
    }

  fmt::print(dripFile,
             "# N    Z   drip[MeV]\n"
             "#------------------\n");

  // Starting values come from the FRLDM file
  int currentSingleProton{ 7 };
  int currentSingleNeutron{ 7 };
  int previousSingleNeutron{ 8 };
  int previousSingleProton{ 0 };

  int currentDoubleProton{ 7 };
  int currentDoubleNeutron{ 7 };
  int previousDoubleNeutron{ 8 };
  int previousDoubleProton{ 0 };

  // Rather than use a vector of Nuclide(216), create a new
  // smaller(56) struct to store the required values and
  // create a vector of these.
  struct isotope
  {
    int A{ 0 };
    int Z{ 0 };
    int N{ 0 };

    double s_n{ 0.0 };
    double s_2n{ 0.0 };
    double s_p{ 0.0 };
    double s_2p{ 0.0 };
    double ME{ 0.0 };
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
                      fmt::print(dripFile,
                                 "{}{}",
                                 WriteDataLine(currentIsotope->N, currentIsotope->Z, currentIsotope->s_n),
                                 WriteDataLine(currentIsotope->N, (currentIsotope->Z + 1), currentIsotope->s_n));

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
                          fmt::print(
                              dripFile,
                              "{}",
                              WriteDataLine((previousSingleNeutron + 1), previousSingleProton, currentIsotope->s_p));
                        }

                      fmt::print(
                          dripFile, "{}", WriteDataLine(currentIsotope->N, currentIsotope->Z, currentIsotope->s_p));

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
                      fmt::print(dripFile,
                                 "{}{}",
                                 WriteDataLine(currentIsotope->N, currentIsotope->Z, currentIsotope->s_n),
                                 WriteDataLine(currentIsotope->N, (currentIsotope->Z + 1), currentIsotope->s_2n));


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
                          fmt::print(
                              dripFile,
                              "{}",
                              WriteDataLine((previousDoubleNeutron + 1), previousDoubleProton, currentIsotope->s_2p));
                        }

                      fmt::print(
                          dripFile, "{}", WriteDataLine(currentIsotope->N, currentIsotope->Z, currentIsotope->s_2p));


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


void DripLine::setDripLineFile(const Options& draw) const
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
          return std::filesystem::path("NoFile");
      }
  }();

  // Check the file exists, and create it if it doesn't
  if (!std::filesystem::exists(drip_file))
    {
      FRDM_file = draw.FRDM;
      createFile(drip_file);
    }
}


int DripLine::EPSWriteLine(std::ostream& outFile) const
{
  // Format: N Z S(x)
  std::ifstream drip(drip_file, std::ios::binary);

  if (!drip)
    {
      fmt::print(stderr, "***ERROR***: {} couldn't be opened to read the drip line data\n", drip_file);
      return 1;
    }

  fmt::print("Reading {} and drawing the drip line", drip_file);

  const std::string header = [&]() {
    switch (the_line)
      {
        case LineType::singleneutron:
        default:
          return "\n%Neutron Drip Line\n";
          break;
        case LineType::doubleneutron:
          return "\n%Two Neutron Drip Line\n";
          break;
        case LineType::singleproton:
          return "\n%Proton Drip Line\n";
          break;
        case LineType::doubleproton:
          return "\n%Two Proton Drip Line\n";
          break;
      }
  }();

  fmt::print(outFile, "{}{}", header, EPSSetup());

  bool initial{ true };
  std::string line;

  while (std::getline(drip, line))
    {
      if (line.empty() || line.at(0) == '#')
        {
          continue;
        }

      int zDrip{ 0 };
      int nDrip{ 0 };

      double dummy{ 0.0 };

      std::istringstream dripData(line);
      dripData >> nDrip >> zDrip >> dummy;

      if (limits.inNRange(nDrip) && limits.inZRange(zDrip))
        {
          fmt::print(
              outFile, "{:>3d} {:>3d} {}\n", (nDrip - limits.Nmin), (zDrip - limits.Zmin), (initial ? 'm' : 'l'));

          initial = false;
        }
    }
  drip.close();

  fmt::print(outFile, "{}", EPSTearDown());

  fmt::print(" - done\n");
  return 0;
}
