/**
 *
 * \class Prolog
 *
 * \brief Set up the definitions and functions that will be used to create the chart
 *
 * To make smaller files, and create easier to read 'code', we define various functions, methods
 * and constants for all type. Use this class to do that.
 */
#ifndef PROLOG_HPP
#define PROLOG_HPP

#include "inch/chartColour.hpp"
#include "inch/limits.hpp"

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <ctime>
#include <iomanip>
#include <string>

struct tm;

class Prolog
{
public:
  Prolog(const int size, const Limits& _limits) : limits(_limits), chart_size(size) {}

  Prolog(const Prolog&) = default;
  Prolog(Prolog&&)      = default;

  Prolog& operator=(Prolog&&) = default;
  Prolog& operator=(const Prolog&) = default;

  ~Prolog() = default;

  /// Get the date and time that chart is created (i.e. runtime)
  [[nodiscard]] inline std::string getTime() const
  {
    std::time_t t = std::time(nullptr);
    return fmt::format("{:%Y-%m-%dT%H:%M:%S}", *std::localtime(&t));
  }

  /**
   * Write the functions needed to create the EPS file
   *
   * \param The ChartColour enum of the currently drawn chart
   *
   * \return The prolog as a string
   */
  std::string EPSWriteProlog(const ChartColour chart_colour) const;

  /**
   * Write the functions needed to create the TikZ file
   *
   * \param Nothing
   *
   * \return The prolog as a string
   */
  std::string TIKZWriteProlog(/*const Options> &options*/) const;

  /**
   * Write the functions needed to create the SVG file
   *
   * \param Nothing
   *
   * \return The prolog as a string
   */
  std::string SVGWriteProlog() const;

private:
  /// Store the N,Z limits of the chart being created
  mutable Limits limits;
  /// The size of the chart in 'iostope units'
  mutable int chart_size{ 0 };
  /// How rounded are the 'squares' representing an isotope, 0=square, 1=circle
  mutable double curve{ 0.25 };

  /// Get a descriptive text string that we can write to the file
  inline std::string getTitle(const ChartColour chart_colour) const
  {
    return [&]() {
      switch (chart_colour)
        {
          case ChartColour::MASSEXCESSERROR:
          default:
            return "Error on mass-excess";
          case ChartColour::REL_MASSEXCESSERROR:
            return "Relative error on mass-excess";
          case ChartColour::GS_DECAYMODE:
            return "Major ground-state decay mode";
          case ChartColour::GS_HALFLIFE:
            return "Ground-state half-life";
          case ChartColour::FIRST_ISOMERENERGY:
            return "First isomer energy";
        }
    }();
  }
};

#endif // PROLOG_HPP
