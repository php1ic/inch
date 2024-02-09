/**
 *
 * \class Chart
 *
 * \brief Base class, file type classes can inherit from
 *
 * Whatever file format we create, we want to do the same thing. Set up the
 * methods and implement common functionality in this class so other can inherit.
 */
#ifndef CHART_HPP
#define CHART_HPP

#include <inch/dripline.hpp>
#include <inch/options.hpp>

#include <fmt/chrono.h>
#include <fmt/format.h>
#include <fmt/std.h>

#include <string>
#include <vector>

class Nuclide;
class Partition;


class Chart
{
public:
  explicit Chart(Options& _options) : options(_options){};

  Chart(const Chart& Chart)     = default;
  Chart(Chart&& Chart) noexcept = default;

  Chart& operator=(const Chart& Chart)     = delete;
  Chart& operator=(Chart&& Chart) noexcept = delete;

  virtual ~Chart() noexcept = default;


  /**
   * Utility function to write the output file
   *
   * \param nuc A vector of Nuclide containing isotope data
   * \param part An instance of the Parition class to colour the isotopes by
   *
   * \return Nothing
   */
  virtual void write(const std::vector<Nuclide>& nuc, const Partition& part) const = 0;

  /**
   * Everything that needs to be done and defined before we can start drawing isotopes
   *
   * \param Nothing
   *
   * \return text in the appropriate format
   */
  [[nodiscard]] virtual std::string prolog() const = 0;

  /**
   * Text strings and comments requried by the file format specifications
   *
   * \param Nothing
   *
   * \return text in the appropriate format
   */
  [[nodiscard]] virtual std::string header() const = 0;

  /**
   * Function definitions or shortcuts that are used in the file
   *
   * \param Nothing
   *
   * \return text in the appropriate format
   */
  [[nodiscard]] virtual std::string definitions() const = 0;

  /**
   * Conversions and setup to allow use of colour names
   *
   * \param Nothing
   *
   * \return text in the appropriate format
   */
  [[nodiscard]] virtual std::string colours() const = 0;

  /**
   * The text required to allow us to draw an isotope with a single command
   * and perhaps as few arguments
   *
   * \param Nothing
   *
   * \return text in the appropriate format
   */
  [[nodiscard]] virtual std::string isotope() const = 0;

  /**
   * Any additional details for those brave enought to read the source
   *
   * \param Nothing
   *
   * \return text in the appropriate format
   */
  [[nodiscard]] virtual std::string infoComment() const = 0;

  /**
   * A border is used to stop the chart being drawn at the very edge of the canvas,
   * so what needs to be done here
   *
   * \param Nothing
   *
   * \return text in the appropriate format
   */
  [[nodiscard]] virtual std::string setup() const = 0;

  /**
   * Once everything is drawn, tidy up and 'close' the file
   *
   * \param Nothing
   *
   * \return text in the appropriate format
   */
  [[nodiscard]] virtual std::string teardown() const = 0;

  /**
   * Where is the key placed in this chart
   *p
   * \param Nothing
   *
   * \return text in the appropriate format
   */
  [[nodiscard]] virtual std::string KeySetup(const int ZRange) const = 0;

  /**
   * Revert any coordinate changes required for drawing the key
   *
   * \param Nothing
   *
   * \return text in the appropriate format
   */
  [[nodiscard]] virtual std::string KeyTearDown() const = 0;

  /**
   * Run the colour() function
   *
   * \param Nothing
   *
   * \return Colour definitions for the appropriate filetype
   */
  [[nodiscard]] inline std::string colors() const { return colours(); }

  /**
   * Get the date and time that chart is created (i.e. runtime)
   *
   * \param Nothing
   *
   * \return Run time as a std::string
   */
  [[nodiscard]] static inline std::string getTime()
  {
    return fmt::format("{:%Y-%m-%dT%H:%M:%S}", fmt::localtime(std::time(nullptr)));
  }

  /**
   * Get a descriptive text string that we can write to the file
   *
   * \param chart_colour The property the chart is being coloured by
   *
   * \return A human readable title for the chart
   */
  [[nodiscard]] static inline std::string getTitle(const ChartColour chart_colour)
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
   * Calcuate how tall the canvas need to be to allow the isotopes and key to be shown
   *
   * \param key_scale The scaling value of the key
   * \param key_height The total height of the key
   * \param ZRange The Z range that is drawn
   *
   * \return The height the canvas should be
   */
  [[nodiscard]] inline double
  // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
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
   * Calculate how wide the canvas need to be to allow the isotopes and key to be shown
   *
   * \param key_scale The scaling value for the key
   *
   * \return The width the canvas should be
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
