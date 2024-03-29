#include "inch/massTable.hpp"

#include "inch/chartColour.hpp"
#include "inch/converter.hpp"
#include "inch/options.hpp"
#include "inch/partition.hpp"

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <fmt/std.h>

#include <algorithm>
#include <array>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <sstream>
#include <string>
#include <utility>


bool MassTable::populateInternalMassTable()
{
  setFilePaths(table_year);

  // Read mass table
  if (!readNUBASE(mass_table_NUBASE))
    {
      fmt::print("Nuclear data has not been read, exiting...");
      return false;
    }

  if (use_AME)
    {
      if (!readAME(mass_table_AME))
        {
          fmt::print("Values from AME were not read.");
        }
    }

  // Read user defined nuclei
  if (!user_isotopes.empty())
    {
      if (!readOWN(user_isotopes))
        {
          fmt::print("User defined nuclei have not been read.");
        }
    }
  else
    {
      fmt::print("Not drawing any user selected nuclei\n");
    }

  if (user_options.write_csv)
    {
      outputTableToCSV();
    }

  if (user_options.write_json)
    {
      outputTableToJSON();
    }

  return true;
}


bool MassTable::readAME(const std::filesystem::path& ameTable) const
{
  fmt::print("Reading {} for AME mass excess values [--", ameTable);

  std::ifstream file(ameTable, std::ios::binary);

  if (!file.is_open())
    {
      fmt::print("\n***ERROR***: {} couldn't be opened, does it exist?\n\n", ameTable);
      return false;
    }

  int line_number{ 0 };
  std::string line;

  while (std::getline(file, line))
    {
      // Skip the header of the file
      if (line_number < AME_HEADER_LENGTH)
        {
          ++line_number;
          continue;
        }

      int mass_number{ 0 };
      int proton_number{ 0 };
      int exp{ 1 };

      // Will use mass excess for criteria, the last digit is char 52 so if
      // there is a '#' but it's after this we will still say experimental
      auto measured = line.find_first_of('#');

      if (measured != std::string::npos)
        {
          std::replace(line.begin(), line.end(), '#', ' ');

          if (measured < AME_EXPERIMENTAL_MARKER)
            {
              exp = 0;
            }
        }

      mass_number = Converter::StringToInt(line, Nuclide::AME_START_A, Nuclide::AME_END_A);

      proton_number = Converter::StringToInt(line, Nuclide::AME_START_Z, Nuclide::AME_END_Z);

      const auto isotope =
          std::find_if(theTable.cbegin(), theTable.cend(), [mass_number, proton_number](const Nuclide& n) -> bool {
            return (n.A == mass_number && n.Z == proton_number);
          });

      isotope->setAMEMassExcess(line);

      isotope->setAMEMassExcessError(line);

      isotope->setExperimental(exp);
    }

  file.close();

  fmt::print("--] done\n");
  return true;
}


bool MassTable::readNUBASE(const std::filesystem::path& nubaseTable)
{
  fmt::print("Reading {} for nuclear values <--", nubaseTable);
  std::ifstream file(nubaseTable, std::ios::binary);

  if (!file.is_open())
    {
      fmt::print("\n***ERROR***: {} couldn't be opened, does it exist?\n\n", nubaseTable);
      return false;
    }

  std::array<bool, Limits::MAX_Z + 1> pnSide = {};
  pnSide.fill(false);
  std::string line;
  int state{ 0 };

  while (std::getline(file, line))
    {
      // 2012 table has some (26) lines like this:
      // 168 0630W  168Eu                                                        >300ns                  12Ku26i  2012
      // B- ?;B-n ?
      // They are theoretical but don't contain a '#' and give no ME value.
      // All of the isotopes seem to come from the same paper, so just ignore it's data for the moment.
      // Sorry whoever you are
      if (line.find("non-exist") != std::string::npos || line.find("12Ku26i") != std::string::npos)
        {
          continue;
        }
      Nuclide isotope(line);

      isotope.setExperimental();

      isotope.setA();

      isotope.setZ();

      isotope.setSymbol(Converter::ZToSymbol(isotope.Z));

      isotope.setN();

      isotope.setState(state);

      // Other isomeric levels exist, but line formatting is such a PIA!!
      // We curently only provide the option to draw 1st isomer, so only record that.
      if (state > 0)
        {
          if (state == 1)
            {
              isotope.setIsomerData(theTable, state);
            }
          continue;
        }

      isotope.setNubaseMassExcess();

      isotope.setNubaseMassExcessError();

      // Can't have a separation energy if A=1
      if (isotope.A > 1)
        {
          isotope.setSeparationEnergies(theTable);
        }

      isotope.setSpinParity();

      isotope.setHalfLife();

      // Discovery year was added after 2003
      if (table_year != 3)
        {
          isotope.setYear();
        }

      isotope.setDecayMode(pnSide, table_year);

      isotope.setNeutronOrProtonRich(pnSide);

      theTable.emplace_back(std::move(isotope));
    }

  file.close();

  fmt::print("--> done\n");
  return true;
}


bool MassTable::readOWN(const std::filesystem::path& ownTable) const
{
  fmt::print("Reading {} for user selected nuclei (--", ownTable);
  std::ifstream inFile(ownTable, std::ios::binary);

  if (!inFile.is_open())
    {
      fmt::print("\n***ERROR***: {} couldn't be opened\n\n", ownTable);
      return false;
    }

  // This file is user created, so take a bit more time and care to read
  // i.e. allow empty lines and comments
  std::string line;

  while (std::getline(inFile, line))
    {
      if (line.empty() || line.front() == '#')
        {
          continue;
        }

      int neutron_number{ 0 };
      int proton_number{ 0 };
      int state{ 0 };

      std::istringstream ownData(line);
      ownData >> neutron_number >> proton_number >> state;

      const auto isotope =
          std::find_if(theTable.cbegin(), theTable.cend(), [neutron_number, proton_number](const Nuclide& n) -> bool {
            return (n.N == neutron_number && n.Z == proton_number);
          });

      isotope->setOwn(true);
    }

  inFile.close();

  fmt::print("--) done\n");
  return true;
}


void MassTable::setFilePaths(const int tableYear) const noexcept
{
  switch (tableYear)
    {
      default:
      case 3:
        mass_table_NUBASE = Options::getAbsolutePath() / "nubtab03.asc";
        mass_table_AME    = Options::getAbsolutePath() / "mass.mas03";
        break;
      case 12:
        mass_table_NUBASE = Options::getAbsolutePath() / "nubase.mas12";
        mass_table_AME    = Options::getAbsolutePath() / "mass.mas12";
        break;
      case 16:
        mass_table_NUBASE = Options::getAbsolutePath() / "nubase2016.txt";
        mass_table_AME    = Options::getAbsolutePath() / "mass16.txt";
        break;
    }
}


void MassTable::setIsotopeAttributes(Partition& part, const Options& draw)
{
  // Using the region specified, flag that the isotope should be drawn
  // together with the corresponding part of the key.
  for (auto& isotope : theTable)
    {
      if (isotope.isShown(draw))
        {
          isotope.show = 1;

          // Error on mass excess units of keV
          if (draw.chart_colour == ChartColour::MASSEXCESSERROR)
            {
              const double mass_excess = draw.AME ? isotope.AME_dME : isotope.NUBASE_dME;

              // There should be 2 partitions to differentiate stable isotopes,
              // if the isotope is stable, we can avoid a lot of checking
              if (isotope.decay == "stable")
                {
                  const int index = (mass_excess <= part.values.front().value) ? 0 : 1;

                  isotope.colour          = part.values[index].colour;
                  part.values[index].draw = true;
                  continue;
                }

              // Only get here if the isotope is not stable
              // Can skip the first 2 partitions as they are only for stable isotopes
              isotope.colour = part.getColour(mass_excess, 2);
            }
          // Relative error on mass excess units of keV
          else if (draw.chart_colour == ChartColour::REL_MASSEXCESSERROR)
            {
              isotope.colour = part.getColour(isotope.getRelativeMassExcessError(draw.AME, min_relative_error));
            }
          // Major ground-state decay mode
          else if (draw.chart_colour == ChartColour::GS_DECAYMODE)
            {
              isotope.colour = part.getColour(isotope.decay);
            }
          // Half-life of ground-state
          else if (draw.chart_colour == ChartColour::GS_HALFLIFE)
            {
              isotope.colour = part.getColour(isotope.hl);
            }
          // 1st isomer energy
          else if (draw.chart_colour == ChartColour::FIRST_ISOMERENERGY)
            {
              if (!isotope.energy_levels.empty() && isotope.energy_levels.front().level == 1)
                {
                  isotope.colour = part.getColour(isotope.energy_levels.front().energy);
                }
              // As not every nucleus has an isomer, draw empty boxes as a visual aid
              // This relies on the vector being sorted as it was in the data file
              else
                {
                  isotope.show            = 2;
                  isotope.colour          = (isotope.decay == "stable") ? "black" : part.values.back().colour;
                  part.values.back().draw = true;
                }
            }
        }
    }
}


std::pair<int, int> MassTable::GetNeutronRange(const int proton_number, const std::string& decayMode) const
{
  int Nmin{ Limits::MAX_N };
  int Nmax{ Limits::MIN_N };

  const std::regex decay(decayMode);

  for (const auto& isotope : theTable)
    {
      if (isotope.Z == proton_number && std::regex_search(isotope.decay, decay))
        {
          if (isotope.N < Nmin)
            {
              Nmin = isotope.N;
            }

          if (isotope.N > Nmax)
            {
              Nmax = isotope.N;
            }
        }
    }

  return std::make_pair(Nmin, Nmax);
}


void MassTable::SetNeutronLimitForZ(const int proton_number, std::string_view limit) const
{
  const auto Nrange = GetNeutronRange(proton_number);

  fmt::print(
      "{}({}) has N from {} to {}", Converter::ZToSymbol(proton_number), proton_number, Nrange.first, Nrange.second);

  if (proton_number > 83 || proton_number == 43 || proton_number == 0)
    {
      fmt::print(" with no stable isotope\n");
    }
  else
    {
      const auto stableNrange = GetStableNeutronRange(proton_number);
      fmt::print(" and the {} stable isotope has N={}\n",
                 limit == "Nmin" ? "lightest" : "heaviest",
                 limit == "Nmin" ? stableNrange.first : stableNrange.second);
    }

  user_options.limits.setExtreme(limit);
}


void MassTable::setUserNeutronRange() const
{
  fmt::print("---------------------------\n"
             "Enter range of N [0,{}]\n",
             Limits::MAX_N);

  // Bottom left (N,Z)
  SetNeutronLimitForZ(user_options.limits.Zmin, "Nmin");
  // Top right (N,Z)
  SetNeutronLimitForZ(user_options.limits.Zmax, "Nmax");
}


bool MassTable::outputTableToCSV() const
{
  auto outfile = mass_table_NUBASE;
  outfile.replace_extension(".csv");

  fmt::print("New file: {}\n", outfile);
  std::ofstream out(outfile);

  if (!out.is_open())
    {
      fmt::print("\n***ERROR***: {} couldn't be opened", outfile);
      return false;
    }

  fmt::print(out, "{}\n", Nuclide::CSVHeader());
  for (const auto& isotope : theTable)
    {
      fmt::print(out, "{}\n", isotope.writeAsCSV());
    }
  out.close();

  return true;
}


bool MassTable::outputTableToJSON() const
{
  auto outfile = mass_table_NUBASE;
  outfile.replace_extension(".json");

  fmt::print("New file: {}\n", outfile);
  std::ofstream out(outfile);

  if (!out.is_open())
    {
      fmt::print("\n***ERROR***: {} couldn't be opened", outfile);
      return false;
    }

  fmt::print(out, "[\n");
  // The final element can't have a trailing comma, otherwise we'd use a range loop here
  for (auto isotope = theTable.cbegin(); isotope != theTable.cend(); ++isotope)
    {
      fmt::print(out, "{}{}", isotope->writeAsJSON(), (isotope != std::prev(theTable.end(), 1)) ? ",\n" : "");
    }
  fmt::print(out, "\n]\n");
  out.close();

  return true;
}
