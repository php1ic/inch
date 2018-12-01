#ifndef MAGICNUMBERS_HPP
#define MAGICNUMBERS_HPP

#include "inputs.hpp"

#include <fstream>
#include <vector>

class MagicNumbers
{
public:
  // Constructor
  // default
  MagicNumbers(int minZ, int maxZ, int minN, int maxN) :
      Zmin(minZ),
      Zmax(maxZ),
      Nmin(minN),
      Nmax(maxN),
      numbers{ 2, 8, 20, 28, 50, 82, 126 },
      values{
        { 0, 22, 0, 12 },    // 2
        { 1, 30, 0, 22 },    // 8
        { 7, 52, 1, 33 },    // 20
        { 16, 56, 7, 38 },   // 28
        { 45, 92, 19, 56 },  // 50
        { 81, 140, 43, 84 }, // 82
        { 0, 0, 76, 95 }     // Z<=118 so Z=126 magic number is not needed
      }
  {
    constructMap();
  }
  // copy
  MagicNumbers(const MagicNumbers&) = default;
  // move
  MagicNumbers(MagicNumbers&&) noexcept = default;

  // Assignment
  // copy
  MagicNumbers& operator=(const MagicNumbers&) = delete;
  // move
  MagicNumbers& operator=(MagicNumbers&&) = delete;

  // Destructor
  ~MagicNumbers() noexcept = default;

  struct Number
  {
    Number(int minN, int maxN, int minZ, int maxZ) : min_n{ minN }, max_n{ maxN }, min_z{ minZ }, max_z{ maxZ } {}

    int min_n;
    int max_n;
    int min_z;
    int max_z;
  };

  const int Zmin;
  const int Zmax;
  const int Nmin;
  const int Nmax;

  const int min_val = -1;
  const int max_val = 2;

  mutable std::vector<int> numbers;
  mutable std::vector<Number> values;
  mutable std::map<int, Number> coords;

  inline void EPSSetup(std::ofstream& outFile) const noexcept
  {
    outFile << "\n%Magic Numbers\n"
            << "gs\n"
            << "black rgb\n"
            << "1 u div sl" << std::endl;
  }

  inline void EPSTearDown(std::ostream& outFile) const noexcept { outFile << "gr" << std::endl; }

  void EPSWriteProtonNumber(std::ofstream& outFile, const int number) const;
  void EPSWriteNeutronNumber(std::ofstream& outFile, const int number) const;

private:
  void constructMap() const;
};

#endif // MAGICNUMBERS_HPP
