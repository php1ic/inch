#include "massTable.hpp"

#include "chartColour.hpp"
#include "converter.hpp"
#include "inputs.hpp"
#include "partition.hpp"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>


void MassTable::populateInternalMassTable()
{
  setFilePaths(table_year);

  // Read mass table
  if (!readNUBASE(mass_table_NUBASE))
    {
      std::cout << "Nuclear data has not been read, exiting..." << std::endl;
      exit(-1);
    }

  if (use_AME)
    {
      if (!readAME(mass_table_AME))
        {
          std::cout << "Values from AME were not read." << std::endl;
        }
    }

  // Read user defined nuclei
  if (!user_isotopes.empty())
    {
      if (!readOWN(user_isotopes))
        {
          std::cout << "User defined nuclei have not been read." << std::endl;
        }
    }
  else
    {
      std::cout << "Not drawing any user selected nuclei" << std::endl;
    }
}


bool MassTable::readAME(const std::string& ameTable)
{
  std::cout << "Reading " << ameTable << " for AME mass excess values [--";

  std::ifstream file(ameTable, std::ios::binary);

  if (!file)
    {
      std::cout << "\n"
                << "***ERROR***: " << ameTable << " couldn't be opened, does it exist?\n"
                << std::endl;
      return false;
    }

  int i = 0;
  std::string line;

  const Nuclide isotope("");
  while (std::getline(file, line))
    {
      // Skip the header of the file
      if (i < AME_HEADER_LENGTH)
        {
          ++i;
          continue;
        }

      int A   = 0;
      int Z   = 0;
      int exp = 1;

      // Will use mass excess for criteria, the last digit is char 52 so if
      // there is a '#' but it's after this we will still say experimental
      auto measured = line.find_first_of('#');

      if (measured != std::string::npos)
        {
          std::replace(std::begin(line), std::end(line), '#', ' ');

          if (measured < AME_EXPERIMENTAL_MARKER)
            {
              exp = 0;
            }
        }

      isotope.extractValue(line, Nuclide::AME_START_A, Nuclide::AME_END_A, A);

      isotope.extractValue(line, Nuclide::AME_START_Z, Nuclide::AME_END_Z, Z);

      const auto it = std::find_if(std::cbegin(theTable), std::cend(theTable), [A, Z](const Nuclide& n) -> bool {
        return (n.A == A && n.Z == Z);
      });

      it->setAMEMassExcess(line);

      it->setAMEMassExcessError(line);

      it->setExperimental(exp);
    }

  file.close();

  std::cout << "--] done" << std::endl;
  return true;
}


bool MassTable::readNUBASE(const std::string& nubaseTable)
{
  std::cout << "Reading " << nubaseTable << " for nuclear values <--";

  std::ifstream file(nubaseTable, std::ios::binary);

  if (!file)
    {
      std::cout << "\n"
                << "***ERROR***: " << nubaseTable << " couldn't be opened, does it exist?\n"
                << std::endl;
      return false;
    }

  std::vector<bool> pnSide(MAX_Z + 1, false);
  std::string line;
  int state = 0;

  const Converter converter;

  while (std::getline(file, line))
    {
      if (line.find("non-exist") != std::string::npos)
        {
          continue;
        }

      Nuclide isotope(line);

      isotope.setExperimental();

      isotope.setA();

      isotope.setZ();

      isotope.setSymbol(converter.convertZToSymbol(isotope.Z));

      isotope.setN();

      isotope.setState(state);

      if (state > 0)
        {
          isotope.setIsomerData(theTable, state);
          continue;
        }

      isotope.setNubaseMassExcess();

      isotope.setNubaseMassExcessError();

      if (isotope.A > 1)
        {
          isotope.setSeparationEnergies(theTable);
        }

      isotope.setSpinParity();

      isotope.setHalfLife();

      isotope.setDecayMode(pnSide, table_year);

      isotope.setNeutronOrProtonRich(pnSide);

      theTable.emplace_back(std::move(isotope));
    }

  file.close();

  std::cout << "--> done" << std::endl;
  return true;
}


bool MassTable::readOWN(const std::string& ownTable)
{
  std::cout << "Reading " << ownTable << " for user selected nuclei (--";

  std::ifstream inFile(ownTable, std::ios::binary);

  if (!inFile)
    {
      std::cout << "\n"
                << "***ERROR***: " << ownTable << " couldn't be opened.\n"
                << std::endl;
      return false;
    }

  std::string line;

  while (std::getline(inFile, line))
    {
      if (line.empty() || line.at(0) == '#')
        {
          continue;
        }

      int N  = 0;
      int Z  = 0;
      int st = 0;

      std::istringstream ownData(line);
      ownData >> N >> Z >> st;

      const auto it = std::find_if(std::cbegin(theTable), std::cend(theTable), [N, Z](const Nuclide& n) -> bool {
        return (n.N == N && n.Z == Z);
      });

      it->setOwn(true);
    }

  inFile.close();

  std::cout << "--) done" << std::endl;

  return true;
}


void MassTable::setFilePaths(const int tableYear) const noexcept
{
  switch (tableYear)
    {
      default:
      case 3:
        mass_table_NUBASE = data_path + "nubtab03.asc";
        mass_table_AME    = data_path + "mass.mas03";
        break;
      case 12:
        mass_table_NUBASE = data_path + "nubase.mas12";
        mass_table_AME    = data_path + "mass.mas12";
        break;
      case 16:
        mass_table_NUBASE = data_path + "nubase2016.txt";
        mass_table_AME    = data_path + "mass16.txt";
        break;
    }
}


void MassTable::setIsotopeAttributes(std::unique_ptr<Partition>& part, const std::unique_ptr<inputs>& draw)
{
  /// Using the region specified, flag that the isotope should be drawn
  /// together with the corresponding part of the key.
  for (auto& it : theTable)
    {
      if (it.Z >= draw->Zmin && it.Z <= draw->Zmax && it.N >= draw->Nmin && it.N <= draw->Nmax
          && it.exp != static_cast<int>(draw->chart_type) && it.rich % draw->np_rich == 0)
        {
          /// Error on mass excess units of keV
          if (draw->chart_colour == ChartColour::MASSEXCESSERROR)
            {
              it.show = 1;

              const double me = draw->AME ? it.AME_dME : it.NUBASE_dME;

              /// There should be 2 partitions to differentiate stable isotopes,
              /// if the isotope is stable, we can avoid a lot of checking
              if (it.decay == "stable")
                {
                  const int index = (me <= part->values.front().value) ? 0 : 1;

                  it.colour                = part->values[index].colour;
                  part->values[index].draw = true;
                  continue;
                }

              /// Only get here if the isotope is not stable
              /// Can skip the first 2 partitions as they are only for stable isotopes
              auto val = std::find_if(std::next(std::begin(part->values),2),
                                      std::end(part->values),
                                      [me](const Partition::section& s) -> bool { return (me <= s.value); });

              it.colour = val->colour;
              val->draw = true;
            }
          /// Relative error on mass excess units of keV
          else if (draw->chart_colour == ChartColour::REL_MASSEXCESSERROR)
            {
              it.show = 1;

              constexpr double min = 1.0e-7;
              const double dme     = [&]() {
                /// Be explicit as switch statements implicitly convert bool -> int
                switch (static_cast<int>(draw->AME))
                  {
                    case 1: /// true
                      return (fabs(it.AME_ME) < min) ? 0.0 : fabs(it.AME_dME / it.AME_ME);
                    case 0: /// false
                    default:
                      return (fabs(it.NUBASE_ME) < min) ? 0.0 : fabs(it.NUBASE_dME / it.NUBASE_ME);
                  }
              }();

              auto val = std::find_if(std::begin(part->values),
                                      std::end(part->values),
                                      [dme](const Partition::section& s) -> bool { return (dme <= s.value); });

              it.colour = val->colour;
              val->draw = true;
            }
          /// Major ground-state decay mode
          else if (draw->chart_colour == ChartColour::GS_DECAYMODE)
            {
              it.show = 1;

              if (it.decay == "stable")
                {
                  it.colour            = part->values[0].colour;
                  part->values[0].draw = true;
                }
              else if (it.decay == "A")
                {
                  it.colour            = part->values[1].colour;
                  part->values[1].draw = true;
                }
              else if (it.decay == "B-")
                {
                  it.colour            = part->values[2].colour;
                  part->values[2].draw = true;
                }
              else if (it.decay == "B+")
                {
                  it.colour            = part->values[3].colour;
                  part->values[3].draw = true;
                }
              else if (it.decay == "SF")
                {
                  it.colour            = part->values[4].colour;
                  part->values[4].draw = true;
                }
              else if (it.decay == "n")
                {
                  it.colour            = part->values[5].colour;
                  part->values[5].draw = true;
                }
              else if (it.decay == "2n")
                {
                  it.colour            = part->values[6].colour;
                  part->values[6].draw = true;
                }
              else if (it.decay == "p")
                {
                  it.colour            = part->values[7].colour;
                  part->values[7].draw = true;
                }
              else if (it.decay == "2p")
                {
                  it.colour            = part->values[8].colour;
                  part->values[8].draw = true;
                }
              else if (it.decay == "unknown")
                {
                  it.colour            = part->values[9].colour;
                  part->values[9].draw = true;
                }
              else if (it.decay == "EC")
                {
                  it.colour             = part->values[10].colour;
                  part->values[10].draw = true;
                }
            }
          /// Half-life of ground-state
          else if (draw->chart_colour == ChartColour::GS_HALFLIFE)
            {
              it.show = 1;

              auto val = std::find_if(std::begin(part->values),
                                      std::end(part->values),
                                      [&it](const Partition::section& s) -> bool { return (it.hl <= s.value); });

              it.colour = val->colour;
              val->draw = true;
            }
          /// 1st isomer energy
          else if (draw->chart_colour == ChartColour::FIRST_ISOMERENERGY)
            {
              if (!it.energy_levels.empty() && it.energy_levels.front().level == 1)
                {
                  it.show = 1;

                  auto val = std::find_if(
                      std::begin(part->values), std::end(part->values), [&it](const Partition::section& s) -> bool {
                        return (it.energy_levels.front().energy <= s.value);
                      });

                  it.colour = val->colour;
                  val->draw = true;
                }
              /// As not every nucleus has an isomer, draw empty boxes as a visual aid
              /// This relies on the vector being sorted as it was in the data file
              else
                {
                  it.show                  = 2;
                  it.colour                = part->values.back().colour;
                  part->values.back().draw = true;
                }
            }
        }
    }
}
