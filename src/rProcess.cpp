#include "inch/rProcess.hpp"

#include "inch/options.hpp"

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <fmt/std.h>

#include <fstream>
#include <limits>


bool rProcess::readData() const
{
  data.reserve(150);

  const auto theFile = Options::getAbsolutePath() / file;

  fmt::print("Reading {} for the r-process nuclei", theFile);

  std::ifstream rp(theFile, std::ios::binary);

  if (!rp.is_open())
    {
      fmt::print(stderr, "***ERROR***: {} couldn't be opened to read the r-process path\n", file);
      return false;
    }

  int n{ 0 };
  int z{ 0 };
  constexpr int header_size{ 4 };

  // Know the file format, and know:
  // - # are comments
  // - header is 4 lines long
  // - there are no empty lines
  for (int i = 0; i < header_size; ++i)
    {
      rp.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
    }

  while (rp >> n >> z)
    {
      data.emplace_back(n, z);
    }

  rp.close();

  fmt::print(" - done\n");

  return true;
}
