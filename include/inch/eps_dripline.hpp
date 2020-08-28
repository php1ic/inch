/**
 *
 * \class DripLine
 *
 * \brief Container class for the driplines
 *
 * The date storage and creation of all four drip lines is identical, so
 * wrap it up here for convenience
 *
 */
#ifndef ESPDRIPLINE_HPP
#define ESPDRIPLINE_HPP

#include <string_view>

#include "inch/dripline.hpp"
#include "inch/limits.hpp"

#include <fmt/format.h>

#include <filesystem>
#include <fstream>
#include <string>
#include <utility>
#include <vector>


class EPSDripLine : public DripLine
{
public:
  EPSDripLine(const double nMass, const double pMass, Limits _limits, const LineType& line, std::string_view colour) :
      DripLine(nMass, pMass, std::move(_limits), line, colour)
  {
  }

  EPSDripLine(const EPSDripLine&) = default;
  EPSDripLine(EPSDripLine&&)      = default;

  /// Delete both due to const members
  EPSDripLine& operator=(const EPSDripLine&) = delete;
  EPSDripLine& operator=(EPSDripLine&&) = delete;

  ~EPSDripLine() override = default;

  /**
   * Do the necesary to prepare for writing dripline data
   *
   * \param Nothing
   *
   * \return The text necessary to start writing the drip line in the eps file
   */
  [[nodiscard]] inline std::string Setup() const override
  {
    return fmt::format("gs\n"
                       "{} rgb\n"
                       "1 u div sl\n",
                       line_colour);
  }

  /**
   * Do the necessary to clean up after writing dripline data
   *
   * \param Nothing
   *
   * \return The text necessary to tidy up after writing the drip line in the eps file
   */
  [[nodiscard]] inline std::string TearDown() const override
  {
    return fmt::format("st\n"
                       "gr\n");
  }

  /**
   * Read the necessary file for data and output eps code into the chart being created
   *
   * \param outFile An std::ostream representing the chart that is being drawn
   *
   * \return 0 Success
   * \return 1 Faliure
   */
  int WriteLine(std::ostream& outFile) const override;
};

#endif // EPSDRIPLINE_HPP
