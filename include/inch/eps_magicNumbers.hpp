/**
 *
 * \class MagicNumbers
 *
 * \brief Encapsulation of the magic number lines
 *
 * All magic numbers are identical beyond their start and end points
 * wrap it up here
 *
 */
#ifndef EPSMAGICNUMBERS_HPP
#define EPSMAGICNUMBERS_HPP

#include "inch/limits.hpp"
#include "inch/magicNumbers.hpp"

#include <fmt/format.h>

#include <algorithm>
#include <iterator>
#include <map>
#include <string>
#include <utility>
#include <vector>


class EPSMagicNumbers : public MagicNumbers
{
public:
  explicit EPSMagicNumbers(Limits _limits) : MagicNumbers(std::move(_limits)) {}

  EPSMagicNumbers(const EPSMagicNumbers&)     = default;
  EPSMagicNumbers(EPSMagicNumbers&&) noexcept = default;

  /// Delete both due to const members
  EPSMagicNumbers& operator=(const EPSMagicNumbers&) = delete;
  EPSMagicNumbers& operator=(EPSMagicNumbers&&) = delete;

  ~EPSMagicNumbers() noexcept override = default;

  /**
   * Save the state and set the line colour and thickness to use
   *
   * Nothing
   *
   * \return The string to write to file
   */
  [[nodiscard]] inline std::string Setup() const override
  {
    return fmt::format("\n%Magic Numbers\n"
                       "gs\n"
                       "black rgb\n"
                       "1 u div sl\n");
  }

  /**
   * The lines are drawn so return the postscript interpreter to it's previous state
   *
   * \param Nothing
   *
   * \return The string to write to file
   */
  [[nodiscard]] inline std::string TearDown() const override { return fmt::format("gr\n"); }

  /**
   * Write the EPS code that draws the proton line
   *
   * \param The stream to write to, i.e. the chart
   * \param The magic number to draw
   *
   * \return Nothing
   */
  [[nodiscard]] std::string WriteProtonNumber(const int number) const override;

  /**
   * Write the EPS code that draws the neutron line
   *
   * \param The stream to write to, i.e. the chart
   * \param The magic number to draw
   *
   * \return Nothing
   */
  [[nodiscard]] std::string WriteNeutronNumber(const int number) const override;
};

#endif // EPSMAGICNUMBERS_HPP
