#include "inch/dripline.hpp"

#include <sys/stat.h>

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

// We need to define this member before we can use it
std::vector<DripLine::drop_model_data> DripLine::dm_data;


bool DripLine::readFRDMFile(const bool overwrite) const
{
  // Don't overwrite the existing data structure by default
  if (!DripLine::dm_data.empty() && !overwrite)
    {
      return true;
    }

  // Prepend the path to where the data files are
  const auto theFile = Options::getAbsolutePath() / FRDM_file;

  std::ifstream modelFile(theFile, std::ios::binary);

  if (!modelFile.is_open())
    {
      fmt::print(stderr, "\n***ERROR***: {} couldn't be opened, does it exist?\n", FRDM_file);
      return false;
    }

  fmt::print("Reading {} for additional ME data\n", FRDM_file);

  // We know the following about the file we are reading
  // - header is 7 lines long
  // - the '#' character represents comments
  // - no comments after the header
  // - no empty lines

  // Ignore the header
  constexpr int header_lines{ 7 };
  for (int i = 0; i < header_lines; ++i)
    {
      modelFile.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
    }

  // Read the remainder of the file into the container created for it
  int A{ 0 };
  int Z{ 0 };
  double ME{ 0.0 };
  std::string dummy;

  while (modelFile >> A >> Z >> dummy >> ME)
    {
      DripLine::dm_data.emplace_back(drop_model_data(A, Z, ME));
    }

  modelFile.close();
  return true;
}


int DripLine::GetDripValue(const int number, std::string_view particle) const
{
  // The equations are similar once you have the correct isotopes, main difference is the multiplication factor on the
  // n0 or p+ mass.
  // Store that factor here so we can create a generic algorithm
  const int particle_line = (the_line == LineType::singleproton || the_line == LineType::singleneutron) ? 1 : 2;
  // Set the required particle mass here so it's only done once
  const double particle_mass =
      (the_line == LineType::singleproton || the_line == LineType::doubleproton) ? proton_mass : neutron_mass;

  // Initialise the numbe we are going to return
  // Use a non physical number so we know if a drip value has not been calculated
  int drip_number{ 0 };

  // Assume the vector, like the file it's data was read from, is ordered by Z then A
  for (const auto isotope : dm_data)
    {
      // We need access to both N and Z in all cases
      const int RequestedConstValue    = (particle == "Z") ? isotope.Z : isotope.N;
      const int RequestedVariableValue = (particle == "Z") ? isotope.N : isotope.Z;

      // We are only interested in the specified <number>
      // If the current value is not what we want, skip to the next
      if (RequestedConstValue != number)
        {
          continue;
        }

      //// For the case of Z being requested, the ordering of the data means that once the current Z value is too high
      //// there is no point in continuing to check values
      //// This will save processing, but is entirely dependent on the ordering of the data.
      // else if (!particle.compare("Z") && RequestedConstValue > number)
      //  {
      //    break;
      //  }

      // We now have (N,Z) as <isotope> and need to find the matching isotope to calculate the separation energy
      // See equations below for 'matching' isotopes
      //
      // S_n  = M(Z,N-1) - M(Z,N) + 1*M(0,1)
      // S_2n = M(Z,N-2) - M(Z,N) + 2*M(0,1)
      // S_p  = M(Z-1,N) - M(Z,N) + 1*M(1,0)
      // S_2p = M(Z-2,N) - M(Z,N) + 2*M(1,0)
      for (const auto otherIsotope : dm_data)
        {
          const int ComparisonConstValue    = (particle == "Z") ? otherIsotope.Z : otherIsotope.N;
          const int ComparisonVariableValue = (particle == "Z") ? otherIsotope.N : otherIsotope.Z;

          // Equations necessitate the not requested particle number is equal
          //  OR
          // If the difference in values does not provide the correct isotopes keep going
          if (RequestedConstValue != ComparisonConstValue
              || (RequestedVariableValue - ComparisonVariableValue) != particle_line)
            {
              continue;
            }

          // Calculate separation energy
          // If it's negative we have found the drip line. Store the nucleon value and get out
          if ((otherIsotope.ME - isotope.ME + (particle_line * particle_mass)) < 0.0)
            {
              drip_number = RequestedVariableValue;
              // fmt::print(">>>{} {} {}\n", particle, number, drip_number);
              break;
            }
        }

      // Once <drip_number> has been set we can stop processing data and return
      if (drip_number != 0)
        {
          break;
        }
    }

  return drip_number;
}


int DripLine::createFileIfDoesNotExist() const
{
  // Make sure the filename has been set that we are going to write to
  if (drip_file.compare("") == 0)
    {
      setDripLineFile();
    }

  // Check the file exists, and create it if it doesn't
  if (!std::filesystem::exists(drip_file))
    {
      // Make sure the ME data is stored for all the simulated values
      // setDropModelFile(DropModelFile);
      createFile();
    }

  return 0;
}


int DripLine::createFile() const
{
  readFRDMFile();

  std::ofstream dripFile(drip_file, std::ios::binary);

  if (!dripFile.is_open())
    {
      fmt::print(stderr, "\n***ERROR***: {} couldn't be opened to write to\n", drip_file);
      return 1;
    }

  // Generic header
  fmt::print(dripFile, "#  N   Z\n");

  // For the neutron drip line, loop through all Z and look for the N value when the separation energy is negative
  if (the_line == LineType::singleneutron || the_line == LineType::doubleneutron)
    {
      for (int z = DripLine::single_p_lower_limits.first; z <= Limits::MAX_Z; ++z)
        {
          // Ignore cases when there is no dripline
          if (auto N = GetNeutronDripValue(z); N != 0)
            {
              fmt::print(dripFile, "{}", WriteDataLine(N, z));
            }
        }
    }
  // For the proton drip line, loop through all N and look for the Z value when the separation energy is negative
  else // must be a proton dripline
    {
      for (int n = DripLine::single_n_lower_limits.second; n <= Limits::MAX_N; ++n)
        {
          // Ignore cases when there is no dripline
          if (auto Z = GetProtonDripValue(n); Z != 0)
            {
              fmt::print(dripFile, "{}", WriteDataLine(n, Z));
            }
        }
    }

  dripFile.close();

  return 0;
}


void DripLine::setDripLineFile() const
{
  const auto data_path = Options::getAbsolutePath();

  drip_file = [&]() {
    switch (the_line)
      {
        case LineType::singleneutron:
          return data_path / neutron_drip;
        case LineType::doubleneutron:
          return data_path / two_neutron_drip;
        case LineType::singleproton:
          return data_path / proton_drip;
        case LineType::doubleproton:
          return data_path / two_proton_drip;
        default:
          return std::filesystem::path("NoFile");
      }
  }();

  createFileIfDoesNotExist();
}


int DripLine::EPSWriteLine(std::ostream& outFile) const
{
  // Format: N Z
  std::ifstream drip(drip_file, std::ios::binary);

  if (!drip.is_open())
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

  // We make the file that is read and it always has
  // - one line as a header
  // - no empty lines
  // - no additional comments

  // Ignore header
  drip.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

  // Read the remainder of the file
  bool initial{ true };
  int zDrip{ 0 };
  int nDrip{ 0 };

  while (drip >> nDrip >> zDrip)
    {
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
