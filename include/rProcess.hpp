/**
 *
 * \class rProcess
 *
 * \brief Encapsulate the reading and writing of r-process data
 */
#ifndef RPROCESS_HPP
#define RPROCESS_HPP

#include <filesystem>
#include <string>
#include <string_view>
#include <utility>
#include <vector>


class rProcess
{
public:
  rProcess(int _Zmin, int _Zmax, int _Nmin, int _Nmax) : Zmin(_Zmin), Zmax(_Zmax), Nmin(_Nmin), Nmax(_Nmax) {}

  rProcess(const rProcess&) = default;
  rProcess(rProcess&&)      = default;

  rProcess& operator=(const rProcess&) = delete;
  rProcess& operator=(rProcess&&) = delete;

  ~rProcess() = default;

  /// The N and Z range of the chart that will be drawn
  const int Zmin = 0;
  const int Zmax = 0;
  const int Nmin = 0;
  const int Nmax = 0;

  /// Minimum Z value in the file that is read
  const int min_Z = 26;

  /// The file containing the r-process data
  mutable std::filesystem::path file;

  /// A container for all of the data
  mutable std::vector<std::pair<int, int>> data;

  /**
   * Read the data file for the r-process data
   *
   * \param Nothing
   *
   * \return Nothing
   */
  bool readData() const;

  /// Specify where the data file is
  inline void setRProcessFile(std::filesystem::path _file) const noexcept { file = std::move(_file); }

  /**
   * Write the postscript code to actually draw the path
   *
   * \param An ofstream representing the chart that is being drawn
   * \param Draw the path shaded/outlined, false/true
   *
   * \return Nothing
   */
  void EPSWritePath(std::ofstream& outFile, const bool shaded) const;
};

#endif // RPROCESS_HPP
