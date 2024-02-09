/**
 *
 * \class rProcess
 *
 * \brief Encapsulate the reading and writing of r-process data
 */
#ifndef RPROCESS_HPP
#define RPROCESS_HPP

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
  rProcess& operator=(rProcess&&)      = delete;

  virtual ~rProcess() = default;

  /// Store the N,Z limits of the chart being created
  mutable Limits limits;

  /// Minimum Z value in the file that is read
  static constexpr int min_Z{ 26 };

  /// The file containing the r-process data
  mutable std::filesystem::path file{ "r-process.dat" };

  /// A container for all of the data
  mutable std::vector<std::pair<int, int>> data;

  /// Specify where the data file is
  inline void setRProcessFile(std::filesystem::path _file) const noexcept { file = std::move(_file); }

  /// Check if the Zmax value means we should or should not draw the r-process
  [[nodiscard]] inline bool isZHighEnough() const { return limits.Zmax > min_Z; }

  /**
   * Read the data file for the r-process data
   *
   * \param Nothing
   *
   * \return Nothing
   */
  bool readData() const;

  /**
   * Write the postscript code to actually draw the path
   *
   * \param An ofstream representing the chart that is being drawn
   * \param Draw the path shaded/outlined, false/true
   *
   * \return Nothing
   */
  virtual void WritePath(std::ofstream& outFile, const bool shaded) const = 0;

  /**
   * Get the necessary text to preparte postscript for writing the r-process.
   * The value <shaded> specifies if we should shade to outline the region drawn.
   *
   * \param Should the region be shaded/outlines, true/false
   *
   * \return The text required that can than be written to a file
   */
  virtual std::string PathSetup(const bool shaded) const = 0;

  /**
   * Tidy up after writing the r-process
   *
   * \param Are we tyding up after drawing a shaded or outlined region
   *
   * \return The text required that can than be written to a file
   */
  virtual std::string PathTearDown(const bool shaded) const = 0;
};

#endif // RPROCESS_HPP
