#ifndef RPROCESS_HPP
#define RPROCESS_HPP

#include <string>
#include <utility>
#include <vector>


class rProcess
{
public:
  // Constructors
  // default
  rProcess(int minZ, int maxZ, int minN, int maxN) : Zmin(minZ), Zmax(maxZ), Nmin(minN), Nmax(maxN) {}
  // copy
  rProcess(const rProcess&) = default;
  // move
  rProcess(rProcess&&) = default;

  // Assignment
  /// Delete both due to const members
  // copy
  rProcess& operator=(const rProcess&) = delete;
  // move
  rProcess& operator=(rProcess&&) = delete;

  // Destructor
  ~rProcess() = default;

  const int Zmin = 0;
  const int Zmax = 0;
  const int Nmin = 0;
  const int Nmax = 0;

  mutable std::string file;

  std::vector<std::pair<int, int>> data;

  bool readData();

  inline void setRProcessFile(const std::string& _file) const noexcept { file = _file; }
  void EPSWritePath(std::ofstream& outFile, const bool shaded) const;
};

#endif // RPROCESS_HPP
