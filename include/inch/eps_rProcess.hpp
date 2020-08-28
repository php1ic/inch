/**
 *
 * \class rProcess
 *
 * \brief Encapsulate the reading and writing of r-process data
 */
#ifndef EPSRPROCESS_HPP
#define EPSRPROCESS_HPP

#include "inch/limits.hpp"
#include "inch/rProcess.hpp"

#include <filesystem>
#include <string>
#include <utility>
#include <vector>

class EPSrProcess : public rProcess
{
public:
  explicit EPSrProcess(Limits _limits) : rProcess(std::move(_limits)) {}

  EPSrProcess(const EPSrProcess&) = default;
  EPSrProcess(EPSrProcess&&)      = default;

  EPSrProcess& operator=(const EPSrProcess&) = delete;
  EPSrProcess& operator=(EPSrProcess&&) = delete;

  ~EPSrProcess() override = default;

  /**
   * Write the postscript code to actually draw the path
   *
   * \param An ofstream representing the chart that is being drawn
   * \param Draw the path shaded/outlined, false/true
   *
   * \return Nothing
   */
  void WritePath(std::ofstream& outFile, const bool shaded) const override;

  /**
   * Get the necessary text to preparte postscript for writing the r-process.
   * The value <shaded> specifies if we should shade to outline the region drawn.
   *
   * \param Should the region be shaded/outlines, true/false
   *
   * \return The text required that can than be written to a file
   */
  std::string PathSetup(const bool shaded) const override;

  /**
   * Tidy up after writing the r-process
   *
   * \param Are we tyding up after drawing a shaded or outlined region
   *
   * \return The text required that can than be written to a file
   */
  std::string PathTearDown(const bool shaded) const override;
};

#endif // EPSRPROCESS_HPP
