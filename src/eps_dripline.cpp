#include "inch/eps_dripline.hpp"

#include "inch/options.hpp"

#include <fmt/ostream.h>

#include <algorithm>
#include <filesystem>
#include <limits>
#include <sstream>
#include <vector>


int EPSDripLine::WriteLine(std::ostream& outFile) const
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
        case LineType::doubleneutron:
          return "\n%Two Neutron Drip Line\n";
        case LineType::singleproton:
          return "\n%Proton Drip Line\n";
        case LineType::doubleproton:
          return "\n%Two Proton Drip Line\n";
      }
  }();

  fmt::print(outFile, "{}{}", header, Setup());

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

  fmt::print(outFile, "{}", TearDown());

  fmt::print(" - done\n");
  return 0;
}
