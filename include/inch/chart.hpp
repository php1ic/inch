/**
 *
 * \class Chart
 *
 * \brief Utility class to actually create the chart.
 *
 * There are lots of classes that store and write various aspects
 * of the chart, including the file type use. This class gathers
 * everything together to create a single entry point for the
 * user to call.
 *
 */
#ifndef CHART_HPP
#define CHART_HPP

#include <inch/dripline.hpp>
#include <inch/options.hpp>

#include <iostream>
#include <vector>

class Nuclide;
class Partition;


class Chart
{
public:
  Chart() = default;

  Chart(const Chart& Chart)     = default;
  Chart(Chart&& Chart) noexcept = default;

  Chart& operator=(const Chart& Chart) = default;
  Chart& operator=(Chart&& Chart) noexcept = default;

  ~Chart() noexcept = default;

  /**
   * Top level function called by the user.
   *
   * \param nuc A std::vector of Nuclides representing the mass table
   * \param draw An instance of the Options class containing how the chart will be drawn
   * \param part An instance of the Partition class containing colour partition information
   *
   * \return Nothing
   */
  void write(const std::vector<Nuclide>& nuc, const Options& draw, const Partition& part) const;

  /**
   * Function to create the *.eps file type
   *
   * \param nuc A std::vector of Nuclides representing the mass table
   * \param draw An instance of the Options class containing how the chart will be drawn
   * \param part An instance of the Partition class containing colour partition information
   *
   * \return Nothing
   */
  void writeEPS(const std::vector<Nuclide>& nuc, const Options& draw, const Partition& part) const;

  /**
   * Function to create the *.svg file type
   *
   * \param nuc A std::vector of Nuclides representing the mass table
   * \param draw An instance of the Options class containing how the chart will be drawn
   *
   * \return Nothing
   */
  void writeSVG(const std::vector<Nuclide>& nuc, const Options& draw) const;

  /**
   * Function to create the *.tikz file type
   *
   * \param nuc A std::vector of Nuclides representing the mass table
   * \param draw An instance of the Options class containing how the chart will be drawn
   *
   * \return Nothing
   */
  void writeTIKZ(const std::vector<Nuclide>& nuc, const Options& draw) const;

  /**
   * Create the string that is inserted into the file
   *
   * \param in The massTable use to draw the figure
   * \param draw An instance of the Options class containing how the chart will be drawn
   * \param outFile An open output stream pointing to the file being populated
   *
   * \return Nothing
   */
  void drawNuclei(const std::vector<Nuclide>& in, const Options& draw, std::ostream& outFile) const;

  /**
   *
   */
  [[nodiscard]] inline double setCanvasHeight(const double key_scale, const double key_height, const int ZRange) const
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
  [[nodiscard]] inline double setCanvasWidth(const double key_scale, const Options& draw) const
  {
    // HACKS
    // When all nuclei are drawn, key is in top left.
    // The below stops extra space being created on the right.
    //
    // max_key_width*scale extends the width to fit the widest key
    // This should really be set as a function of the variable
    // used to colour the isotopes. Either way, this cannot be
    // set dynamically in the file so we need to use 'magic numbers'
    double chart_width = draw.limits.getNRange() + 2 * BORDER;

    if (draw.chart_selection != ChartSelection::FULL_CHART && draw.limits.getZRange() < Limits::MAX_Z)
      {
        width += (max_key_width * key_scale);
      }

    return chart_width;
  }

  /**
   * Use details about what is being drawn to calculate how big the canvas should be
   *
   * \param key_scale The size of an element in the key, relative to one in the body of the chart
   * \param key_height The height of the key in 'chart units'
   * \param draw An instance of the Options class containing how the chart will be drawn
   */
  inline void setCanvasSize(const double key_scale, const double key_height, const Options& draw) const
  {
    width  = setCanvasWidth(key_scale, draw);
    height = setCanvasHeight(key_scale, key_height, draw.limits.getZRange());
  }

  /**
   * Setup commands to do before starting to draw the chart. This is independent and after the prolog
   *
   * \param Nothing
   *
   * \return The string to set up writing the EPS file
   */
  inline std::string EPSsetup() const
  {
    return fmt::format("u dup scale\n"
                       "0.5 dup translate\n");
  }

  /**
   * Final commands to finish off writing the EPS file.
   *
   * \param Nothing
   *
   * \return The string to tidy up and 'finish' the EPS file
   */
  inline std::string EPSteardown() const
  {
    return fmt::format("end grestore\n"
                       "\n"
                       "%%Trailer\n"
                       "%%BoundingBox: 0 0 {} {}\n"
                       "%%EOF\n",
                       std::ceil(width * size),
                       std::ceil(height * size));
  }

  /**
   * Check if the key should be shifted relative to the chart and create the required string
   *
   * \param The z range of the current chart
   *
   * \return The necessary string to position the chart relative to the key
   */
  inline std::string EPSRelativeKeySetup(const int ZRange) const
  {
    return (key_relative) ? fmt::format("\n%Shift coordinates so chart is vertically centered\n"
                                        "gs\n"
                                        "0 {} translate\n",
                                        0.5 * (height - (ZRange + 2 * BORDER)))
                          : "";
  }

  /**
   * Check if the key is shifted relative to the chart and create the required string to tidy up
   *
   * \param Nothing
   *
   * \return The necessary string to tidy up if the chart position was shifted
   */
  inline std::string EPSRelativeKeyTearDown() const
  {
    return (key_relative) ? fmt::format("\n%Put coordinates back now that chart is drawn\n"
                                        "gr\n")
                          : "";
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

  /// Container for the drip lines that will actually be drawn
  mutable std::vector<DripLine> drip_lines;
};

#endif // CHART_HPP
