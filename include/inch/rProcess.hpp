/**
 *
 * \class rProcess
 *
 * \brief Encapsulate the reading and writing of r-process data
 */
#ifndef RPROCESS_HPP
#define RPROCESS_HPP

#include <string_view>

#include "inch/limits.hpp"

#include <filesystem>
#include <string>
#include <utility>
#include <vector>

class rProcess
{
public:
  explicit rProcess(Limits _limits) : limits(std::move(_limits)) {}

  rProcess(const rProcess&) = default;
  rProcess(rProcess&&)      = default;

  rProcess& operator=(const rProcess&) = delete;
  rProcess& operator=(rProcess&&) = delete;

  ~rProcess() = default;

  /// Store the N,Z limits of the chart being created
  const Limits limits;

  /// Minimum Z value in the file that is read
  static constexpr int min_Z{ 26 };

  /// The file containing the r-process data
  mutable std::filesystem::path file{};

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

private:
  /**
   *
   */
  std::string EPSPathSetup(const bool shaded) const;

  /**
   *
   */
  std::string EPSPathTearDown(const bool shaded) const;
};

#endif // RPROCESS_HPP
