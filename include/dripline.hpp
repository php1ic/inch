#ifndef DRIPLINE_HPP
#define DRIPLINE_HPP

#include <fstream>
#include <memory>
#include <string>

class Options;

enum class LineType
{
  singleneutron = 0,
  doubleneutron = 1,
  singleproton  = 2,
  doubleproton  = 3
};


class DripLine
{
public:
  // Constructors
  // default
  DripLine(double nMass, double pMass, int minZ, int maxZ, int minN, int maxN, LineType line) :
      neutron_mass(nMass),
      proton_mass(pMass),
      Zmin(minZ),
      Zmax(maxZ),
      Nmin(minN),
      Nmax(maxN),
      the_line(line)
  {
  }
  // copy
  DripLine(const DripLine&) = default;
  // move
  DripLine(DripLine&&) = default;

  // Assignment
  /// Delete both due to const members
  // copy
  DripLine& operator=(const DripLine&) = delete;
  // move
  DripLine& operator=(DripLine&&) = delete;

  // Destructor
  ~DripLine() = default;

  mutable double neutron_mass = 0.0;
  mutable double proton_mass  = 0.0;

  const int Zmin = 0;
  const int Zmax = 0;
  const int Nmin = 0;
  const int Nmax = 0;

  const LineType the_line = LineType::singleneutron;

  mutable std::string FRDM_file;
  mutable std::string drip_file;
  mutable std::string line_colour;

  inline void setDripLineColour(const std::string& colour) const noexcept { line_colour = colour; }

  int createFile(const std::string& file) const noexcept;
  int EPSWriteLine(std::ostream& outFile) const noexcept;
  void setDripLineFile(const Options& draw) const noexcept;
};

#endif // DRIPLINE_HPP
