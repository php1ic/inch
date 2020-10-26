/**
 *
 * \class SVGChart
 *
 * \briefDerived class to create the chart in SVG format
 *
 * Implement the purely virtual methods from the base
 */

#ifndef SVGCHART_HPP
#define SVGCHART_HPP

#include "inch/chart.hpp"
//#include "inch/svg_dripline.hpp"

#include <vector>

class Nuclide;
class Options;

class SVGChart : public Chart
{
public:
  explicit SVGChart(Options& _options) : Chart(_options){};

  SVGChart(const SVGChart& SVGChart)     = default;
  SVGChart(SVGChart&& SVGChart) noexcept = default;

  SVGChart& operator=(const SVGChart& SVGChart) = delete;
  SVGChart& operator=(SVGChart&& SVGChart) noexcept = delete;

  ~SVGChart() noexcept override = default;

  /// Container for the drip lines that will actually be drawn
  // mutable std::vector<SVGDripLine> drip_lines;

  void write(const std::vector<Nuclide>& massTable, const Partition& /*part*/) const override;

  [[nodiscard]] std::string prolog() const override;

  [[nodiscard]] std::string header() const override;

  [[nodiscard]] std::string definitions() const override;

  [[nodiscard]] std::string colours() const override;

  [[nodiscard]] inline std::string isotope() const override { return std::string(); }

  [[nodiscard]] inline std::string infoComment() const override { return std::string(); }

  [[nodiscard]] std::string setup() const override;

  [[nodiscard]] std::string teardown() const override;

  [[nodiscard]] inline std::string KeySetup(const int /*ZRange*/) const override { return std::string(); }

  [[nodiscard]] inline std::string KeyTearDown() const override { return std::string(); };
};

#endif // SVGCHART_HPP
