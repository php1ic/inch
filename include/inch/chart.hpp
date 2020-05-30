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

#include <iostream>
#include <vector>


class Nuclide;
class Partition;
class Options;


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
   * Use details about what is being drawn to calculate how big the canvas should be
   *
   * \param key_scale The size of an element in the key, relative to one in the body of the chart
   * \param key_height The height of the key in 'chart units'
   * \param draw An instance of the Options class containing how the chart will be drawn
   */
  void setCanvasSize(const double key_scale, const double key_height, const Options& draw) const;

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
