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

#include <inch/limits.hpp>

#include "inch/chartColour.hpp"

#include <iomanip>
#include <memory>

struct tm;

class Prolog
{
public:
  Prolog(const int size, const Limits _limits) : limits(_limits), chart_size(size) { setTime(std::time(nullptr)); }

  Prolog(const Prolog&) = default;
  Prolog(Prolog&&)      = default;

  Prolog& operator=(Prolog&&) = default;
  Prolog& operator=(const Prolog&) = default;

  ~Prolog() = default;

  /// Get the date and time that chart is created (i.e. runtime)
  inline auto getTime() const { return std::put_time(now, "%Y-%m-%dT%H:%M:%S"); }

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
  /// The date and time at runtime
  mutable std::tm* now{ nullptr };
  /// The size of the chart in 'iostope units'
  mutable int chart_size{ 0 };
  /// How rounded are the 'squares' representing an isotope, 0=square, 1=circle
  mutable double curve{ 0.25 };

  /// Write the current date and time into the member variable
  inline void setTime(const std::time_t theTime) const { now = std::localtime(&theTime); }

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
