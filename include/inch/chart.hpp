/**
 *
 * \class BaseChart
 *
 * \brief
 */

#ifndef CHART_HPP
#define CHART_HPP

#include <inch/dripline.hpp>
#include <inch/options.hpp>

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

class Nuclide;
class Partition;


class Chart
{
public:
  explicit Chart(Options& _options) : options(_options) { setTime(std::time(nullptr)); };

  Chart(const Chart& Chart)     = default;
  Chart(Chart&& Chart) noexcept = default;

  Chart& operator=(const Chart& Chart) = delete;
  Chart& operator=(Chart&& Chart) noexcept = delete;

  virtual ~Chart() noexcept = default;


  virtual void write(const std::vector<Nuclide>& nuc, const Partition& part) const = 0;

  virtual std::string prolog() const = 0;

  virtual std::string setup() const = 0;

  virtual std::string teardown() const = 0;

  virtual std::string KeySetup(const int ZRange) const = 0;

  virtual std::string KeyTearDown() const = 0;

  /// Get the date and time that chart is created (i.e. runtime)
  [[nodiscard]] inline auto getTime() const { return std::put_time(now, "%Y-%m-%dT%H:%M:%S"); }

  /// Write the current date and time into the member variable
  inline void setTime(const std::time_t theTime) const { now = std::localtime(&theTime); }

  /// Get a descriptive text string that we can write to the file
  [[nodiscard]] inline std::string getTitle(const ChartColour chart_colour) const
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

  /**
   *
   */
  [[nodiscard]] inline double
  calculateCanvasHeight(const double key_scale, const double key_height, const int ZRange) const
  {
    double chart_height = ZRange + 2 * BORDER;

    if (key_height * key_scale > chart_height)
      {
        key_relative = true;
        chart_height = (key_height + BORDER) * key_scale;
      }

    return chart_height;
  }

  /**
   *
   */
  [[nodiscard]] inline double calculateCanvasWidth(const double key_scale) const
  {
    // HACKS
    // When all nuclei are drawn, key is in top left.
    // The below stops extra space being created on the right.
    //
    // max_key_width*scale extends the width to fit the widest key
    // This should really be set as a function of the variable
    // used to colour the isotopes. Either way, this cannot be
    // set dynamically in the file so we need to use 'magic numbers'
    double chart_width = options.limits.getNRange() + 2 * BORDER;

    if (options.chart_selection != ChartSelection::FULL_CHART && options.limits.getZRange() < Limits::MAX_Z)
      {
        chart_width += (max_key_width * key_scale);
      }

    return chart_width;
  }

  /**
   * Use details about what is being drawn to calculate how big the canvas should be
   *
   * \param key_scale The size of an element in the key, relative to one in the body of the chart
   * \param key_height The height of the key in 'chart units'
   */
  inline void setCanvasSize(const double key_scale, const double key_height) const
  {
    width  = calculateCanvasWidth(key_scale);
    height = calculateCanvasHeight(key_scale, key_height, options.limits.getZRange());
  }

  /**
   * If the file is to be converted into some other format (eg.jpg,png),
   * without any resizing, and the whole chart is drawn.
   *
   * The following
   * values are the largest allowed for the given page size.
   * A0 -> size = 18
   * A1 -> size = 13
   * A2 -> size =  9
   * A3 -> size =  6
   * A4 -> size =  4
   * A5 -> size =  3
   */
  mutable int size{ 4 };

  /// How rounded are the 'squares' representing an isotope, 0=square, 1=circle
  mutable double curve{ 0.25 };

  /// The date and time at runtime
  mutable std::tm* now{ nullptr };

  /// The options used to create the chart
  Options& options;

  /// Amount of space to leave on the edge, before the first, and after the last, isotope
  mutable double BORDER{ 1.0 };

  /// Is the key taller than the Z range
  mutable bool key_relative{ false };

  /// How much additional width do we need for the key if it's to the side of the chart
  mutable double max_key_width{ 14.5 };

  /// Total height (Z range) of the chart
  mutable double height{ 0.0 };
  /// Total width (N range) of the chart
  mutable double width{ 0.0 };
};

#endif // CHART_HPP
