#include "inch/eps_rProcess.hpp"

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <fstream>


std::string EPSrProcess::PathSetup(const bool shaded) const
{
  return shaded ? fmt::format("\n%r-process -- shaded path\n"
                              "gs\n"
                              "0.9 setgray\n")
                : fmt::format("\n%r-process -- path outline\n"
                              "black rgb\n"
                              "0.1 u div sl\n");
}


void EPSrProcess::WritePath(std::ofstream& outFile, const bool shaded) const
{
  fmt::print(outFile, "{}", PathSetup(shaded));

  bool initial{ true };

  for (const auto& point : data)
    {
      // it.first = N
      // it.second = Z
      if (limits.inZRange(point.second) && limits.inNRange(point.first))
        {
          fmt::print(outFile,
                     "{:>3d} {:>3d} {}\n",
                     (point.first - limits.Nmin),
                     (point.second - limits.Zmin),
                     (initial ? 'm' : 'l'));

          initial = false;
        }
    }

  fmt::print(outFile, "{}", PathTearDown(shaded));
}


std::string EPSrProcess::PathTearDown(const bool shaded) const
{
  return shaded ? fmt::format("fill\n"
                              "gr\n")
                : fmt::format("st\n");
}
